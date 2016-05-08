/*
 * listener.c -- joins a multicast group and echoes all data it receives from
 *the group to its stdout...
 *
 * Antony Courtney,25/11/94
 * Modified by: Frdric Bastien (25/03/04)
 * to compile without warning and work correctly
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/****** Adjustable variables **************/
#define AVGOVER 1
#define ONTIMEOUT 999 // After how long turn off everything to redetermine state 

#define POWERFACTOR  1.00
#define VOLTAGE      245
#define PLUG1ON_C    0.47 
#define PLUG2ON_C    2.70
#define PLUGSON_C    3.10
/*******************************************/



#define PLUG1ON     (PLUG1ON_C * VOLTAGE * POWERFACTOR) 
#define PLUG2ON     (PLUG2ON_C * VOLTAGE * POWERFACTOR)
#define PLUGSON     (PLUGSON_C * VOLTAGE * POWERFACTOR)


#define HELLO_PORT 22600
#define HELLO_GROUP "224.192.32.19"
#define MSGBUFSIZE 2000

// vvvvvv -- Pimote control code -- vvvvvv 
//    Insert at the top of the code 
#include <wiringPi.h>
  #define	D0	    0
  #define	D1	    3
  #define	D2	    4
  #define	D3	    2
  #define	ModSel  5
  #define	CE      6 
int pimote_setup (void) {
  wiringPiSetup () ;
  // Set GPIO modes 
  pinMode (D0, OUTPUT);
  pinMode (D1, OUTPUT);
  pinMode (D2, OUTPUT);
  pinMode (D3, OUTPUT);
  pinMode (ModSel, OUTPUT);
  pinMode (CE, OUTPUT);
  delay (100);    // in ms 
  // Set initial values 
  digitalWrite (D0, LOW);
  digitalWrite (D1, LOW);
  digitalWrite (D2, LOW);
  digitalWrite (D3, LOW);
  digitalWrite (ModSel, LOW);
  digitalWrite (CE, LOW);
  delay (100);    // in ms  
}
int pimote_onoff (int socket, int on1off) {
  int r = 0;
  if (on1off == 1) {
    printf ("Pimote turning ON ");
    digitalWrite (D3, HIGH);  // Turn on 
  } else {
    printf ("Pimote turning OFF ");
    digitalWrite (D3, LOW);  // Turn off 
  } 
  switch (socket) {
    case 1:
      printf ("socket 1.\n");
      digitalWrite (D2, HIGH);
      digitalWrite (D1, HIGH);
      digitalWrite (D0, HIGH);
      r = 1;
      break;
    case 2:
      printf ("socket 2.\n");
      digitalWrite (D2, HIGH);
      digitalWrite (D1, HIGH);
      digitalWrite (D0, LOW);
      r = 2;
      break;
    default:
      printf ("ALL sockets.\n");
      digitalWrite (D2, LOW);
      digitalWrite (D1, HIGH);
      digitalWrite (D0, HIGH);
      r = 0;
  }
  // Execute by toggling Chip Enable 
  delay (100) ;
  digitalWrite (CE, HIGH);
  delay (250) ; 
  digitalWrite (CE, LOW);
  delay (650) ;  
  return r;
}
// ^^^^^^ -- Pimote control code -- ^^^^^^  


int main(int argc, char *argv[])
{
  struct sockaddr_in addr;
  int fd, nbytes,addrlen;
  struct ip_mreq mreq;
  char msgbuf[MSGBUFSIZE];
  
    int i;
    char *msg1, *msg2;
    char msg3[9];
    unsigned long valUsage =0, valGenerating =0, valExporting =0;
    unsigned long avgUsage =0, avgGenerating =0, avgExporting =0;
    unsigned long sumUsage =0, sumGenerating =0, sumExporting =0;
    unsigned long aryUsage[AVGOVER] ={0}, aryGenerating[AVGOVER] ={0}, aryExporting[AVGOVER] ={0};
    int countUsage =0, countGenerating =0, countExporting =0;
    int statusSocket, countON;
    
    FILE * pLogFile = NULL;
    FILE * pGraphFile = NULL;
    const char defaultlogfilename[] = "/var/tmp/solar.log";
    const char defaultgraphname[] = "/var/tmp/solar_graph.log";
    char logfilename[100];
    time_t rawtime;
    char timestr[30];
    
    
    
    pimote_setup (); // Setup Pimote GPIO 
    
    if (argc >= 2) {  // Determine if we use command parameter or default logfilename 
      strncpy(logfilename, argv[1], 99);
      logfilename[99] = '\0';
    } else {
      strncpy(logfilename, defaultlogfilename, sizeof(defaultlogfilename));
    }
    printf("Writing to log file:- %s\n\n", logfilename);
    
    

  u_int yes=1;            /*** MODIFICATION TO ORIGINAL */
  /* create what looks like an ordinary UDP socket */
  if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
    perror("socket");
    exit(1);
  }
  /**** MODIFICATION TO ORIGINAL */
  /* allow multiple sockets to use the same PORT number */
  if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
    perror("Reusing ADDR failed");
    exit(1);
  }
  /*** END OF MODIFICATION TO ORIGINAL */
  /* set up destination address */
  memset(&addr,0,sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
  addr.sin_port=htons(HELLO_PORT);
  /* bind to receive address */
  if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
  }
  /* use setsockopt() to request that the kernel join a multicast group */
  mreq.imr_multiaddr.s_addr=inet_addr(HELLO_GROUP);
  mreq.imr_interface.s_addr=htonl(INADDR_ANY);
  if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
    perror("setsockopt");
    exit(1);
  } /* now just enter a read-print loop */
  while (1) {
    addrlen=sizeof(addr);
    if ((nbytes=recvfrom(fd,msgbuf,MSGBUFSIZE,0,
             (struct sockaddr *) &addr,&addrlen)) < 0) {
      perror("recvfrom");
      exit(1);
    }
    msgbuf[nbytes] = 0x00; // null terminate before printing
    puts(msgbuf);
    
    
    // MY own addition: 
    if (msgbuf[1] == 'e'){  // Electricity usage only 
        msg1 = strstr(msgbuf, "<curr"); // Find generating section of XML 
        msg1 = strstr(msg1, ">"); // Find the start of number 
        msg2 = strstr(msg1, "."); // Find the end of number 
        strncpy(msg3, msg1 + 1, msg2 - msg1); // Extract the number out to a string 
        printf("Current Usage String found:- %s\n", msg3);
        valUsage = strtoul(msg3, NULL, 10); // Convert the number to unsigned long 
        
        // Averaging function: 
        sumUsage = sumUsage + valUsage - aryUsage[countUsage]; 
        aryUsage[countUsage] = valUsage; 
        if (countUsage < AVGOVER -1) countUsage++; else countUsage = 0;
        avgUsage = sumUsage / AVGOVER;
        // printf ("avg: %lu / %lu / %d / %lu \n", sumUsage, valUsage, countUsage, avgUsage);
    }
    
    if (msgbuf[1] == 's'){  // Solar information only

        msg1 = strstr(msgbuf, "generating"); // Find generating section of XML 
        msg1 = strstr(msg1, ">"); // Find the start of number 
        msg2 = strstr(msg1, "."); // Find the end of number 
        strncpy(msg3, msg1 + 1, msg2 - msg1); // Extract the number out to a string 
        printf("Generating String found:- %s\n", msg3);
        valGenerating = strtoul(msg3, NULL, 10); // Convert the number to unsigned long 
        // printf("Integer valGenerating is %d  \n", valGenerating); 
        
        msg1 = strstr(msgbuf, "exporting"); // Find exporting section of XML 
        msg1 = strstr(msg1, ">"); // Find the start of number 
        msg2 = strstr(msg1, "."); // Find the end of number 
        strncpy(msg3, msg1 + 1, msg2 - msg1); // Extract the number out to a string 
        printf("Exporting String found:- %s\n", msg3);
        valExporting = strtoul(msg3, NULL, 10); // Convert the number to unsigned long 
        // printf("Integer valExporting is %d  \n", valExporting); 

        
        // Socket switching logic: 
        countON++;  // Keep a count of how long something has been turned on. 
        switch(statusSocket) {
        case 9 :
            if (valExporting >= PLUGSON) { // Turn everything on! 
                pimote_onoff (0,1);     statusSocket = 9;
            } else if (valExporting <= 5) { // Everything off! 
                pimote_onoff (0,0);     statusSocket = 0;   countON = 0;
            }
            break;
        case 2 :
            if (valExporting >= (PLUGSON - PLUG2ON + 20)) { // Turn everything on! 
                pimote_onoff (0,1);     statusSocket = 9;   countON = 0;
            } else if (valExporting >= PLUG2ON) {
                pimote_onoff (2,1);     statusSocket = 2; 
                pimote_onoff (1,0);
            } else if (valExporting <= 5) { // Everything off! 
                pimote_onoff (0,0);     statusSocket = 0;   countON = 0;
            } else {
                pimote_onoff (2,1);     statusSocket = 2; 
                pimote_onoff (1,0);
            }
            break;
        case 1 :
            if (valExporting >= (PLUGSON - PLUG1ON + 50)) { // Turn everything on! 
                pimote_onoff (0,1);     statusSocket = 9;   countON = 0;
            } else if (valExporting >= PLUG2ON) {
                pimote_onoff (2,1);     statusSocket = 2;   countON = 0; 
                pimote_onoff (1,0);
            } else if (valExporting >= PLUG1ON) {
                pimote_onoff (2,0);
                pimote_onoff (1,1);     statusSocket = 1;
            } else if (valExporting <= 5) { // Everything off! 
                pimote_onoff (0,0);     statusSocket = 0;   countON = 0;
            } else {
                pimote_onoff (2,0);
                pimote_onoff (1,1);     statusSocket = 1;
            }
            break;
        default : 
            if (valExporting >= PLUGSON) { // Turn everything on! 
                pimote_onoff (0,1);     statusSocket = 9;   countON = 0;
            } else if (valExporting >= PLUG2ON) {
                pimote_onoff (2,1);     statusSocket = 2;   countON = 0;
                pimote_onoff (1,0);
            } else if (valExporting >= PLUG1ON) {
                pimote_onoff (2,0);
                pimote_onoff (1,1);     statusSocket = 1;   countON = 0;
            } else {
                pimote_onoff (0,0);     statusSocket = 0;   countON = 0;
            }
        }
        if (countON == ONTIMEOUT) {// Everything off for 2 cycles to reassess power usage 
            pimote_onoff (0,0);
        } else if (countON > ONTIMEOUT) {
            pimote_onoff (0,0);     statusSocket = 0;   countON = 0;
        }
        
        
        // Get time for later log files         
        time (&rawtime);
        strftime(timestr, 30, "%Y/%m/%d %H:%M:%S", localtime(&rawtime)); // generate desired time format 
        
        // Averaging and graph log output: 
        sumExporting = sumExporting + valExporting - aryExporting[countExporting]; 
        aryExporting[countExporting] = valExporting; 
        if (countExporting < AVGOVER -1) countExporting++; else countExporting = 0;
        avgExporting = sumExporting / AVGOVER;
        sumGenerating = sumGenerating + valGenerating - aryGenerating[countGenerating]; 
        aryGenerating[countGenerating] = valGenerating; 
        if (countGenerating < AVGOVER -1) countGenerating++; else countGenerating = 0;
        avgGenerating = sumGenerating / AVGOVER;
        // printf("--- avg counters:  %d | %d | %d ---\n", countUsage, countExporting, countGenerating);
        if (countExporting == 0) {
            pGraphFile = fopen(defaultgraphname, "a"); // append to the end of the file 
            if (pGraphFile == NULL){
                printf("---ERROR--------graph log file open failed--------ERROR---\n");
                fflush(stdout); // print everything in the stdout buffer
            } else {
                sprintf(msgbuf, "%s,%lu,%lu,%lu\n", timestr, avgUsage, avgGenerating, avgExporting);
                fprintf(pGraphFile, "%s", msgbuf);
                printf("Writen to graph log file:- %s", msgbuf);
                fclose(pGraphFile);
            }
        }
        
        // Log current status 
        pLogFile = fopen(logfilename, "a"); // append the information into a file 
        if (pLogFile == NULL){
            printf("---ERROR--------file open failed--------ERROR---\n");
            fflush(stdout); // print everything in the stdout buffer
            exit(1);
        } else {
            sprintf(msgbuf, "%s | %d|%3d | %4lu | %4lu | %4lu \n", timestr, statusSocket, countON, valUsage, valGenerating, valExporting);
            fprintf(pLogFile, "%s", msgbuf);
            printf("Writen to log file:- %s", msgbuf);
            fclose(pLogFile);
        }
        
        printf ("\n");
        fflush(stdout); // print everything in the stdout buffer
    }

  }
}


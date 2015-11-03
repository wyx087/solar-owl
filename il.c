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


#define HELLO_PORT 22600
#define HELLO_GROUP "224.192.32.19"
#define MSGBUFSIZE 2000

int main(int argc, char *argv[])
{
  struct sockaddr_in addr;
  int fd, nbytes,addrlen;
  struct ip_mreq mreq;
  char msgbuf[MSGBUFSIZE];
  
    char *msg1, *msg2;
    char msg3[9];
    unsigned long valGenerating, valExporting;
    
    FILE * pFile = NULL;
    const char defaultlogfilename[] = "solargen.log";
    char logfilename[100];
    time_t rawtime;
    char timestr[30];
    
    if (argc >= 2) {  // Determine if we use command parameter or default 
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
  }
  
  

  /* now just enter a read-print loop */
  while (1) {
    addrlen=sizeof(addr);
    if ((nbytes=recvfrom(fd,msgbuf,MSGBUFSIZE,0,
             (struct sockaddr *) &addr,&addrlen)) < 0) {
      perror("recvfrom");
      exit(1);
    }
    msgbuf[nbytes] = 0x00; // null terminate before printing
    puts(msgbuf);
    
    
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

        
        pFile = fopen(logfilename, "a"); // append the information into a file 
        if (pFile == NULL){
            printf("---ERROR--------file open failed--------ERROR---");
            exit(1);
        } else {
            time (&rawtime);
            strftime(timestr, 30, "%H:%M:%S", localtime(&rawtime)); // generate desired time format 
            fprintf(pFile, "%s|%lu|%lu\n", timestr, valGenerating, valExporting);
            printf("Writen to file:- %s|%lu|%lu\n", timestr, valGenerating, valExporting);
            fclose(pFile);
        }
        
        printf ("\n");
        fflush(stdout); // print everything in the stdout buffer
    }

  }
}


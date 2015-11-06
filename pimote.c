/*
 * blink.c:
 *	Standard "blink" program in wiringPi. Blinks an LED connected
 *	to the first GPIO pin.
 *
 * Copyright (c) 2012-2013 Gordon Henderson. <projects@drogon.net>
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>


// vvvvvv -- Pimote control code -- vvvvvv 
//    Insert at the top of the code 
#include <wiringPi.h>
  #define	D0	0
  #define	D1	3
  #define	D2	4
  #define	D3	2
  #define	ModSel 5
  #define	CE  6 
int pimote_setup (void) {
  wiringPiSetup () ;
  // Set GPIO modes 
  pinMode (D0, OUTPUT) ;
  pinMode (D1, OUTPUT) ;
  pinMode (D2, OUTPUT) ;
  pinMode (D3, OUTPUT) ;
  pinMode (ModSel, OUTPUT) ;
  pinMode (CE, OUTPUT) ;
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
int pimote_1on (void) {
  printf ("Pimote turning ON socket 1 \n") ;
  digitalWrite (D0, HIGH) ;	
  digitalWrite (D1, HIGH) ;	
  digitalWrite (D2, HIGH) ;	
  digitalWrite (D3, HIGH) ;	
  delay (100) ;
  digitalWrite (CE, HIGH);
  delay (250) ; 
  digitalWrite (CE, LOW);
  delay (150) ;
}
int pimote_1off (void) {
  printf ("Pimote turning OFF socket 1 \n") ;
  digitalWrite (D0, HIGH) ;	
  digitalWrite (D1, HIGH) ;	
  digitalWrite (D2, HIGH) ;	
  digitalWrite (D3, LOW) ;	
  delay (100) ;
  digitalWrite (CE, HIGH);
  delay (250) ; 
  digitalWrite (CE, LOW);
  delay (150) ;
}
int pimote_2on (void) {
  printf ("Pimote turning ON socket 2 \n") ;
  digitalWrite (D0, LOW) ;	
  digitalWrite (D1, HIGH) ;	
  digitalWrite (D2, HIGH) ;	
  digitalWrite (D3, HIGH) ;	
  delay (100) ;
  digitalWrite (CE, HIGH);
  delay (250) ; 
  digitalWrite (CE, LOW);
  delay (150) ;
}
int pimote_2off (void) {
  printf ("Pimote turning OFF socket 2 \n") ;
  digitalWrite (D0, LOW) ;	
  digitalWrite (D1, HIGH) ;	
  digitalWrite (D2, HIGH) ;	
  digitalWrite (D3, LOW) ;	
  delay (100) ;
  digitalWrite (CE, HIGH);
  delay (250) ; 
  digitalWrite (CE, LOW);
  delay (150) ;
}
// ^^^^^^ -- Pimote control code -- ^^^^^^  



int main (void)
{
  pimote_setup();
  
  for (;;) {
    pimote_1on();
    delay (5000) ;
    
    pimote_1off();
    delay (10000) ;
  }
  return 0 ;
}

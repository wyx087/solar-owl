# solar
Solar generation logging and reacting 

My setup: 

1. Raspberry Pi is connected to my home network 
2. OWL Intuition broadcasts message on the home network everytime it gets a new reading. 
3. There is a listener program always running on RPi ( ~/Auto/owl_background) 
4. My listener in RPi captures the broadcast and does 2 things:  
  a. Decides whether there is any export and turns on 1 or both of Pimote sockets.  
  b. Records its state and current usage/generation/export information into a file on RAMdisk (/var/tmp) 
5. You can then point your web browser to RaspberryPi and see a graph and log files  (/var/www) 
6. At night, a bash script is run to archive the log file (~/solar/log) and trim the live log file if it's too big (~/Auto/logtrim.sh)

A few things to note: 

- The log files will disappear if power gets turned off because it is stored in RAMdisk. 
- You'll need wiringPi library for RaspberryPi GPIO interactions 
- To build your C-code, use the following command: gcc -lwiringPi -o il il.c 
- Inside www/LINKS.TXT details the links required for the graph webpage 
- There are 2 background scheduled tasks: owl_background on boot and logtrim.sh at night. 
- The log file columns are as follow: date and time | statusSocket | current status counter | Usage reading | Generation reading | Export reading 


To do for il.c:
- INI parser: https://github.com/benhoyt/inih

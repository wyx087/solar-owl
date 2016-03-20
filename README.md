# solar
Solar generation logging and reacting 

My setup: 

1. Raspberry Pi is connected to my home network 
2. OWL Intuition broadcasts message on the home network everytime it gets a new reading. 
3. There is a listener program always running on RPi ( ~/Auto/owl_background) 
4. My listener in RPi captures the broadcast and does 2 things:  
  a. Decides whether there is any export and turns on 1 or both of Pimote sockets.  
  b. Records its state and current usage/generation/export information into a file on RAMdisk.  
5. You can then point your web browser to RaspberryPi and see a graph and log files  
6. At night, a bash script is run to archive the log file and trim the live log file if it's too big (~/Auto/logtrim.sh)

A few things to note: 

- The log files will disappear if power gets turned off because it is stored in RAMdisk. 
- You’ll want to over estimate your appliance power usage slightly, otherwise the remote switches will be constantly turning on and off. 
- The C code is written like a finite state machine, “statusSocket” is the state. 
- You'll need wiringPi library for GPIO interactions 
- To build your C-code, use the following command: gcc -lwiringPi -o il il.c 
- There are 2 background scheduled tasks: owl_background on boot and logtrim.sh at night. 
- The log file columns are as follow: date and time | statusSocket | current status counter | Usage reading | Generation reading | Export reading 

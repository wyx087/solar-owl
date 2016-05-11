# Raspberry Pi Solar Monitor and Usage Automation 

This is what's contained in this repository, there are 3 parts: 

- il.c : Data logging and electricity usage automation on Raspberry Pi 
- www : Website files for viewing the data logs with graphical representation 
- pc.c : Runs on PC to automatically control BOINC according to electricity usage 


Requirements for il.c: 

- OWL Intuition Solar PV monitor 
- Raspberry Pi 1 (should work with newer) 
- Pimote and 2 sockets from Energenie 
- wiringPi library for RaspberryPi GPIO interactions 

The dataflow for il.c and www parts on Raspberry Pi: 

1. Raspberry Pi is connected to my home network 
2. OWL Intuition broadcasts message on the home network everytime it gets a new reading. 
3. There is a listener program always running on RPi (the compiled il.c) 
4. The listener program in RPi captures the broadcast and does 2 things:  
  a. Decides whether there is any export and turns on Pimote sockets and send WakeOnLan PC 
  b. Records its state and current usage/generation/export information into a file on RAMdisk (/var/tmp) 
5. Use any web browser navigate to RaspberryPi and see a graph and log files  (/var/www) 
6. At night, a bash script is run to archive the log file and trim the live log file if it's too big (logtrim.sh)

A few things to note: 

- The log files will disappear if RPi power gets turned off because it is stored in RAMdisk. 
- To build il.c, use the following command: gcc -lwiringPi -o il il.c 
- Inside www/LINKS.TXT details the links required for the graph webpage 
- On RPi, there should be 2 background scheduled tasks: compiled il.c on boot and logtrim.sh at 23:59. 
- The log file columns are defined near the end of C files 
- Attempt has been made to keep similarity between il.c and pc.c so that the updates and bug fixes can be applied to both easily. 


To do:
- INI parser: https://github.com/benhoyt/inih

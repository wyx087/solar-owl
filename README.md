# Raspberry Pi Solar Monitor and Usage Automation 

This is what's contained in this repository, there are 3 parts: 

- il.c : Data logging and electricity usage automation on Raspberry Pi 
- ilm.c : Everything il.c does + monitoring thread to power cycle OWL if timed out 
- www : Website files for viewing the data logs with graphical representation 
- pc.c : Runs on PC to automatically control BOINC according to electricity usage 


Requirements for il.c: 

- OWL Intuition Solar PV monitor 
- Raspberry Pi (I used version 1B, 3B and 4B, should work with all) 
- Setup a RAMdisk at /var/ramdisk (https://www.domoticz.com/wiki/Setting_up_a_RAM_drive_on_Raspberry_Pi) 
- Pimote and plugs from Energenie (see pc.c on how to comment out this part if you only want data logging) 
- wiringPi library for RaspberryPi GPIO interactions (not needed for data logging) 

Requirements for www: 

- Install lighttpd and php-cgi (sudo apt install)
- Create symbolic links as detailed in LINKS.txt 

The dataflow for il.c and www parts on Raspberry Pi: 

1. Raspberry Pi is connected to my home network 
2. OWL Intuition broadcasts message on the home network everytime it gets a new reading. 
3. There is a listener program always running on RPi (the compiled ilm.c) 
4. The listener program in RPi captures the broadcast and does 2 things:  
  a. Decides whether there is any export and turns on Pimote sockets and send WakeOnLan PC   
  b. Records its state and current usage/generation/export information into a file on RAMdisk (/var/ramdisk) 
5. Use any web browser navigate to RaspberryPi and see a graph and log files  (/var/www) 
6. At night, a bash script is run to archive the log file and trim the live log file if it's too big (solarlog-save.sh)
    Log files are archived at /home/pi/solar/log 

A few things to note: 

- The current log file will disappear if RPi power is interrupted. Use "sudo halt" command to safely shutdown RPi 
- On RPi, there should be the following:
  - 1 start up task: compiled ilm (or il)
  - 2 background scheduled tasks: solarlog-save.sh at 23:59, solarlog-persist sync every hour 
  - 1 systemd using solarlog-persist.service, which runs solarlog-persist 
- To build il.c, use the following command: gcc -lwiringPi -o il il.c 
- To build ilm.c, use the following command: gcc -pthread -lwiringPi -o ilm ilm.c 
- To build pc.c, no special command is needed, Linux or Cygwin environment recommended. 
- Inside LINKS.TXT details the links required for the graph webpage 
- The definition for log file columns are found near the end of C files 
- Attempt has been made to keep similarity between il.c, ilm.c and pc.c so that the updates and bug fixes can be applied to both easily. 

#!/bin/bash

#   Save CSV log file archive to disk  

file=/var/ramdisk/solar_graph.log
DATE=$(date +%Y-%m-%d)
cat $file >> /home/pi/solar/log/solar_$DATE.csv


while true; do
    read -p "Do you wish to [r]reboot, [s]shutdown or [c]cancel?" yn
    case $yn in
        [Rr]* ) sudo reboot; exit;;
        [Ss]* ) sudo halt; exit;;
        [Cc]* ) break;;
        * ) echo "Please answer r, s or c.";;
    esac
done

echo "     Log files have been appended to formal log directory. "

while true; do
    read -p "Do you wish to [d]delete or [n]not delete?" yn
    case $yn in
        [Dd]* ) sudo rm $file; exit;;
        [Nn]* ) exit;;
        * ) echo "Please answer d or n.";;
    esac
done


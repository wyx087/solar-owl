#!/bin/bash

#   Save CSV log file archive to disk  

file=/var/ramdisk/solar_graph.log
DATE=$(date +%Y-%m-%d)
cat $file >> /home/pi/solar/log/solar_$DATE.csv

rm $file

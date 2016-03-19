#!/bin/bash

#   Log file trim script 

file=/var/tmp/solar.log
minimumsize=200000
actualsize=$(wc -c <"$file")
if [ $actualsize -ge $minimumsize ]; then
  echo $file  size is $actualsize bytes over $minimumsize bytes, renaming
  mv $file /var/tmp/solar-1.log 
fi


#   Smooth log file archive to disk  

file=/var/tmp/solar_graph.log
DATE=$(date +%Y-%m-%d)
cp $file /home/pi/solar/log/solar.csv
mv $file /home/pi/solar/log/solar_$DATE.csv


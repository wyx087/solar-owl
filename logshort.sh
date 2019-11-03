#!/bin/bash

#   Log only keep last few lines
#   250 lines roughly equals to 2 hours 

filein=/var/ramdisk/solar_graph.log
fileout=/var/ramdisk/solar_graphshort.log
size=250

while [ 1 ]; do

  if [ -f "$filein" ]
  then
    tail -n $size $filein > $fileout
  fi

  sleep 8
done


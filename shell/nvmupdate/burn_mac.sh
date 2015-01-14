#!/bin/bash

# 1. get MAC address from ifconfig
# 2. set awk field separator
# 3. eeupdate32 usage 

mac=`ifconfig | grep 'HWaddr' | awk '{print $5}' | awk -F"[:]" '{print $1$2$3$4$5$6}'`
echo "MAC: $mac"
./eeupdate32 /nic=1 /mac=$mac /invmupdate /file=I211.txt

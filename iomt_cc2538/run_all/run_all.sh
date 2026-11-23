#!/bin/bash

# Mở terminal 1
gnome-terminal --working-directory=/home/user/contiki/iomt_cc2538/python_sub_pub -e "bash -c 'python pub.py; exec bash'" &


# Mở terminal 2
gnome-terminal --working-directory=/home/user/contiki/iomt_cc2538/python_sub_pub -e "bash -c 'python sub.py; exec bash'" &

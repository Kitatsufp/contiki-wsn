#!/usr/bin/env python3

import socket
import binascii
import sys
import socket 
import time
from datetime import datetime
import os
import subprocess
from ping3 import ping

ip_list = ["aaaa::c30c:0:0:2",
'aaaa::c30c:0:0:3',
'aaaa::c30c:0:0:4',
'aaaa::c30c:0:0:5',
'aaaa::c30c:0:0:6',
'aaaa::c30c:0:0:7',
'aaaa::c30c:0:0:8',
'aaaa::c30c:0:0:9',
'aaaa::c30c:0:0:a',
'aaaa::c30c:0:0:b',
'aaaa::c30c:0:0:c',
'aaaa::c30c:0:0:d',
'aaaa::c30c:0:0:e',
'aaaa::c30c:0:0:f',
'aaaa::c30c:0:0:10',
'aaaa::c30c:0:0:11',
'aaaa::c30c:0:0:12',
'aaaa::c30c:0:0:13',
'aaaa::c30c:0:0:14']


def ping_ipv6(address):
	response = os.system("ping6 -c 10 -s 20 -i 1 " + address)
	return response == 0


#if ping_ipv6("aaaa::c30c:0:0:2"):
#	print("IPv6 address is reachable!")
#else:
#	print("IPv6 address is not reachable.")

max_num = 1000
for num in range(1, max_num+1):
	for ip in ip_list:
		ping_ipv6(ip)
		time.sleep(2)

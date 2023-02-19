#!/usr/bin/env python3
from scapy.all import *
ip = IP()


ip.src = "11.111.111.11" # fake source
ip.dst = "10.0.2.6"

ic = ICMP()

packet  = ip/ic
send(packet)
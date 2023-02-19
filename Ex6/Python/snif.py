#!usr/bin/env python3
from scapy.all import *

def print_pkt(pkt):
    pkt.show()

# uncomment the desired command

pkt = sniff(filter='icmp', prn=print_pkt)

pkt = sniff(filter='host 172.217.16.238 and tcp port 23', prn=print_pkt)

# pkt = sniff(filter='net 128.230.0.0/16 ', prn=print_pkt)
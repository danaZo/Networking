#!/usr/bin/env python3
from scapy.all import *

def spoofPkt(pkt):
    # extract dest ip from the packet
    dest = pkt[IP].dst
    src= pkt[IP].src

    # check if this is an echo request
    icmpType = pkt[ICMP].type

    if icmpType != 8:
        return # we only want to spoof ICMP echo requests

    spoofedReply = IP()
    spoofedReply.src = dest
    icmp = ICMP()
    icmp.type = 0
    icmp.id = 33

    send(spoofedReply / icmp)

    print("destination: {}".format(dest))
    print("source: {}".format(src))

# uncomment the desired command
while True:
    pkt = sniff(filter='icmp', prn=spoofPkt)



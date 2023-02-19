from scapy.all import *

ip = IP()

DEST = "8.8.8.8"

MAX_TTL = 150 # the max number of routers we traverse


ttl = 1


ip.dst = DEST
ic = ICMP()

while(ttl < MAX_TTL):
    ip.ttl = ttl
    packet = ip/ic

    response = sr1(packet , timeout = 3, verbose = 0)
    # print(response)
    # we didnt got an answer : the destination is further ahead then the current TTL

    if response == None:
        print("~~ Timeout ~~")


    if response.src == DEST:
        print("Number of routers from your machine to - {} is : {}".format(DEST, ttl))
        break

    ttl += 1
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#ifndef SIZE_ETHERNET
#define SIZE_ETHERNET 14
#endif
#define ETHER_ADDR_LEN 6
#define PCKT_LEN 1024

#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip) (((ip)->ip_vhl) >> 4)




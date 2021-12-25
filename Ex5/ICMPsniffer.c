#include "headers.h"


int main(){


    //Open a raw socket
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sock < 0){
        perror("Socket opening failure: ");
    }
    struct sockaddr_in src;
    socklen_t srclen;
    struct icmp* icmpD;//create a icmp header to get the data of the message
    struct ip*  ipptr ;
    char buff[IP_MAXPACKET];
    int e;
    //have the socket listen to the traffic constantly
    while(1){

        bzero(buff,IP_MAXPACKET);
        e = recvfrom(sock , buff, sizeof(buff), 0, (struct sockaddr *) &src, &srclen);
        if( e < 0){
            perror("receiving error: ");
        }
       
        //we want to read only the ICMP header, for that we have to 
        //skip the Ethernet header and ip header which precedes it.
        //The Ethernet protocol has a fixed size, the tricky part is the ip header
        
        //first we read the ipheader into struct - while skiping the ethernet header
        ipptr = (struct ip *) (buff + SIZE_ETHERNET);

        //now we get the ip header lenght from the ip_hl field. this filed keep the lenght
        //of the hedear in words - which in 32 bit machine consists of 4 Bytes
        size_t iphlen = (ipptr->ip_hl) *4;

        //now we know where the icmp header start
        icmpD = (struct icmp *) (buff + SIZE_ETHERNET +iphlen);

        printf("~New ICMP packet~\n");
        printf("source: %s\n" , inet_ntoa(src.sin_addr));
        printf("Type: %u\n", icmpD->icmp_type);
        printf("Code: %d\n" ,icmpD->icmp_code);
        

     exit(0);

    }
    return 0;
}
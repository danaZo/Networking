#include <stdio.h>
#include <string.h>


#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>


	void send_ip_pkt();

int main()
{
	char buffer[1024];
	memset(buffer, 0, 1024);
	struct icmphdr *ic;
	struct iphdr *ip;
	
	
	//open a Raw socket
	int soc = socket(AF_INET,SOCK_RAW,IPPROTO_RAW);
	if(soc < 0){
	
		perror("creating problem: ");
	}
	
	
	int e = 0;
	int opt = 1;
	//setting to socket to promiscuous mode
	e = setsockopt(soc, IPPROTO_IP, IP_HDRINCL,&opt, sizeof(opt));
	if(e < 0){
	
		perror("setting problem: ");
	}
	


	//Create IP header with some valid and some arbitrary values	
	ip = (struct iphdr *)buffer;
	ip->version = 4;
	ip->saddr = inet_addr("1.1.1.1"); // Non-existing IP on the internet
	ip->daddr = inet_addr("10.0.2.6");
	ip->ihl = 5; // the length of a word on our machine is 4  and the basix size of ip header is 20 so :20/4 = 5
	ip->ttl = 10; 

	ip->protocol = IPPROTO_ICMP;
	ip->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));

	//Creating ICMP header
	struct icmphdr *icmp = (struct icmphdr *)(buffer + sizeof(struct iphdr));
	icmp->type = 8; 
	
	send_ip_pkt(ip , soc);
	
	


	close(soc);


return 0;
}


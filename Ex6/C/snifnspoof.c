#include <pcap.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/ether.h>
#include <netinet/tcp.h>

#include <arpa/inet.h>

#include <sys/socket.h>


#include <ctype.h>
#include <unistd.h>


void send_ip_pkt(struct iphdr *ip,int soc){
	struct sockaddr_in dest;
	
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr("10.0.2.6");
	sendto(soc, ip, ntohs(ip->tot_len), 0, (struct sockaddr *)&dest, sizeof(dest));


}

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    struct sockaddr_in src, dest, temp;
    memset(&dest, 0, sizeof(dest));
	memset(&src, 0, sizeof(src));

    printf("Got a packet\n");
    const struct iphdr *ip = (struct iphdr *)(packet + ETH_HLEN);
    if (ip->protocol == IPPROTO_ICMP)
    {
        
        src.sin_addr.s_addr = ip->saddr;
        dest.sin_addr.s_addr = ip->daddr;
		
		struct iphdr * new_ip;
		struct icmphdr * new_ic;
		int opt = 1;
		const char new_packet[1024];


        printf("Sniffed an ICMP echo request from %s\n", inet_ntoa(src.sin_addr));
        printf("to  %s\n", inet_ntoa(dest.sin_addr));
        

		printf("Spoofing a reply...\n");
        
		
		// cook a new packet and headers

		int ip_len = ip->ihl * 4;
		

		//zeroing the new_packet
		memset((char *)new_packet, 0, 1024);
		//copying the privous values to the new packet
		memcpy((char *)new_packet, ip, ntohs(ip->tot_len));

		new_ip = (struct iphdr *)new_packet;
		new_ic = (struct icmphdr *)(new_packet + ip_len);

		
		new_ip->saddr = ip->daddr;
		new_ip->daddr = ip->saddr;
		new_ip->ttl = 10;

		// returning an echo reply --> type = 0
		new_ic->type = 0; 

		
		int soc = socket(AF_INET,SOCK_RAW,IPPROTO_RAW);

		int e = setsockopt(soc, IPPROTO_IP, IP_HDRINCL,&opt, sizeof(opt));


		send_ip_pkt(new_ip,soc);


		close(soc);

    }
}




int main()
{	


	pcap_t *handle = NULL;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fp;
	char filter_exp[] = "icmp";
	bpf_u_int32 net;
	
	
	handle = pcap_open_live("br-425368701c37", BUFSIZ, 1, 1000, errbuf);
	if (handle ==NULL){

		perror("open live problem:" );
	}
	// Step 2: Compile filter_exp into BPF psuedo-code
	
	pcap_compile(handle, &fp, filter_exp, 0, net);
	if (pcap_setfilter(handle, &fp) !=0) {
		pcap_perror(handle, "Error:");
		exit(EXIT_FAILURE);
	}
	
	// Step 3: Capture packets
	
	pcap_loop(handle, -1, got_packet, NULL);
	pcap_close(handle); //Close the handle
	
	return 0;
	}
#include<pcap.h>
#include<stdio.h>
#include <ctype.h>
#include<stdlib.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ether.h>




void got_packet(u_char *args, const struct pcap_pkthdr *header,const u_char *packet)
{

	// to reach the data sections we have to skip the ethernet,IP, and tcp headers
	//the Ethernet header size is always 14 , we have to parse the rest from the packet
	const u_char *data;
	//getting the IP length
	
	struct iphdr* ip = (struct iphdr*) (packet + ETH_HLEN ); //14 being the ehternet header size
	int ip_len = ip->ihl *4; //4 is the size of a work on our machine
	
	struct tcphdr* tchd = (struct tcphdr*) (packet + ETH_HLEN  + ip_len);
	int tcp_len = tchd->doff * 4; 
	data = (u_char*) packet + ETH_HLEN + ip_len + tcp_len;
	
	int dataLen = ntohs(ip->tot_len) - (ip_len + tcp_len);



	if(dataLen > 0){
		
		printf("%s",data);
	}
	

}



int main()
{	
	//the filters we used in the assignment are:
	// "icmp && src host 10.9.0.6 && dst host 10.9.0.5"
	// portrange 10-100
	pcap_t *handle = NULL;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fp;
	char filter_exp[] = "src host 10.9.0.5 && dst host 10.9.0.6";
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
	// Note: don’t forget to add "-lpcap" to the co
	


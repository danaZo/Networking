#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


void send_file(FILE *f, int socket){

    char s[1024];
    size_t siz;
    while ((siz = fread(s, 1, sizeof s, f)) > 0){
       int t = send(socket, s, sizeof(s), 0);
            if(t < 0){
                perror("sending failed \n");
                exit(1);
            }
    }
    if (ferror(f)) {
        perror("Error: ");
    }
    printf("Message sent \n");
}

int main(){

    char buf[256];
    socklen_t len;


    FILE *f;
    char *filename = "50kb.txt";

    //create a socket
    int soc;
    
    
    
    struct sockaddr_in measure_addr;
    measure_addr.sin_family = AF_INET;
    measure_addr.sin_port = htons(6789);
    measure_addr.sin_addr.s_addr = INADDR_ANY;

    int con;
    float totalTime = 0;
    
    printf("CC algorithm: Cubic\n");
    //5 times in Cubic
    for(int i = 0 ; i <5 ; i++){
        soc = socket(AF_INET, SOCK_STREAM, 0);
        con = connect(soc, (struct  sockaddr*) &measure_addr, sizeof(measure_addr));

        if(con < 0 ){
            perror("connection failed");
        }
        else{
            printf("connection established successfuly\n");
        }
        
        f = fopen(filename, "r");
        if(!f){

            printf("~ file opening failed ~");
        }
        clock_t start = clock();
        send_file(f, soc);
        clock_t end = clock();
        totalTime += (float)(end - start);
        close(soc);
        fclose(f);
            
    }
    printf("Total sending time: %f seconds\n", totalTime/1000000);
    printf("Average sending time: %f seconds\n", totalTime/5000000);
    

    
    printf("Switched CC to: reno\n");
    
    // 5 times in reno
    for(int i = 0 ; i <5 ; i++){
        soc = socket(AF_INET, SOCK_STREAM, 0);
      	 strcpy(buf, "reno"); 
    	 len = strlen(buf);
   	 if (setsockopt(soc, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
	perror("setsockopt"); 
	return -1;
    }
        
        con = connect(soc, (struct  sockaddr*) &measure_addr, sizeof(measure_addr));
        

        if(con < 0 ){
            perror("connection failed");
        }
        else{
            printf("connection established successfuly\n");
        }
        
        f = fopen(filename, "r");
        if(!f){

            printf("~ file opening failed ~");
        }
        clock_t start = clock();
        send_file(f, soc);
        clock_t end = clock();
        totalTime += (float)(end - start);
        close(soc);
        fclose(f);
            
    }
    printf("Total sending time: %f seconds\n", totalTime/1000000);
    printf("Average sending time: %f seconds\n", totalTime/5000000);
    
    

}

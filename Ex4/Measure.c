#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>



int main(){

    struct timeval start, end;
    //create a socket   
    int soc, e;
    soc = socket(AF_INET, SOCK_STREAM, 0);
    if( soc < 0){
        printf("Socket creation failed\n");
        exit(1);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(6789);
    address.sin_addr.s_addr = INADDR_ANY;

    e = bind(soc, (struct sockaddr*) &address, sizeof(address) );
    if( e < 0){
        printf("Binding failed \n");
        exit(1);
    }

    char buf[256]; // for CC algorithm change


    int senderSoc;
    double totalTime = 0;



    //5 times in Cubic
    for(int i = 0 ; i < 5 ; i++){
        e = listen(soc, 2);
        if( e < 0){
            printf("Listening Failure \n");
        }

        senderSoc = accept(soc, NULL , NULL);
        if( senderSoc < 0){
        perror("Accepting Failure: ");
    }

        char buffer[1024];
        int KbytesRec = 0 ;
        gettimeofday(&start , NULL);
        while((e = recv(senderSoc, &buffer , sizeof(buffer) ,0) > 0)){
            
            if( e < 0){
                perror("Error: ");
          }
            KbytesRec += e;
        }
        gettimeofday(&end , NULL);
        
        printf("%d\n", KbytesRec);

        double timeTaken =((end.tv_sec *1000000 + end.tv_usec) -
             (start.tv_sec*1000000 + start.tv_usec)) ;
        // printf("Message recieved: %s \n", buffer);
        totalTime += timeTaken;
        bzero(buffer, 1024);
        close(senderSoc);
    } 

    printf("Total receiving time Cubic: %f seconds\n", totalTime/1000000);
    printf("Average receiving time Reno: %f seconds\n", totalTime/5000000.0);   

    close(soc);

    return 0;
}
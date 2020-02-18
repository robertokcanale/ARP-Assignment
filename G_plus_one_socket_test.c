#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define SIZE 256

void error(char *msg)
{
printf("%s\n",msg);
exit(0);
}



//little guide for this, most of it is already written.
// CORRECT the ARGV[i] indexes for :
//-the PORT NUMBER when executing this file!
//-the IP argument!
//-the close and write pipes when sending arguments

int main(int argc, char *argv[])
{
    printf("G_test process: starting execution.\n");
    printf("G+1: Portno =\n");
    //initializing my tokens
    char token_received[SIZE], token_to_send[SIZE];
    char* pend;



    //printf("G+1: Portno = %f", token_received_float);


//creating SERVER socket
    //variables initialization.socket
    int sockfd, portno, newsockfd, clilen, n;
    struct sockaddr_in serv_addr, cli_addr; // Internet addresses are here!
    struct hostent *server;
    portno = 8080;
    printf("G+1: Portno = %d\n", portno);

    //my port number is argv[10]


    //new socket created here
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        error((char*)"ERROR opening socket\n");
    }

    //initialize socket
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);


    //binding socket to adress
        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
            error((char*)"ERROR on binding\n");
        }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,  (socklen_t*)&clilen);
    if (newsockfd < 0){
         error((char*)"ERROR on accepting the client\n");
    }

    //while(1) actually should use a while

    for (int i = 0; i <50; i++){

    //read from Socket
    n = read(newsockfd, &token_received, sizeof(token_received));
    if (n < 0)
    error((char*)"ERROR reading from socket\n");
    printf("%s\n", token_received);

    //token_received_float = strtof(token_received, &pend);
    //printf("Float received on Socket: %f", token_received_float);

    //write on PIPE (which pipe? user who receives this should properly configure argv numbers

    }
    return 0;
}

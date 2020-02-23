#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define SIZE 256

void error(char *msg)
{
printf("%s\n",msg);
exit(0);
}


int main(int argc, char *argv[]){
    printf("G_test process: starting execution.\n");

//creating SERVER socket
    //variables initialization.socket
    int sockfd, portno, newsockfd, clilen, n;
    struct sockaddr_in serv_addr, cli_addr; // Internet addresses are here!
    struct hostent *server;
    portno = 3050; //modify accordingly if needed
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

    //initializing my tokens
    char token_buffer[SIZE];

    while(1) {

    //read from Socket
    n = read(newsockfd, &token_buffer, sizeof(token_buffer));
    if (n < 0)
    error((char*)"ERROR reading from socket\n");

    write(atoi(argv[2]), &token_buffer, sizeof(token_buffer));

    sleep(2);

    }

}

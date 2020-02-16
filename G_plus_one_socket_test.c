#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include "functions.h"

void error(char *msg)
{
perror(msg);
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

//initializing my tokens
float token_received, token_to_send;
token_received =0;
token_to_send = 0;

//Creating my CLIENT here
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
char buffer[256];

    //my port number is argv[10]
    portno = atoi(argv[10]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        error((char*)"ERROR opening socket");
    }

    //idetifying the server (on the same machine)
    server = gethostbyname(argv[7]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    //variables initialization
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);


    //Trying to connect to the server!
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        error((char*)"ERROR connecting");
    }
    //now that the connection is estabilshed, we can keep receivig datafrom the server through the socket!

    while(1){
    //read from Socket
    n = read(sockfd, &token_received, sizeof(token_received));
    if (n < 0)
    error((char*)"ERROR reading from socket");
    printf("%s\n",buffer);

    //write on PIPE (which pipe? user who receives this should properly configure argv numbers

    token_to_send = token_received;

    close(atoi(argv[1]));

    write(atoi(argv[2]), &token_to_send, sizeof(token_to_send));

    close(atoi(argv[2]));

    //Write a PIPE to the pass tokens received to the next P
    }
    }

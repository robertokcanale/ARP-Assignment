#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 7878
#define SIZE 256

//server
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char*argv[]){

    close(atoi(argv[1]));

    //acts as the server and will receive tokens
    //from the previous machine via socket and
    //dispatch them to P via pipe
    int g_sockfd, g_newsockfd, g_clilen, g_n;
    char g_buffer[SIZE];
    struct sockaddr_in g_serv_addr, g_cli_addr;

    //create new socket
    g_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (g_sockfd < 0){
        error("ERROR opening socket");
    }

    //set values in buffer to 0
    bzero((char *) &g_serv_addr, sizeof(g_serv_addr));
    g_serv_addr.sin_family = AF_INET;
    g_serv_addr.sin_port = htons(PORT);
    g_serv_addr.sin_addr.s_addr = INADDR_ANY;

    //bind socket to address
    if (bind(g_sockfd, (struct sockaddr *) &g_serv_addr, sizeof(g_serv_addr)) < 0){
        error("ERROR on binding");
    }

    listen(g_sockfd,5);

    g_clilen = sizeof(g_cli_addr);
    g_newsockfd = accept(g_sockfd, (struct sockaddr *) &g_cli_addr, (socklen_t*)&g_clilen);
    if (g_newsockfd < 0){
        error("ERROR on accept");
    } else{
      printf("G_justin: I have Connected!\n");
    }


    bzero(g_buffer,SIZE);
    //reading token from client through socket


    //initialize and send token
    strncpy(g_buffer,"0", SIZE);
    g_n = write(atoi(argv[2]), &g_buffer, sizeof(g_buffer));
    if (g_n < 0){
        error("ERROR writing to pipe");
    }
    while (1){
        //read token from socket
        g_n = read(g_newsockfd, &g_buffer, sizeof(g_buffer));
        if (g_n < 0){
            error("ERROR reading from socket");
        }
        printf("G: is sending %s\n", g_buffer );

        //send token to P via pipe
        g_n = write(atoi(argv[2]), &g_buffer, sizeof(g_buffer));
        if (g_n < 0){
            error("ERROR writing to pipe");
        }


        memset(g_buffer, 0, SIZE);
        sleep(1);
    }

    return 0;
}

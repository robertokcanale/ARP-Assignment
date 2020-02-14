#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <time.h>
#include "functions.h"

int main(int argc, char *argv[]){

//variables initialization.general
    fd_set file_descriptor_select;
    int signal_received, my_select;
    float token_received, token_to_send, DT;  //I initialize R_frequency as a global variable in Lanucher (maybe better in header?
    int fd1 = atoi(argv[1]); //reads the two  file descriptors of pipe1 and 2
    int fd2 = atoi(argv[3]); //these are for reading pipe1 and pipe2
    time_t t_received, t_sent;

   token_received = 0;
   token_to_send = 0;

//creating SERVER socket
    //variables initialization.socket
    int sockfd, portno, newsockfd, clilen, n;
    struct sockaddr_in serv_addr, cli_addr; // Internet addresses are here!
    struct hostent *server;
    char buffer[256]; //server reads charachters in this buffer

    //my port number is argv[10]
    portno = atoi(argv[10]);

    //new socket created here
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        error("ERROR opening socket");
    }

    //setting the buffer to 0, initializing the server socket
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //address family
    serv_addr.sin_port = htons(portno); //contains port number
    serv_addr.sin_addr.s_addr = INADDR_ANY; //contains host IP

    //binding socket to adress
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
            error("ERROR on binding");
        }

    while(1){

    //Stuff needed for the select
    FD_ZERO(&file_descriptor_select); //initialize file_descriptor_select to 0
    FD_SET(fd1, &file_descriptor_select); //Sets the bit for the file descriptor fd1 in the file descriptor set file_descriptor_select
    FD_SET(fd2, &file_descriptor_select); //Sets the bit for the file descriptor fd2 in the file descriptor set file_descriptor_select

    my_select = select(3, &file_descriptor_select, NULL, NULL, NULL); //3 because REASONS; check select libriary description

    if(my_select == 2){//if there are two pipes available, reads the first

            //read PIPE1 from S and check the message it received!
           //close the write, read the integer sent by console
            close(atoi(argv[2]));

            read(fd1, &signal_received, sizeof(signal_received));

            close(fd1);


        if(signal_received == 1){ //Start Receiving Tokens and sending them!

            //write on the log
            Write_Log("Receved token, seding it.\n");
            printf("\nReceiving and sending tokens tokens.\n");

            //receive Tokens from G-1 over PIPE
            close(atoi(argv[4]));

            read(fd2, &token_received, sizeof(token_received));

            t_received = time(NULL);

            close(fd2);

            //interface here to SEND TOKENS OVER THE SOCKET

            // listen for socket connections (i.e. if any client is connecting
            listen(sockfd,5);

            // a new connection can arrive from a client, handled here, and checked if it is accepted
            clilen = sizeof(cli_addr);
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
                if (newsockfd < 0){
                error("ERROR on accepting the client");
                }

            //COMPUTING Token to Send
            t_sent = time(NULL);
            DT = difftime( t_sent, t_received);
            token_to_send = New_Token(token_received, DT, R_Frequency); //smt here, it's the result of New_Token() operation

            //writing on socket
            n = write(sockfd, &token_to_send, sizeof(token_to_send));//write on socket
            if (n < 0)//check on writing
                {
                    error("\nERROR writing to socket.");
                }


        } else if(signal_received == 0){ //Stop receiving tokens and sending them

            Write_Log("Stop receiving and sending tokens.\n");
            printf("\nStopped Receiving and Sending tokens.\n");

        } else {
            printf("\nSomething went really wrong.\n");
        }


    } else {
        printf("Something wrong with the reading the pipes and select");
    }


    //sending data to the L pipe outside of the if but inside the while
    //initialize the message
    struct message_L message_to_send;

    message_to_send.received_token = token_received;

    //strncpy(message_to_send.from); I'll have to revise this

    message_to_send.sent_token = token_to_send;
    //write on  pipe3

    close(atoi(argv[5]));

    write(atoi(argv[6]), &message_to_send, sizeof(message_to_send));

    close(atoi(argv[6]));

    }

return 0;

}

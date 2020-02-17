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


#define SIZE 256

float New_Token(float received_token, float time_delay, float RF );

void error(char *msg);

void Write_Log(char string[50]);


int main(int argc, char *argv[]){

printf("P process: starting execution.\n");

//variables initialization.general
    fd_set file_descriptor_select;
    int  signal_received_int, my_select, max_fd, res;
    float token_received_float, token_to_send, DT, R_Frequency;  //I initialize R_frequency as a global variable in Lanucher (maybe better in header?
    int fd1 = atoi(argv[1]); //reads the two  file descriptors of pipe1 and 2
    int fd2 = atoi(argv[3]); //these are for reading pipe1 and pipe2
    time_t t_received, t_sent;

    //buffers for my select pipes
    char signal_received[SIZE];
    char token_received[SIZE];

    R_Frequency = atof(argv[14]); //giving a value to my RF

    token_received_float = 0;
    token_to_send = 0;

//creating SERVER socket
    //variables initialization.socket
    int sockfd, portno, newsockfd, clilen, n;
    struct sockaddr_in serv_addr, cli_addr; // Internet addresses are here!
    struct hostent *server;


    //my port number is argv[10]
    portno = atoi(argv[10]);
    //initialize socket
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);


    //new socket created here
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        error((char*)"ERROR opening socket");
    }

    //setting the buffer to 0, initializing the server socket
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //address family
    serv_addr.sin_port = htons(portno); //contains port number
    serv_addr.sin_addr.s_addr = INADDR_ANY; //contains host IP

    //binding socket to adress
        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
            error((char*)"ERROR on binding");
        }



    while(1){
    //Stuff needed for the select
    FD_ZERO(&file_descriptor_select); //initialize file_descriptor_select to 0
    FD_SET(fd1, &file_descriptor_select); //Sets the bit for the file descriptor fd1 in the file descriptor set file_descriptor_select
    FD_SET(fd2, &file_descriptor_select); //Sets the bit for the file descriptor fd2 in the file descriptor set file_descriptor_select

    max_fd = fd1 >fd2 ? fd1 : fd2;


    my_select = select(max_fd+1, &file_descriptor_select, NULL, NULL, NULL);

    if (my_select == -1) {error((char*)"Error with select()");}


    if (FD_ISSET(fd1, &file_descriptor_select))
    {
        // We can read from fd1
        res = read(fd1, &signal_received, sizeof(signal_received));

        //convert from char buffer to Int
        sscanf(signal_received, "%d", &signal_received_int);

        if (res > 0)
        {
            printf("Read %d bytes from Pipe1: %d\n", res, signal_received_int);
        }
    }
    if (FD_ISSET(fd2, &file_descriptor_select))
    {
        // We can read from fd2
        res = read(fd2, &token_received, sizeof(token_received));

        //convert from char buffer to float
        token_received_float = (float)atof(token_received);

        t_received = time(NULL);

        if (res > 0)
        {
            printf("Read %d bytes from Pipe2: %f\n", res, token_received_float);
        }
      }

    //printf("P gets here. signal_receive_int = %d, token_received: %f\n", signal_received_int,token_received_float);

    //now perform operations according to the received signal
    if(signal_received_int == 1){ //Start Receiving Tokens and sending them!

                //write on the log
                Write_Log((char*)"P: Received token, seding it.\n");
                printf("\nP: Receiving and sending tokens. Token Received: %f.\n", token_received_float);

                //interface here to SEND TOKENS OVER THE SOCKET

                // listen for socket connections (i.e. if any client is connecting)
                listen(sockfd,5);

                // a new connection can arrive from a client, handled here, and checked if it is accepted
                clilen = sizeof(cli_addr);
                newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,  (socklen_t*)&clilen);
                if (newsockfd < 0){
                error((char*)"ERROR on accepting the client");
                }

                //COMPUTING Token to Send
                t_sent = time(NULL);
                DT = difftime( t_sent, t_received);
                token_to_send = New_Token(token_received_float, DT, R_Frequency); //smt here, it's the result of New_Token() operation

                //writing on socket
                n = write(sockfd, &token_to_send, sizeof(token_to_send));//write on socket
                if (n < 0)//check on writing
                    {
                        error((char*)"\nERROR writing to socket.");
                    }

            } else if(signal_received_int == 0){ //Stop receiving tokens and sending them

                Write_Log((char*)"P: Stop receiving and sending tokens.\n");
                //printf("\nP: Stop receiving and sending tokens.\n");

            } else {

            printf("\nSomething went really wrong.\n");
                }

    //sending data to the L pipe outside of the if but inside the while

    //write on  pipe3.1

    close(atoi(argv[5]));

    write(atoi(argv[6]), &token_received, sizeof(token_received));

    close(atoi(argv[6]));

    //write on  pipe3.2

    close(atoi(argv[12]));

    write(atoi(argv[13]), &token_to_send, sizeof(token_to_send));

    close(atoi(argv[13]));

    }// end of while
return 0;

} // end of main


float New_Token(float received_token, float time_delay, float RF ){
float new_token;

    new_token = received_token + time_delay*(1 - pow(received_token,2.0)/2)*2*M_PI*RF;

    return new_token;
}


void error(char *msg)
{
perror(msg);
exit(0);
}

void Write_Log(char string[50])
{
    FILE * f;
    time_t t = time(NULL);
    struct tm * tm = localtime(&t);

    f = fopen("Log_File.log", "a+");

    if (f == NULL)
    {
        printf("Cannot open file\n");
        exit(0);
    }
    fprintf(f, "\nP: <%s> %s\n", asctime(tm), string );
    fclose(f);
}


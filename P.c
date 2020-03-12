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
#include <arpa/inet.h>


#define SIZE 256

float New_Token(float received_token, float time_delay, float RF );

void error(char *msg);

void Write_Log(char string[50]);


int main(int argc, char *argv[])
{

    printf("P process: starting execution.\n");

//variables initialization.general
    fd_set file_descriptor_select;
    int  signal_received_int, my_select, max_fd, res, res1, res2, res3, res4;
    float token_received_float, token_to_send_float, DT, R_Frequency;  //I initialize R_frequency as a global variable in Lanucher (maybe better in header?
    int fd1 = atoi(argv[1]); //reads the two  file descriptors of pipe1 and 2
    int fd2 = atoi(argv[3]); //these are for reading pipe1 and pipe2
    //stuff for the time
    struct timespec time_received; //define timespec used to compute the time
    struct timespec time_sent;
    float ns_received, ns_sent;
    bool unconnected, cont_while;
    //unconnected = true;
    //cont_while = true;

    //buffers for my select pipes
    char signal_received[SIZE];
    char token_received[SIZE];
    char token_to_send[SIZE];
    char L_buffer_received[SIZE];
    char L_buffer_sent[SIZE];

    R_Frequency = atof(argv[9]); //giving a value to my RF
    signal_received_int= 1;
    token_received_float = 1.0f;
    token_to_send_float = 0.0f;


    //Creating my CLIENT here
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;

    //my port number is argv[10]
    portno = atoi(argv[8]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("P: Next IP %s, my port number %d\n", argv[7], portno);

    if (sockfd < 0)
    {
        error((char*)"ERROR opening socket\n");
    }


    //variables initialization
    bzero((char *) &serv_addr, sizeof(serv_addr));

    //idetifying the server (on the same machine)
    if(inet_pton(AF_INET, argv[7], &serv_addr.sin_addr)<=0)
    {
        error((char*)"Invalid addrs:Address not supported\n");
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);


    //Trying to connect to the server!


    do {
            if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
            } else {
                unconnected = false;
                printf("I have connected!\n");
            }
            if (!unconnected){
                cont_while = false;
            }
        } while (cont_while);

    //now that the connection is estabilshed, we can keep receivig datafrom the server through the socket!
    sprintf(token_to_send, "%.10f", token_to_send_float);
    n = write(sockfd, &token_to_send, SIZE);



    while(1)
    {
        //Stuff needed for the select
        FD_ZERO(&file_descriptor_select); //initialize file_descriptor_select to 0
        FD_SET(fd1, &file_descriptor_select); //Sets the bit for the file descriptor fd1 in the file descriptor set file_descriptor_select
        FD_SET(fd2, &file_descriptor_select); //Sets the bit for the file descriptor fd2 in the file descriptor set file_descriptor_select


        max_fd = fd1 >fd2 ? fd1 : fd2;


        my_select = select(max_fd+1, &file_descriptor_select, NULL, NULL, NULL);

        if (my_select == -1)
        {
            error((char*)"Error with select()\n");
        }

      if(FD_ISSET(fd1, &file_descriptor_select) && (FD_ISSET(fd2, &file_descriptor_select) && (signal_received_int == 1))){

            // We can read from fd1
            res1 = read(fd1, &signal_received, sizeof(signal_received));

            //convert from char buffer to Int
            sscanf(signal_received, "%d", &signal_received_int);

            // We can read from fd2
            res2 = read(fd2, &token_received, sizeof(token_received));

            //convert from char buffer to float
            token_received_float = atof(token_received);

            clock_gettime (CLOCK_REALTIME, &time_received);
            ns_received = (float)time_received.tv_sec + (float)time_received.tv_nsec;


        }
        else if (FD_ISSET(fd1, &file_descriptor_select))
        {
            // We can read from fd1
            res = read(fd1, &signal_received, sizeof(signal_received));

            //convert from char buffer to Int
            sscanf(signal_received, "%d", &signal_received_int);

        } else if (FD_ISSET(fd2, &file_descriptor_select) && (signal_received_int == 1))
        {
            // We can read from fd2
            res = read(fd2, &token_received, sizeof(token_received));

            //convert from char buffer to float
            token_received_float = strtof(token_received,NULL);
            printf("RECEIVED: %s, %f\n",token_received, token_received_float);

            clock_gettime (CLOCK_REALTIME, &time_received);
            ns_received = (float)time_received.tv_sec + (float)time_received.tv_nsec;

        }

        //now perform operations according to the received signal
        if(signal_received_int == 1)  //Start Receiving Tokens and sending them!
        {
            //Send Token Received to L
            strcpy(L_buffer_received, "L: Token Received:");
            strcat(L_buffer_received, token_received);

            write(atoi(argv[6]), &L_buffer_received, sizeof(L_buffer_received));


            //interface here to SEND TOKENS OVER THE SOCKET

            //COMPUTING Token to Send

            clock_gettime (CLOCK_REALTIME, &time_sent);
            ns_sent = (float)time_sent.tv_sec + (float)time_sent.tv_nsec;
            DT = (ns_sent - ns_received)/1000000; //getting the divided by 1000000 to have non-exploding results
            //printf("\nTime Received: %f.\nTime Sent: %f\nDT: %f\n", ns_received, ns_sent, DT);


            token_to_send_float = New_Token(token_received_float, DT, R_Frequency);

            sprintf(token_to_send, "%.10f", token_to_send_float);

            printf("\nReceiving and sending tokens.\nToken Received: %f.\nToken Sent: %s\n", token_received_float, token_to_send);

            //writing on socket
            n = write(sockfd, &token_to_send, SIZE);//write on socket
            if (n < 0)//check on writing
            {
                error((char*)"\nERROR writing to socket.\n");
            }


            strcpy(L_buffer_sent, "L: Token Sent:");
            strcat(L_buffer_sent, token_to_send);

            write(atoi(argv[6]), &L_buffer_sent, sizeof(L_buffer_sent));
            memset(L_buffer_received, 0, SIZE);
            memset(L_buffer_sent, 0, SIZE);
            memset(token_received, 0, SIZE);

        }
        else if(signal_received_int == 0)    //Stop receiving tokens and sending them
        {
            memset(token_received, 0, SIZE);
            Write_Log((char*)"Stop receiving and sending tokens.\n");

        }
        else
        {
            printf("\nSomething went really wrong.\n");
        }


        //memset(token_to_send, 0, SIZE);

        sleep(1);
    }// end of while
    return 0;

} // end of main


float New_Token(float received_token, float time_delay, float RF )
{
    float new_token;

    new_token = received_token + time_delay*(1.0 - pow(received_token,2.0)/2)*2*M_PI*RF;

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
    fprintf(f, "\n-%s\nP: %s\n", asctime(tm), string);
    fclose(f);
}

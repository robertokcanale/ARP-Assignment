#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/select.h>

#define SIZE 256

void Write_Log_Received(float received_token);

void Write_Log_Sent(float sent_token);

void error(char *msg);


int main(int argc, char *argv[])
{
    FILE * f;
    f = fopen("Log_File.log", "w");
    fclose(f);

    printf("L process: starting execution.\n");

    float token_received_float;  //array to store received token
    float token_sent_float; //array to store sent token
    char  token_received[SIZE];
    char  token_sent[SIZE];
    token_received_float = 0.0f;
    token_sent_float = 0.0f;

    fd_set file_descriptor_select;
    int max_fd, my_select;
    int fd1 = atoi(argv[1]); //reads the two  file descriptors of pipe1 and 2
    int fd2 = atoi(argv[3]);


    while(1)
    {
        FD_ZERO(&file_descriptor_select); //initialize file_descriptor_select to 0
        FD_SET(fd1, &file_descriptor_select); //Sets the bit for the file descriptor fd1 in the file descriptor set file_descriptor_select
        FD_SET(fd2, &file_descriptor_select); //Sets the bit for the file descriptor fd2 in the file descriptor set file_descriptor_select


        max_fd = fd1 >fd2 ? fd1 : fd2;


        my_select = select(max_fd+1, &file_descriptor_select, NULL, NULL, NULL);

        if (my_select == -1)
        {
            error((char*)"Error with select()\n");
        }

        if (FD_ISSET(fd1, &file_descriptor_select))
        {
            // We can read from fd1
            //read  pipe3.1

            read(fd1, &token_received, sizeof(token_received));  //reading from pipe 3.1
            token_received_float = atof(token_received);

            printf("L: Token Received: %f\n", token_received_float);

        }
        else if (FD_ISSET(fd2, &file_descriptor_select))
        {
            // We can read from fd2
            //read  pipe3.2

            read(fd2, &token_sent, sizeof(token_sent));  //reading from pipe 3.1
            token_sent_float = atof(token_sent);

            printf("L: Token Sent: %f\n", token_sent_float);

        }



        //calling my functions and writing on my token
        Write_Log_Received(token_received_float);

        Write_Log_Sent(token_sent_float);

        memset(token_received, 0, SIZE);
        memset(token_sent, 0, SIZE);


        sleep(2);


    }

    return(0);
}

void error(char *msg)
{
    perror(msg);
    exit(0);
}



void Write_Log_Received(float received_token)
{
    FILE * f;
    time_t t = time(NULL);
    struct tm * tm = localtime(&t);

    f = fopen("Log_File.log", "a");

    if (f == NULL)
    {
        printf("Cannot open file. \n");
        exit(0);
    }

    fprintf(f, "\n<%s> <L: Received:> <%f>", asctime(tm), received_token );
    fclose(f);
}

//writes the the sent token on the logfile
void Write_Log_Sent(float sent_token)
{
    FILE * f;
    time_t t = time(NULL);
    struct tm * tm = localtime(&t);

    f = fopen("Log_File.log", "a");

    if (f == NULL)
    {
        printf("Cannot open file. \n");
        exit(0);
    }

    fprintf(f, "\n<%s> <L: Token Sent:> <%f>", asctime(tm), sent_token );
    fclose(f);
}







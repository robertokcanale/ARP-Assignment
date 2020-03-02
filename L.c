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

void Write_Log(char string[50]);

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

    close(atoi(argv[2]));


    while(1)
    {


        read(atoi(argv[1]), &token_received, sizeof(token_received));  //reading from pipe 3.1


        //printf("%s\n", token_received);

        //calling my functions and writing on my token
        Write_Log(token_received);


        memset(token_received, 0, SIZE);


        //sleep(2);


    }

    return(0);
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
    fprintf(f, "\n -%s\n %s\n", asctime(tm), string );
    fclose(f);
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







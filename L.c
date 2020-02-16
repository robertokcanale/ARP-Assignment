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
#include "functions.h"

void Write_Log_Received(float received_token);

void Write_Log_Sent(float sent_token);


int main(int argc, char *argv[])
{

  printf("L process: starting execution.\n");

  struct message_L message;
  char from[5];
  float token_received  = 0;  //array to store received token
  float token_sent = 0; //array to store sent token
  token_received  = 0;
  token_sent = 0;



  sleep(2);

  //read  pipe3.1
  close(atoi(argv[6]));

  read(atoi(argv[5]), &token_received, sizeof(token_received));  //reading from pipe 3.1

  close(atoi(argv[5]));

  //read  pipe3.2
  close(atoi(argv[13]));

  read(atoi(argv[12]), &token_sent, sizeof(token_sent));  //reading from pipe 3.1

  close(atoi(argv[12]));

  //calling my functions and writing on my token
  Write_Log_Received(token_received);

  Write_Log_Sent(token_sent);


  return(0);
}


void Write_Log_Received(float received_token)
{
    FILE * f;
    time_t t = time(NULL);
    struct tm * tm = localtime(&t);

    f = fopen("Log_File.log", "a+");

    if (f == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    fprintf(f, "\n<%s> <Received:> <%f>", asctime(tm), received_token );
    fclose(f);
}

//writes the the sent token on the logfile
void Write_Log_Sent(float sent_token)
{
    FILE * f;
    time_t t = time(NULL);
    struct tm * tm = localtime(&t);

    f = fopen("Log_File.log", "a+");

    if (f == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    fprintf(f, "\n<%s> <Token Sent:> <%f>", asctime(tm), sent_token );
    fclose(f);
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "functions.h"

SIZE = 100;

int main(int argc, char *argv[])
{


  // maybe neeeedi dont know printf("\nThis is childL with PID %d and my parent is %d\n",getpid(),getppid());

  struct message_L message;
  char from[5];
  float token_received  = 0;  //array to store received token
  float token_sent = 0; //array to store sent token
  token_received  = 0;
  token_sent = 0;

  logFile("L process: starting execution.\n");

  sleep(2);

  //process L reads data from process G from pipe
  logFile("L process: Reading data on pipe3 from P.\n");

  //read  3.1 for char
  close(atoi(argv[6]));

  read(atoi(argv[5]), message, sizeof(message));  //reading from pipe 3.1

  close(atoi(argv[5]));

  //copying my received inputs to my my loval variables.
  token_received = message.received_token;
  token_sent = message.sent_token;
  strncpy(from, message.from, 5);

  //calling my functions and writing on my token
  Write_Log_Received(from, token_received);

  Write_Log_Sent(token_sent);


  return(0);
}


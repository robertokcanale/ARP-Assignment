#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/select.h>

#define SIZE 256
int pipe1[2];


void Print_Log();
void Write_Log(char string[50]);

void Sig_Handler1(int signo){

    char Signal_select[SIZE];


    printf("received SIGUSR1: Start Sending Tokens.\n");
    Write_Log((char*)"Start Receiving and Sending Tokens.\n");

    strcpy(Signal_select, "1");

    //write 1 on  pipe1
    close(pipe1[0]);

    write(pipe1[1], &Signal_select, sizeof(Signal_select));

    close(pipe1[1]);

}



void Sig_Handler2(int signo){

    char Signal_select[SIZE];

    printf("received SIGUSR2: Stop Sending Tokens.\n");
    Write_Log((char*)"Stop Receiving Tokens.\n");

    strcpy(Signal_select, "0");

    //write on 0  pipe1
    close(pipe1[0]);

    write(pipe1[1], &Signal_select, sizeof(Signal_select));

    close(pipe1[1]);

 }

    void Sig_Handler3(int signo){

    //Print the LOG

    printf("received SIGINT: Printing Log.\n");
    Write_Log((char*)"Print Log.\n");
    Print_Log(); // Prints out the Log File
}

int main(int argc, char *argv[]){

printf("S process: starting execution.\n");

int console_signal;
pipe1[0] = atoi(argv[1]);
pipe1[0] = atoi(argv[2]);

Write_Log((char*)"S process: waiting for the user to Select input\n");
printf("\nThere are 3 available inputs.\n");
printf("\nSIGUSR1 - Start Receiving Tokens \nSIGUSR2 - Stop Receiving Tokens \nSIGINT - Output Log File\n");
printf("\nIn another terminal, please write \nkillall  (SIGUSR1 or SIGUSR2 or SIGINT) S\n");
printf("\nPlease select SIGUSR1, SIGUSR2 or SIGINT:\n");

if (signal(SIGUSR1, Sig_Handler1) == SIG_ERR){
printf("\ncan't catch SIGUSR1\n");
}

if (signal(SIGUSR2, Sig_Handler2) == SIG_ERR){
printf("\ncan't catch SIGUSR2\n");
}

if (signal(SIGINT, Sig_Handler3) == SIG_ERR){
printf("\ncan't catch SIGINT\n");int selection = 0;
}

while(1){
    }
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
    fprintf(f, "\nS: <%s> %s\n", asctime(tm), string );
    fclose(f);
}

//prints the content of the Log
void Print_Log(){

    FILE * f;
    char c;

    f = fopen("Log_File.log", "r");
    if (f == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    // Read contents from file
    c = fgetc(f);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(f);
    }

    fclose(f);

}

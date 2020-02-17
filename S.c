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

void Print_Log();
void Write_Log(char string[50]);



int main(int argc, char *argv[]){

printf("S process: starting execution.\n");

int selection = 0;
char Signal_select[SIZE];

Write_Log((char*)"S process: waiting for the user to Select input\n");
printf("\nThere are 3 available inputs.\n");
printf("\n1 - Start Receiving Tokens \n2 - Stop Receiving Tokens \n3 - Output Log File");
printf("\nPlease select 1, 2 or 3:\n");
while(1)
    {

        scanf("%d", &selection);//take the input


        if(selection == 1){//Start Receiving Tokens

            strcpy(Signal_select, "1");
            //write on  pipe1
            close(atoi(argv[1]));

            write(atoi(argv[2]), &Signal_select, sizeof(Signal_select));

            close(atoi(argv[2]));

            printf("\nStart, Signal_select = %s\n", Signal_select);

            Write_Log((char*)"Start Receiving and Sending Tokens.\n");

        } else if(selection == 2 ){ //Stop Receiving Tokens

            strcpy(Signal_select, "0");
            //write on  pipe1
            close(atoi(argv[1]));

            write(atoi(argv[2]), &Signal_select, sizeof(Signal_select));

            close(atoi(argv[2]));

            printf("\nStop, Signal_select = %s\n", Signal_select);

            Write_Log((char*)"Stop Receiving Tokens.\n");

        } else if( selection == 3 ){ //Dump log File

            Write_Log((char*)"Print Log.\n");
            Print_Log(); // Prints out the Log File

        } else {
            Write_Log((char*)"S process: User inserted invalid choice.\n");
            printf("\nInvalid choice.\n");//user inserted invalid process number
        }

    sleep(5);
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/select.h>
#include "functions.h"


int main(int argc, char *argv[]){


while(1)
    {
        int selection, datum;

        Write_Log("S process: waiting for the user to Select input\n");

        printf("\nThere are 3 available inputs.\n");
        printf("\n1 - Start Receiving Tokens \n2 - Stop Receiving Tokens \n3 -Output Log File");
        printf("\nPlease select 1, 2 or 3:\n");
        scanf("%d", &selection);//take the input


        if(selection == 1){//Start Receiving Tokens

            datum = 1;
            //write on  pipe1
            close(atoi(argv[1]));

            write(atoi(argv[2]), datum, sizeof(datum));

            close(atoi(argv[2]));

        } else if(selection = 2 ){ //Stop Receiving Tokens

            datum = 0;
            //write on  pipe1
            close(atoi(argv[1]));

            write(atoi(argv[2]), datum, sizeof(datum));

            close(atoi(argv[2]));

        } else if(selection = 3 ){ //Dump log File

            Print_Log(); // Prints out the Log File

        } else {

            Write_Log("S process: User inserted invalid choice.\n");
            printf("\nInvalid choice.\n");//user inserted invalid process number
        }
    }
}

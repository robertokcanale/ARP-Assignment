#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/select.h>
#include "functions.h"


// IN REALITY THIS is a DUMMY Sending variables!
// I need to send tokens over pipe2

int main(int argc, char *argv[]){
        //initializing my tokens
    float token_to_send;
    token_to_send = 0;

    printf("G process: starting execution.\n");

    while(1){

    //just something random.
    token_to_send = token_to_send + 1;
    if(token_to_send >= 5 ){
        token_to_send = token_to_send / 8;
    }

    printf("Token to send: %f", token_to_send);

    //write on PIPE (which pipe? user who receives this should properly configure argv numbers

    close(atoi(argv[3]));

    write(atoi(argv[4]), &token_to_send, sizeof(token_to_send));

    close(atoi(argv[4]));

    sleep(2);

    }
}

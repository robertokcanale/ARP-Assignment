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

#define SIZE 256

// IN REALITY THIS is a DUMMY Sending variables!
// I need to send tokens over pipe2

void Write_Log_Sent(float sent_token);


int main(int argc, char *argv[])
{
    //initializing my tokens
    char token_buffer[SIZE];
    float token_to_send;
    token_to_send = 0.521652561;

    printf("G process: starting execution.\n");

    while(1)
    {

        //just something random.
        token_to_send = token_to_send + 1.2;
        if(token_to_send >= 10 )
        {
            token_to_send = token_to_send / 6;
        }

        sprintf(token_buffer, "%.10f", token_to_send);

        close(atoi(argv[1]));

        write(atoi(argv[2]), &token_buffer, sizeof(token_buffer));

        sleep(2);
    }

}

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

    fprintf(f, "\n<%s> <G: Token Sent:> <%f>", asctime(tm), sent_token );
    fclose(f);
}


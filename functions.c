#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define M_PI 3.14159265358979323846

#include "functions.h"


//writes a string on the logfile, and takes a timestamp of it, along with the received token
void Write_Log_Received(const char string[5], float received_token)
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

    fprintf(f, "\n<%s> <From %s:> <%f>", asctime(tm), string, received_token );
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

void Write_Log(char string[50])
{
    FILE * f;
    time_t t = time(NULL);
    struct tm * tm = localtime(&t);

    f = fopen("Ass4.log", "a+");
    fprintf(f, "\n[%s]%s", asctime(tm), string );
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

//computes the new token to be sent
float New_Token(float received_token, float time_delay, float RF ){
float new_token;

    new_token = received_token + time_delay*(1 - pow(received_token,2.0)/2)*2*M_PI*RF;

    return new_token;
}


void error(char *msg)
{
perror(msg);
exit(0);
}

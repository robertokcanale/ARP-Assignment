#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define M_PI 3.14159265358979323846

#include "functions.h"


//writes a string on the logfile, and takes a timestamp of it
void Write_Log_Received(const char string[10], float received_token)
{
    time_t t = time(NULL);
    struct tm * tm = localtime(&t);

    f = fopen("Log_File.log", "a+");
    fprintf(f, "\n<%s> <From %s:> <%f>", asctime(tm), string, received_token );
    fclose(f);
}

void Write_Log_Sent(float sent_token)
{
    time_t t = time(NULL);
    struct tm * tm = localtime(&t);

    f = fopen("Log_File.log", "a+");
    fprintf(f, "\n<%s> <Token Sent:> <%f>", asctime(tm), sent_token );
    fclose(f);
}


float New_Token(float received_token, float time_delay, float RF ){
    float new_token;

    new_token = received_token + time_delay*(1 - (received_token^2)/2)*2*M_PI*RF;

    return new_token;
}

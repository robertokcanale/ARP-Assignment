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

void reverse(char* str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char* res, int afterpoint);

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


   while(1){


    //read  pipe3.1

    read(atoi(argv[1]), &token_received, sizeof(token_received));  //reading from pipe 3.1

    printf((char*)"L: Token Received: %f\n", token_received_float);

    //read  pipe3.2

    read(atoi(argv[3]), &token_sent, sizeof(token_sent));  //reading from pipe 3.1

    printf((char*)"L: Token Sent: %f\n", token_sent_float);

    token_received_float = atof(token_received);
    token_sent_float = atof(token_sent);

    //calling my functions and writing on my token
    Write_Log_Received(token_received_float);

    Write_Log_Sent(token_sent_float);

    memset(token_received, 0, SIZE);
    sleep(2);

    }

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

    f = fopen("Log_File.log", "a+");

    if (f == NULL)
    {
        printf("Cannot open file. \n");
        exit(0);
    }

    fprintf(f, "\n<%s> <L: Token Sent:> <%f>", asctime(tm), sent_token );
    fclose(f);
}

// Reverses a string 'str' of length 'len'
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}


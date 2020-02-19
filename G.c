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

void ftoa(float n, char* res, int afterpoint) ;
int intToStr(int x, char str[], int d);
void reverse(char* str, int len);

int main(int argc, char *argv[]){
        //initializing my tokens
    char token_buffer[SIZE];
    float token_to_send;
    token_to_send = 1;

    printf("G process: starting execution.\n");

    while(1){

    //just something random.
    token_to_send = token_to_send + 1;
    if(token_to_send >= 5 ){
        token_to_send = token_to_send / 8;
    }
    ftoa(token_to_send, token_buffer, 5);

    //printf("Token to send: %s", token_buffer); //should work all good!

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

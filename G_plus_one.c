#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define SIZE 256

void error(char *msg)
{
printf("%s\n",msg);
exit(0);
}

void Write_Log_Sent(float sent_token);

void ftoa(float n, char* res, int afterpoint) ;
int intToStr(int x, char str[], int d);
void reverse(char* str, int len);

int main(int argc, char *argv[]){
    printf("G_test process: starting execution.\n");

//creating SERVER socket
    //variables initialization.socket
    int sockfd, portno, newsockfd, clilen, n;
    struct sockaddr_in serv_addr, cli_addr; // Internet addresses are here!
    struct hostent *server;
    portno = 9999;
    printf("G+1: Portno = %d\n", portno);

    //my port number is argv[10]


    //new socket created here
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        error((char*)"ERROR opening socket\n");
    }

    //initialize socket
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);


    //binding socket to adress
        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
            error((char*)"ERROR on binding\n");
        }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,  (socklen_t*)&clilen);
    if (newsockfd < 0){
         error((char*)"ERROR on accepting the client\n");
    }

    //initializing my tokens
    char token_buffer[SIZE];

    while(1) {

    //read from Socket
    n = read(newsockfd, &token_buffer, sizeof(token_buffer));
    if (n < 0)
    error((char*)"ERROR reading from socket\n");


    close(atoi(argv[1]));

    write(atoi(argv[2]), &token_buffer, sizeof(token_buffer));


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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h>


#define SIZE 256

float New_Token(float received_token, float time_delay, float RF );

void error(char *msg);

void Write_Log(char string[50]);

void ftoa(float n, char* res, int afterpoint) ;

int intToStr(int x, char str[], int d);

void reverse(char* str, int len);

int main(int argc, char *argv[]){

printf("P process: starting execution.\n");

//variables initialization.general
    fd_set file_descriptor_select;
    int  signal_received_int, my_select, max_fd, res;
    float token_received_float, token_to_send_float, DT, R_Frequency;  //I initialize R_frequency as a global variable in Lanucher (maybe better in header?
    int fd1 = atoi(argv[1]); //reads the two  file descriptors of pipe1 and 2
    int fd2 = atoi(argv[3]); //these are for reading pipe1 and pipe2
    time_t t_received, t_sent;

    //buffers for my select pipes
    char signal_received[SIZE];
    char token_received[SIZE];
    char token_to_send[SIZE];

    R_Frequency = atof(argv[14]); //giving a value to my RF
    signal_received_int= 1;
    token_received_float = 0.0f;
    token_to_send_float = 0.0f;


    //Creating my CLIENT here
int sockfd, portno, n;
struct sockaddr_in serv_addr;

    //my port number is argv[10]
    portno = atoi(argv[10]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        error((char*)"ERROR opening socket\n");
    }


    //variables initialization
    bzero((char *) &serv_addr, sizeof(serv_addr));

    //idetifying the server (on the same machine)
    if(inet_pton(AF_INET, argv[9], &serv_addr.sin_addr)<=0) {
        error((char*)"Invalid address:Address not supported\n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);


    //Trying to connect to the server!
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        error((char*)"ERROR connecting\n");
    }
    //now that the connection is estabilshed, we can keep receivig datafrom the server through the socket!




    while(1){

    //Stuff needed for the select
    FD_ZERO(&file_descriptor_select); //initialize file_descriptor_select to 0
    FD_SET(fd1, &file_descriptor_select); //Sets the bit for the file descriptor fd1 in the file descriptor set file_descriptor_select
    FD_SET(fd2, &file_descriptor_select); //Sets the bit for the file descriptor fd2 in the file descriptor set file_descriptor_select


    max_fd = fd1 >fd2 ? fd1 : fd2;


    my_select = select(max_fd+1, &file_descriptor_select, NULL, NULL, NULL);

    if (my_select == -1) {error((char*)"Error with select()\n");}


    if (FD_ISSET(fd1, &file_descriptor_select))
    {
        // We can read from fd1
        res = read(fd1, &signal_received, sizeof(signal_received));

        //convert from char buffer to Int
        sscanf(signal_received, "%d", &signal_received_int);

        if (res > 0)
        {
            printf("Read %d bytes from Pipe1: %d\n", res, signal_received_int);
        }
    }

    if (FD_ISSET(fd2, &file_descriptor_select) && (signal_received_int == 1))
    {
        // We can read from fd2
        res = read(fd2, &token_received, sizeof(token_received));

        //convert from char buffer to float
        token_received_float = atof(token_received);

        t_received = time(NULL);

        if (res > 0)
        {
            printf("Read %d bytes from Pipe2: %f\n", res, token_received_float);
        }
      }

    printf("P: signal_receive_int = %d\n", signal_received_int);

    //now perform operations according to the received signal
    if(signal_received_int == 1){ //Start Receiving Tokens and sending them!

                //write on the log
                Write_Log((char*)"P: Received token, seding it.\n");

                //interface here to SEND TOKENS OVER THE SOCKET

                //COMPUTING Token to Send
                t_sent = time(NULL);
                DT = difftime( t_sent, t_received);
                token_to_send_float = New_Token(token_received_float, DT, R_Frequency); //smt here, it's the result of New_Token() operation
                ftoa(token_to_send_float, token_to_send, 5);

                printf("\nP: Receiving and sending tokens.\nToken Received: %f.\nToken Sent: %f\n", token_received_float, token_to_send_float);

                //writing on socket
                n = write(sockfd, &token_to_send, SIZE);//write on socket
                if (n < 0)//check on writing
                    {
                        error((char*)"\nERROR writing to socket.\n");
                    }

            } else if(signal_received_int == 0){ //Stop receiving tokens and sending them

                Write_Log((char*)"P: Stop receiving and sending tokens.\n");
                printf("\nP: Stop receiving and sending tokens.\n");

            } else {
            printf("\nSomething went really wrong.\n");
                }

    //sending data to the L pipe outside

    //write on  pipe3.1

    close(atoi(argv[5]));

    write(atoi(argv[6]), &token_received, sizeof(token_received));


    //write on  pipe3.2

    close(atoi(argv[7]));

    write(atoi(argv[8]), &token_to_send, sizeof(token_to_send));

    memset(token_received, 0, SIZE);
    memset(token_to_send, 0, SIZE);

    sleep(2);
    }// end of while
return 0;

} // end of main


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
    fprintf(f, "\nP: <%s> %s\n", asctime(tm), string );
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

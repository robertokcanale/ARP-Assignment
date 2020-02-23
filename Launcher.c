#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>


//here is my Launcher function. We tailor it to our needs:
//-SELECT between G and S from P
//-PIPE from P to L
//-SOCKET from P to G+1
//-SIGNALS to S

// TO DO
// SOCKETS
// REVIEW G
// CHECK THE MSG SENT FROM P->L
// TEST IT MF
// (P -> L) here we do it the stupid way, as P is going to write on 3 pipes communicationg with L
//its going to send the RECEIVED VALUE, THE SENT VALUE and wether it received it from G-1 or S

//Hardcode your portnumber in G+1


int main()
{
    //reading data from the config file, honestly too lazy to make a function for it, DONT CHANGE CONFIGURATION FILE.
    FILE *config_file;
    config_file = fopen("configuration_file.txt", "r");

    char next_machine[20], my_portnumber[20], RF[20];  // I only need my next IP, as I set up P as client and G a server

    //ad hoc file reading for my function
    fscanf(config_file, "%s", next_machine);
    fscanf(config_file, "%s", my_portnumber);
    fscanf(config_file, "%s", RF);

    //Initializing the processes, their return status and the required Pipes
    pid_t processP;
    pid_t processL;
    pid_t processS;
    pid_t processG;

    int childReturnStatusP, childReturnStatusL, childReturnStatusS, childReturnStatusG; //Return variables for child processes

    int fd1[2];//pipe #1 (S -> P)
    int fd2[2];//pipe #2 (G-1 -> P)
    int fd3[2];//pipe #3.1 for char G or S for received token
    int fd4[2];//pipe #3.2 for char G or S for sent token

    //check condition on pipes creation
    if (pipe(fd1) < 0)
    {
        perror("Error! Pipe #1 failed.\n");
        return -1;
    }
    if (pipe(fd2) < 0)
    {
        perror("Error! Pipe #2 failed.\n");
        return -1;
    }
    if (pipe(fd3) < 0)
    {
        perror("Error! Pipe #3.1 failed.\n");
        return -1;
    }
    if (pipe(fd4) < 0)
    {
        perror("Error! Pipe #3. failed.\n");
        return -1;
    }



    //arrays containing data to pass to the exec
    char *argP[13];
    char *argG[5];
    char *argS[5];
    char *argL[6];

    char r_arg1[5]; //read pipe1
    char w_arg1[5]; //write pipe1
    char r_arg2[5]; //read pipe2
    char w_arg2[5]; //write pipe2
    char r_arg3[5]; //read pipe3.1 for received token
    char w_arg3[5]; //write pipe3.1 for received token
    char r_arg4[5]; //read pipe3.2 for sent token
    char w_arg4[5]; //write pipe3.2 for sent token


    //convert int filedes into char targ with sprintf
    sprintf(r_arg1, "%d", fd1[0]);
    sprintf(w_arg1, "%d", fd1[1]);
    sprintf(r_arg2, "%d", fd2[0]);
    sprintf(w_arg2, "%d", fd2[1]);
    sprintf(r_arg3, "%d", fd3[0]);
    sprintf(w_arg3, "%d", fd3[1]);
    sprintf(r_arg4, "%d", fd4[0]);
    sprintf(w_arg4, "%d", fd4[1]);


    //Arg for P
    argP[1] = r_arg1; //read, pipe1
    argP[2] = w_arg1; //write, pipe1
    argP[3] = r_arg2; //read, pipe2
    argP[4] = w_arg2; //write, pipe2
    argP[5] = r_arg3; //read, pipe3.1
    argP[6] = w_arg3; //write, pipe3.1
    argP[7] = r_arg4; //read, pipe3.2
    argP[8] = w_arg4; //write, pipe3.2
    argP[9] = next_machine; //next IP
    argP[10] = my_portnumber; //my receivng port
    argP[11] = RF; //the read RF
    argP[12] = NULL;

    //Arg for L
    argL[1] = r_arg3; //read, pipe3.1
    argL[2] = w_arg3; //write, pipe3.1
    argL[3] = r_arg4; //read, pipe3.2
    argL[4] = w_arg4; //read, pipe3.2
    argL[5] = NULL;

    //Arg for S
    argS[1] = r_arg1; //read, pipe1
    argS[2] = w_arg1; //write, pipe1
    argL[3] = NULL;

    //Arg for G
    argG[1] = r_arg2; //read, pipe2
    argG[2] = w_arg2; //write, pipe2
    argL[3] = NULL;

    //starting from P
    processP = fork();//Creates child process

    //Check if fork worked without issues
    if (processP < 0)
    {
        perror("Error! Fork returned a negative number for child1P");
        return -1;
    }
    if (processP == 0)
    {
        //printf("\nLauncher: process P");
        fflush(stdout);
        char *nameP; //executable file name
        nameP = (char *)"./P";
        argP[0] = nameP;
        execvp(nameP, argP); //Executing P
    }
    else
    {

        processL = fork();

        if (processL < 0)
        {
            perror("Error! Fork returned a negative number for child1L");
            return -1;
        }
        if (processL == 0)
        {
            //printf("\nLauncher: process L");
            fflush(stdout);
            char *nameL; //executable file name
            nameL = (char *)"./L";
            argL[0] = nameL; //add the process name to the parameters
            execvp(nameL, argL); //Executing L
        }
        else
        {
            processS = fork();

            if (processS < 0)
            {
                perror("Error! Fork returned a negative number for childS!");
                return -1;
            }
            if (processS == 0)
            {
                //printf("\nLauncher: process S");
                fflush(stdout);
                char *nameS; //executable file name
                nameS = (char *)"./S";
                argS[0] = nameS;
                execvp(nameS, argS); //Executing S
            }
            else  //This is the one I will receive from another student.
            {
                processG = fork();

                if (processG < 0)
                {
                    perror("Error! Fork returned a negative number for childG!");
                    return -1;
                }
                if (processG == 0)
                {
                    //printf("\nLauncher: process G");
                    fflush(stdout);
                    char *nameG; //executable file name
                    nameG = (char *)"./G";
                    argG[0] = nameG;
                    execvp(nameG, argG); //Executing G
                }
            }
        }


        //Waits for all the process to terminate and verifis it

        waitpid(processP, &childReturnStatusP, 0);
        waitpid(processL, &childReturnStatusL, 0);
        waitpid(processS, &childReturnStatusS, 0);
        waitpid(processG, &childReturnStatusG, 0);


        if (childReturnStatusP == 0) //verify child1 terminated without error
        {
            printf("\nThe childP process terminated normally\n");
        }
        if (childReturnStatusP == 1)
        {
            printf("\nThe childP process terminated with an error\n");
        }

        if (childReturnStatusL == 0) //verify child2 terminated without error
        {
            printf("\nThe childL process terminated normally\n");
        }
        if (childReturnStatusL == 1)
        {
            printf("\nThe childL process terminated with an error\n");
        }

        if (childReturnStatusS == 0) //verify child3 terminated without error
        {
            printf("\nThe childS process terminated normally\n");
        }
        if (childReturnStatusS == 1)
        {
            printf("\nThe childS process terminated with an error\n");
        }

        if (childReturnStatusG == 0) //verify child4 terminated without error
        {
            printf("\nThe childG process terminated normally\n");
        }
        if (childReturnStatusG == 1)
        {
            printf("\nThe childG process terminated with an error\n");
        }

        exit(0);
    }
    return 0;
}

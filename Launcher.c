#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "functions.h"

//here is my Launcher function. We tailor it to our needs:
//-SELECT between G and S from P
//-PIPE from P to L
//-SOCKET from P to G+1
//-SIGNALS to S

SIZE = 100;

int main()
{
    //Initializing the processes, their return status and the required Pipes
    pid_t processP;
    pid_t processL;
    pid_t processS;
    pid_t processG;

    int childReturnStatusP, childReturnStatusL, childReturnStatusS, childReturnStatusG; //Return variables for child processes

    int fd1[2];//pipe #1 (S -> P)
    int fd2[2];//pipe #2 (G-1 -> P)
    // (P -> L) here we do it the stupid way, as P is going to write on 3 pipes communicationg with L
    //its going to send the RECEIVED VALUE, THE SENT VALUE and wether it received it from G-1 or S
    int fd3[2];//pipe #3 for char G or S

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



    //arrays containing data to pass to the exec
    char *arg[8];

    char r_arg1[5]; //read pipe1
    char w_arg1[5]; //write pipe1
    char r_arg2[5]; //read pipe2
    char w_arg2[5]; //write pipe2
    char r_arg3[5]; //read pipe3.1
    char w_arg3[5]; //write pipe3.1


    //convert int filedes into char targ with sprintf
    sprintf(r_arg1, "%d", fd1[0]);
    sprintf(w_arg1, "%d", fd1[1]);
    sprintf(r_arg2, "%d", fd2[0]);
    sprintf(w_arg2, "%d", fd2[1]);
    sprintf(r_arg3, "%d", fd3[0]);
    sprintf(w_arg3, "%d", fd3[1]);



    arg[1] = r_arg1; //read, pipe1
    arg[2] = w_arg1; //write, pipe1
    arg[3] = r_arg2; //read, pipe2
    arg[4] = w_arg2; //write, pipe2
    arg[5] = r_arg3; //read, pipe3
    arg[6] = w_arg3; //write, pipe3

    arg[7] = NULL;

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
        printf("\nLauncher: process P");
        fflush(stdout);
        char *nameP; //executable file name
        nameP = (char *)"./P";
        arg[0] = name1;
        execvp(name1, arg); //Executing P
    }
    else
    {
        sleep(1);
        processL = fork();

        if (processL < 0)
        {
            perror("Error! Fork returned a negative number for child1L");
            return -1;
        }
        if (processL == 0)
        {
            printf("\nLauncher: process L");
            fflush(stdout);
            char *name2; //executable file name
            name2 = (char *)"./L";
            arg[0] = nameL; //add the process name to the parameters
            execvp(nameL, arg); //Executing L
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
                printf("\nLauncher: process S");
                fflush(stdout);
                char *name3; //executable file name
                name3 = (char *)"./S";
                arg[0] = nameS;
                execvp(nameS, arg); //Executing S
            }
            else  //NOT SURE ABOUT THIS, REVIEW IT LATER
            {
                processG = fork();

                if (processG < 0)
                {
                    perror("Error! Fork returned a negative number for childG!");
                    return -1;
                }
                if (processG == 0)
                {
                    printf("\nLauncher: process G");
                    fflush(stdout);
                    char *nameG; //executable file name
                    name4 = (char *)"./G";
                    arg[0] = nameG;
                    execvp(nameG, arg); //Executing M
                }
            }

        }
        waitpid(processP, &childReturnStatusP, 0); //Waits for all the process to terminate
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

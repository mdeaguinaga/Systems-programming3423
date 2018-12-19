// Manuel I. De Aguinaga
// uyi362

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cs3423p8.h"
void errExit(const char szFmt[], ...);
int concCmd(Cmd cmdM[], int iCmdCnt, Token tokenM[], int iTokenCnt);
int pipeCmd(Cmd cmdM[], int iCmdCnt, Token tokenM[], int iTokenCnt);

/****************************** int concCmd ****************************

This is passed a pointer to a CmdList structure which has already 
been populated by the getCmdList function.
concCmd returns 0 if all children were launched successfully.  
See the description of the conc command for more information

Parameters:
   Cmd cmdM[],          Array that contain the commands
   int iCmdCnt,         number of commands
   Token tokenM[],      Array with total of tokens
   int iTokenCnt        number of tokens

Each of the ls commands are executing in parallel.  
The conc command prints each of the parallel commands showing 
the parent's process Id, child's process ID, the command, and command arguments.

conc causes PELL to execute the commands concurrently (i.e., in parallel).  
There is no communication between the commands, these simply happen in parallel.   
fork each of the children.  The maximum number of commands is 5.   
If any of the commands redirect input or output, it will do the redirection after 
forking, but before execing.  

***********************************************************************/

int concCmd(Cmd cmdM[], int iCmdCnt, Token tokenM[], int iTokenCnt){
 
    long lForkPid;              // variable for lforkPid
    long lWaitPid;              // variable for lWaitPid
    int iExitStatus = 0;        // checl for the Exit Status
    int count = 1;              // counter for the number of interactions
    int pPid, fPid;             // variables for pid
    int cPid[iCmdCnt];          // Array for cPid
    int i;                      // will be user in the forloop
    int j;                      // will be used in the inter loop
    int iX = 0;                 // checks for inputs
    int iA = 0;                 // check for outputs
    char *exArgv[20];           // ecArgv Array
    char *cmd;                  // String for cmd cmdM
    char *cmdlist[iCmdCnt+1];   // String for cmdlist
    char *arglist[iCmdCnt][iTokenCnt];  // String for arglisr double dimention
    int fd[2];                  // Array for fd
    int flagI;                  // integer variable for flagI
    int flagO;                  // integer variable for flagO

    if (pipe(fd) == -1) // checks for errors
        errExit("pipe not created: %s", strerror(errno));

    for(i=0; i<iCmdCnt; i++){ // loop thart checks for the number of commands
        iX = cmdM[i].iBeginIdx;
        cmd = cmdM[i].szCmdNm;
        while (iX <= cmdM[i].iEndIdx){
            arglist[i][iA] = tokenM[iX];
            strcat(cmd, " ");
            strcat(cmd, tokenM[iX]);
            iA++;
            iX++;
        }
        iX = 0;
        arglist[i][iA+1] = NULL;
        iA = 0;
        cmdlist[i] = cmd;
    }

    for(i=0; i<iCmdCnt; i++){ // loop that checks for the commands and creates lForks
        count = 0;
        // create a child process
        lForkPid = fork();  
        // Both the parent and child continue here
        switch(lForkPid)
        {
            case -1:
                errExit("fork failed: %s", strerror(errno));
                break;
            case 0: // child process
                exArgv[0] = cmdM[i].szCmdNm;
                if (cmdM[i].iStdinRedirectIdx != 0) {
                    fd[0] = open(tokenM[cmdM[i].iStdinRedirectIdx], O_RDONLY);
                    dup2(fd[0], STDIN_FILENO);
                    flagI = 1;
                }

                else if (cmdM[i].iStdoutRedirectIdx != 0) {
                    fd[1] = open(tokenM[cmdM[i].iStdoutRedirectIdx], O_WRONLY | O_CREAT | O_EXCL);
                    dup2(fd[1],STDOUT_FILENO);
                    printf("stdout redirect file %d Out %s \n",fd[1],tokenM[cmdM[i].iStdoutRedirectIdx]);
                    flagO = 1;
                }

                else {
                    execvp(cmdM[i].szCmdNm, arglist[i]);
                    if(flagI == 1){
                        close(fd[0]);
                    }
                    if(flagO == 1){
                        close(fd[1]);   
                    }
                    flagI = 0;
                    flagO = 0;
                }
                exit(0);
            default: // parent process
                cPid[i] = lForkPid;
                printf("%d %ld: %s\n",pPid, lForkPid, cmdlist[i]);
        }
    }
    return 0;
}

/*************************** int pipeCmd ********************************

This is passed a pointer to a CmdList structure which has already 
been populated by the getCmdList function.
pipeCmd returns 0 if all children were launched successfully.
See the description of the pipe command for more information
pipe causes PELL to create a pipe and fork a child for each cmdi.  There are only two command
cmd1 can have stdin redirected from a file.  cmd2 can have stdout redirected to a file.  

Parameters:
   Cmd cmdM[],          Array that contain the commands
   int iCmdCnt,         number of commands
   Token tokenM[],      Array with total of tokens
   int iTokenCnt        number of tokens

The pipe command prints each step showing a sequence, 
parent's process ID the child's process ID, and its command
*************************************************************************/

int pipeCmd(Cmd cmdM[], int iCmdCnt, Token tokenM[], int iTokenCnt){

    int fd[2];                  // array for the variable fd
    int i;                      // will be used for the forloop
    int x=0;                    // check for inputs in the forloop
    int a=0;                    // check for the comands in the for loop
    int iRFd, iWFd;             // variables iRFd and iWFd
    int pPid, fPid;             // variables pPid, fPid
    char *cmdlist[iCmdCnt+1];   // Array for the comand list
    int cPid[iCmdCnt+1];        // array of integers for cPid
    char *cmnd;                 // String for cmnd
    char *arglist[iCmdCnt][iTokenCnt];  // vidimentional array for arglist
    char *exArgv[20];           // array for lForkPid
    long lForkPid;              // variable lForkPid
    int flagI = 0;              // integer for flagI
    int flagO = 0;              // integer for flagO

    if (pipe(fd) == -1)
        errExit("pipe not created: %s", strerror(errno));

    for(i = 0; i < iCmdCnt; i++){ // loop that check for the total of Commands
        x = cmdM[i].iBeginIdx;
        cmnd = cmdM[i].szCmdNm;
        while (x <= cmdM[i].iEndIdx) {
            arglist[i][a] = tokenM[x];
            strcat(cmnd, " ");
            strcat(cmnd, tokenM[x]);
            a++;
            x++;
        }
        x = 0;
        arglist[i][a+1] = NULL;
        a = 0;
        cmdlist[i] = cmnd;
    }

    pPid = getpid();

    // loop that will be interacting woth the total commands
    for(i = 0; i < iCmdCnt; i++){ 
        lForkPid = fork();
        iRFd = fd[0];
        iWFd = fd[1];
        switch(lForkPid){
            case -1:
                errExit("fork failed: %s", strerror(errno));
                break;
            case 0:
                exArgv[0] = cmdM[i].szCmdNm;
                if(cmdM[i].iStdinRedirectIdx != 0) {
                    if(iRFd = open(tokenM[cmdM[i].iStdinRedirectIdx], O_RDONLY) == -1){
                        errExit("failed to redirect stdin: %s", strerror(errno));
                    }
                    else{
                        dup2(iRFd, STDIN_FILENO);
                        flagI = 0;
                    }
                    flagI = 0;
                    flagO = 0;
                }
                exit(0);
            default:
                cPid[i] = lForkPid;
                printf("%d %ld: %s\n", pPid, lForkPid, cmdlist[i]);
        }
    }
    return 0;
}

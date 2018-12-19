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

int concCmd(Cmd cmdM[], int iCmdCnt, Token tokenM[], int iTokenCnt){

//"   This is passed a pointer to a CmdList structure which has already been populated by the getCmdList function.
//"   concCmd returns 0 if all children were launched successfully.  
//"   See the description of the conc command for more information
 
    long lForkPid;
    long lWaitPid;
    int iExitStatus = 0;
    int count = 1;
    int pPid, fPid;
    int cPid[iCmdCnt];
    int i;
    int j;
    int iX = 0;
    int iA = 0;
    char *exArgv[20];
    char *cmd;
    char *cmdlist[iCmdCnt+1];
    char *arglist[iCmdCnt][iTokenCnt];
    int fd[2];
    int flagI;
    int flagO;

    if (pipe(fd) == -1)
        errExit("pipe not created: %s", strerror(errno));

    for(i=0; i<iCmdCnt; i++){
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

    for(i=0; i<iCmdCnt; i++){
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




int pipeCmd(Cmd cmdM[], int iCmdCnt, Token tokenM[], int iTokenCnt){

//"   This is passed a pointer to a CmdList structure which has already been populated by the getCmdList function.
//"   pipeCmd returns 0 if all children were launched successfully.
//"   See the description of the pipe command for more information

    int fd[2];
    int i; 
    int x=0; 
    int a=0;
    int iRFd, iWFd;
    int pPid, fPid;
    char *cmdlist[iCmdCnt+1];
    int cPid[iCmdCnt+1];
    char *cmnd;
    char *arglist[iCmdCnt][iTokenCnt];
    char *exArgv[20];
    long lForkPid;
    int flagI = 0;
    int flagO = 0;

    if (pipe(fd) == -1)
        errExit("pipe not created: %s", strerror(errno));

    for(i = 0; i < iCmdCnt; i++){
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

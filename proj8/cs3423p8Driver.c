/**********************************************************************
cs3723p8Driver.c by clark
Purpose:    
    This driver reads a stream input file to receive commands which 
    will exercise the student's concCmd and pipeCmd functions.  

Command Parameters:
    pell < commandFile
Input:
    Lines of text containing conc or pipe commands:
    conc cmd1 args1 , cmd2 args2 , ...
        conc causes PELL to execute the commands concurrently (i.e., parallel).  
        There is no communication between the commands, these simply happen
        concurrently (in parallel).  
    pipe cmd1 args1 , cmd2 args2 
        pipe causes PELL to create a pipe and fork a child for each cmdi.  
        The pipe is the output for step 1 and the input for step 2.  Also,
        cmd1 can have stdin redirected from a file.  cmd2 can have stdout 
        redirected to a file.  
Results:
    For each command read from stdin:
        - Prints the tokens
        - Prints the command information
        - Depending on the command:
          concCmd:
              - prints the parent PID, child PID, and the command
                Example:
                33009 33011: ls -l /bin > lsOne.txt
                33009 33012: ls -l /usr/bin > lsTwo.txt
                33009 33013: ls -l  /etc > lsThree.txt
          pipeCmd, for each child:
              - prints step, parent PID, child Pid, and the command
                Example:
                1 33043 33045: ls -l Data
                2 33043 33046: sort -k5 -n
Notes:
    We print the parent PID and child PID information to stderr to not
    interfere with stdout.
**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "cs3423p8.h"
#define MAX_BUFFER_SZ 500
void processCommands(FILE *pfileCommand);
void prtCmdList(Cmd cmdM[], int iCmdCnt);
int getCmdList(Cmd cmdM[], Token tokenM[], int iTokenCnt);
int split(Token tokenM[], int iMaxToken, char szInput[], char cDelim);

int main(int argc, char *argv[])
{
    // We don't expect a command argument
    if (argc > 1)
        errExit("Usage: pell < inputFile");
    processCommands(stdin);
    return 0;
}

/******************** processCommands **************************************
    void processCommands(FILE *pfileCommand)
Purpose:
    Reads the Command file to process commands.  There are several types of
    records (see the program header for more information).
Parameters:
    I FILE *pfileCommand    command stream input
Notes:
    This calls:
        split
        getCmdList
        concCmd
        pipeCmd
**************************************************************************/
void processCommands(FILE *pfileCommand)
{
    // variables for command processing
    char szInputBuffer[MAX_BUFFER_SZ+1];    // input buffer for a single text line
    // variables for tokenizing
    int iTokenCnt;
    Token tokenM[MAX_TOKENS];
    // variables to understand the commands in the input text
    int iCmdCnt;
    Cmd cmdM[MAX_COMMANDS];
    // misc
    int rc;
    
    //  get command data until EOF
    while (fgets(szInputBuffer, MAX_BUFFER_SZ, pfileCommand) != NULL)
    {
        // if the line is just a line feed, ignore it
        if (szInputBuffer[0] == '\n')
            continue;
        // see if the command is a comment
        if (szInputBuffer[0]== '*')
        {
            printf("%s", szInputBuffer);
            continue;       // it was just a comment
        }
        printf(">>> %s", szInputBuffer);
        // split the line based on spaces
        iTokenCnt = split(tokenM, MAX_TOKENS, szInputBuffer, ' ');
        // print the tokens
        int i;
        printf("%3s %s\n", "Seq", "Token");
        for (i = 0; i < iTokenCnt; i++)
            printf("  %3d '%s'\n", i, tokenM[i]);
        if (iTokenCnt <= 0)
            errExit("Command was blank");
        // get the command list for this command
        memset(cmdM, 0, sizeof(cmdM));
        iCmdCnt = getCmdList(cmdM, tokenM, iTokenCnt);
        prtCmdList(cmdM, iCmdCnt);
        // process the particular command
        if (strcmp(tokenM[0], "conc")==0)
        {   // conc command
            rc = concCmd(cmdM, iCmdCnt, tokenM, iTokenCnt);
            if (rc != 0)
                printf("*** concCmd returned %d\n", rc);
        }
        else if (strcmp(tokenM[0], "pipe")==0)
        {   // pipe command
            rc = pipeCmd(cmdM, iCmdCnt, tokenM, iTokenCnt);
            if (rc != 0)
                printf("*** pipeCmd returned %d\n", rc);
        }
        else
            errExit("Invalid command: '%s'", tokenM[0]);
    }
    printf("\n");   // good place for a breakpoint
}

/******************** split **************************************
int split(Token tokenM[], int iMaxToken, char szInput[], char cDelim)
Purpose:
    Tokenizes the input text by splitting it on the specified 
    delimiter. 
Parameters:
    O Token tokenM[] array of tokens for the input test
    I int iMaxToken  the maximum number of characters in a token (not 
                     including zero byte
    I char szInput[] input text to be tokenized
    I char cDelim    delimiter character (e.g., ' ')
Returns:
    Count of number of entries in tokenM. 
Notes:
    - Linefeed and '\0' are also used as delimiters for the last token
    - If we encounter two or more adjacnet delimiters, we ignore them.
    - If a token is larger than the specified max token size, we 
      truncate the string.
**************************************************************************/
int split(Token tokenM[], int iMaxToken, char szInput[], char cDelim)
{
    int i;                  // used to traverse the input text string
    int iTokenBeg = 0;      // subscript where the token begins
    int iTokenEnd = -1;     // subscript to the delimiter following the token
    int iTokenIdx = 0;      // where to place next token in tokenM
    int iTokenSize;         // size (in bytes) of the token without zero byte
    // We will actually include touching the '\0' or '\n' since that will
    // mark the end of the last token.
    int iLen = strlen(szInput);
    for (i = 0; i <= iLen; i +=1)
    {
        // Is it end of line, line feed or the delim?
        if (szInput[i]== '\0' || szInput[i] == '\n' || szInput[i] == cDelim)
        {
            if (iTokenBeg == i)
            {   // only a delimiter, nothing in token so ignore it
                iTokenBeg = i+1;
                continue;
            }
            // see if the token is too long
            if ( (i-iTokenBeg) > iMaxToken)
                iTokenSize = iMaxToken; // truncate it
            else
                iTokenSize = i-iTokenBeg;
            memcpy(&tokenM[iTokenIdx][0], &szInput[iTokenBeg], iTokenSize);
            tokenM[iTokenIdx][iTokenSize] = '\0'; // terminate it
            iTokenIdx++;
            iTokenBeg = i+1;
        } 
    }
    return iTokenIdx;
}
/******************** gettCmdList **************************************
int getCmdList(Cmd cmdM[], Token tokenM[], int iTokenCnt)
Purpose:
    Parse through the token array to determine the commands.  It
    saves the beginning and ending subscripts for each command's arguments.
    It also determines whether the command has a redirected stdin 
    and/or stdout.
Parameters:
    O Cmd cmdM[]     array of commands
    I Token tokenM[] array of tokens for the input test
    I int iTokenCnt  number of entries in tokenM
Returns:
    Count of number of entries in cmdM. 
Notes:
    - commands are separated by commas
**************************************************************************/
int getCmdList(Cmd cmdM[], Token tokenM[], int iTokenCnt)
{
    int i;              // subscript to current token
    char cChar;         // current character in input text
    int iCmdCnt = 0;    // count of number of entries in cmdM
    // Iterate through the array of tokens.  We actually 
    // go to one item beyond the end so that we can process 
    // the last token normally.  (We pretend there is an 
    // ending token after the last token.)
    for (i = 1; i <= iTokenCnt; i += 1)
    {
        Cmd *pCmd = &(cmdM[iCmdCnt]); 
        if (i == iTokenCnt)
            cChar = ',';  // pretend an ending delim
        else
            cChar = tokenM[i][0];
        switch(cChar)
        {
            case ',':  // delimiter between commands
                if (pCmd->iBeginIdx == 0)
                    errExit("no command, cmd arg: %d\n", i);
                // If we haven't yet marked the end of the command's
                // arguments, assume it is right before the comma.
                // Note that redirection also set the iEndIdx.
                if (pCmd->iEndIdx == 0)
                     pCmd->iEndIdx = i-1;
                // Check for no command arguments
                if (pCmd->iBeginIdx > pCmd->iEndIdx)
                {   // no args
                    pCmd->iBeginIdx = 0;
                    pCmd->iEndIdx = -1;
                }
                iCmdCnt += 1;
                break;
            case '<':  // stdin redirection
                if (i+1 >= iTokenCnt)  // need another arg
                    errExit("redirect requires additional arg, cmd arg: %d\n", i);
                pCmd->iStdinRedirectIdx = i+1;
                // If we haven't yet marked the end of the command's
                // arguments, assume it is right before the <.
                if (pCmd->iEndIdx == 0)
                     pCmd->iEndIdx = i-1;
                break;
            case '>':
                if (i+1 >= iTokenCnt)  // need another arg
                    errExit("redirect requires additional arg, cmd arg: %d\n", i);
                pCmd->iStdoutRedirectIdx = i+1;
                // If we haven't yet marked the end of the command's
                // arguments, assume it is right before the >.
                if (pCmd->iEndIdx == 0)
                     pCmd->iEndIdx = i-1;
                break;
            default:
                // check if at the beginning of the command
                if (pCmd->iBeginIdx == 0)
                {   // not comma, <, > if iBeginIdx is 0, we need to record
                    // where the arguments might begin
                    strcpy(pCmd->szCmdNm, tokenM[i]);
                    pCmd->iBeginIdx = i+1;
                }
        }
    }
    return iCmdCnt;
}
/******************** prtCmdList **************************************
void prtCmdList(Cmd cmdM[], int iCmdCnt)
Purpose:
    Prints information for each command in the list of commands.
Parameters:
    I Cmd cmdM[]    array of commands
    I int iCmdCnt   count of number of entries in cmdM. 
**************************************************************************/
void prtCmdList(Cmd cmdM[], int iCmdCnt)
{
    int i;
    printf("%-20s  %5s %-5s %-5s %-6s\n"
        , "Command", "Begin", "End", "stdin", "stdout");
    for (i = 0; i < iCmdCnt; i +=1)
    {
        printf("%-20s %5d %5d %5d %6d\n"
            , cmdM[i].szCmdNm, cmdM[i].iBeginIdx, cmdM[i].iEndIdx
            , cmdM[i].iStdinRedirectIdx, cmdM[i].iStdoutRedirectIdx);
    }
}

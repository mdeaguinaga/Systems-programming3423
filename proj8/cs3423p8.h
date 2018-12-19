/**********************************************************************
cs3423p8.h
Purpose:
    Defines constants for
        boolean values
        maximum sizes
    Defines typedefs for
        Cmd - describes a command
        Token - character string token
    Prototypes
Notes:
    
**********************************************************************/
#define TRUE 1
#define FALSE 0
#define MAX_COMMANDS 5
#define MAX_TOKEN_SZ 100
#define MAX_PATH 500
#define MAX_TOKENS 30
#define MAX_ARGS 6

// Cmd - represents a command, its list of arguments and 
// subscripts into the token array for redirection of stdin and/or stdout
typedef struct Cmd
{
    int iBeginIdx;                // Beginning subscript in tokenM for first arg
                                  // (this will be 0 when there aren't any arguments)
    int iEndIdx;                  // Ending subscript in tokenM for last arg.  If
                                  // there is redirection, this subscript would be
                                  // before that.  (this is -1 for no arguments)
    char szCmdNm[MAX_TOKEN_SZ+1]; // command name (e.g., ls)
    int iStdinRedirectIdx;        // Subscript in tokenM for the stdin redirect; 0 for no redirect
    int iStdoutRedirectIdx;       // Subscript in tokenM for the stdout redirect; 0 for no redirect
}Cmd;
typedef char Token [MAX_TOKEN_SZ+1];
int concCmd (Cmd cmdM[], int iCmdCnt, Token tokenM[], int iTokenCnt);
int pipeCmd (Cmd cmdM[], int iCmdCnt, Token tokenM[], int iTokenCnt);



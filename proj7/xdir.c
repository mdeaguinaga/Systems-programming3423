#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_LINE_SIZE 500

/***********************************************************************************
                                     xdir
Programmer:     Manuel Israel De Aguinaga
abc123:         uyi362

xdir is a program that search for document that are into the the file that is
being passed vased in the switches
Parameters 
    int argc        - number of commands/names (data) that were enter
    char *argv[]    - String of the information that is passed 
- xdir will first print the directory name followed by a colon. xdir will not print 
    any files that begin with ".". xdir will print the filenames (unqualified) in the order they a
    re provided by the readdir function. 
    xdir will print the files in the order they are provided by the
    readdir function. 
- -l switch xdir will not print any files that begin with "." For each file, xdir 
    will print (on one line):
        -file name (unqualified) 
        -file type (F ‐ regular file, D ‐ directory, L ‐ link, P ‐ Pipe)
        -number of 512 byte blocks 
        -size in bytes
- -a switch xdir  will print any files in the directory including ones that begin with 
    a ".". xdir will print the filenames (unqualified) in the order they are provided 
    by the readdir
- -r switch xdir is used to recurse to subdirectories. 

- -l -a -r switches in any order or any convination will do the description sayed before
    in convination

***********************************************************************************/

void errExit(const char szFmt[], ... );
void rCommand (char string_file[], char *argv[] , char d_name[], struct stat statBuf );
void lCommand (char string_file[], char *argv[] , char d_name[], struct stat statBuf );
void rSubCommand (char *argv[], char string_file[], struct stat statBuf , char curren_dir[], int count);
void lSubCommand (char string_file[], char *argv[] , char d_name[], struct stat statBuf );


int main(int argc, char *argv[])
{
    char szBuff[MAX_LINE_SIZE];     // Buffer to read the lines
    DIR *pDir;                      // Directory that will be analice
    struct dirent *pDirent;         // Struct with directory description
    struct stat statBuf;            // Struct for stat
    char cFileType;                 // Checks for the file type
    char string_file[100];          // String that will be printed
    
    if (argc < 2)       // checks for errors if there is not a parameter
        errExit("too few arguments, directory name needed");
    pDir = opendir(argv[1]);
    if (pDir == NULL)   // checks for a non existing or wrong directories
        errExit("opendir could not open '%s': %s", argv[1], strerror(errno));
        
    printf("%s :\n", argv[1]);   // prints the name of the directory being analized 

    // show information about each directory entry

    while ((pDirent = readdir(pDir)) != NULL) // loop untill no more directories exist
    {
        // checks for an empty array location
        if(!argv[2]) //if there is not a second command
        {
            if(pDirent->d_name[0] != '.')
            {
                printf("    %s\n", pDirent->d_name);
            }
        }
        /***************************************
             checks for the command -l
        ***************************************/
        else if(strcmp(argv[2] , "-l")==0)
        {  
            
            if(!argv[3]){ //if there is nor a third command
                lCommand (string_file, argv , pDirent->d_name, statBuf );
            }
            else if(!argv[4]){ //if there is nor a four command
                if(strcmp(argv[3] , "-a")==0){   
                    lCommand (string_file, argv , pDirent->d_name, statBuf );
                }
                else if(strcmp(argv[3] , "-r")==0){
                    rCommand (string_file, argv , pDirent->d_name, statBuf );
                }
                else{
                    printf("Wrong command %s\n", argv[3]);
                    return;
                }
            }
            else if (argv[4]){ //if there is a four command
                if(strcmp(argv[4] , "-a")==0 && strcmp(argv[3] , "-r")==0 || 
                    strcmp(argv[4] , "-r")==0 && strcmp(argv[3] , "-a")==0){ 
                     if(argv[5]){ // checks for a fifth command
                        printf("Error command : %s Cannot enter more than 3 switches\n",argv[5]);
                        return;
                     }
                    argv[2] = "-l";
                    argv[3] = "-a";
                    argv[4] = "-r";
                    rCommand (string_file, argv , pDirent->d_name, statBuf );
                }
                else{       //prints an error if there is a duplication or wrong command
                    printf("Wrong command %s, Available commands are -l -a -r\n", argv[4]);
                    return;
                }
            }
        }
        /************************************
           checks for the command -a
        ************************************/
        else if(strcmp(argv[2] , "-a")==0)
        {       
            if(!argv[3]){ //if there is nor a third command
                printf("    %s\n", pDirent->d_name);
            }

            else if (!argv[4]){ //if there is nor a four command
                if(strcmp(argv[3] , "-l")==0){
                    argv[2] = argv[3];
                    argv[3] = "-a";

                    lCommand (string_file, argv , pDirent->d_name, statBuf );
                }
                else if(strcmp(argv[3] , "-r")==0){ //if there is nor a third command
                    rCommand (string_file, argv , pDirent->d_name, statBuf );
                }
                else{
                    printf("Wrong command %s\n", argv[3]);
                    return;
                }
            }
            else{ //if there is a four command
                if(strcmp(argv[4] , "-l")==0 && strcmp(argv[3] , "-r")==0 || 
                    strcmp(argv[4] , "-r")==0 && strcmp(argv[3] , "-l")==0){ 
                     if(argv[5]){ // checks for a fifth command
                        printf("Error command : %s Cannot enter more than 3 switches\n",argv[5]);
                        return;
                     }
                    argv[2] = "-l";
                    argv[3] = "-a";
                    argv[4] = "-r";
                    rCommand (string_file, argv , pDirent->d_name, statBuf );
                }
                else{   //prints an error if there is a duplication or wrong command
                    printf("Wrong command %s, Available commands are -l -a -r\n", argv[4]);
                    return;
                }

            }
        }
        /************************************
            checks for the command -r
        ************************************/
        else if(strcmp(argv[2] , "-r")==0)
        {
            if(!argv[3]){   //if there is nor a third command
                rCommand (string_file, argv , pDirent->d_name, statBuf );
            }
            else if(!argv[4]){  //if there is nor a four command
                if(strcmp(argv[3] , "-l") == 0 || strcmp(argv[3] , "-a") == 0){
                    argv[2] = argv[3];
                    argv[3] = "-r";

                    rCommand (string_file, argv , pDirent->d_name, statBuf );
                }
                else{   //prints an error if there is a duplication or wrong command
                    printf("Wrong command, %s not founded \n", argv[3]);
                    break;
                }   
            }
            else{
                if(strcmp(argv[4] , "-l")==0 && strcmp(argv[3] , "-a")==0 ||
                    strcmp(argv[4] , "-a")==0 && strcmp(argv[3] , "-l")==0){
                     if(argv[5]){ // checks for a fifth command
                        printf("Error command : %s Cannot enter more than 3 switches\n",argv[5]);
                        return;
                     }
                     argv[2] = "-l";
                     argv[3] = "-a";
                     argv[4] = "-r";
                     rCommand (string_file, argv , pDirent->d_name, statBuf );
                }
                else{   //prints an error if there is a duplication or wrong command
                    printf("Wrong command %s, Available commands are -l -a -r\n", argv[4]);
                    return;
                }
            }
        }
        else{ //prints an error if there is a wrong command
            printf("wrong command %s \n",argv[2]);
            return;
        }
    }
    closedir(pDir);
    return 0;
}

/**********************************************************************************
                            void rCommand
Paremeters:
    char string_file[]      contains the directory
    char *argv[]            contains the commands being passed
    char d_name[]           contains the name of the directory searching for
    struct stat statBuf     stat buffer for verifications propuses

rCommand is a void function tat supports the r Command soing the description that
main instructions spesify
Since is a void fuction, it just check, validates and print data

**********************************************************************************/

void rCommand (char string_file[], char *argv[] , char d_name[], struct stat statBuf ){
    int rc;                         // chechs -1, 1 or 0 depending the result of stat
    struct dirent *pDirent;         // struct of dirent
    char new_string_file[500];      // String that will be printed
    int count = 0;                  // count the subdirectories based on the loops
    int counter;                    // count the number of loops and prevert repetitions

    // concatenate the order of the directory            
    strcpy(string_file, argv[1]);
    strcat(string_file, "/");
    strcat(string_file, d_name);

    rc = stat ((string_file), &statBuf);

    if (rc < 0)     // checks for errors with directories
        errExit("stat: %s", strerror(errno));
    if(d_name[0] != '.')    // checks for non "."
    {

        if(strcmp(argv[2] , "-r") == 0){ // checks for -r command
            if(S_ISDIR(statBuf.st_mode)){ //checks if is a directory
                count ++;   // add one if is a directory
                printf("    %s :\n", d_name);   // prints name of file
                rSubCommand (argv, string_file, statBuf , d_name, count); // calls rSubcommand function
            }
            else // prints the file that is not a directory
                printf("    %s\n", d_name);
        }
        else if(!argv[4] && strcmp(argv[2] , "-a")==0){ // checks for -a command
            if(S_ISDIR(statBuf.st_mode)){   // checks for directory
                count ++;   // adds one if is a directory
                printf("    %s :\n", d_name);
                rSubCommand (argv, string_file, statBuf , d_name, count); // calls rSubcommand function
            }
            else // prints the file that is not a directory
                printf("    %s\n", d_name);
        }
        else if( !argv[4] && strcmp(argv[2] , "-l")==0){ // checks for -l command
            if(S_ISDIR(statBuf.st_mode)){   // checks for directories
                count ++;   // adds one if is a directory
                printf("    %s :\n", d_name);
                rSubCommand (argv, string_file, statBuf , d_name, count); // calls rSubCommand function
            }
            else // else calls lCommand
                lCommand (string_file, argv , d_name, statBuf );              
        }
    }
    // prints the name of the file that starts with "."
    else if( d_name[0] == '.' && strcmp(argv[2] , "-a") == 0 && !argv[4]){
        printf("    %s\n", d_name);
    }
    // checks for a "." and for a command -l
    else if ( d_name[0] == '.' && strcmp(argv[2] , "-l") == 0 && argv[4]){
        // checks for more than one loop preventing extra prints
        if(counter == 1){
            return; 
        }
        counter += 1;   // adds one for unesesary looping
        if(S_ISDIR(statBuf.st_mode)){    // checks for directories and calls rSubcommand
            count ++;
            rSubCommand (argv, string_file, statBuf , d_name, count);
        }
        else{   // else calls lCommand Function
            lCommand (string_file, argv , d_name, statBuf );
        }
    }
}


/**********************************************************************************
                            void rSubCommand
Paremeters:
    char string_file[]      contains the directory
    char *argv[]            contains the commands being passed
    char d_name[]           contains the name of the directory searching for
    struct stat statBuf     stat buffer for verifications propuses

rCommand is a void function that supports the r Command soing the description that
main instructions spesify
Since is a void fuction, it just check, validates and print data

**********************************************************************************/

void rSubCommand (char *argv[], char string_file[], struct stat statBuf, char curren_dir[], int count)
{
    DIR *pDir;                  // Directory that will be using for subdirectories
    int rc;                     // checks for errors in stat -1 , 1, 0.
    struct dirent *pDirent;     // Calls the struct dirent to check 
    char new_string_file[500];  // Name of the file that will be printed

    pDir = opendir(string_file);    // Opens the subdirectory to be analised
    
    while ((pDirent = readdir(pDir)) != NULL)   // loop that will occur while there is more files in the directory
    {      
        // concatenate the order of the directory
        strcpy(new_string_file, string_file); 
        strcat(new_string_file, "/");
        strcat(new_string_file, pDirent->d_name);
        
        rc = stat ((new_string_file), &statBuf);
        if (rc < 0)
            errExit("stat: %s", strerror(errno));
        // check for the four and tirth arguments from the commands -a and -r
        if(!argv[4] && strcmp(argv[2], "-a")==0  || !argv[3] && strcmp(argv[2], "-r")==0)
        {   // checks for non "." at the bigining
            if(pDirent->d_name[0] != '.')
            {
                if(S_ISDIR(statBuf.st_mode)){ //if is a Directory will be use the folowing
                    count ++;
                    printf("        %s :\n", pDirent->d_name);
                    rSubCommand (argv, new_string_file, statBuf , pDirent->d_name , count);
                    count -= 1;
                }
                else{   // else will print a space (if id not a directory)
                    if(count == 2){ 
                        printf("    ");
                    }
                    printf("        %s\n", pDirent->d_name);
                }
            }
            else if (strcmp(argv[2], "-a")==0){ // checks for -a command
                if(count == 2){ // will print a space and the name of the file
                    printf("    ");
                }
                printf("        %s\n", pDirent->d_name);
            }
        }

        else if(strcmp(argv[2], "-l")==0) // checks for -l command
        {
            if(pDirent->d_name[0] != '.' && !argv[4]) //checks for "." at the begining
            {
                if(S_ISDIR(statBuf.st_mode)){   // checks for a directory
                    count ++;
                    printf("        %s :\n", pDirent->d_name);        
                    // Calls the rSubcommand Function
                    rSubCommand (argv, new_string_file, statBuf , pDirent->d_name , count);
                    count -= 1;
                }
                else{   // else will print spaces 
                    if(count == 2){ // prints an extra space if is a subdirectory file
                        printf("    ");
                    }
                    printf("    ");
                    lSubCommand (pDirent->d_name, argv, new_string_file, statBuf); 
                }
            }
            if (argv[4]){   // checks for a four command
                if(S_ISDIR(statBuf.st_mode)){   // checks if is a directory
                    if(pDirent->d_name[0] == '.'){ //checks for "." at the begining
                        if(count == 2 ){ // prints an extra space if is a subdirectory file
                            printf("    ");
                            // Calls lSubcommand function
                            lSubCommand (pDirent->d_name, argv, new_string_file, statBuf);
                        }
                        else if(count >= 3 ){ // prints an extra space if is a subdirectory file
                            printf("        ");
                            lSubCommand (pDirent->d_name, argv, new_string_file, statBuf);
                        }
                        else {
                            lSubCommand (pDirent->d_name, argv, new_string_file, statBuf);
                        }
                    }
                    else{
                        count ++;
                        if(count >= 3){ // prints an extra space if is a subdirectory file
                            printf("        ");
                        }
                        else if (count == 2){
                            printf("    ");
                        }
                        printf("%s :\n", pDirent->d_name);
                        rSubCommand (argv, new_string_file, statBuf , pDirent->d_name , count);
                        count -= 1;
                    }
                }

                else {
                    if(count >= 3){ // prints an extra space if is a subdirectory file
                        printf("        ");
                    }
                    else if (count == 2){
                        printf("    ");
                    }
                    lSubCommand (pDirent->d_name, argv, new_string_file, statBuf);
                }
            }
        }
    }
    closedir(pDir); // Closes the open file
}

/**********************************************************************************
                            void lCommand
Paremeters:
    char string_file[]      contains the directory
    char *argv[]            contains the commands being passed
    char d_name[]           contains the name of the directory searching for
    struct stat statBuf     stat buffer for verifications propuses

lCommand is a void function that supports the r Command soing the description that
main instructions spesify
Since is a void fuction, it just check, validates and print data

**********************************************************************************/

void lCommand (char string_file[], char *argv[] , char d_name[], struct stat statBuf )
{          
    int rc;                     // checks for errors in stat -1, 0, 1
    char cFileType;             // Checks for the tpe of file
    int count = 0;              // checks for subdirectories
    // concatenate the order of the directory            
    strcpy(string_file, argv[1]);
    strcat(string_file, "/");
    strcat(string_file, d_name);
    // checks for existing info and adds to rc
    rc = stat ((string_file), &statBuf);

    if (rc < 0) // checks for wrong files or directories
        errExit("stat: %s", strerror(errno));

    //file type (F ‐ regular file, D ‐ directory, L ‐ link, P ‐ Pipe)
    if (S_ISLNK(statBuf.st_mode))
        cFileType = 'L';
    else if (S_ISFIFO(statBuf.st_mode))
        cFileType = 'P';
    else if (S_ISREG(statBuf.st_mode))
        cFileType = 'F';
    else if (S_ISDIR(statBuf.st_mode))
        cFileType = 'D';
    else
        cFileType = 'L';
    
    // checks for a non tirth command and for "."
    if(!argv[3] && d_name[0] != '.'){
        printf("    %s %c %ld blks %ld bytes\n"
            , d_name
            , cFileType
            , statBuf.st_blocks
            , statBuf.st_size);
    }
    // checks for non four command and for -r command
    else if(!argv[4] && strcmp(argv[3] , "-r")==0){ 
        if(d_name[0] != '.')
        {
            printf("    %s %c %ld blks %ld bytes\n"
                , d_name
                , cFileType
                , statBuf.st_blocks
                , statBuf.st_size);
        }
    }
    // checks for the command -a
    else if(!argv[4] && strcmp(argv[3] , "-a")==0)
    {

        printf("    %s %c %ld blks %ld bytes\n"
            , d_name
            , cFileType
            , statBuf.st_blocks
            , statBuf.st_size);
    }
    else if (strcmp(argv[2] , "-l") != 0)   
    {    // checks for wrong commands and prints an error
        printf("wrong command, %s not founded\n", argv[2]);
    }
}

/**********************************************************************************
                            void lSubCommand
Paremeters:
    char string_file[]      contains the directory
    char *argv[]            contains the commands being passed
    char d_name[]           contains the name of the directory searching for
    struct stat statBuf     stat buffer for verifications propuses

lSubCommand is a void function that supports the r Command soing the description that
main instructions spesify
Since is a void fuction, it just check, validates and print data

**********************************************************************************/

void lSubCommand (char string_file[], char *argv[] , char d_name[], struct stat statBuf )
{          
    int rc;             // checks for wrong Files
    char cFileType;     // checks type of file
    int count = 0;      //

    rc = stat ((d_name), &statBuf);
                
    if (rc < 0) //
        errExit("stat: %s", strerror(errno));
    
    // //file type (F ‐ regular file, D ‐ directory, L ‐ link, P ‐ Pipe)
    if (S_ISLNK(statBuf.st_mode))
        cFileType = 'L';
    else if (S_ISFIFO(statBuf.st_mode))
        cFileType = 'P';
    else if (S_ISREG(statBuf.st_mode))
        cFileType = 'F';
    else if (S_ISDIR(statBuf.st_mode))
        cFileType = 'D';
    else
        cFileType = 'L';
    // checks for the command -r
    if(!argv[4] && strcmp(argv[3] , "-r")==0){
        if(d_name[0] != '.')
        {
            printf("    %s %c %ld blks %ld bytes\n"
                , string_file
                , cFileType
                , statBuf.st_blocks
                , statBuf.st_size);
        }
    }
    // checks for the command -a
    else if (!argv[4] && strcmp(argv[3] , "-a")==0)
    {

        printf("    %s %c %ld blks %ld bytes\n"
            , string_file
            , cFileType
            , statBuf.st_blocks
            , statBuf.st_size);
    }
    // checks for more than 4 commands
    else if (strcmp(argv[4] , "-r")==0){

        printf("    %s %c %ld blks %ld bytes\n"
            , string_file
            , cFileType
            , statBuf.st_blocks
            , statBuf.st_size);
    }
}

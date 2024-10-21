#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "string_parser.h"


void *memA(int size) {
    return malloc(size);
}

void memF(void *address) {
    free(address);
}

// function to translate user inputted command into a function call
void translator(char **userCom, int numTok){
    
    if(strcmp(userCom[0], "ls") == 0) {     // ls
            if(numTok == 1) {
            listDir();
            } else printf("Error! ls has 0 parameters\n");
    } 
    else if(strcmp(userCom[0], "pwd") == 0) {   //pwd
        if(numTok == 1) {
            showCurrentDir();
        } else printf("Error! pwd has 0 parameters\n");
    } 
    else if(strcmp(userCom[0], "mkdir") == 0) { // mkdir
        if(numTok == 2) {
            makeDir(userCom[1]);
        } else printf("Error! mkdir must have 1 paramater\n");
    }
    else if(strcmp(userCom[0], "cd") == 0) {    //cd
        if(numTok == 2) {
            changeDir(userCom[1]);
        } else printf("Error! cd must have 1 paramater\n");
    }
    else if(strcmp(userCom[0], "cp") == 0) {    //cp
        if(numTok == 3) {
            copyFile(userCom[1], userCom[2]);
        } else printf("Error! cp requires 2 parameters\n");
    }
    else if(strcmp(userCom[0], "mv") == 0) {    //mv
        if(numTok == 3) {
            moveFile(userCom[1], userCom[2]);
        } else printf("Error! mv requires 2 parameters\n");
    }
    else if(strcmp(userCom[0], "rm") == 0) {    //rm
        if(numTok == 2) {
            deleteFile(userCom[1]);
        } else printf("Error! cd has 1 paramater\n");
    }
    else if(strcmp(userCom[0], "cat") == 0) {   //cat
        if(numTok == 2) {
            displayFile(userCom[1]);
        } else printf("Error! cat has 1 paramater\n");
    }
    else if(strcmp(userCom[0], "exit")){
        printf("Error! Unreconized command: %s\n", userCom[0]);
    }
}


int main(int argc, char const *argv[]){
    if(argc == 1){
        //variable to hold user command
        char *com = NULL; // pointer to hold user command
        size_t len = 0; //size of buffer
        ssize_t read = 0; //number of characters read


        
        // command_line struct declaration
        command_line token_buffer;

        while(1) {
            printf(">>> ");
            // Read the entire line including spaces and handle newline
            // Read the entire line
            read = getline(&com, &len, stdin);

            // Check for errors
            if (read == -1) {
                perror("Error reading input");
                free(com);
                return 1;
            }

            token_buffer = str_filler(com, " ");

            translator(token_buffer.command_list, token_buffer.num_token);


            // Exit if the command is "exit"
            if (strcmp(com, "exit") == 0) {
                free(com);
                return 0;
            }
        }
    return 0;    
    } else {
        if(argc == 3){ //checking for command line argument && (strcmp(argv[1], "f") == 0)
            //opening file to read
            FILE *inFPtr;
            inFPtr = fopen (argv[2], "r");

            //declear line_buffer
            size_t len = 128;
            char* line_buf = malloc (len);

            command_line large_token_buffer;
            command_line small_token_buffer;

            int line_num = 0;

            //loop until the file is over
            while (getline (&line_buf, &len, inFPtr) != -1)
            {
                //tokenize line buffer
                //large token is seperated by ";"
                large_token_buffer = str_filler (line_buf, ";");
                //iterate through each large token
                for (int i = 0; large_token_buffer.command_list[i] != NULL; i++)
                {
                    //tokenize large buffer
                    //smaller token is seperated by " "(space bar)
                    small_token_buffer = str_filler (large_token_buffer.command_list[i], " ");

                    //iterate through each smaller token to print
                    for (int j = 0; small_token_buffer.command_list[j] != NULL; j++)
                    {
                        // printf("Small token buffer %s\n", small_token_buffer.command_list[j]);
                        translator(small_token_buffer.command_list, small_token_buffer.num_token);
                    }

                    //free smaller tokens and reset variable
                    free_command_line(&small_token_buffer);
                    //memset (&small_token_buffer, 0, 0);
                }

                //free smaller tokens and reset variable
                free_command_line (&large_token_buffer);
                memset (&large_token_buffer, 0, 0);
            }
            fclose(inFPtr);
            //free line buffer
            free (line_buf);
            return 0;
        } else{
            printf("Error! Proper usage: ./pseudo-shell or ./pseudo-shell -f <filename>");
        }
    } 
}
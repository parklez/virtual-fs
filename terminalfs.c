/*
* Meme project I'm working on
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 1024
#define MAX_LINE_LENGTH 80
#define DEBUG 1

char command_input[MAX_LINE_LENGTH]
char *command, *param1, *param2;


int main(){
    system("clear");
    
    printf("(　’ω’)旦~~> ");
    gets(command_input);
    
    command = strtok(command_input, " ");
    param1 = strtok(NULL, " ");
    param2 = strtok(NULL, "\0");
    if (DEBUG == 1){
        printf("command: '%s', param1: '%s', param2: '%s'", command, param1, param2);
    }
    
    return 0;
}
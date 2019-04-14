/*
* C
* Linux
* Can not create directories with the same name in current position
* Can not remove directories with children
* Must warn users about limitations
* Operations must occur in current directory
* Max entries is 1024
* Nomes must be 8 chars long, only characters, lowercased.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ENTRIES 1024
#define MAX_LINE_LENGTH 80
#define MAX_NAME_LENGTH 8
#define DEBUG 0

char command_input[MAX_LINE_LENGTH];
char *command, *param1, *param2;

int pai[MAX_ENTRIES];
char nome[MAX_ENTRIES][MAX_NAME_LENGTH];
char data[MAX_ENTRIES][10];
char hora[MAX_ENTRIES][10];

int current_position = 0, directory_code = 0;

char thedate[10];
char thetime[10];

void update_time(){
    /* Updates global variables "thetime" and "thedate"
    with the current time/date in hh:mm:ss, dd/mm/yyyy format */
    time_t t = time(NULL);
    struct tm current_time = *localtime(&t);

    sprintf(thetime, "%d:%d:%d", current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
    sprintf(thedate, "%d/%d/%d", current_time.tm_mday, current_time.tm_mon + 1, current_time.tm_year + 1900);
}

void pwd(){
    // TODO: /dir1/dir1.1/dir1.1.1
    printf("Current Working Directory: '%s'\n", nome[current_position]);
}

void mkdir(dirname){
    /* Checks if dirname exists in current directory,
    if not, creates an entry. */

    // Checks if dirname is no longer than 8 characters.
    if (strlen(dirname) > 8){
        puts("mkdir: names only up to 8 characters allowed!");
        return;
    }

    // Creating a copy of dirname (which is a pointer) into a char variable.
    // For some reason I can only iterate like this, as well as use some string.h functions.
    char tempdir[8];
    strcpy(tempdir, dirname);

    // Checks if dirname consists of only alphabetic characters.
    for (int i = 0; i < strlen(tempdir); i++){
        if (isalpha(tempdir[i]) == 0){
            puts("mkdir: names must consist of only alphabetic characters!");
            return;
        }
    }
    // tolower() dirname to avoid upper/lower case conflicts
    for(int i = 0; i < strlen(tempdir); i++){
        tempdir[i] = tolower(tempdir[i]);
    }

    // Checks whether or not dirname exists in current directory
    int exists = 0;

    for (int i = 0; i < MAX_ENTRIES; i++){
        if (pai[i] == current_position){
            if (strcmp(tempdir, nome[i]) == 0){
                puts("mkdir: directory with same name already exists!");
                exists = 1;
                return;
            }
        }
    }

    // If dirname doesn't exist, time to create it!
    if (exists == 0){
        directory_code += 1;
        update_time();
        
        pai[directory_code] = current_position;
        strcpy(nome[directory_code], tempdir);
        strcpy(data[directory_code], thedate);
        strcpy(hora[directory_code], thetime);
        
        printf("mkdir: '%s' created!\n", tempdir);
    }
}

void ls(arg){
    /* Prints directories at current_position */

    // Checks if arg is NULL or -l, in case not, warns user.
    if (arg != NULL && strcmp(arg, "-l") != 0){
        printf("ls: invalid parameter '%s'\n", arg);
        return;
    }

    printf("ls: entries at '%s'\n", nome[current_position]);

    for (int i = 1; i < MAX_ENTRIES; i++){
        if (pai[i] == current_position){
            if (arg == NULL){
                printf("-> '%s'\n", nome[i]);
            }
            else if (strcmp(arg, "-l") == 0){
                printf("-> %s %s '%s'\n", data[i], hora[i], nome[i]);
            }
        }
    }
}

void debug(){
    for (int i = 0; i < 20; i++){
        printf("parent[%i]: %i | name[%i]: '%s' | date[%i]: %s | time[%i]: %s\n", i, pai[i], i, nome[i], i, data[i], i, hora[i]);
    }
}

int main_menu(){
    int running = 1;

    while (running == 1){
        printf("senpai$ ");
        gets(command_input);

        command = strtok(command_input, " ");
        param1 = strtok(NULL, " ");
        param2 = strtok(NULL, "\0");

        if (DEBUG == 1){
            printf("[DEBUG] command: '%s', param1: '%s', param2: '%s'\n", command, param1, param2);
        }
        if (strcmp(command, "pwd") == 0){
            pwd();
        }
        else if (strcmp(command, "mkdir") == 0){
            mkdir(param1);
        }
        else if (strcmp(command, "cd") == 0){
            puts("cd...");
        }
        else if (strcmp(command, "rmdir") == 0){
            puts("rmdir");
        }
        else if (strcmp(command, "ls") == 0){
            ls(param1);
        }
        else if (strcmp(command, "rem") == 0){
            puts("rem...");
        }
        else if (strcmp(command, "poweroff") == 0){
            puts("poweroff...");
            return 0;
        }
        else if (strcmp(command, "copyright") == 0){
            puts("copyright...");
        }
        else if (strcmp(command, "clear") == 0){
            system("cls");
        }
        else if (strcmp(command, "help") == 0){
            puts("help...");
        }
        else if (strcmp(command, "debug") == 0){
            debug();
        }
        else{
            printf("Unknown command '%s'\n", command);
        }
    }
}

int main(){
    // Setting all vectors to -1 (does not exist)
    for (int i = 0; i < 1024; i++){
        pai[i] = -1;
        strcpy(nome[i], "");
        strcpy(data[i], "");
        strcpy(hora[i], "");
    }
    update_time();

    // Defining the root directory
    pai[0] = 0;
    strcpy(nome[0], "/");
    strcpy(data[0], thedate);
    strcpy(hora[0], thetime);
    

    system("cls");
    main_menu();

    return 0;
}
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
    /* Questionable code to print current path */
    
    int cp = current_position;
    int back[MAX_ENTRIES];
    int i = 0;
    
    // In case current_position is 0
    if (cp == 0){
        printf("%s\n", nome[0]);
        return;
    }
    
    // Store parent codes from children to grandparent
    while (cp > 0){
        back[i] = cp;
        cp = pai[cp];
        i++;
    }
    // Print directories from grandparent to children
    i--;
    for (;i >= 0; i--){
        printf("/%s", nome[back[i]]);
    }
    puts("");
}

void mkdir(char *dirname){
    /* Checks if dirname exists in current directory,
    if not, creates an entry. */
    
    // Warns user in case no arguments are given
    if (dirname == NULL){
        puts("mkdir: <dirname> to create a directory in current position.");
        return;
    }
    
    // Checks if dirname is no longer than 8 characters.
    if (strlen(dirname) > 8){
        puts("mkdir: names only up to 8 characters allowed!");
        return;
    }

    // Checks if dirname consists of only alphabetic characters.
    int i;
    for (i = 0; i < strlen(dirname); i++){
        if (isalpha(dirname[i]) == 0){
            puts("mkdir: names must consist of only alphabetic characters!");
            return;
        }
    }
    // tolower() dirname to avoid upper/lower case conflicts
    for(i = 0; i < strlen(dirname); i++){
        dirname[i] = tolower(dirname[i]);
    }

    // Checks whether or not dirname exists in current directory
    int exists = 0;

    for (i = 0; i < MAX_ENTRIES; i++){
        if (pai[i] == current_position){
            if (strcmp(dirname, nome[i]) == 0){
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
        strcpy(nome[directory_code], dirname);
        strcpy(data[directory_code], thedate);
        strcpy(hora[directory_code], thetime);
    }

    if (DEBUG == 1){
        printf("mkdir: '%s' created!\n", dirname);
    }
}

void rmdir(char *dirname){
    /* Attempts to remove an entry, making sure such entry does not countain any children */
    
    // Warns user in case no arguments are given
    if (dirname == NULL){
        puts("rmdir: <dirname> to erase a directory in current position.");
        return;
    }
    
    int exists = 0;
    
    // Checks if dirname exists in current_position
    int i;

    for (i = 1; i < MAX_ENTRIES; i++){
        if (pai[i] == current_position){
            if (strcmp(nome[i], dirname) == 0){
                exists = 1;
                break;
            }
        }
    }
    
    // Warns user in case dirname doesn't exist
    if (exists == 0){
        printf("rmdir: can not erase '%s', does not exist!\n", dirname);
        return;
    }
    
    // Checks if dirname has any children, then warns user
    int k;
    for (k = 1; k < MAX_ENTRIES; k++){
        if (pai[k] == i){
            printf("rmdir: can not erase '%s', directory has children!\n", dirname);
            return;
        }
    }
    
    // In case there's no children, time to erase it!
    if (exists == 1){
        pai[i] = -1;
        strcpy(nome[i], "");
        strcpy(data[i], "");
        strcpy(hora[i], "");
    }
}
    
void rem(char *dirname, char *newname){
    /* Attempts to rename a directory in current_position
    rmdir() and mkdir() could be used to shorten this code... maybe.*/
    
    // Warns user in case no arguments are given
    if (dirname == NULL || newname == NULL){
        puts("rem: <dirname> <newname>.");
        return;
    }

    // Checks if newname is no longer than 8 characters
    if (strlen(newname) > 8){
        puts("rem: <newname> must not be longer than 8 characters!");
        return;
    }

    // Checks if newname consists of only characters
    int i;
    for (i = 0; i < strlen(newname); i++){
        if (isalpha(newname[i]) == 0){
            puts("rem: <newname> must consist of only alphabetic characters!");
            return;
        }
    }

    // tolower() newname to avoid upper/lower case conflicts
    for(i = 0; i < strlen(newname); i++){
        newname[i] = tolower(newname[i]);
    }
    
    int exists = 0;
    int where;
    
    // Checks if dirname exists (and where) as well if newname doesn't in current_position
    for (i = 0; i < MAX_ENTRIES; i++){
        if (pai[i] == current_position){
            if (strcmp(nome[i], dirname) == 0){
                exists = 1;
                where = i;
            }
            if (strcmp(nome[i], newname) == 0){
                printf("rem: directory '%s' already exists in current directory!\n", newname);
                return;
            }
        }
    }
    
    // In case dirname doesn't exist
    if (exists == 0){
        printf("rem: directory '%s' does not exist.\n", dirname);
        return;
    }
    
    // Time to replace nome[where] with newname
    if (exists == 1){
        strcpy(nome[where], newname);
    }
}

void ls(char *arg){
    /* Prints directories at current_position */

    // Checks if arg is NULL or -l, in case not, warns user.
    if (arg != NULL && strcmp(arg, "-l") != 0){
        printf("ls: invalid parameter '%s'\n", arg);
        return;
    }

    printf("ls: entries at '%s'\n", nome[current_position]);
    
    int i;
    for (i = 1; i < MAX_ENTRIES; i++){
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

void cd(char *arg){
    /* Attempts to change current_position */
    
    // Warns user in case no arguments are given
    if (arg == NULL){
        puts("cd: 'cd <dirname>' to enter a directory or 'cd ..' to go back to parent.");
        return;
    }

    // Parent ..
    if (strcmp(arg, "..") == 0){
        if (current_position == 0){
            puts("cd: '/'");
        }
        else {
            current_position = pai[current_position];
        }
        return;
    }

    // tolower() dirname to avoid upper/lower case conflicts
    int i;
    for(i = 0; i < strlen(arg); i++){
        arg[i] = tolower(arg[i]);
    }
    
    // Look for all entries within current_position, if that exists, change current_position to i
    int exists = 0;
    for (i = 0; i < MAX_ENTRIES; i++){
        if (pai[i] == current_position){
            if (strcmp(nome[i], arg) == 0){
                exists = 1;
                current_position = i;
                break;
            }
        }
    }

    // In case such entry does not exist... warn user.
    if (exists == 0){
        printf("cd: directory '%s' not found in current position '%s'.\n", arg, nome[current_position]);
    }
}

void debug(){
    /* Prints a bunch of stuff */

    printf("current_position: %i | directory_code: %i\n", current_position, directory_code);
    int i;
    for (i = 0; i < 20; i++){
        printf("parent[%i]: %i | name[%i]: '%s' | date[%i]: %s | time[%i]: %s\n", i, pai[i], i, nome[i], i, data[i], i, hora[i]);
    }
}

void help(){
    /* prints all the commands we've created */
    
    puts("'cd' - changes directory.");
    puts("'clear' - wipes the prompt.");
    puts("'copyright' - shows the author of this code.");
    puts("'help' - shows all available commands and their short descriptions.");
    puts("'ls' - lists directories within current working directory. 'ls -l' to see when created.");
    puts("'mkdir' - creates a new directory.");
    puts("'poweroff' - shuts down this terminal.");
    puts("'rem' - renames a directory.");
    puts("'rmdir' - erases a directory.");
    puts("'debug' - shows the first 20 entries of the file system.");
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
        if (command == NULL){
            printf("");
        }
        else if (strcmp(command, "pwd") == 0){
            pwd();
        }
        else if (strcmp(command, "mkdir") == 0){
            mkdir(param1);
        }
        else if (strcmp(command, "cd") == 0){
            cd(param1);
        }
        else if (strcmp(command, "rmdir") == 0){
            rmdir(param1);
        }
        else if (strcmp(command, "ls") == 0){
            ls(param1);
        }
        else if (strcmp(command, "rem") == 0){
            rem(param1, param2);
        }
        else if (strcmp(command, "poweroff") == 0){
            puts("goodbye...");
            return 0;
        }
        else if (strcmp(command, "copyright") == 0){
            puts("(c) 2019 github.com/parklez");
        }
        else if (strcmp(command, "clear") == 0){
            system("clear");
        }
        else if (strcmp(command, "help") == 0){
            help();
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
    int i;
    for (i = 0; i < MAX_ENTRIES; i++){
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
    

    system("clear");
    main_menu();

    return 0;
}
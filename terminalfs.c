/*
* Alunos:
* Murilo Ceotto Azzi
* Giovani Menuzzo
* Rodrigo
* Renan
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENTRIES 1024
#define MAX_LINE_LENGTH 80
#define MAX_NAME_LENGTH 8
#define DEBUG 0

char command_input[MAX_LINE_LENGTH];
char *command, *param1, *param2;

int pai[MAX_ENTRIES];
char nome[MAX_ENTRIES][MAX_NAME_LENGTH];
char data[MAX_ENTRIES][MAX_NAME_LENGTH];
char hora[MAX_ENTRIES][MAX_NAME_LENGTH];

int posicao_atual = 0, codigo_do_diretorio = 0;
int running = 1;

// These are global buffers
char thedate[10];
char thetime[10];

void update_time(){
    /* Returns date in dd:mm:yyyy format */
    time_t t = time(NULL);
    struct tm current_time = *localtime(&t);

    sprintf(thetime, "%d:%d:%d", current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
    sprintf(thedate, "%d/%d/%d", current_time.tm_mday, current_time.tm_mon + 1, current_time.tm_year + 1900);
}

int main(){

    // Setting all vectors to -1 (does not exist)
    for (int i = 0; i < 1024; i++){
    pai[i] = -1;
    strcpy(nome[i], "");
    strcpy(data[i], "");
    strcpy(hora[i], "");
    }

    // Defining the root directory
    pai[0] = 0;
    strcpy(nome[0], "/");


    system("cls");
    main_menu();

    return 0;
}

void pwd(){
    printf("Current Working Directory: '%s'\n", nome[posicao_atual]);
}

void mkdir(dirname){
    int exists = 0;

    for (int i = 0; i < MAX_ENTRIES; i++){
        if (pai[i] == posicao_atual){
            if (strcmp(dirname, nome[i]) != 0){
                strcpy(nome[i], dirname);
                printf("mkdir: '%s' created!\n");
                codigo_do_diretorio += 1;
            }
            else{
                puts("mkdir: directory with same name already exists!");
            }
        }
    }
}

int main_menu(){
    while (running == 1){
        printf("master$ ");
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
            puts("ls...");
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
        else{
            printf("Unknown command '%s'\n", command);
        }
    }
}

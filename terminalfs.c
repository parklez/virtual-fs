/*
* Meme project I'm working on
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 1024
#define MAX_LINE_LENGTH 80
#define DEBUG 1

char command_input[MAX_LINE_LENGTH];
char *command, *param1, *param2;
int pai[MAX_ENTRIES];
char nome[MAX_ENTRIES], data[MAX_ENTRIES], hora[MAX_ENTRIES];
int posicao_atual = 0, codigo_do_diretorio = 0;
int running = 1;


int main(){

    // Setting all vectors to -1 (does not exist)
    for (int i = 0; i < 1024; i++){
    pai[i] = -1;
    nome[i] = "";
    data[i] = "";
    hora[i] = "";
    }

    system("clear");
    main_menu();

    return 0;
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
            puts("pwd...");
        }
        else if (strcmp(command, "mkdir") == 0){
            puts("mkdir...");
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
            puts("clear...");
            system("clear");
        }
        else if (strcmp(command, "help") == 0){
            puts("help...");
        }
        else{
            puts("Unknown command '%s'");
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "../include/parser.h"
#include "../include/types.h"
#include "../include/exec.h"
#include <unistd.h>



void Run_shell(FILE* file , int mode){
    char* command_line=NULL;
    size_t length = 0;
    while(1){
    if(mode){
        printf("wish> ");
        fflush(stdout);
    }
    ssize_t readLine = getline(&command_line, &length, file);
    if(readLine == -1) break;
    if(command_line[strlen(command_line)-1] == '\n')
        command_line[strlen(command_line)-1] = '\0';

    ParsedCommands* cmds = parse_Commands(command_line);
    if(cmds){
        execute_commands(cmds);
        free_commands(cmds);
    }
}
free(command_line);
}

int main(int argc , char *argv[]){

    init_Path();
    if(argc == 1){ // interactive mode
        Run_shell(stdin,1);
    }
    else if(argc == 2){ // batch mode
        FILE* file= fopen(argv[1],"r");
        if(!file){
            showError();
            exit(EXIT_FAILURE);
        }
        Run_shell(file,0);
        fclose(file);
    }
    else{
        showError();
        exit(EXIT_FAILURE);
    }
    return 0;
}
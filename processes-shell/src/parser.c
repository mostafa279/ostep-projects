#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "../include/types.h"
#include "../include/parser.h"
# define MAX_COMMAND_LENGTH 1024
#define MAX_COMMANDS 10


 char* trim(char *str) {
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str;

    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
    return str;
}


void showError () {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}

int search_key(Command* cmd , char* key){
    for(int i=0 ; cmd->argv[i]!=NULL;i++){
        if(strcmp(cmd->argv[i],key)==0)return i;    
    }
    return -1;
}

int get_command_length(Command* cmd){
    if(!cmd)return 0;
    int count=0;
    for(int i=0;cmd->argv[i]!= NULL ;i++)count+=1;
    return count;
}
Command* parse_single_command(char* command_line){
    if (!command_line) return NULL;
    
    Command *cmd = malloc(sizeof(Command));
    char** command = calloc(MAX_COMMAND_LENGTH, sizeof(char*));
    
    char *normalized = normalize_redirection(command_line);
    char *rest = normalized; 

    int counter = 0;
    char *token;
    while((token = strsep(&rest, " \t")) != NULL){
        if(strcmp(token, "") != 0){
            command[counter] = strdup(token);
            counter += 1;
        }
    }
    command[counter] = NULL;
    cmd->argv = command;
    cmd->redirect_file = NULL;

    free(normalized); 
    return cmd;
}
ParsedCommands* parse_Commands(char* command_line){
    if(!command_line)return NULL;

    ParsedCommands *cmds = malloc(sizeof(ParsedCommands));
    cmds->commands = malloc(sizeof(Command*) * MAX_COMMANDS);
    cmds->command_count = 0;

    char *piece;
    while((piece = strsep(&command_line,"&")) != NULL ){
        
        piece = trim(piece);
        if(strlen(piece) ==0 )continue;
        cmds->commands[cmds->command_count]=parse_single_command(piece);
        cmds->command_count+=1;
    }
    cmds->commands[cmds->command_count] = NULL;
    return cmds;
}

Command* Redirection(Command* cmd){
    if(!cmd)return NULL;
    int pos = search_key(cmd , ">");
    if(pos == -1)return cmd;
    if(pos==0){
        showError();
        return NULL;
    }
    int argc=get_command_length(cmd);
    if(pos + 2 != argc || (pos+1 >=argc)){
       showError(); 
       return NULL; 
    }
    cmd->redirect_file = strdup(cmd->argv[pos+1]);
    cmd->argv[pos]=NULL;
    return cmd;
}

void free_command(Command *cmd)
{
    if (!cmd)
        return;
    if (cmd->argv) {
        for (int i = 0; cmd->argv[i] != NULL; i++) {
            free(cmd->argv[i]);
        }
        free(cmd->argv);
    }
    free(cmd);
}
void free_commands(ParsedCommands* cmds){
    if(!cmds)return;
    for(int i=0;i<cmds->command_count;i++){
        free_command(cmds->commands[i]);
    }
    free(cmds);
}

char* normalize_redirection(char* input) {
    char* out = malloc(strlen(input) * 3 + 1);
    int j = 0;

    for (int i = 0; input[i]; i++) {
        if (input[i] == '>') {
            out[j++] = ' ';
            out[j++] = '>';
            out[j++] = ' ';
        } else {
            out[j++] = input[i];
        }
    }

    out[j] = '\0';
    return out;
}

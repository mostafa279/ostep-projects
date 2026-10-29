#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "../include/exec.h"
#include "../include/parser.h"
#include "../include/types.h"
#define MAX_COMMANDS 10
Path path;



char* concat (char* a, char* b) {
    char* ans = (char*) malloc((strlen(a) + strlen(b) + 1) * sizeof(char));
    strcpy(ans, a);
    strcat(ans, b);
    return ans;
}

void init_Path(){
    path.path_length=1;
    path.path = malloc(sizeof(char*) * path.path_length);
    path.path[0]= strdup("/bin");
}
void free_path(Path* path){
    for(int i=0;i<path->path_length;i++)free(path->path[i]);
    free(path->path);

    path->path = NULL;
    path->path_length = 0;
}



pid_t execute_command(Command* cmd){
    if(!cmd)return -1;
    int argc=get_command_length(cmd);
    char* command=cmd->argv[0];
      
    //check for  built in command 
    if(strcmp(command,"exit")==0){
            if(argc==1)
                exit(EXIT_SUCCESS);
            else{
                showError();
                return -1;     
            }           
    }
    else if(strcmp(command,"cd")==0){
        if(argc!=2){
            showError();
            return -1;
        }
        int result = chdir(cmd->argv[1]);
        if(result<0){
            showError();
            return -1;
        }
        return 0;
    }

    else if(argc >=1 && strcmp(command,"path")==0){
        free_path(&path);

        path.path_length = argc - 1;

        if (path.path_length > 0){
            path.path = malloc(sizeof(char*) * path.path_length);
            for (int i = 0; i < path.path_length; i++){
                path.path[i] = strdup(cmd->argv[i + 1]);
            }
        }
        else{
            path.path = NULL;
        }
        return 0;
    }  
    
    // check for the bin of the command
    for(int i=0;i<path.path_length;i++){
        char* FullPath;
        char* p;
        p = concat(path.path[i],"/");
        FullPath = concat(p,command);
        free(p);
        if(access(FullPath ,X_OK)==0){
            int rc=fork();
            if(rc==0){
                if(cmd->redirect_file != NULL){                   
                    int fd = open(
                    cmd->redirect_file,
                    O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR
                    );
                    if (fd < 0) {
                        showError();
                        exit(EXIT_FAILURE);
                    }
                    dup2(fd, STDOUT_FILENO);
                    dup2(fd, STDERR_FILENO);
                }    
                execv(FullPath , cmd->argv);
                showError();
                exit(EXIT_FAILURE);
            }
            free(FullPath);
            return rc;
            }
        }    
    showError();
    return -1;   
}
void execute_commands(ParsedCommands* cmds){
    int children[MAX_COMMANDS];
    int child_count = 0;

    for (int i = 0; i < cmds->command_count; i++){
        cmds->commands[i]=Redirection(cmds->commands[i]);
        if(!cmds->commands[i])return;

        int pid = execute_command(cmds->commands[i]);
        if (pid > 0)
            children[child_count++] = pid;
    }

    for (int i = 0; i < child_count; i++){
        waitpid(children[i], NULL, 0);
    }     
}

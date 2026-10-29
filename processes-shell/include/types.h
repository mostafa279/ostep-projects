#pragma once

typedef struct {
char** argv;
char* redirect_file;
}Command;

typedef struct{
    Command** commands;
    int command_count;

}ParsedCommands;

typedef struct{
    char** path;
    int path_length;
}Path;

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "types.h"
#include <sys/types.h>

pid_t execute_command(Command *cmd);
char* concat (char* a, char* b);
void init_Path();
void free_path(Path* path);
void execute_commands(ParsedCommands* cmds);
#endif
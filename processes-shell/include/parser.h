#include "../include/types.h"

int get_command_length(Command* cmd);
Command* parse_single_command(char* command_line);
void free_command(Command* cmd);
void free_commands(ParsedCommands* cmds);
void showError ();
Command* Redirection(Command* cmd);
ParsedCommands* parse_Commands(char* command_line);
char* normalize_redirection(char* input);
char* trim(char *str);
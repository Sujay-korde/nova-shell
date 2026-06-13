#ifndef BUILTINS_H
#define BUILTINS_H

// Checks if a command is an internal shell builtin.
int is_builtin(const char *cmd);

// Handles the execution of builtin commands.
// Returns 1 if handled, 0 if it should exit the shell loop completely.
int execute_builtin(char **args, int arg_count);

// Allows other modules (like parser.c) to look up stored shell variables
const char* get_shell_variable(const char *name);

#endif
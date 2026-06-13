#ifndef PARSER_H
#define PARSER_H

// Parses raw input into a NULL-terminated array of arguments.
// Returns the total number of arguments found.
int parse_input(const char *input, char **args, int max_args);

// Cleans up dynamically allocated memory within the args array.
void free_arguments(char **args, int arg_count);

#endif
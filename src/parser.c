#include "parser.h"
#include <string.h>
#include <stdlib.h>

int parse_input(const char *input, char **args, int max_args) {
    int arg_count = 0;
    char current_arg[100];
    int token_len = 0;
    int in_single_quotes = 0; 
    int in_double_quotes = 0; 

    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];

        if (in_single_quotes) {
            if (c == '\'') {
                in_single_quotes = 0; 
            } else {
                if (token_len < 99) current_arg[token_len++] = c;
            }
        } 
        else if (in_double_quotes) {
            if (c == '"') {
                in_double_quotes = 0; 
            } 
            else if (c == '\\') {
                char next = input[i + 1];
                if (next == '"' || next == '\\' || next == '$') {
                    if (token_len < 99) current_arg[token_len++] = next;
                    i++; 
                } else {
                    if (token_len < 99) current_arg[token_len++] = c;
                }
            } 
            else {
                if (token_len < 99) current_arg[token_len++] = c;
            }
        } 
        else {
            if (c == '\'') {
                in_single_quotes = 1; 
            } 
            else if (c == '"') {
                in_double_quotes = 1; 
            } 
            else if (c == ' ') {
                if (token_len > 0 && arg_count < (max_args - 1)) {
                    current_arg[token_len] = '\0';
                    args[arg_count++] = strdup(current_arg); 
                    token_len = 0;                           
                }
            } 
            else if (c == '\\') {
                char next = input[i + 1];
                if (next != '\0') {
                    if (token_len < 99) current_arg[token_len++] = next;
                    i++; 
                }
            }
            else {
                if (token_len < 99) current_arg[token_len++] = c;
            }
        }
    }

    if (token_len > 0 && arg_count < (max_args - 1)) {
        current_arg[token_len] = '\0';
        args[arg_count++] = strdup(current_arg);
    }
    args[arg_count] = NULL;
    return arg_count;
}

void free_arguments(char **args, int arg_count) {
    for (int i = 0; i < arg_count; i++) {
        if (args[i] != NULL) {
            free(args[i]);
            args[i] = NULL;
        }
    }
}
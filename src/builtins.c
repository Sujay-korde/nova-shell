#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_VARIABLES 100

typedef struct {
    char name[50];
    char value[256];
} ShellVariable;

static ShellVariable variable_table[MAX_VARIABLES];
static int var_count = 0;

void set_shell_variable(const char *name, const char *value){
    for(int i = 0; i < var_count; i++){
        if(strcmp(variable_table[i].name, name) == 0){
            strncpy(variable_table[i].value,value, sizeof(variable_table[i].value) - 1);
            return;
        } 
    }

    if(var_count < MAX_VARIABLES){
        strncpy(variable_table[var_count].name, name, sizeof(variable_table[var_count].name) - 1);
        strncpy(variable_table[var_count].value, value, sizeof(variable_table[var_count].value) - 1);
        var_count++;
    }
}

int is_builtin(const char *cmd) {
    if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "pwd") == 0  || strcmp(cmd, "cd") == 0   ||
        strcmp(cmd, "type") == 0 || strcmp(cmd, "declare") == 0) {
        return 1;
    }
    return 0;
}

static void handle_type(char **args, int arg_count) {
    if (arg_count < 2) {
        printf("type: missing operand\n");
        return;
    }
    
    char* cmd = args[1];
    if (is_builtin(cmd)) {
        printf("%s is a shell builtin\n", cmd);
        return;
    }

    char *path = getenv("PATH");
    if (!path) {
        printf("%s: not found\n", cmd);
        return;
    }

    char *copy = strdup(path);
    if (!copy) {
        perror("strdup");
        return;
    }

#ifdef _WIN32
    char *delim = ";";
#else
    char *delim = ":";
#endif
    char *dir = strtok(copy, delim);
    int found = 0;
    while (dir) {
        char fullpath[512];
#ifdef _WIN32
        snprintf(fullpath, sizeof(fullpath), "%s\\%s.exe", dir, cmd);
#else
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, cmd);
#endif
        if (access(fullpath, X_OK) == 0) {
            printf("%s is %s\n", cmd, fullpath);
            found = 1;
            break;
        }
        dir = strtok(NULL, delim);
    }
    free(copy);

    if (!found) {
        printf("%s: not found\n", cmd);
    }
}

int execute_builtin(char **args, int arg_count) {
    if (strcmp(args[0], "exit") == 0) {
        return 0; // Signals main execution to terminate loop safely
    }
    
    if (strcmp(args[0], "echo") == 0) {
        for (int i = 1; i < arg_count; i++) {
            printf("%s%s", args[i], (i == arg_count - 1) ? "" : " ");
        }
        printf("\n");
        return 1;
    }
    
    if (strcmp(args[0], "pwd") == 0) {
        char cwd[512];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd failed");
        }
        return 1;
    }
    
    if (strcmp(args[0], "cd") == 0) {
        if (arg_count < 2) {
            char *home = getenv("HOME");
            if (home) chdir(home);
        } else {
            char *target_dir = args[1];
            if (strcmp(target_dir, "~") == 0) {
                target_dir = getenv("HOME");
            }
            if (chdir(target_dir) != 0) {
                printf("cd: %s: No such file or directory\n", args[1]);
            }
        }
        return 1;
    }
    
    if (strcmp(args[0], "type") == 0) {
        handle_type(args, arg_count);
        return 1;
    }

    if(strcmp(args[0], "declare") == 0){
        if(arg_count < 2){
            return 1; // No variable name provided, just return
        }
        char * assignment = args[1];
        char *equal_sign = strchr(assignment, '=');
        if(equal_sign !=NULL){
            *equal_sign = '\0';
            char *var_name = assignment;
            char *var_value = equal_sign + 1;

            set_shell_variable(var_name, var_value);
            *equal_sign = '=';
        }
        return 1;
    }

    return 1;
}
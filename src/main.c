#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Only include Unix-specific headers if we are NOT on Windows
#ifndef _WIN32
#include <sys/wait.h>
#endif

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1){
    printf("$ ");

    char input[100];
    if(!fgets(input, sizeof(input), stdin)){
      break;
    }
  
    input[strlen(input) - 1] = '\0';

    // 1. TOKENIZE FIRST: Parse the command and arguments into an array right away
    char *args[20]; 
    int arg_count = 0;
    
    char *token = strtok(input, " ");
    while (token != NULL && arg_count < 19) {
      args[arg_count++] = token;
      token = strtok(NULL, " ");
    }
    args[arg_count] = NULL; // The argument array must be NULL-terminated

    // If the user just hits enter, skip processing and loop back
    if (arg_count == 0) {
      continue;
    }

    // 2. PROCESS COMMANDS USING THE UNIFORM args ARRAY
    if(strcmp(args[0], "exit") == 0){
      break;
    }
    else if(strcmp(args[0], "echo") == 0){
      // Print all arguments separated by spaces cleanly
      for (int i = 1; i < arg_count; i++) {
        printf("%s%s", args[i], (i == arg_count - 1) ? "" : " ");
      }
      printf("\n");
    }
    else if(strcmp(args[0], "pwd") == 0){
      char cwd[512];
      if(getcwd(cwd, sizeof(cwd)) != NULL){
        printf("%s\n", cwd);
      } else {
        perror("pwd failed");
      }
    }
    else if(strcmp(args[0], "cd") == 0){
      if (arg_count < 2) {
        // Fallback or default behavior if no path is given
        char *home = getenv("HOME");
        if (home) chdir(home);
      } else {
        char *target_dir = args[1];
        
        // Handle the "~" shorthand for home directory
        if (strcmp(target_dir, "~") == 0) {
          target_dir = getenv("HOME");
        }
        
        if (chdir(target_dir) != 0) {
          printf("cd: %s: No such file or directory\n", args[1]);
        }
      }
    }
    else if(strcmp(args[0], "type") == 0){
      if (arg_count < 2) {
        printf("type: missing operand\n");
        continue;
      }
      
      char* cmd = args[1];
      if(strcmp(cmd, "echo") == 0 || strcmp(cmd, "type") == 0 || 
         strcmp(cmd, "exit") == 0 || strcmp(cmd, "pwd") == 0 || strcmp(cmd, "cd") == 0){
        printf("%s is a shell builtin\n", cmd);
      } else {
        char *path = getenv("PATH");
        if(!path){
          printf("%s: not found\n", cmd);
          continue;
        }

        char *copy = strdup(path);
        if(!copy){
          perror("strdup");
          continue;
        }

#ifdef _WIN32
        char * delim = ";";
#else
        char * delim = ":";
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
    }
    else {
      // --- EXTERNAL PROGRAM EXECUTION LOGIC ---
      char *cmd = args[0];
      char *path = getenv("PATH");
      char exec_path[512] = {0};
      int found = 0;

      if (path) {
        char *copy = strdup(path);
        if (copy) {
#ifdef _WIN32
          char *delim = ";";
#else
          char *delim = ":";
#endif
          char *dir = strtok(copy, delim);
          while (dir) {
#ifdef _WIN32
              snprintf(exec_path, sizeof(exec_path), "%s\\%s.exe", dir, cmd);
#else
              snprintf(exec_path, sizeof(exec_path), "%s/%s", dir, cmd);
#endif
              if (access(exec_path, X_OK) == 0) {
                  found = 1;
                  break;
              }
              dir = strtok(NULL, delim);
          }
          free(copy);
        }
      }

      if (found) {
#ifdef _WIN32
        printf("Execution of external programs is only supported on Unix/Linux targets.\n");
#else
        pid_t pid = fork();
        
        if (pid == 0) {
          // Inside Child Process
          execv(exec_path, args);
          perror("execv failed");
          exit(1);
        } 
        else if (pid > 0) {
          // Inside Parent Process
          int status;
          waitpid(pid, &status, 0);
        } 
        else {
          perror("fork failed");
        }
#endif
      } 
      else {
        printf("%s: command not found\n", cmd);
      }
    }
  }
  return 0;
}
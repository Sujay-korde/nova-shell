#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "builtins.h"

#ifndef _WIN32
#include <sys/wait.h>
#endif

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  while(1){
    printf("$ ");

    char input[100];
    if(!fgets(input, sizeof(input), stdin)){
      break;
    }
  
    input[strlen(input) - 1] = '\0';

    char *args[20]; 
    int arg_count = parse_input(input, args, 20);

    if (arg_count == 0) {
      continue;
    }

    // Check if the command is an internal shell builtin
    if (is_builtin(args[0])) {
      int keep_running = execute_builtin(args, arg_count);
      free_arguments(args, arg_count);
      if (!keep_running) {
        break;
      }
    } 
    else {
      // --- EXTERNAL PROGRAM EXECUTION LAYER ---
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
          execv(exec_path, args);
          perror("execv failed");
          exit(1);
        } 
        else if (pid > 0) {
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

      free_arguments(args, arg_count);
    }
  }
  return 0;
}
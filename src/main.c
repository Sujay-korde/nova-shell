#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  
  while(1){
    printf("$ ");

    char input[100];
    if(!fgets(input,sizeof(input), stdin)){
      break;
    }
  
    input[strlen(input) - 1] = '\0';

    if(strcmp(input, "exit") == 0){
      break;
    }
    else if(strncmp(input, "echo ",5) == 0){
      printf("%s\n",input + 5);
    }
    else if(strncmp(input, "type ",5) == 0){
      char* cmd = input + 5;
      if(strcmp(cmd, "echo") == 0 || strcmp(cmd, "type") == 0 || strcmp(cmd, "exit") == 0){
        printf("%s is a shell builtin\n", cmd);
      }else{
        char *path = getenv("PATH");
        if(!path){
          printf("%s: not found\n",cmd);
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
        char *dir = strtok(copy,delim);
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
  
    else{
          // --- NEW EXTERNAL PROGRAM EXECUTION LOGIC ---
          
          // 1. Parse the command and all of its arguments into an array
          char *args[20]; // Array to hold command and arguments
          int arg_count = 0;
          
          char *token = strtok(input, " ");
          while (token != NULL && arg_count < 19) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
          }
          args[arg_count] = NULL; // The argument array must be NULL-terminated

          if (arg_count > 0) {
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

            // 2. If the executable is found in the PATH, run it
            if (found) {
              pid_t pid = fork();
              
              if (pid == 0) {
                // Inside Child Process: Execute the program with its arguments
                execv(exec_path, args);
                
                // If execv returns, an error occurred
                perror("execv failed");
                exit(1);
              } 
              else if (pid > 0) {
                // Inside Parent Process: Wait for the child program to finish running
                int status;
                waitpid(pid, &status, 0);
              } 
              else {
                perror("fork failed");
              }
            } 
            else {
              // If not found in PATH, print the required error message
              printf("%s: command not found\n", cmd);
            }
        }
      }
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>
int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  
  while(1){
    printf("$ ");

    char input[100];
    fgets(input, 100, stdin);
  
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
        char *copy = strdup(path);
        char *dir = strtok(copy, ":");

        int found = 0;
        while (dir) {
            char fullpath[512];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, cmd);

            if (access(fullpath, X_OK) == 0) {
                printf("%s is %s\n", cmd, fullpath);
                found = 1;
                break;
            }
            dir = strtok(NULL, ":");
        }
        free(copy);

        if (!found) {
            printf("%s: not found\n", cmd);
        }
        
      }
    }
    else{
      printf("%s: command not found\n",input);
    }

  }
  return 0;
}

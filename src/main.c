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
      char *first = strtok(input, " ");
      if(first){
        printf("%s: command not found\n",input);
      }
    }

  }
  return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>


int changeDir(char ***list) {
    if (strcmp(list[0][0], "cd") == 0) {
        char *home = getenv("HOME");
        if (list[0][1] == NULL || strcmp(list[0][0], "~") == 0) {
            chdir(home);
        } else {
            if(chdir(list[0][1]) != 0) {
                perror("Error can't change directory");
            }
        }
        return 1;
    }
    return 0;
}

void cmd(char ***list, int backgroundFlag) {
    if (changeDir(list) == 1) {
        return;
    }
    int pid;
    int i;
    for (i = 0; list[i] != NULL; i++) {
        pid = fork();
        if (pid > 0) {
            if (backgroundFlag == 0) {
                wait(NULL);
            }
        } else if (pid < 0) {
            perror("Error");
            exit(1);
        } else if (pid == 0) {
            if (execvp(list[i][0], list[i]) < 0) {
                perror("Error, exec failed");
                exit(1);
            }
        }
    }
    if (backgroundFlag == 0) {
        for (; i >= 0; i--) {
            wait(NULL);
        }
    }
}

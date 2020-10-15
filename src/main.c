#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#include "../include/reading.h"
#include "../include/ui.h"

int ioFind(char **list, int flag) {
    if (list[0][0] != '\0') {
        for (int i = 0; list[i] != NULL; i++) {
            if (strcmp(list[i], "<") == 0 && flag == 0) {
                return i;
            }
            if (strcmp(list[i], ">") == 0 && flag == 1) {
                return i;
            }
        }
    }
    return 0;
}

void handler(int signo) {
    putchar('\n');
    user();
}

int checkBG(char ***list) {                //фоновый режим
    int i, j, k;
    char ch;
    if (list[0] == NULL || list[0][0] == NULL || list[0][0][0] == '\0')
            return -1;
    for (i = 0; list[i] != NULL; i++) {
        for (j = 0; list[i][j] != NULL; j++) {
            for (k = 0; list[i][j][k] != '\0'; k++) {
                ch = list[i][j][k];
            }
        }
    }
    if (ch == '&') {
        if (k == 1) {
            free(list[i - 1][j - 1]);
            list[i - 1][j - 1] = NULL;
        } else {
            list[i - 1][j - 1][k - 1] = '\0';
        }
        if (list[0] == NULL || list[0][0] == NULL || list[0][0][0] == '\0') {
            return -1;
        }
        return 1;
    }
    return 0;
}

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

void cmd(char ***list) {
    int i = 0;
    if (changeDir(list) == 1) {
        return;
    }
    int pid;
    for (i = 0; list[i] != NULL; i++) {
        pid = fork();
        if (pid > 0) {
            wait(NULL);
        } else if (pid < 0) {
            perror("Error");
            exit(1);
        } else {
            if (execvp(list[i][0], list[i]) < 0) {
                perror("Error, exec failed");
                exit(1);
            }
        }
    }
}

// void cmdConvAndBG(char ***list, int *numCurBGP, int *convFlag) {
//     int pid = fork();
//     if (pid > 0) {
//         wait(NULL);
//     } else if (pid < 0) {
//         perror("Error");
//         exit(1);
//     } else if (pid == 0) {
//         cmd(list);
//         freeArrList(list);
//         exit(0);
//     } else {
//         if (*convFlag != 0) {          //&
//             int wstatus;
//             wait(&wstatus);
//             printf("%d\n", WEXITSTATUS(wstatus));
//         } else {
//             *numCurBGP += 1;
//             printf("[%d] %d\n", *numCurBGP, pid);
//         }
//     }
// }
//
int cmdExit(char ***list) {
    if (strcmp(list[0][0], "exit") != 0 && strcmp(list[0][0], "quit") != 0) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    signal(SIGINT, handler);
    char ***list;
    int numCurBGP = 0;
    int convFlag = 0, backgroudFlag = 0;
    user();
    list = getArrList(&convFlag);
    // printArrList(list);
    while (list[0][0] == NULL) {
        freeArrList(list);
        user();
        list = getArrList(&convFlag);
        // printArrList(list);

    }
    while (cmdExit(list)) {
        // numCurBGP = 0;
        // backgroudFlag = checkBG(list);
        // if (convFlag == 0 && backgroudFlag == 0) {
        //     cmd(list);
        // } else if (backgroudFlag || (backgroudFlag >= 0 && convFlag)) {
        //     cmdConvAndBG(list, &numCurBGP, &convFlag);
        // }
        cmd(list);
        freeArrList(list);
        if (convFlag == 0) {
            user();
        }
        list = getArrList(&convFlag);
        // printArrList(list);
        while (list[0][0] == NULL) {
            freeArrList(list);
            user();
            list = getArrList(&convFlag);
            // printArrList(list);
        }
    }
    freeArrList(list);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#include "../include/reading.h"
#include "../include/ui.h"
#include "../include/execute.h"

int *pidPhone = NULL;

void printPidPhone(int numPidPhone) {
    for (int i = 0; i < numPidPhone; i++) {
        printf("[%d] %d", i, pidPhone[i]);
    }
}

void pidPhoneAppend(int pid, int *ptrNumPidPhone) {
    int bytes = (*ptrNumPidPhone + 1) * sizeof(int);
    pidPhone = realloc(pidPhone, bytes);
    pidPhone[*ptrNumPidPhone] = pid;
    (*ptrNumPidPhone)++;
}

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
    puts("SIGINT recieved\n");
    user();
}

int checkBackGround(char ***list) {                //фоновый режим
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

void cmdBackGround(char ***list, int *numPidPhone) {
    int pid;
    int i;
    if (list[0][0] == NULL) {
        perror("Error");
        exit(1);
    }
    for(i = 0; list[i] != NULL; i++) {
        pid = fork();
        pidPhoneAppend(pid, numPidPhone);
        if (pid < 0) {
            perror("Error");
            exit(1);
        } else if (pid == 0) {
            cmd(list, 1);
            freeArrList(list);
            exit(0);
        } else {
            printf("[%d] %d\n", *numPidPhone, pid);
        }
    }
}

void killPidPhone(int numPidPhone) {
    for (int i = 0; i <= numPidPhone; i++) {
        kill(pidPhone[i], SIGKILL);
    }
    free(pidPhone);
}

int cmdExit(char ***list, int numPidPhone) {
    if (strcmp(list[0][0], "exit") != 0 && strcmp(list[0][0], "quit") != 0) {
        return 1;
    } else {
        killPidPhone(numPidPhone);
        return 0;
    }
}

int main() {
    signal(SIGINT, handler);
    char ***list;
    int numPidPhone = 0;
    int convFlag = 0, backgroundFlag = 0;
    user();
    list = getArrList(&convFlag);
    // printArrList(list);
    while (list[0][0] == NULL) {
        freeArrList(list);
        user();
        list = getArrList(&convFlag);
        // printArrList(list);

    }
    while (cmdExit(list, numPidPhone)) {
        backgroundFlag = checkBackGround(list);
        if (backgroundFlag == 0) {
            cmd(list, 0);
        } else if (backgroundFlag) {
            cmdBackGround(list, &numPidPhone);
        }
        freeArrList(list);
        // if (backgroundFlag == 0) {
            // user();
        // }
        user();
        list = getArrList(&convFlag);
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

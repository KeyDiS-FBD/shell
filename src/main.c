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
#include "../include/pidList.h"

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

void customKill(int pid) {
    kill(pid, 2);
}

void handler(int signo) {
    puts("\nSIGINT recieved");
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



void killPidList(ptrList headPidList) {
    ptrList tmpPidNode = headPidList;
    while (tmpPidNode != NULL) {
        int pid = tmpPidNode->pid;
        kill(pid, 2);

        tmpPidNode = tmpPidNode->next;
    }
    freePidList(&headPidList);
}

int cmdExit(char ***list) {
    if (strcmp(list[0][0], "exit") != 0 && strcmp(list[0][0], "quit") != 0) {
        return 1;
    } else {
        return 0;
    }
}
void cmd(char ***list, ptrList headPidCurrList) {
    int pid;
    if (changeDir(list) == 1) {
        return;
    }
    for (int i = 0; list[i] != NULL; i++) {
        pid = fork();
        headPidCurrList = pidListAppend(pid, headPidCurrList);
        if (pid > 0) {
            wait(NULL);
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
}

void cmdBackGround(char ***list, int *numPidPhone, ptrList *headPidPhoneList) {
    int pid;
    if (list[0][0] == NULL) {
        perror("Error");
        exit(1);
    }
    for(int i = 0; list[i] != NULL; i++) {
        pid = fork();
        (*numPidPhone)++;
        if (pid < 0) {
            perror("Error");
            exit(1);
        } else if (pid == 0) {
            if (execvp(list[i][0], list[i]) < 0) {
                perror("Error, exec failed");
                exit(1);
            }
            freeArrList(list);
            // exit(0);
        } else {
            printf("[%d] %d\n", *numPidPhone, pid);
            *headPidPhoneList = pidListAppend(pid, *headPidPhoneList);
        }
    }
}


int main() {
    signal(SIGINT, handler);
    char ***list;
    int numPidPhone = 0;
    ptrList headPidPhoneList = NULL;
    ptrList headPidCurrList = NULL;
    int convFlag = 0, backgroundFlag = 0;
    user();
    list = getArrList(&convFlag);
    while (list[0][0] == NULL) {
        freeArrList(list);
        user();
        list = getArrList(&convFlag);
    }
    while (cmdExit(list)) {
        backgroundFlag = checkBackGround(list);
        if (backgroundFlag == 0) {
            cmd(list, headPidCurrList);
        } else if (backgroundFlag == 1) {
            cmdBackGround(list, &numPidPhone, &headPidPhoneList);
        }
        freeArrList(list);
        user();
        list = getArrList(&convFlag);
        while (list[0][0] == NULL) {
            freeArrList(list);
            user();
            list = getArrList(&convFlag);
        }
    }
    killPidList(headPidPhoneList);
    freeArrList(list);
    return 0;
}

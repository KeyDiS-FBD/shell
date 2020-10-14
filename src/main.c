#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#include "../include/reading.h"

struct fdArgList {
    int fdOld[2];
    int fdNew[2];
    int inNum, outNum;
    int lastListNum;
    int fdIn, fdOut;
    char *inVar, *outVar;
};

char *getCurrentDir(char *pwd) {
    char *directory;
    directory = malloc(256 * sizeof(char));
    int i = 0, j = 0;
    while (pwd[i] != '\0') {
        if (pwd[i] == '/') {
            j = 0;
            i++;
            continue;
        }
        directory[j] = pwd[i];
        i++;
        j++;
    }
    directory[j] = '\0';
    free(pwd);
    return directory;
}

void user() {
    char *pwd = getcwd(NULL, 256);
    char *userName = getenv("USER");
    char host[256];
    gethostname(host, _SC_HOST_NAME_MAX);
    pwd = getCurrentDir(pwd);
    printf("[%s@%s %s]$ ", userName, host, pwd);
    fflush(stdout);
    free(pwd);
}

void dupWithCheck(int fdOld, int fdNew) {
    if (fdOld >= 0) {
        dup2(fdOld, fdNew);
    }
}

void nextPipe(int *fdOld, int *fdNew) {
    fdOld[0] = fdNew[0];
    fdOld[1] = fdNew[1];
}

void closeWithCheck(int *fd) {
    if (fd != NULL) {
        if (fd[0] >= 0) {
            close(fd[0]);
            fd[0] = -1;
        }
        if (fd[1] >= 0) {
            close(fd[1]);
            fd[1] = -1;
        }
    }
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

struct fdArgList *ioFile(struct fdArgList *ArgList, char ***list) {
    while (list[ArgList->lastListNum + 1] != NULL) {
        ArgList->lastListNum++;
    }
    ArgList->inNum = ioFind(list[0], 0);
    if (ArgList->inNum != 0) {
        ArgList->fdIn = open(list[0][ArgList->inNum + 1], O_RDONLY);
    }
    ArgList->outNum = ioFind(list[ArgList->lastListNum], 1);
    if (ArgList->outNum != 0) {
        ArgList->fdOut = open(list[ArgList->lastListNum][ArgList->outNum + 1],
            O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    }
    if (ArgList->inNum != 0) {
        ArgList->inVar = list[0][ArgList->inNum];
        list[0][ArgList->inNum] = NULL;
    }
    if (ArgList->outNum != 0) {
        ArgList->outVar = list[ArgList->lastListNum][ArgList->outNum];
        list[ArgList->lastListNum][ArgList->outNum] = NULL;
    }
    ArgList->fdOld[0] = ArgList->fdIn;
    return ArgList;
}


// void handler(int signo) {
//     putchar('\n');
//     user();
// }
//
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
    if (changeDir(list) == 1) {
        return;
    }
    int pid;
    struct fdArgList *ArgList = NULL;
    ArgList = malloc(sizeof(struct fdArgList));
    ArgList->lastListNum = 0;
    ArgList->fdOld[0] = -1;
    ArgList->fdOld[1] = -1;
    ArgList->fdNew[0] = -1;
    ArgList->fdNew[1] = -1;
    ArgList = ioFile(ArgList, list);
    for (int i = 0; list[i] != NULL; i++) {
        pid = fork();
        if (i != ArgList->lastListNum) {
            pipe(ArgList->fdNew);
        } else {
            ArgList->fdNew[1] = ArgList->fdOut;
        }
        if (pid < 0) {
            perror("Error");
            exit(1);
        } else if (pid == 0) {
            dupWithCheck(ArgList->fdOld[0], 0);
            dupWithCheck(ArgList->fdNew[1], 1);
            closeWithCheck(ArgList->fdOld);
            closeWithCheck(ArgList->fdNew);
            if (execvp(list[i][0], list[i]) < 0) {
                perror("Error");
                exit(1);
            }
        }
        closeWithCheck(ArgList->fdOld);
        nextPipe(ArgList->fdOld, ArgList->fdNew);
    }
}



void freeList(char **list) {
    for (int i = 0; list[i] != NULL; i++) {
        free(list[i]);
    }
    free(list);
}

void freeArrList(char ***arrList) {
    for (int i = 0; arrList[i] != NULL; i++) {
        freeList(arrList[i]);
    }
    free(arrList);
}

int main() {
    // signal(SIGINT, handler);
    char ***list;
    user();
    list = getArrList();
    while (list[0][0] == NULL) {
        freeArrList(list);
        user();
        list = getArrList();
    }
    while (strcmp(list[0][0], "exit") != 0 && strcmp(list[0][0], "quit") != 0) {
        cmd(list);
        freeArrList(list);
        user();
        list = getArrList();
        while (list[0][0] == NULL) {
            freeArrList(list);
            user();
            list = getArrList();
        }
    }
    freeArrList(list);
    return 0;
}

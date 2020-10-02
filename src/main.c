#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BLUE    "\x1b[34m"

void superUser() {
    char *pwd = getcwd(NULL, 256);
    char *user = getenv("USER");
    char host[256];
    gethostname(host, _SC_HOST_NAME_MAX);
    printf(COLOR_GREEN "%s@%s" COLOR_RESET ":" COLOR_BLUE "%s" COLOR_RESET "$ ", user, host, pwd);
    fflush(stdout);
    free(pwd);
}

void cmd(char ***list) {
    int pid;
    for (int i = 0; list[i] != NULL; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Error");
            exit(1);
        } else if (pid == 0) {
            if (execvp(list[i][0], list[i]) < 0) {
                perror("Error");
                exit(1);
            }
        }
    }
}

int ioFind(char **list, int flag) {
    if (list[0][0] != '\0') {
        // int inNum;
        // inNum = ioFind(list[0], 0);
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

char *getWord(char *end) {
    char *word = NULL, ch;
    int n = 0, bytes;
    do {
        ch = getchar();
    } while (ch == ' ');
    while (ch != ' ' && ch != '\n' && ch != '\t') {
        if (ch != '"') {
            bytes = (n + 1) * sizeof(char);
            word = realloc(word, bytes);
            word[n] = ch;
            n++;
        }
        ch = getchar();
    }
    bytes = (n + 1) * sizeof(char);
    word = realloc(word, bytes);
    word[n] = '\0';
    *end = ch;
    return word;
}

char **getList(char *listEnd) {
    char end = '\0', **list = NULL;
    int i = 0, flag = 0;
    while (end != '\n') {
        list = realloc(list, (i + 1) * sizeof(char*));
        list[i] = getWord(&end);
        if (strcmp(list[i], "|") == 0) {
            flag = 1;
            *listEnd = end;
            break;
        }
        if (strcmp(list[i], "&&") == 0) {
            flag = 1;
            *listEnd = 'c';
            break;
        }
        i++;
    }
    if (flag == 1) {
        free(list[i]);
        list[i] = NULL;
    } else {
        list = realloc(list, (i + 1) * sizeof(char*));
        list[i] = NULL;
        *listEnd = '\n';
    }
    return list;
}

char ***getArrList() {
    char ***list = NULL, listEnd = '\0';
    int i = 0;
    while (listEnd != '\n' && listEnd != 'c') {
        list = realloc(list, (i + 1) * sizeof(char**));
        list[i] = getList(&listEnd);
        i++;
    }
    list = realloc(list, (i + 1) * sizeof(char**));
    list[i] = NULL;
    return list;
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
    char ***list;
    superUser();
    list = getArrList();
    while (list[0][0] == NULL) {
        freeArrList(list);
        superUser();
        list = getArrList();
    }
    while (strcmp(list[0][0], "exit") != 0 && strcmp(list[0][0], "quit") != 0) {
        cmd(list);
        freeArrList(list);
        list = getArrList();
        while (list[0][0] == NULL) {
            freeArrList(list);
            list = getArrList();
        }
    }
    freeArrList(list);
    return 0;
}

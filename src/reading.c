#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *getWord(char *end) {
    char *word = NULL, ch;
    int i = 0, bytes;
    do {
        ch = getchar();
    } while (ch == ' ');
    while (ch != ' ' && ch != '\n' && ch != '\t') {
        if (ch != '"') {
            bytes = (i + 1) * sizeof(char);
            word = realloc(word, bytes);
            word[i] = ch;
            i++;
        }
        ch = getchar();
    }
    bytes = (i + 1) * sizeof(char);
    word = realloc(word, bytes);
    word[i] = '\0';
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

char ***getArrList(int *convFlag) {
    char ***list = NULL, listEnd = '\0';
    int i = 0;
    while (listEnd != '\n' && listEnd != 'c') {
        list = realloc(list, (i + 1) * sizeof(char**));
        list[i] = getList(&listEnd);
        i++;
    }
    if(listEnd == 'c') {
        *convFlag = 1;
    } else {
        *convFlag = 0;
    }
    list = realloc(list, (i + 1) * sizeof(char**));
    list[i] = NULL;
    return list;
}

//Functions for memory clean
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

//Functions for check correct input
void printLine(char **list) {
    int i = 0;
    while (list[i] != NULL) {
        printf("[%s] ", list[i]);
        i++;
    }
}

void printArrList(char ***list) {
    int i = 0;
    while (list[i] != NULL) {
        printLine(list[i]);
        puts("");
        i++;
    }
}

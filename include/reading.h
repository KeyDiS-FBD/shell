#ifndef READING_H
#define READING_H

char *getWord(char *end);
char **getList(char *listEnd);
char ***getArrList();

void freeList(char **list);
void freeArrList(char ***arrList);

void printLine(char **list);
void printArrList(char ***list);

#endif

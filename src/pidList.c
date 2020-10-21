#include <stdio.h>
#include <stdlib.h>

typedef struct pidNode* ptrList;

struct pidNode {
    int pid;
    int numPid;
    ptrList next;
};

// ptrList headPidPhoneList = NULL;
// ptrList headPidCurrList = NULL;

ptrList pidListAppend(int pid, ptrList headPidList) {
    int bytes = sizeof(struct pidNode);
    ptrList ptrPidList = headPidList;
    if (ptrPidList == NULL) {
        ptrPidList = malloc(bytes);
        ptrPidList->pid = pid;
        ptrPidList->numPid = 1;
        ptrPidList->next = NULL;
        return ptrPidList;
    }
    while (ptrPidList->next != NULL) {
        // prevPtr = ptrPidPhoneList;
        ptrPidList = ptrPidList->next;
    }
    // printf("Append\n");
    ptrList tmpPidNode = NULL;
    tmpPidNode = malloc(bytes);
    tmpPidNode->pid = pid;
    tmpPidNode->numPid = (ptrPidList->numPid) + 1;
    ptrPidList->next = tmpPidNode;
    tmpPidNode->next = NULL;
    return headPidList;
}


void removeAndMerge(ptrList pidList) {
    ptrList tmpPidNode;
    tmpPidNode = pidList->next;
    tmpPidNode = tmpPidNode->next;
    free(pidList->next);
    pidList->next = tmpPidNode;
}

ptrList findAndRemove(int pid, ptrList headPidList) {
    ptrList tmpPidNode = headPidList;
    if (tmpPidNode->pid == pid) {
        headPidList = headPidList->next;
        free(tmpPidNode);
        return headPidList;
    }
    while (tmpPidNode->next != NULL) {
        if (tmpPidNode->next->pid == pid) {
            removeAndMerge(tmpPidNode);
            return headPidList;
        }
    }
    return headPidList;
}

void printList(ptrList headPidList) {
    ptrList tmpPidNode = headPidList;
    if(headPidList == NULL) {
        puts("Head if empty");
    }
    while (tmpPidNode != NULL) {
        printf("[%d] %d\n", tmpPidNode->numPid, tmpPidNode->pid);
        tmpPidNode = tmpPidNode->next;
    }
}

void freePidList(ptrList *headPidList) {
    ptrList tmpPidNodeSwap;
    while (*headPidList != NULL) {
        tmpPidNodeSwap = (*headPidList)->next;
        free(*headPidList);
        *headPidList = tmpPidNodeSwap;
    }
}

// int main() {
//     int pid;
//     scanf("%d", &pid);
//     while (pid != 0) {
//         headPidPhoneList = pidListAppend(pid, headPidPhoneList);
//         scanf("%d", &pid);
//     }
//     printList(headPidPhoneList);
//     puts("Which pid kill: ");
//     scanf("%d", &pid);
//     headPidPhoneList = findAndRemove(pid, headPidPhoneList);
//     printList(headPidPhoneList);
//     return 0;
// }

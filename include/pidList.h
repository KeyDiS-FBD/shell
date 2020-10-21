#ifndef PIDLIST_H
#define PIDLIST_H



typedef struct pidNode* ptrList;

struct pidNode {
    int pid;
    int numPid;
    ptrList next;
};

// ptrList headPidPhoneList;
// ptrList headPidCurrList;

ptrList pidListAppend(int pid, ptrList headPidList);

void removeAndMerge(ptrList pidList);
ptrList findAndRemove(int pid, ptrList headPidList);

void printList(ptrList headPidList);
void freePidList(ptrList *headPidList);

#endif

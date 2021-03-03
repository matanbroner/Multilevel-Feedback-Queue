//
// Created by Matan Broner on 2/23/21.
//

#ifndef MLFQ_PROCESS_H
#define MLFQ_PROCESS_H

#include "util.h"

const int PROCESS_OP_COUNT;

enum operationType {
    CPU,
    IO
};

typedef struct ProcessOperation {
    enum operationType opType;
    int timeLength;
} ProcessOperation;

typedef struct Process {
    int pid;
    int cpuUsage;
    struct ProcessOperation *ops;
    int activeOp;
    int returnFromIO;
    int priority;
} Process;

// used for process tracking and queues

typedef struct ProcessListNode {
    struct Process *process;
    struct ProcessListNode *prev;
    struct ProcessListNode *next;
} ProcessListNode;

struct Process *createProcess();

void printProcess(Process *);

void printProcessList(ProcessListNode *);

void deleteProcess(Process *);

ProcessListNode *createProcessList(Process *);

ProcessListNode *createRandomProcessList(int);

void addProcessNode(ProcessListNode *, Process *);

void removeProcessNode(ProcessListNode *, int);

ProcessListNode *setNext(ProcessListNode *);

ProcessListNode *setPrev(ProcessListNode *);

int getCurrentOpTimeLength(Process *);

int processIsIO(Process *);

void deleteProcessList(ProcessListNode *);

#endif //MLFQ_PROCESS_H

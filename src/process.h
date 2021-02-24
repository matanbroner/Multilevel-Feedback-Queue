//
// Created by Matan Broner on 2/23/21.
//

#ifndef MLFQ_PROCESS_H
#define MLFQ_PROCESS_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

const int PROCESS_OP_COUNT = 4;

enum operationType
{
    CPU,
    IO
};

typedef struct ProcessOperation
{
    enum operationType opType;
    int timeLength;
} ProcessOperation;

typedef struct Process
{
    int pid;
    int cpuUsage;
    struct ProcessOperation *ops;
} Process;

// used for process tracking and queues

typedef struct ProcessListNode
{
    struct Process *process;
    struct ProcessListNode *prev;
    struct ProcessListNode *next;
} ProcessListNode;

struct Process *createProcess();
void printProcess(Process *);
void deleteProcess(Process *);

ProcessListNode *createProcessList(Process *);
void addProcessNode(ProcessListNode *, Process *);
void removeProcessNode(ProcessListNode *);

#endif //MLFQ_PROCESS_H

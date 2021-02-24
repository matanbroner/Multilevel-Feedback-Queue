//
// Created by Matan Broner on 2/23/21.
//

#include "process.h"
#include <stdio.h>

int _randomInRange(int start, int end)
{
    return start + rand() % (end + 1 - start);
}

struct ProcessOperation *createProcessOperations()
{
    struct ProcessOperation *pOps = (ProcessOperation *)calloc(PROCESS_OP_COUNT, sizeof(struct ProcessOperation));
    for (int i = 0; i < PROCESS_OP_COUNT; i++)
    {
        pOps[i].opType = (enum operationType)(rand() % 2); // choose random of two operation types
        pOps[i].timeLength = _randomInRange(10, 100);      // 10ms - 100ms
    }
    return pOps;
}

struct Process *createProcess()
{
    Process *p = malloc(sizeof(Process));
    p->cpuUsage = 0;
    p->pid = _randomInRange(1000, 9999);
    p->ops = createProcessOperations();
    return p;
}

void printProcess(Process *process)
{
    const char *stringEnumTypes[2] = {"CPU", "IO"};
    printf("PID: %d\n", process->pid);
    printf("Ops:\n");
    for (int i = 0; i < PROCESS_OP_COUNT; i++)
    {
        printf("\t- Type: %s, Time-Length: %d\n", stringEnumTypes[process->ops[i].opType], process->ops[i].timeLength);
    }
}

void deleteProcess(Process *process)
{
    free(process->ops);
    free(process);
}

int main()
{
    srand(time(NULL));
    Process *p = createProcess();
    printProcess(p);
    deleteProcess(p);
}

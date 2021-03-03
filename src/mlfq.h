#ifndef MLFQ_MLFQ_H
#define MLFQ_MLFQ_H

#include "process.h"

typedef struct MLFQ {
    ProcessListNode *readyQueue;
    ProcessListNode *IOQueue;
    int timeSliceMultiplier;
    int queueCount;
} MLFQ;

struct MLFQ *createMLFQ();

void run(MLFQ *);

void deleteMLFQ(MLFQ *);


#endif 
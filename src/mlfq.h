#ifndef MLFQ_MLFQ_H
#define MLFQ_MLFQ_H

#include "process.h"

typedef struct QueueWrapper {
    ProcessListNode* queue;
    int tag;
} QueueWrapper;

typedef struct MLFQ
{
    QueueWrapper* q1;
    QueueWrapper* q2;
    QueueWrapper* q3;
    QueueWrapper* activeQueue;
    int timeSliceMultiplier;
} MLFQ;

struct MLFQ* createMLFQ();
void run(MLFQ*);

void deleteMLFQ(MLFQ*);


#endif 
#ifndef MLFQ_MLFQ_H
#define MLFQ_MLFQ_H

#include "process.h"

typedef struct MLFQ
{
    ProcessListNode* q1;
    ProcessListNode* q2;
    ProcessListNode* q3;
    ProcessListNode* activeQueue;
    int timeSliceMultiplier;
} MLFQ;

struct MLFQ* createMLFQ();
void run(MLFQ*);


#endif 
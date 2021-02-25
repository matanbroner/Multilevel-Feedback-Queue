
#include "mlfq.h"

struct MLFQ *createMLFQ()
{
    MLFQ *mlfq = malloc(sizeof(MLFQ));
    mlfq->q1 = createRandomProcessList(5); // all processes start in Q1
    mlfq->q2 = NULL;                       // init with empty (NULL) queue for Q2/3
    mlfq->q3 = NULL;
    mlfq->activeQueue = mlfq->q1;   // begin pulling from Q1
    mlfq->timeSliceMultiplier = 20; // 20ms * queue number
}

void run(MLFQ *mlfq)
{
}

int main()
{
    srand(time(NULL));
}

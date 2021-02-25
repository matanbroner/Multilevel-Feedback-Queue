
#include "mlfq.h"

struct MLFQ *createMLFQ()
{
    MLFQ *mlfq = malloc(sizeof(MLFQ));
    mlfq->q1 = malloc(sizeof(QueueWrapper));
    mlfq->q2 = malloc(sizeof(QueueWrapper));
    mlfq->q3 = malloc(sizeof(QueueWrapper));

    mlfq->q1->queue = createRandomProcessList(5); // all processes start in Q1
    mlfq->q1->tag = 1;
    mlfq->q2->queue = NULL; // init with empty (NULL) queue for Q2/3
    mlfq->q2->tag = 2;
    mlfq->q3->queue = NULL;
    mlfq->q3->tag = 3;
    mlfq->activeQueue = mlfq->q1; // begin pulling from Q1

    mlfq->timeSliceMultiplier = 20; // 20ms * queue number
    return mlfq;
}

void run(MLFQ *mlfq)
{
}

void deleteMLFQ(MLFQ *mlfq)
{
    QueueWrapper *queues[3] = {mlfq->q1, mlfq->q2, mlfq->q3};
    for (int i = 0; i < 3; i++)
    {
        printf("Removing queue %d\n", i);
        if (queues[i]->queue != NULL)
        {
            deleteProcessList(queues[i]->queue);
        }
        free(queues[i]);
    }
    free(mlfq);
}

int main()
{
    srand(time(NULL));
    MLFQ *mlfq = createMLFQ();
    printf("Got mlfq");
    deleteMLFQ(mlfq);
}

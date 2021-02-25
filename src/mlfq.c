
#include "mlfq.h"

struct MLFQ *createMLFQ() {
    MLFQ *mlfq = malloc(sizeof(MLFQ));
    mlfq->q1 = malloc(sizeof(QueueWrapper));
    mlfq->q2 = malloc(sizeof(QueueWrapper));
    mlfq->q3 = malloc(sizeof(QueueWrapper));

    mlfq->q1->queue = createRandomProcessList(5); // all processes start in Q1
    mlfq->q1->tag = 1;
    mlfq->q2->tag = 2;
    mlfq->q3->tag = 3;
    mlfq->q3->queue = mlfq->q2->queue = NULL; // init with empty (NULL) queue for Q2/3
    mlfq->activeQueue = mlfq->q1; // begin pulling from Q1

    mlfq->timeSliceMultiplier = 20; // 20ms * queue number
    mlfq->queueCount = 3; // we have 3 queues

    return mlfq;
}

int runRoundRobin(QueueWrapper *activeQueue, ProcessListNode *bump, int timeSliceMultiplier) {
    int elapsed = 0;
    int timeSlice = activeQueue->tag * timeSliceMultiplier;
    while (activeQueue->queue != NULL) {
        Process *process = activeQueue->queue->process;
        if (process->activeOp >= PROCESS_OP_COUNT) {
            // we should never reach this block
            removeProcessNode(activeQueue->queue, 1); // if we missed this process, delete it
        } else {
            // check we don't have IO "still running"

            if (process->ops[process->activeOp].opType == IO) { // if op is IO,
                process->startIOTime = elapsed; // mark where we start IO
            } else { // else we have a CPU op

                if (process->ops[process->activeOp].timeLength > timeSlice) { // if more than allowed time slice
                    process->ops[process->activeOp].timeLength -= timeSlice; // do legal amount of work
                    removeProcessNode(activeQueue->queue, 0); // pop node
                    elapsed += timeSlice;

                    // bump our node to the next queue
                    if (bump == NULL) {
                        bump = createProcessList(process);
                    } else {
                        addProcessNode(bump, process);
                    }
                } else {
                    process->ops[process->activeOp].timeLength -= timeSlice; // clear CPU work
                    if (process->ops[process->activeOp].timeLength < 0) { // if we released the CPU early
                        elapsed += (timeSlice +
                                    process->ops[process->activeOp].timeLength); // negative time + time slice
                    } else {
                        elapsed += timeSlice;
                    }
                }
            }
        }
    }
}

void run(MLFQ *mlfq) {
    QueueWrapper *activeQueue = mlfq->activeQueue;
    while (activeQueue->queue != NULL) {


    }
}

void deleteMLFQ(MLFQ *mlfq) {
    QueueWrapper *queues[3] = {mlfq->q1, mlfq->q2, mlfq->q3};
    for (int i = 0; i < 3; i++) {
        printf("Removing queue %d\n", i);
        if (queues[i]->queue != NULL) {
            deleteProcessList(queues[i]->queue);
        }
        free(queues[i]);
    }
    free(mlfq);
}

int main() {
    srand(time(NULL));
    MLFQ *mlfq = createMLFQ();
    printf("Got mlfq");
    deleteMLFQ(mlfq);
}

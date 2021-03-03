
#include "mlfq.h"

struct MLFQ *createMLFQ() {
    MLFQ *mlfq = malloc(sizeof(MLFQ));
    mlfq->readyQueue = createRandomProcessList(5); // all processes start in Q1
    mlfq->IOQueue = NULL; // no items in IO at start

    mlfq->timeSliceMultiplier = 20; // 20ms * queue number
    mlfq->queueCount = 3;           // we have 3 queues

    return mlfq;
}


ProcessListNode *nextReadyQueueProcess(MLFQ *mlfq) {
    if (mlfq->readyQueue == NULL) {
        return NULL;
    }
    ProcessListNode *highestPriority = mlfq->readyQueue;
    ProcessListNode *iter = mlfq->readyQueue->next;
    while (iter != NULL) {
        if (iter->process->priority >= highestPriority->process->priority) { // if iter is in same or higher queue
            highestPriority = iter;
        }
        iter = iter->next;
    }
    return highestPriority;
}

ProcessListNode *nextFinishedIO(MLFQ *mlfq) {
    if (mlfq->IOQueue == NULL) {
        return NULL;
    }
    ProcessListNode *highestPriority = mlfq->IOQueue;
    ProcessListNode *iter = mlfq->IOQueue->next;
    while (iter != NULL) {
        if (iter->process->priority >= highestPriority->process->priority) { // if iter is in same or higher queue
            if (iter->process->returnFromIO <= highestPriority->process->returnFromIO) { // and has leq arrival after IO
                highestPriority = iter;
            }
        }
        iter = iter->next;
    }
    return highestPriority;
}

int compoundIOTime(Process *process) {
    if (process->ops[process->activeOp].opType != IO) {
        die("Called compoundIOTime when current operation is not IO based.");
    }
    int total = 0;
    while (process->activeOp < PROCESS_OP_COUNT && process->ops[process->activeOp].opType == IO) {
        total += getCurrentOpTimeLength(process);
        process->activeOp += 1;
    }
    return total;
}

void roundRobin(MLFQ *mlfq) {
    int elapsed = 0;
    while (mlfq->readyQueue != NULL || mlfq->IOQueue != NULL) {
        ProcessListNode *readyIO = nextFinishedIO(mlfq);
        ProcessListNode *nextReady = nextReadyQueueProcess(mlfq);
        printf("Ready IO PID: %d\n", readyIO == NULL ? -1 : readyIO->process->pid);
        printf("Ready Next PID: %d\n", nextReady == NULL ? -1 : nextReady->process->pid);
        if (processIsIO(nextReady->process)) {
            int timeFinish = compoundIOTime(nextReady->process) + elapsed;
            nextReady->process->returnFromIO = timeFinish;
            if (mlfq->IOQueue == NULL) {
                mlfq->IOQueue = createProcessList(nextReady->process);
            } else {
                addProcessNode(mlfq->IOQueue, nextReady->process); // move process to IO waiting state
            }
            removeProcessNode(nextReady, 0);
        } else if (readyIO == NULL || getCurrentOpTimeLength(nextReady->process) + elapsed <
                                      readyIO->process->returnFromIO) { // if process is not long enough to intersect with IO
            elapsed += getCurrentOpTimeLength(nextReady->process);
            nextReady->process->activeOp += 1;
            if (nextReady->process->activeOp >= PROCESS_OP_COUNT) {
                printf("Completing PID #%d @ Time: %d\n", nextReady->process->pid, elapsed);
                removeProcessNode(nextReady, 1);
            }
        } else { // next CPU has to do enough to let IO finish and pass
            if (readyIO != NULL) {
                int allowedToFinish = readyIO->process->returnFromIO - elapsed; // CPU op can do this much work
                nextReady->process->ops[nextReady->process->activeOp].timeLength -= allowedToFinish;
                elapsed += allowedToFinish;
                if (readyIO->process->activeOp >= PROCESS_OP_COUNT) { // if IO was last op
                    printf("Completing PID #%d @ Time: %d\n", readyIO->process->pid, elapsed);
                    removeProcessNode(readyIO, 1);
                } else {
                    if (mlfq->readyQueue == NULL) {
                        mlfq->readyQueue = createProcessList(readyIO->process);
                    } else {
                        addProcessNode(mlfq->readyQueue, readyIO->process);
                    }
                    removeProcessNode(readyIO, 0);
                }
            } else { // if no IO process to consider
                elapsed += getCurrentOpTimeLength(nextReady->process);
                nextReady->process->activeOp += 1;
                if (nextReady->process->activeOp >= PROCESS_OP_COUNT) { // if IO was last op
                    printf("Completing PID #%d @ Time: %d\n", nextReady->process->pid, elapsed);
                    removeProcessNode(nextReady, 1);
                }
            }
        }
    }
}


void run(MLFQ *mlfq) {
    roundRobin(mlfq);
}

void deleteMLFQ(MLFQ *mlfq) {
    ProcessListNode *queues[2] = {mlfq->readyQueue, mlfq->IOQueue};
    for (int i = 0; i < 2; i++) {
        if (queues[i] != NULL) {
            deleteProcessList(queues[i]);
        }
    }
    free(mlfq);
}

int main() {
    srand(time(NULL));
    MLFQ *mlfq = createMLFQ();
    printProcessList(mlfq->readyQueue);
    run(mlfq);
    deleteMLFQ(mlfq);
}


//int runRoundRobin(QueueWrapper *activeQueue, ProcessListNode *bump, int timeSliceMultiplier) {
//    int elapsed = 0;
//    int leftOverIOTime = 0;
//    int timeSlice = activeQueue->tag * timeSliceMultiplier;
//    while (activeQueue->queue != NULL) {
//        Process *process = activeQueue->queue->process;
//        // check we don't have IO "still running"
//        if (process->returnFromIO > -1) {
//            if (elapsed >= process->returnFromIO) { // IO is done for this op
//                process->activeOp += 1; // push to next op
//            } else { // we haven't finished the IO op
//                if (setNext(activeQueue->queue) == NULL) { // is last process in queue
//                    // steal from the bump queue
//                    if (bump != NULL) {
//
//                    }
//                }
//            }
//
//            process->returnFromIO = -1; // revoke active IO
//        }
//        if (process->activeOp >= PROCESS_OP_COUNT) {
//            removeProcessNode(activeQueue->queue, 1); // if we missed this process, delete it
//        } else {
//            if (process->ops[process->activeOp].opType == IO) {                                   // if op is IO,
//                process->returnFromIO = elapsed +
//                                        getCurrentOpTimeLength(
//                                                process); // mark where we start IO, also don't push op idx forward
//            } else { // else we have a CPU op
//
//                if (getCurrentOpTimeLength(process) >
//                    timeSlice) {                                                            // if more than allowed time slice
//                    process->ops[process->activeOp].timeLength -= timeSlice; // do legal amount of work
//                    removeProcessNode(activeQueue->queue, 0);                // pop node
//                    elapsed += timeSlice;
//
//                    // bump our node to the next queue
//                    if (bump == NULL) {
//                        bump = createProcessList(process);
//                    } else {
//                        addProcessNode(bump, process);
//                    }
//                } else {
//                    process->ops[process->activeOp].timeLength -= timeSlice; // clear CPU work
//                    if (getCurrentOpTimeLength(process) < 0) { // if we released the CPU early
//                        elapsed += (timeSlice + getCurrentOpTimeLength(process)); // negative time + time slice
//                    } else {
//                        elapsed += timeSlice;
//                    }
//                }
//            }
//        }
//    }
//}
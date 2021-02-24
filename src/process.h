//
// Created by Matan Broner on 2/23/21.
//

#ifndef MLFQ_PROCESS_H
#define MLFQ_PROCESS_H

#include <stdlib.h>

enum operation{CPU, IO};

struct ProcessOperation {
    enum operation op_type;
    int length;
};

struct Process {
    int pid;
    int cpu_use;
    struct ProcessOperation ops[3];
};

struct Process* createProcess();
struct Process* createSeededProcess();

#endif //MLFQ_PROCESS_H

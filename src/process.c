//
// Created by Matan Broner on 2/23/21.
//

#include "process.h"

const int PROCESS_OP_COUNT = 4;

struct ProcessListNode *_getListHead(ProcessListNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    ProcessListNode *temp = node;
    while (temp->prev != NULL)
    {
        temp = temp->prev;
    }
    return temp;
}

struct ProcessListNode *_getListTail(ProcessListNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    ProcessListNode *temp = node;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    return temp;
}

struct ProcessOperation *createProcessOperations()
{
    struct ProcessOperation *pOps = (ProcessOperation *)calloc(PROCESS_OP_COUNT, sizeof(struct ProcessOperation));
    for (int i = 0; i < PROCESS_OP_COUNT; i++)
    {
        pOps[i].opType = (enum operationType)(rand() % 2); // choose random of two operation types
        pOps[i].timeLength = randomInRange(10, 100);       // 10ms - 100ms
    }
    return pOps;
}

struct Process *createProcess()
{
    Process *p = malloc(sizeof(Process));
    p->cpuUsage = 0;
    p->pid = randomInRange(1000, 9999);
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

struct ProcessListNode *createProcessList(Process *process)
{
    ProcessListNode *head = malloc(sizeof(ProcessListNode));
    head->process = process;
    head->next = head->prev = NULL;
    return head;
}

ProcessListNode *createRandomProcessList(int length)
{
    if (length < 1)
    {
        die("Called createRandomProcessList with invalid length");
    }
    Process *process = createProcess();
    ProcessListNode *head = createProcessList(process);
    for (int i = 1; i < length; i++)
    {
        addProcessNode(head, createProcess());
    }
    return head;
}

void addProcessNode(ProcessListNode *node, Process *process)
{
    ProcessListNode *tail = _getListTail(node);
    if (tail == NULL)
    {
        die("Called addProcessNode on NULL list");
    }
    ProcessListNode *newNode = malloc(sizeof(ProcessListNode));
    newNode->process = process;
    newNode->prev = tail;
    newNode->next = NULL;
    tail->next = newNode;
}

void removeProcessNode(ProcessListNode *node, int shouldDeleteProcess)
{
    if (shouldDeleteProcess == 1)
    {
        deleteProcess(node->process);
    }
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    free(node);
}

ProcessListNode *setNext(ProcessListNode *node)
{
    if (node == NULL)
    {
        die("Called setNext on NULL node");
        return node->next;
    }
    return node->next;
}
ProcessListNode *setPrev(ProcessListNode *node)
{
    if (node != NULL)
    {
        die("Called setPrev on NULL node");
    }
    return node->prev;
}

void deleteProcessList(ProcessListNode *node)
{
    if (node == NULL)
    {
        die("Called deleteProcessList on NULL list");
    }
    ProcessListNode *head = _getListHead(node);
    while (head != NULL)
    {
        ProcessListNode *temp = head->next;
        removeProcessNode(head, 1);
        head = temp;
    }
}

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdlib.h>

typedef void* queueItem;

typedef struct queueNode{
    queueItem item;
    struct queueNode* next;
    struct queueNode* prev;
}queueNode;

typedef struct queue{
    queueNode* head;
    int count;
}queue;

void queueInit(queue *list);

void queueEnqueue(queue *list, queueItem item);

#endif

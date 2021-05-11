#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>

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

void queueInit(queue* list);
void queueAddItem(queue* list, queueItem item);
void queueAddNode(queue* list, queueNode* node);
queueItem queueGetFirstItem(queue* list);
queueNode* queueGetFirstNode(queue* list);
void queueDestroy(queue* list);

#endif

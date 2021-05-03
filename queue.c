#include "queue.h"

void queueInit(queue *list) {
    list->head = NULL;
    list->count = 0; 
};

void queueEnqueue(queue *list, queueItem item){
    queueNode* node = (queueNode*) malloc(sizeof(queueNode));
    node->item = item;
    
    if (list->head == NULL){
        node->next = NULL;
        node->prev = NULL;
        node->item = item;
        list->count++;
    } else {
        queueNode* tmp = list->head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = node;
        node->prev = tmp;
        node->item = item;
        list->count++;
    }
}

int queueSize(queue * list){
    return list->count;
}


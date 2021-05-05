#include "queue.h"

void queueInit(queue* list) {
    list->head = NULL;
    list->count = 0; 
};

void queueAddItem(queue* list, queueItem item) {
    queueNode* node = (queueNode*) malloc(sizeof(queueNode));
    node->item = item;
    
    if (list->head == NULL){
        node->next = NULL;
        node->prev = NULL;
        node->item = item;
        list->head = node;
    } else {
        queueNode* tmp = list->head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = node;
        node->prev = tmp;
        node->item = item;
    }

    list->count++;
}

queueItem queueGetFirstItem(queue* list) {
    if(list->head == NULL){
        fprintf(stderr, "Error, no hay elementos en la lista\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    queueNode* tmp = list->head;
    queueItem item = tmp->item;

    list->head = list->head->next;

    if (list->count > 1) {
        list->head->prev = NULL;   
    }

    free(tmp);
    list->count--;
    return item;
}

void queueDestroy(queue* list) {
    while(list->count != 0) {
        queueGetFirstItem(list);
    }
    free(list);
    return;
}
#include "queue.h"

void queueInit(queue* list) {
    list->head = NULL;
    list->count = 0; 
    return;
};

void queueAddItem(queue* list, queueItem item) {
    queueNode* node = (queueNode*) malloc(sizeof(queueNode));
    node->item = item;
    node->next = NULL;
    node->prev = NULL;

    if (list->head == NULL){
        node->prev = NULL;
        node->item = item;
        list->head = node;
        list->count++;
        return;
    } else {
        queueNode* tmp = list->head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        //node->prev = tmp;
        node->prev=tmp;
        tmp->next = node;
        node->item = item;
        list->count++;
        return;
    }
}

void queueAddNode(queue* list, queueNode* node) {
    node->next = NULL;

    if (list->head == NULL){
        node->prev = NULL;
        list->head = node;
        list->count++;
        return;
    } else {
        queueNode* tmp = list->head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        node->prev = tmp;
        tmp->next = node;
        list->count++;
        return;
    }
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

queueNode* queueGetFirstNode(queue* list) {
    if(list->head == NULL){
        fprintf(stderr, "Error, no hay elementos en la lista\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    queueNode* tmp = list->head;

    list->head = list->head->next;

    if (list->count > 1) {
        list->head->prev = NULL;   
    }

    list->count--;
    return tmp;
}

void queueDestroy(queue* list) {
    while(list->count != 0) {
        queueGetFirstItem(list);
    }
    //free(list);
    return;
}
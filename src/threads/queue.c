#include "queue.h"

/**
 * Funcion encargada de inicializar las listas enlazadas
 * Recibe la lista enlazada a inicializar
 */
void queueInit(queue* list) {
    list->head = NULL;
    list->count = 0; 
    return;
};

/**
 * Funcion encargada de agregar un objeto a una lista enlazada
 * Recibe la lista enlazada a la que se le agregara el objeto y el objeto a agregar
 */
void queueAddItem(queue* list, queueItem item) {
    queueNode* node = (queueNode*) malloc(sizeof(queueNode));
    if (node == NULL) {
        printf("Error, no se pudo alocar memoria");
        exit(EXIT_FAILURE);
    }
    
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
        node->prev=tmp;
        tmp->next = node;
        node->item = item;
        list->count++;
        return;
    }
}

/**
 * Funcion encargada de agregar un nodo a una lista enlazada
 * Recibe la lista enlazada a agregar el nodo y el nodo a agregar
 */
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

/**
 * Funcion encargada de retornar el primer elemento de una lista enlazada
 * Recibe la lista enlazada a tratar
 * Retorna el primer objeto de la lista enlazada
 */
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

/**
 * Funcion encargada de obtener el primer nodo de una lista enlazada
 * Recibe la lista enlazada a tratar
 * Retorna el primer nodo de la lista enlazada
 */
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

/**
 * Funcion encargada de limpiar una lista enlazada
 * Recibe la lista enlazada a limpiar
 */
void queueDestroy(queue* list) {
    while(list->count != 0) {
        queueGetFirstItem(list);
    }
    return;
}
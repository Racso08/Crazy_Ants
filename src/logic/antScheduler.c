#include "antScheduler.h"

void scheduleAntsAux(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, int dest, queue* izqPositions, queue* derPositions);
void updateAntPositions(queue* channelQueue, queue* positions);
int scheduleChannel(int scheduler, queue* list);
void queuePrintPriority(queue* list);
void queuePrintTime(queue* list);
char* queueTimes(queue* list);
char* queuePriority(queue* list);
int prioritySchedule(queue* list);
int sjfSchedule(queue* list);

/**
 * Funcion encargada de calendarizar las hormigas
 * Recibe el canal y el destino de la hormiga
 */
void scheduleAnts(int channel, int dest) {
    switch (channel) {
        case 1:
            scheduleAntsAux(channel1, &channel1LeftQueue, &channel1RightQueue, dest, &izq1, &der1);
            break;
        
        case 2:
            scheduleAntsAux(channel2, &channel2LeftQueue, &channel2RightQueue, dest, &izq2, &der2);
            break;

        case 3:
            scheduleAntsAux(channel3, &channel3LeftQueue, &channel3RightQueue, dest, &izq3, &der3);
            break;
    }

    return;
}

/**
 * Funcion auxiliar encargada de calendarizar las hormigas
 * Recibe el canala, la lista de espera izquierda y derecha del canal, el destino de la hormiga y las listas de posiciones izquierdas y derechas del canal
 */
void scheduleAntsAux(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, int dest, queue* izqPositions, queue* derPositions) {
    switch (dest) {
        case 0:
            if (channelRightQueue->count > 1) {
                channel->scheduled = scheduleChannel(channel->scheduler, channelRightQueue);
                if (channel->scheduled == 0) {
                    updateAntPositions(channelRightQueue, derPositions);
                    channel->scheduled = 1;
                }
            }
            break;
        case 1:
            if (channelLeftQueue->count > 1) {
                channel->scheduled = scheduleChannel(channel->scheduler, channelLeftQueue);
                if (channel->scheduled == 0) {
                    updateAntPositions(channelLeftQueue, izqPositions);
                    channel->scheduled = 1;
                }
            }
            break;
    }

    return;
}

/**
 * Funcion encargada de actualizar las posiciones de las hormigas
 * Recibe la lista de hormiga y las posiciones a colocar
 */
void updateAntPositions(queue* channelQueue, queue* positions) {
    queueNode* antNode = (queueNode*) channelQueue->head;
    queueNode* positionNode = (queueNode*) positions->head;

    ant_t* ant;
    int position;

    while (antNode != NULL) {
        ant = (ant_t*) antNode->item;
        position = (int) (__intptr_t) positionNode->item;

        ant->finalDest = position;
        ant->reorganizing = 1;
        ant->waiting = 0;

        antNode = antNode->next;
        positionNode = positionNode->next;        
    }

    return;
}

/**
 * Funcion encargada de calendarizar las hormigas en espera de un canal
 * Recibe el tipo de calendarizador y la lista a analizar
 */
int scheduleChannel(int scheduler, queue* list) {
    switch (scheduler) {
        case 1:
            return prioritySchedule(list);
            break;
        case 2:
            return sjfSchedule(list);
            break;
    }

    return 1;
}

/**
 * Funcion encargada de obtener los tiempos de las hormigas y colocarlos en una lista de caracteres
 * Recibe la lista a analizar
 * Retorna el puntero a esa lista
 */
char* queueTimes(queue* list) {
    times=malloc(sizeof(char)*20);
    if (times == NULL) {
        printf("Error, no se pudo alocar memoria");
        exit(EXIT_FAILURE);
    }
    char time[50];
    queueNode *tmp = list->head;
    ant_t* ant;
    while (tmp!=NULL)
    {
        ant=(ant_t*) tmp->item;
        sprintf( time, "%d", ant->time );
        strcat(times, time);
        tmp = tmp->next;
    }
    //printf("tiempos1: %s\n: ",times);
    return times;
}

/**
 * Funcion encargada de obtener las prioridades de las hormigas y colocarlos en una lista de caracteres
 * Recibe la lista a analizar
 * Retorna el puntero a esa lista
 */
char* queuePriority(queue* list) {
    priorities=malloc(sizeof(char)*20);
    if (priorities == NULL) {
        printf("Error, no se pudo alocar memoria");
        exit(EXIT_FAILURE);
    }
    char priority[50];
    queueNode *tmp = list->head;
    ant_t* ant;
    while (tmp!=NULL)
    {
        ant=(ant_t*) tmp->item;
        sprintf( priority, "%d", ant->priority );
        strcat(priorities, priority);
        tmp = tmp->next;
    }
    //printf("tiempos1: %s\n: ",priorities);
    return priorities;
}

/**
 * Funcion encargada de ordenar la lista por prioridad de las hormigas
 * Recibe la lista a ordenar
 * Retorna 0 en caso de cambios, 1 en caso de que no
 */
int prioritySchedule(queue* list) {
    int changes=0;
    char priority_init[50]="";
	strcat(priority_init,queuePriority(list));
    free(priorities);
    queueNode* tmp=list->head;
    queueNode* tmp_next;
    queueNode* tmp_prev;
    ant_t* ant_actual;
    ant_t* ant_next;
    ant_t* ant_ante;
    int swapped;
  
    if (tmp == NULL)
        return 1;
    do{    
        swapped = 0;
        tmp=list->head;
        while(tmp->next!=NULL){
            tmp_next=tmp->next;
            tmp_prev=tmp->prev;
            ant_actual=(ant_t*)tmp->item;
            ant_next=(ant_t*)tmp_next->item;
            if(tmp->prev!=NULL){
                ant_ante=(ant_t*)tmp_prev->item;
            }
            if(ant_actual->priority < ant_next->priority){
                swapped=1;
                if(tmp==list->head){
                    if(tmp_next->next!=NULL){
                        tmp_next->next->prev=tmp;
                    }
                    list->head=tmp_next;
                    tmp->next=tmp_next->next;
                    tmp_next->next=tmp;
                    tmp->prev=list->head;
                }
                else{
                    if(tmp_next->next!=NULL){
                        tmp_next->next->prev=tmp;
                    }
                    tmp_prev->next=tmp_next;
                    tmp->next=tmp_next->next;
                    tmp_next->prev=tmp_prev;
                    tmp->prev=tmp_next;
                    tmp_next->next=tmp;
                }
            }
            else{
                tmp=tmp->next;
            }
        }

    } 
    while(swapped);
    char priority_final[50]="";
	strcat(priority_final,queuePriority(list)); 
    free(priorities);
    if(strcmp(priority_init,priority_final)==0){
        changes=1;
    }
    return changes;  
}

/**
 * Funcion encargada de ordenar la lista por tiempos de las hormigas
 * Recibe la lista a ordenar
 * Retorna 0 en caso de cambios, 1 en caso de que no
 */
int sjfSchedule(queue* list) {
    int changes=0;
    char times_init[50]="";
	strcat(times_init,queueTimes(list));
    free(times);
    queueNode* tmp=list->head;
    queueNode* tmp_next;
    queueNode* tmp_prev;
    ant_t* ant_actual;
    ant_t* ant_next;
    ant_t* ant_ante;
    int swapped;
  
    if (tmp == NULL)
        return 1;
    do{     
        swapped = 0;
        tmp=list->head;
        while(tmp->next!=NULL){
            tmp_next=tmp->next;
            tmp_prev=tmp->prev;
            ant_actual=(ant_t*)tmp->item;
            ant_next=(ant_t*)tmp_next->item;
            if(tmp->prev!=NULL){
                ant_ante=(ant_t*)tmp_prev->item;
            }
            if(ant_actual->time > ant_next->time){
                swapped=1;
                if(tmp==list->head){
                    if(tmp_next->next!=NULL){
                        tmp_next->next->prev=tmp;
                    }
                    list->head=tmp_next;
                    tmp->next=tmp_next->next;
                    tmp_next->next=tmp;
                    tmp->prev=list->head;
                }
                else{
                    if(tmp_next->next!=NULL){
                        tmp_next->next->prev=tmp;
                    }
                    tmp_prev->next=tmp_next;
                    tmp->next=tmp_next->next;
                    tmp_next->prev=tmp_prev;
                    tmp->prev=tmp_next;
                    tmp_next->next=tmp;
                }
            }
            else{
                tmp=tmp->next;
            }
        }

    } 
    while(swapped);
    char times_final[50]="";
	strcat(times_final,queueTimes(list)); 
    free(times);
    if(strcmp(times_init,times_final)==0){
        changes=1;
    }
    return changes;    
}

/**
 * Funcion encargada de imprimir las prioridad de las hormigas
 * Recibe la lista de hormigas
 */
void queuePrintPriority(queue* list){
    queueNode *tmp = list->head;
    ant_t* ant;
    while (tmp!=NULL)
    {
        ant=(ant_t*) tmp->item;
        printf("prioridad: %d\n",ant->priority);
        tmp = tmp->next;
    }
}

/**
 * Funcion encargada de imprimir los tiempos de las hormigas
 * Recibe la lista de hormigas
 */
void queuePrintTime(queue* list){
    queueNode *tmp = list->head;
    ant_t* ant;
    while (tmp!=NULL)
    {
        ant=(ant_t*) tmp->item;
        printf("tiempo: %d\n",ant->time);
        tmp = tmp->next;
    }
}
#include "antScheduler.h"

void scheduleAntsAux(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, int dest);
void scheduleChannel(int scheduler, queue* list);
//oid prioritySchedule(queue* list);
//void sjfSchedule(queue* list);


void queuePrintPriority(queue* list){
    queueNode *tmp = list->head;
    ant_t* ant;
    while (tmp!=NULL)
    {
        ant=(ant_t*) tmp->item;
        printf("prioridad: %d\n",ant->priority);
        //printf("tiempo: %d\n",ant->time);
        tmp = tmp->next;
    }
}

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

void scheduleAnts(int channel, int dest) {
    switch (channel) {
        case 1:
            scheduleAntsAux(channel1, &channel1LeftQueue, &channel1RightQueue, dest);
            break;
        
        case 2:
            scheduleAntsAux(channel2, &channel2LeftQueue, &channel2RightQueue, dest);
            break;

        case 3:
            scheduleAntsAux(channel3, &channel3LeftQueue, &channel3RightQueue, dest);
            break;
    }

    return;
}

void scheduleAntsAux(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, int dest) {
    switch (dest) {
        case 0:
            if (channelRightQueue->count > 1) {
                scheduleChannel(channel->scheduler, channelRightQueue);
            }
            break;
        case 1:
            if (channelLeftQueue->count > 1) {
                scheduleChannel(channel->scheduler, channelLeftQueue);
            }
            break;
    }

    return;
}

void scheduleChannel(int scheduler, queue* list) {
    switch (scheduler) {
        case 1:
            prioritySchedule(list);
            break;
        case 2:
            sjfSchedule(list);
            break;
    }

    return;
}

void prioritySchedule(queue* list) {
    queueNode* tmp=list->head;
    queueNode* tmp_next;
    queueNode* tmp_prev;
    ant_t* ant_actual;
    ant_t* ant_next;
    ant_t* ant_ante;
    int swapped;
  
    if (tmp == NULL)
        return;
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
}

void sjfSchedule(queue* list) {
    queueNode* tmp=list->head;
    queueNode* tmp_next;
    queueNode* tmp_prev;
    ant_t* ant_actual;
    ant_t* ant_next;
    ant_t* ant_ante;
    int swapped;
  
    if (tmp == NULL)
        return;
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
}
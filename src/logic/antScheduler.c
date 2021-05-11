#include "antScheduler.h"

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

void scheduleAnts() {
    //CEmutex_lock(&channel1DataMutex);
    if (channel1->scheduler == 1 || channel1->scheduler == 2) {
        if (channel1LeftQueue.count > 1) {
            scheduleChannel(channel1->scheduler, &channel1LeftQueue);
        }
        if (channel1RightQueue.count > 1) {
            scheduleChannel(channel1->scheduler, &channel1RightQueue);
        }
    }
    //CEmutex_unlock(&channel1DataMutex);

    //CEmutex_lock(&channel2DataMutex);
    if (channel2->scheduler == 1 || channel2->scheduler == 2) {
        if (channel2LeftQueue.count > 1) {
            scheduleChannel(channel2->scheduler, &channel2LeftQueue);
        }
        if (channel2RightQueue.count > 1) {
            scheduleChannel(channel2->scheduler, &channel2RightQueue);
        }
    }
    //CEmutex_unlock(&channel2DataMutex);

    //CEmutex_lock(&channel3DataMutex);
    if (channel3->scheduler == 1 || channel3->scheduler == 2) {
        if (channel3LeftQueue.count > 1) {
            scheduleChannel(channel3->scheduler, &channel3LeftQueue);
        }
        if (channel3RightQueue.count > 1) {
            scheduleChannel(channel3->scheduler, &channel3RightQueue);
        }
    }
    //CEmutex_unlock(&channel3DataMutex);

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
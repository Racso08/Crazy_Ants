#include "antScheduler.h"

int scheduleAntsAux(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, int dest);
int scheduleChannel(int scheduler, queue* list);
void queuePrintPriority(queue* list);
void queuePrintTime(queue* list);
char* queueTimes(queue* list);
char* queuePriority(queue* list);
int prioritySchedule(queue* list);
int sjfSchedule(queue* list);

int scheduleAnts(int channel, int dest) {
    switch (channel) {
        case 1:
            return scheduleAntsAux(channel1, &channel1LeftQueue, &channel1RightQueue, dest);
            break;
        
        case 2:
            return scheduleAntsAux(channel2, &channel2LeftQueue, &channel2RightQueue, dest);
            break;

        case 3:
            return scheduleAntsAux(channel3, &channel3LeftQueue, &channel3RightQueue, dest);
            break;
    }

    return 1;
}

int scheduleAntsAux(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, int dest) {
    switch (dest) {
        case 0:
            if (channelRightQueue->count > 1) {
                channel->scheduled = scheduleChannel(channel->scheduler, channelRightQueue);
                return channel->scheduled;
            }
            break;
        case 1:
            if (channelLeftQueue->count > 1) {
                channel->scheduled = scheduleChannel(channel->scheduler, channelLeftQueue);
                return channel->scheduled;
            }
            break;
    }

    return 1;
}

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

char* queueTimes(queue* list) {
    char *times=malloc(sizeof(char)*20);
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
    printf("tiempos1: %s\n: ",times);
    return times;
}

char* queuePriority(queue* list) {
    char *priorities=malloc(sizeof(char)*20);
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
    printf("tiempos1: %s\n: ",priorities);
    return priorities;
}

int prioritySchedule(queue* list) {
    int changes=0;
    char priority_init[50]="";
	strcat(priority_init,queuePriority(list));
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
    if(strcmp(priority_init,priority_final)==0){
        changes=1;
    }
    return changes;  
}

int sjfSchedule(queue* list) {
    int changes=0;
    char times_init[50]="";
	strcat(times_init,queueTimes(list));
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
    if(strcmp(times_init,times_final)==0){
        changes=1;
    }
    return changes;    
}

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
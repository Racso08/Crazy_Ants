#include "antScheduler.h"

void scheduleChannel(int scheduler, queue* list);
void prioritySchedule(queue* list);
void sjfSchedule(queue* list);

void scheduleAnts() {
    if (channel1->scheduler == 1 || channel1->scheduler == 2) {
        if (channel1LeftQueue.count > 1) {
            scheduleChannel(channel1->scheduler, &channel1LeftQueue);
        }
        if (channel1RightQueue.count > 1) {
            scheduleChannel(channel1->scheduler, &channel1RightQueue);
        }
    }
    if (channel2->scheduler == 1 || channel2->scheduler == 2) {
        if (channel2LeftQueue.count > 1) {
            scheduleChannel(channel2->scheduler, &channel2LeftQueue);
        }
        if (channel2RightQueue.count > 1) {
            scheduleChannel(channel2->scheduler, &channel2RightQueue);
        }
    }
    if (channel3->scheduler == 1 || channel3->scheduler == 2) {
        if (channel3LeftQueue.count > 1) {
            scheduleChannel(channel3->scheduler, &channel3LeftQueue);
        }
        if (channel3RightQueue.count > 1) {
            scheduleChannel(channel3->scheduler, &channel3RightQueue);
        }
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
    //Aqui va el codigo de prioridad
    printf("Prioridad\n");
    return;
}

void sjfSchedule(queue* list) {
    //Aqui va el codigo de sjf
    printf("SJF\n");
    return;
}
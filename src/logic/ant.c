#include "ant.h"

typedef struct {
    int posX;
    int posY;
    int dest;
    int type;
    int channel;
    int time;
    int priority;
    CEthread_t* thread;
} ant_t;

void addAntToQueue(ant_t* ant);
void* startAnt(void* arg);

void createAnt(int channel, int priority, int time, int dest, int type) {
    CEthread_t thread;

    ant_t* ant = (ant_t*) malloc(sizeof(ant_t));
    ant->posX = 0;
    ant->posY = 0;
    ant->dest = dest;
    ant->type = type;
    ant->channel = channel;
    ant->time = time;
    ant->priority = priority;
    ant->thread = &thread;

    CEthread_create(&thread, startAnt, (void*) ant);
    CEthread_detach(thread);

    addAntToQueue(ant);

    return;
}

void addAntToQueue(ant_t* ant) {
    switch (ant->channel) {
    case 1:
        switch (ant->dest) {
        case 0:
            queueAddItem(&channel1RightQueue, ant);
            break;
        case 1:
            queueAddItem(&channel1LeftQueue, ant);
            break;
        }
        break;
    case 2:
        switch (ant->dest) {
        case 0:
            queueAddItem(&channel2RightQueue, ant);
            break;
        case 1:
            queueAddItem(&channel2LeftQueue, ant);
            break;
        }
        break;
    case 3:
        switch (ant->dest) {
        case 0:
            queueAddItem(&channel3RightQueue, ant);
            break;
        case 1:
            queueAddItem(&channel3LeftQueue, ant);
            break;
        }
        break;
    }

    return;
}

void* startAnt(void* arg) {
    ant_t* ant = (ant_t*) arg;
    
    printf("Soy la hormiga: posX %d, posY %d, dest %d, type %d, channel %d, time %d, priority %d, thread %d\n", ant->posX, ant->posY, ant->dest, ant->type, ant->channel, ant->time, ant->priority, *(ant->thread));
    
    while (1) {
        printf("AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH1\n");
        CEthread_yield();
        printf("AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH2\n");
    }
    
    return NULL;
}
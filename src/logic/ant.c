#include "ant.h"

int checkIfIsPossibleToAddAnt(int channel, int dest);
void addAntToQueue(ant_t* ant);
void* startAnt(void* arg);

int currentAntAmount = 0;
CEthread_t thread[MAXANTS];

void createAnt(int posX, int posY, int channelLenght, int dest, int type, int channel, int time, int priority, int finalDest, int path) {
    if (checkIfIsPossibleToAddAnt(channel, dest) < 0) {
        printf("Error, no es posible agregar la hormiga en el canal deseado\n");
        return;
    }
    if (currentAntAmount == MAXANTS) {
        printf("Error, se ha alcanzada la maxima cantidad de hormigas del programa\n");
        return;
    }

    ant_t* ant = (ant_t*) malloc(sizeof(ant_t));
    ant->posX = posX;
    ant->posY = posY;
    ant->vel = channelLenght / time;
    ant->currentChannelPosition = 0;
    ant->channelLenght = channelLenght;
    ant->dest = dest;
    ant->type = type;
    ant->channel = channel;
    ant->time = time;
    ant->priority = priority;
    ant->inChannel = 0;
    ant->finalDest = finalDest;
    ant->path = path;
    ant->thread = &(thread[currentAntAmount]);

    CEthread_create(&(thread[currentAntAmount]), startAnt, (void*) ant);
    CEthread_detach(thread[currentAntAmount]);

    addAntToQueue(ant);
    queueAddItem(&allAnts, ant);

    currentAntAmount++;

    return;
}

int checkIfIsPossibleToAddAnt(int channel, int dest) {
    switch (channel) {
        case 1:
            switch (dest) {
            case 0:
                if (channel1RightQueue.count < channel1->antAmount) {
                    return 0;
                }
                break;
            case 1:
                if (channel1LeftQueue.count < channel1->antAmount) {
                    return 0;
                }
                break;
            }
            break;
        case 2:
            switch (dest) {
            case 0:
                if (channel2RightQueue.count < channel2->antAmount) {
                    return 0;
                }
                break;
            case 1:
                if (channel2LeftQueue.count < channel2->antAmount) {
                    return 0;
                }
                break;
            }
            break;
        case 3:
            switch (dest) {
            case 0:
                if (channel3RightQueue.count < channel3->antAmount) {
                    return 0;
                }
                break;
            case 1:
                if (channel3LeftQueue.count < channel3->antAmount) {
                    return 0;
                }
                break;
            }
            break;
    }

    return -1;
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
    
    int id = *(ant->thread) - 1;
    int channel = ant->channel;

    printf("Soy la hormiga %d y estoy esperando a cruzar el canal %d\n", id, channel);
    
    while (1) {
        CEthread_yield();
        if (ant->inChannel == 1) {
            break;
        }
    }

    printf("Soy la hormiga %d y estoy cruzando el canal %d\n", id, channel);

    while (1) {
        ant->currentChannelPosition += ant->vel;
        if (ant->currentChannelPosition >= ant->channelLenght) {
            ant->inChannel = 0;
            break;
        }
        CEthread_yield();
    }

    printf("Soy la hormiga %d y ya cruce el canal %d\n", id, channel);

    free(ant);
    
    return NULL;
}
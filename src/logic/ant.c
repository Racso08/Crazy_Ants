#include "ant.h"

int currentAntAmount = 0;

int checkIfIsPossibleToAddAnt(int channel, int dest);
void addAntToQueue(ant_t* ant);
void* startAnt(void* arg);

void createAnt(int posX, int posY, int channel, int channelLenght, int priority, int time, int dest, int type) {
    if (checkIfIsPossibleToAddAnt(channel, dest) < 0) {
        printf("Error, no es posible agregar la hormiga en el canal deseado");
        return;
    }
    if (currentAntAmount == MAXANTS) {
        printf("Error, se ha alcanzada la maxima cantidad de hormigas del programa");
        return;
    }

    CEthread_t thread[MAXANTS];

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
    ant->thread = &(thread[currentAntAmount]);

    CEthread_create(&(thread[currentAntAmount]), startAnt, (void*) ant);
    CEthread_detach(thread[currentAntAmount]);

    addAntToQueue(ant);

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
            //CEmutex_lock(&channel1DataMutex);
            switch (ant->dest) {
            case 0:
                queueAddItem(&channel1RightQueue, ant);
                break;
            case 1:
                queueAddItem(&channel1LeftQueue, ant);
                break;
            }
            //CEmutex_unlock(&channel1DataMutex);
            break;
        case 2:
            //CEmutex_lock(&channel2DataMutex);
            switch (ant->dest) {
            case 0:
                queueAddItem(&channel2RightQueue, ant);
                break;
            case 1:
                queueAddItem(&channel2LeftQueue, ant);
                break;
            }
            //CEmutex_unlock(&channel2DataMutex);
            break;
        case 3:
            //CEmutex_lock(&channel3DataMutex);
            switch (ant->dest) {
            case 0:
                queueAddItem(&channel3RightQueue, ant);
                break;
            case 1:
                queueAddItem(&channel3LeftQueue, ant);
                break;
            }
            //CEmutex_unlock(&channel3DataMutex);
            break;
    }

    return;
}

void* startAnt(void* arg) {
    ant_t* ant = (ant_t*) arg;
    
    printf("Soy la hormiga %d y estoy esperando a cruzar el canal %d\n", *(ant->thread) - 1, ant->channel);
    
    while (ant->inChannel == 0) {
        CEthread_yield();
    }

    printf("Soy la hormiga %d y estoy cruzando el canal %d\n", *(ant->thread) - 1, ant->channel);

    while (ant->inChannel = 1) {
        ant->currentChannelPosition += ant->vel;
        if (ant->currentChannelPosition >= ant->channelLenght) {
            ant->inChannel = 0;
        }

        CEthread_yield();
    }

    printf("Soy la hormiga %d y ya cruce el canal %d\n", *(ant->thread) - 1, ant->channel);

    free(ant);
    
    return NULL;
}
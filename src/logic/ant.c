#include "ant.h"

int checkIfIsPossibleToAddAnt(int channel, int dest);
void addAntToQueue(ant_t* ant);
int queueGetValueInPosition(queue* list, int position);
void* startAnt(void* arg);
int waze(ant_t *ant);

int currentAntAmount = 0;
CEthread_t thread[MAXANTS];

void initializePositions() {
    queueInit(&izq1);
    queueInit(&izq2);
    queueInit(&izq3);
    queueInit(&der1);
    queueInit(&der2);
    queueInit(&der3);

    int antsChannel1 = channel1->antAmount;
    int antsChannel2 = channel2->antAmount;
    int antsChannel3 = channel3->antAmount;

    int currentIzq = 450;
    int currentDer = 750;
    int i;

    for (i = 0; i < antsChannel1; i++) {
        queueAddItem(&izq1, (void*) (__intptr_t) currentIzq);
        queueAddItem(&der1, (void*) (__intptr_t) currentDer);

        currentIzq -= 28;
        currentDer += 28;
    }

    currentIzq = 450;
    currentDer = 750;

    for (i = 0; i < antsChannel2; i++) {
        queueAddItem(&izq2, (void*) (__intptr_t) currentIzq);
        queueAddItem(&der2, (void*) (__intptr_t) currentDer);

        currentIzq -= 28;
        currentDer += 28;
    }

    currentIzq = 350;
    currentDer = 750;

    for (i = 0; i < antsChannel3; i++) {
        queueAddItem(&izq3, (void*) (__intptr_t) currentIzq);
        queueAddItem(&der3, (void*) (__intptr_t) currentDer);

        currentIzq -= 28;
        currentDer += 28;
    }

    return;
}

void createAnt(int posX, int posY, int channelLenght, int dest, int type, int channel, int time, int priority) {
    int finalDest = checkIfIsPossibleToAddAnt(channel, dest);
    if (finalDest < 0) {
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
    ant->path = 0;
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
                    return queueGetValueInPosition(&der1, channel1RightQueue.count);
                }
                break;
            case 1:
                if (channel1LeftQueue.count < channel1->antAmount) {
                    return queueGetValueInPosition(&izq1, channel1LeftQueue.count);
                }
                break;
            }
            break;
        case 2:
            switch (dest) {
            case 0:
                if (channel2RightQueue.count < channel2->antAmount) {
                    return queueGetValueInPosition(&der2, channel2RightQueue.count);
                }
                break;
            case 1:
                if (channel2LeftQueue.count < channel2->antAmount) {
                    return queueGetValueInPosition(&izq2, channel2LeftQueue.count);
                }
                break;
            }
            break;
        case 3:
            switch (dest) {
            case 0:
                if (channel3RightQueue.count < channel3->antAmount) {
                    return queueGetValueInPosition(&der3, channel3RightQueue.count);
                }
                break;
            case 1:
                if (channel3LeftQueue.count < channel3->antAmount) {
                    return queueGetValueInPosition(&izq3, channel3LeftQueue.count);
                }
                break;
            }
            break;
    }

    return -1;
}

int queueGetValueInPosition(queue* list, int position) {
    queueNode* node = (queueNode*) list->head;
    
    int i = 0;
    while (i != position) {
        node = node->next;
        i++;
    }

    return (int) (__intptr_t) node->item;   
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

    printf("Posx: %d, Posy: %d, Vel: %d, CurrentchannelPos: %d, ChannelLenght: %d, Dest: %d, Type: %d, Channel: %d, Time: %d, Priority: %d, InChannel: %d, FinalDest: %d, Path: %d\n", ant->posX, ant->posY, ant->vel, ant->currentChannelPosition, ant->channelLenght, ant->dest, ant->type, ant->channel, ant->time, ant->priority, ant->inChannel, ant->finalDest, ant->path);
    
    while (1) {
        if (waze(ant) > 0) {
            break;
        }
        CEthread_yield();
    }

    int id = *(ant->thread) - 1;
    int channel = ant->channel;

    printf("Soy la hormiga %d y estoy esperando a cruzar el canal %d\n", id, channel);

    while (1) {
        if (ant->inChannel == 1) {
            break;
        }
        CEthread_yield();
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

int waze(ant_t *ant) {

    if(ant->dest == 1){

        if(ant->path == 0){
            if (ant->posX < 200){
                ant->posX += 2;
            }else{
                ant->path = 1;
            }
            return 0;
        }

        else if(ant->path == 1){
            if(ant->channel == 1){
                if(ant->posY != 340){
                    ant->posY -= 2;
                }else{
                    ant->path = 2;
                }
            }

            if(ant->channel == 2){
                ant->path = 2;
            }

            if(ant->channel == 3){
                if(ant->posY != 540){
                    ant->posY += 2;
                }else{
                    ant->path = 2;
                }
            }
            return 0;
        }

        else if(ant->path == 2){
            if(ant->posX < ant->finalDest){
                ant->posX += 2;
            }
            else if(ant->posX > ant->finalDest){
                ant->posX -= 2;
            }
            else{
                return 1;
            }
            return 0;
        }

        else if(ant->path == 3){
            if(ant->posX < 1000){
                ant->posX += 2;
            }
            else{
                ant->path = 4;
            }
            return 0;
        }

        else if(ant->path == 4){
            if(ant->posY < 440){
                ant->posY += 2;
            }
            else if(ant->posY > 440){
                ant->posY -= 2;
            }
            else{
                ant->path = 5;
            }
            return 0;
        }

        else if(ant->path == 5){
            if(ant->posX < 1100){
                ant->posX += 2;
            }
            else{
                ant->path = 6;
            }
            return 0;
        }

        else{
            return 1;
        } 
    }
    else if(ant->dest == 0){
    
        if(ant->path == 0){
            if (ant->posX > 900){
                ant->posX -= 2;
            }else{
                ant->path = 1;
            }
            return 0;
        }

        else if(ant->path == 1){
            if(ant->channel == 1){
                if(ant->posY != 340){
                    ant->posY -= 2;
                }else{
                    ant->path = 2;
                }
            }

            if(ant->channel == 2){
                ant->path = 2;
            }

            if(ant->channel == 3){
                if(ant->posY != 540){
                    ant->posY += 2;
                }else{
                    ant->path = 2;
                }
            }
            return 0;
        }

        else if(ant->path == 2){
            if(ant->posX < ant->finalDest){
                ant->posX += 2;
            }
            else if(ant->posX > ant->finalDest){
                ant->posX -= 2;
            }
            else{
                return 1;
            }
            return 0;
        }

        else if(ant->path == 3){
            if(ant->posX > 200){
                ant->posX -= 2;
            }
            else{
                ant->path = 4;
            }
            return 0;
        }

        else if(ant->path == 4){
            if(ant->posY < 440){
                ant->posY += 2;
            }
            else if(ant->posY > 440){
                ant->posY -= 2;
            }
            else{
                ant->path = 5;
            }
            return 0;
        }

        else if(ant->path == 5){
            if(ant->posX > 100){
                ant->posX -= 2;
            }
            else{
                ant->path = 6;
            }
            return 0;
        }

        else{
            return 1;
        } 

    }

    else{
        printf("[ERROR] Destination not valid");
    }
}
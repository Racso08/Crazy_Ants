#include "ant.h"

int checkIfIsPossibleToAddAnt(int channel, int dest);
int createAntAux(int posX, int posY, int vel, int channelLenght, int dest, int type, int channel, int time, int priority);
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

    int currentIzq = 520;
    int currentDer = 770;
    int i;

    for (i = 0; i < antsChannel1; i++) {
        queueAddItem(&izq1, (void*) (__intptr_t) currentIzq);
        queueAddItem(&der1, (void*) (__intptr_t) currentDer);

        currentIzq -= 28;
        currentDer += 28;
    }

    currentIzq = 410;
    currentDer = 730;

    for (i = 0; i < antsChannel2; i++) {
        queueAddItem(&izq2, (void*) (__intptr_t) currentIzq);
        queueAddItem(&der2, (void*) (__intptr_t) currentDer);

        currentIzq -= 28;
        currentDer += 28;
    }

    currentIzq = 370;
    currentDer = 850;

    for (i = 0; i < antsChannel3; i++) {
        queueAddItem(&izq3, (void*) (__intptr_t) currentIzq);
        queueAddItem(&der3, (void*) (__intptr_t) currentDer);

        currentIzq -= 28;
        currentDer += 28;
    }

    return;
}

int createAnt(int type, int channel, int dest, int extra) {
    int posX;
    int posY;
    int channelLenght;
    int channelTrueLenght;
    int channelTime;
    int priority;
    int vel;

    switch (dest) {
        case 0:
            posX = 1100;
            posY = 440;
            break;
        case 1:
            posX = 80;
            posY= 440;
            break;
    }

    switch (channel) {
        case 1:
            channelLenght = channel1->lenght;  
            channelTrueLenght = channel1->trueLenght;
        
            if(channel1->scheduler == 1){
                priority = extra;
                channelTime = -1;
            }else if(channel1->scheduler == 2 || channel1->scheduler == 4){
                channelTime = extra;
                priority = -1;
            }else {
                channelTime = -1;
                priority = -1;
            }
            break;
        case 2:
            channelLenght = channel2->lenght; 
            channelTrueLenght = channel2->trueLenght;

            if(channel2->scheduler == 1){
                priority = extra;
                channelTime = -1;
            }else if(channel2->scheduler == 2 || channel2->scheduler == 4){
                channelTime = extra;
                priority = -1;
            }else {
                channelTime = -1;
                priority = -1;
            }

            break;
        case 3:
            channelLenght = channel3->lenght; 
            channelTrueLenght = channel3->trueLenght;

            if(channel3->scheduler == 1){
                priority = extra;
                channelTime = -1;
            }else if(channel3->scheduler == 2 || channel3->scheduler == 4){
                channelTime = extra;
                priority = -1;
            }else {
                channelTime = -1;
                priority = -1;
            } 

            break;
    }

    if (type == 2) {
        channelTime = extra;
    }

    int scale = (int) (channelTrueLenght / channelLenght);

    vel = (int) (channelLenght * scale / channelTime);

    switch (type) {
        case 0:
            if (vel < 0) {
                vel = (int) (channelLenght * scale / 100);
            }
            
            break;
        case 1:
            if (vel < 0) {
                vel = (int) channelLenght * scale / 80;
            }

            break;
        case 2:
            if (vel < 0) {
                vel = (int) channelLenght * scale / 60;
            }
    }

    return createAntAux(posX, posY, vel, channelTrueLenght, dest, type, channel, channelTime, priority);
}

int createAntAux(int posX, int posY, int vel, int channelLenght, int dest, int type, int channel, int time, int priority) {
    int finalDest = checkIfIsPossibleToAddAnt(channel, dest);
    if (finalDest < 0) {
        printf("Error, no es posible agregar la hormiga en el canal deseado\n");
        return 0;
    }
    if (currentAntAmount == MAXANTS) {
        printf("Error, se ha alcanzada la maxima cantidad de hormigas del programa\n");
        return 0;
    }

    ant_t* ant = (ant_t*) malloc(sizeof(ant_t));
    ant->posX = posX;
    ant->posY = posY;
    ant->vel = vel;
    ant->currentChannelPosition = 0;
    ant->channelLenght = channelLenght;
    ant->dest = dest;
    ant->type = type;
    ant->channel = channel;
    ant->time = time;
    ant->priority = priority;
    ant->inChannel = 0;
    ant->waiting = 0;
    ant->finalDest = finalDest;
    ant->path = 0;
    ant->thread = &(thread[currentAntAmount]);

    CEthread_create(&(thread[currentAntAmount]), startAnt, (void*) ant);
    CEthread_detach(thread[currentAntAmount]);

    addAntToQueue(ant);
    queueAddItem(&allAnts, ant);

    currentAntAmount++;

    return 1;
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

    while (1) {
        if (waze(ant) > 0) {
            break;
        }
        CEthread_yield();
    }

    int id = *(ant->thread) - 1;
    int channel = ant->channel;
    ant->waiting = 1;

    printf("Soy la hormiga %d y estoy esperando a cruzar el canal %d\n", id, channel);

    while (1) {
        if (ant->inChannel == 1) {
            ant->waiting = 0;
            break;
        }
        CEthread_yield();
    }

    printf("Soy la hormiga %d y estoy cruzando el canal %d\n", id, channel);

    while (1) {
        ant->currentChannelPosition += ant->vel;
        if (ant->dest == 0) {
            ant->posX -= ant->vel;
        }
        else {
            ant->posX += ant->vel;
        }
        
        if (ant->currentChannelPosition >= ant->channelLenght) {
            ant->inChannel = 0;
            break;
        }
        CEthread_yield();
    }

    printf("Soy la hormiga %d y ya cruce el canal %d\n", id, channel);

    ant->path = 3;

    while (1) {
        if (waze(ant) > 0) {
            break;
        }
        CEthread_yield();
    }
    
    return NULL;
}

int waze(ant_t *ant) {

    if(ant->dest == 1){

        if(ant->path == 0){
            if (ant->posX < 200){
                ant->posX += ant->vel;
            }else{
                ant->path = 1;
            }
            return 0;
        }

        else if(ant->path == 1){
            if(ant->channel == 1){
                if(ant->posY > 300){
                    ant->posY -= ant->vel;
                }else{
                    ant->path = 2;
                }
            }

            if(ant->channel == 2){
                if(ant->posY > 370){
                    ant->posY -= ant->vel;
                }else{
                    ant->path = 2;
                }
            }

            if(ant->channel == 3){
                if(ant->posY < 490){
                    ant->posY += ant->vel;
                }else{
                    ant->path = 2;
                }
            }
            return 0;
        }

        else if(ant->path == 2){
            if(ant->posX < ant->finalDest){
                ant->posX += ant->vel;
            }
            //else if(ant->posX > ant->finalDest){
            //    ant->posX -= ant->vel;
            //}
            else{
                return 1;
                //ant->path = 3;
            }
            return 0;
        }

        else if(ant->path == 3){
            if(ant->posX < 1000){
                ant->posX += ant->vel;
            }
            else{
                ant->path = 4;
            }
            return 0;
        }

        else if(ant->path == 4){

            if(ant->channel==1 && ant->posY < 370){
                ant->posY += ant->vel;  
            }
            
            else if(ant->channel == 2 && ant->posY > 370){
                ant->posY -= ant->vel;
            }

            else if(ant->channel == 3 && ant->posY > 370){
                ant->posY -= ant->vel;
            }

            else{
                ant->path = 5;
            }

            return 0;
        }

        else if(ant->path == 5){
            if(ant->posX < 1200){
                ant->posX += ant->vel;
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
            if (ant->posX > 980){
                ant->posX -= ant->vel;
            }else{
                ant->path = 1;
            }
            return 0;
        }

        else if(ant->path == 1){
            if(ant->channel == 1){
                if(ant->posY > 320){
                    ant->posY -= ant->vel;
                }else{
                    ant->path = 2;
                }
            }

            if(ant->channel == 2){
                if(ant->posY > 390){
                    ant->posY -= ant->vel;
                }else{
                    ant->path = 2;
                }
            }

            if(ant->channel == 3){
                if(ant->posY < 510){
                    ant->posY += ant->vel;
                }else{
                    ant->path = 2;
                }
            }
            return 0;
        }

        else if(ant->path == 2){
            if(ant->posX > ant->finalDest){
                ant->posX -= ant->vel;
            }
            //else if(ant->posX > ant->finalDest){
            //    ant->posX -= ant->vel;
            //}
            else{
                //ant->path = 3;
                return 1;
            }
            return 0;
        }

        else if(ant->path == 3){
            if(ant->posX > 200){
                ant->posX -= ant->vel;
            }
            else{
                ant->path = 4;
            }
            return 0;
        }

        else if(ant->path == 4){
            if(ant->channel==1 && ant->posY < 390){
                ant->posY += ant->vel;  
            }

            else if(ant->channel == 3 && ant->posY > 390){
                ant->posY -= ant->vel;
            }
            else{
                ant->path = 5;
            }
            return 0;
        }

        else if(ant->path == 5){
            if(ant->posX > 0){
                ant->posX -= ant->vel;
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
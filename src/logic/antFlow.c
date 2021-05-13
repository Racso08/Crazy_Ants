#include "antFlow.h"

void manageFlow(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts, long elapsedSignTime, struct timespec signBegin, struct timespec signEnd);

void setNextAntsPositions();

void equity(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts);
void equityAux(channel_t* channel, queue* channelQueue, queue* currentChannelAnts);
void equityRR(channel_t* channel, queue* channelQueue, ant_t* currentChannelAnt);
void equityFCFS(channel_t* channel, queue* channelQueue, queue* currentChannelAnts);

void sign(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, ant_t* currentChannelAnt, long elapsedSignTime, struct timespec signBegin, struct timespec signEnd);
void tico(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, ant_t* currentChannelAnt);
void signAndticoAux(channel_t* channel, queue* channelQueue, ant_t* currentChannelAnt);

queue currentChannel1Ants;
queue currentChannel2Ants;
queue currentChannel3Ants;

long elapsedSign1Time = 0;
long elapsedSign2Time = 0;
long elapsedSign3Time = 0;
int setupQueues = 0;

void moveAnts() {
    if (setupQueues == 0) {
        queueInit(&currentChannel1Ants);
        queueInit(&currentChannel2Ants);
        queueInit(&currentChannel3Ants);
        setupQueues = 1;
    }

    setNextAntsPositions(&channel1LeftQueue);

    manageFlow(channel1, &channel1LeftQueue, &channel1RightQueue, &currentChannel1Ants, elapsedSign1Time, sign1Begin, sign1End);
    manageFlow(channel2, &channel2LeftQueue, &channel2RightQueue, &currentChannel2Ants, elapsedSign2Time, sign2Begin, sign2End);
    manageFlow(channel3, &channel3LeftQueue, &channel3RightQueue, &currentChannel3Ants, elapsedSign3Time, sign3Begin, sign3End);

    return;
}

void setNextAntsPositions(queue* list) {
    int n = list->count;
    if (n > 0) {
        queueNode* node = (queueNode*) list->head;
        ant_t* nextAnt = (ant_t*) node->item;

        nextAnt->nextAntPosX = -1;
        nextAnt->nextAntPosY = -1;

        if (node->next == NULL) {
            return;
        }

        ant_t* prevAnt;

        while (node->next != NULL) {
            prevAnt = (ant_t*) node->next->item;
            prevAnt->nextAntPosX = nextAnt->posX;
            prevAnt->nextAntPosY = nextAnt->posY;

            nextAnt = prevAnt;

            node = node->next;
        }
    }

    return;
}

void manageFlow(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts, long elapsedSignTime, struct timespec signBegin, struct timespec signEnd) {
    switch (channel->flow) {
        case 0:
            equity(channel, channelLeftQueue, channelRightQueue, currentChannelAnts);
            break;
        case 1:
           // sign(channel, channelLeftQueue, channelRightQueue, currentChannelAnt, elapsedSignTime, signBegin, signEnd);
            break;
        case 2:
           // tico(channel, channelLeftQueue, channelRightQueue, currentChannelAnt);
            break;
    }

    return;
}

void equity(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts) {
    if (channelLeftQueue->count == 0 && channelRightQueue->count > 0) {
        channel->currentW = 0;
        channel->sign = 1;
        equityAux(channel, channelRightQueue, currentChannelAnts);
    }
    else if (channelLeftQueue->count > 0 && channelRightQueue->count == 0) {
        channel->currentW = 0;
        channel->sign = 0;
        equityAux(channel, channelLeftQueue, currentChannelAnts);
    }
    else {
        switch (channel->sign) {
            case 0:
                equityAux(channel, channelLeftQueue, currentChannelAnts);
                break;
            case 1:
                equityAux(channel, channelRightQueue, currentChannelAnts);
                break;
        }
    }
    
    return;
}

void equityAux(channel_t* channel, queue* channelQueue, queue* currentChannelAnts) {
    switch (channel->scheduler) {
        case 0:
            //rr
            break;
        case 1:
            // prio
            break;
        case 2:
            // sjf
            break;
        case 3:
            equityFCFS(channel, channelQueue, currentChannelAnts);
            break;
        case 4:
            // tr
            break;
    }

    return;
}

void equityRR(channel_t* channel, queue* channelQueue, ant_t* currentChannelAnt) {


    return;
}

void equityFCFS(channel_t* channel, queue* channelQueue, queue* currentChannelAnts) {
    if (currentChannelAnts->count > 0) {
        ant_t* currentAnt = (ant_t*) currentChannelAnts->head->item;
        if (currentAnt->inChannel == 1) {
            return;
        }
        else {
            queueGetFirstItem(currentChannelAnts);
        }
    }
    if (channelQueue->count > 0) {
        ant_t* ant = (ant_t*) channelQueue->head->item;
        if (ant->waiting == 1) { 
            queueGetFirstItem(channelQueue);

            if (currentChannelAnts->count > 0) {
                queueGetFirstItem(currentChannelAnts);
            }

            ant->waiting = 0;
            ant->inChannel = 1;

            queueAddItem(currentChannelAnts, ant);

            channel->currentW++;
            if (channel->currentW == channel->w) {
                channel->currentW = 0;
                switch (channel->sign) {
                    case 0:
                        channel->sign = 1;
                        break;
                    case 1:
                        channel->sign = 0;
                        break;
                }
            }
        }
    }
    return;
}

void sign(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, ant_t* currentChannelAnt, long elapsedSignTime, struct timespec signBegin, struct timespec signEnd) {
    clock_gettime(CLOCK_REALTIME, &signEnd);
    elapsedSignTime += signEnd.tv_sec - signBegin.tv_sec;
    if (elapsedSignTime >= channel->time) {
        elapsedSignTime = 0;
        channel->sign = !channel->sign;
        clock_gettime(CLOCK_REALTIME, &signBegin);
    }

    if (channel->sign == 1 && channelRightQueue->count > 0) {
        signAndticoAux(channel, channelRightQueue, currentChannelAnt);
    }
    else if (channelLeftQueue->count > 0 && channel->sign == 1) {
        signAndticoAux(channel, channelLeftQueue, currentChannelAnt);
    }
    else if (channelLeftQueue->count == 0 && channelRightQueue->count == 0) {
        return;
    }
    else {
        switch (channel->sign) {
            case 0:
                signAndticoAux(channel, channelLeftQueue, currentChannelAnt);
                break;
            case 1:
                signAndticoAux(channel, channelRightQueue, currentChannelAnt);
                break;
        }
    }
    
    return;
}

void tico(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, ant_t* currentChannelAnt) {
    if (channelLeftQueue->count == 0 && channelRightQueue->count > 0) {
        signAndticoAux(channel, channelRightQueue, currentChannelAnt);
    }
    else if (channelLeftQueue->count > 0 && channelRightQueue->count == 0) {
        signAndticoAux(channel, channelLeftQueue, currentChannelAnt);
    }
    else if (channelLeftQueue->count == 0 && channelRightQueue->count == 0) {
        return;
    }
    else {
        int random = (rand() % 2);
        switch (random) {
            case 0:
                signAndticoAux(channel, channelLeftQueue, currentChannelAnt);
                break;
            case 1:
                signAndticoAux(channel, channelRightQueue, currentChannelAnt);
                break;
        }
    }

    return;
}

void signAndticoAux(channel_t* channel, queue* channelQueue, ant_t* currentChannelAnt) {
    if (currentChannelAnt != NULL) {
        if (currentChannelAnt->inChannel == 1) {
            return;
        }
    }

    ant_t* ant = (ant_t*) queueGetFirstItem(channelQueue);
    ant->inChannel = 1;
    currentChannelAnt = ant;

    return;
}
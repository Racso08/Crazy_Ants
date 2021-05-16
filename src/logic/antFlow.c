#include "antFlow.h"

void manageFlow(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts, queue* channelLeftEndQueue, queue* channelRightEndQueue, queue* izqPositions, queue* derPositions, long* signBegin, struct timespec channelTimer);
void checkIfAntArrived(queue* list);
void setNextAntsPositions(queue* list);

void equity(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts, queue* channelLeftEndQueue, queue* channelRightEndQueue, queue* izqPositions, queue* derPositions);
void sign(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts, queue* channelLeftEndQueue, queue* channelRightEndQueue, queue* izqPositions, queue* derPositions, long* signBegin, struct timespec channelTimer);
void tico(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts, queue* channelLeftEndQueue, queue* channelRightEndQueue, queue* izqPositions, queue* derPositions);

int schedulerHandler(channel_t* channel, queue* channelQueue, queue* currentChannelAnts, queue* channelEndQueue, queue* positions);

int PR__SJF_FCFS_RT(channel_t* channel, queue* channelQueue, queue* currentChannelAnts, queue* channelEndQueue, queue* positions);

void advanceAnts(queue* channelQueue, queue* positions);

void moveAnts() {
    checkIfAntArrived(&channel1LeftEndQueue);
    checkIfAntArrived(&channel1RightEndQueue);
    checkIfAntArrived(&channel2LeftEndQueue);
    checkIfAntArrived(&channel2RightEndQueue);
    checkIfAntArrived(&channel3LeftEndQueue);
    checkIfAntArrived(&channel3RightEndQueue);

    setNextAntsPositions(&channel1LeftQueue);
    setNextAntsPositions(&channel1RightQueue);
    setNextAntsPositions(&channel2LeftQueue);
    setNextAntsPositions(&channel2RightQueue);
    setNextAntsPositions(&channel3LeftQueue);
    setNextAntsPositions(&channel3RightQueue);

    setNextAntsPositions(&currentChannel1Ants);
    setNextAntsPositions(&currentChannel2Ants);
    setNextAntsPositions(&currentChannel3Ants);

    setNextAntsPositions(&channel1LeftEndQueue);
    setNextAntsPositions(&channel1RightEndQueue);
    setNextAntsPositions(&channel2LeftEndQueue);
    setNextAntsPositions(&channel2RightEndQueue);
    setNextAntsPositions(&channel3LeftEndQueue);
    setNextAntsPositions(&channel3RightEndQueue);

    manageFlow(channel1, &channel1LeftQueue, &channel1RightQueue, &currentChannel1Ants, &channel1LeftEndQueue, &channel1RightEndQueue, &izq1, &der1, &sign1Begin, channel1Timer);
    manageFlow(channel2, &channel2LeftQueue, &channel2RightQueue, &currentChannel2Ants, &channel2LeftEndQueue, &channel2RightEndQueue, &izq2, &der2, &sign2Begin, channel2Timer);
    manageFlow(channel3, &channel3LeftQueue, &channel3RightQueue, &currentChannel3Ants, &channel3LeftEndQueue, &channel3RightEndQueue, &izq3, &der3, &sign3Begin, channel3Timer);

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

void checkIfAntArrived(queue* list) {
    if (list->count > 0) {
        queueNode* node = (queueNode*) list->head;
        ant_t* ant = (ant_t*) node->item;

        if (ant->path == -1) {
            queueGetFirstItem(list);

            ant->path = -2;
        }                
    }

    return;
}

void manageFlow(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts, queue* channelLeftEndQueue, queue* channelRightEndQueue, queue* izqPositions, queue* derPositions, long* signBegin, struct timespec channelTimer) {
    switch (channel->flow) {
        case 0:
            equity(channel, channelLeftQueue, channelRightQueue, currentChannelAnts, channelLeftEndQueue, channelRightEndQueue, izqPositions, derPositions);
            break;
        case 1:
            sign(channel, channelLeftQueue, channelRightQueue, currentChannelAnts, channelLeftEndQueue, channelRightEndQueue, izqPositions, derPositions, signBegin, channelTimer);
            break;
        case 2:
            tico(channel, channelLeftQueue, channelRightQueue, currentChannelAnts, channelLeftEndQueue, channelRightEndQueue, izqPositions, derPositions);
            break;
    }

    return;
}

void equity(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts, queue* channelLeftEndQueue, queue* channelRightEndQueue, queue* izqPositions, queue* derPositions) {
    int success = 0;

    if (channelLeftQueue->count == 0 && channelRightQueue->count > 0) {
        channel->currentW = 0;
        channel->sign = 1;
        success = schedulerHandler(channel, channelRightQueue, currentChannelAnts, channelLeftEndQueue, derPositions);
    }
    else if (channelLeftQueue->count > 0 && channelRightQueue->count == 0) {
        channel->currentW = 0;
        channel->sign = 0;
        success = schedulerHandler(channel, channelLeftQueue, currentChannelAnts, channelRightEndQueue, izqPositions);
    }
    else {
        switch (channel->sign) {
            case 0:
                success = schedulerHandler(channel, channelLeftQueue, currentChannelAnts, channelRightEndQueue, izqPositions);
                break;
            case 1:
                success = schedulerHandler(channel, channelRightQueue, currentChannelAnts, channelLeftEndQueue, derPositions);
                break;
        }
    }

    if (success == 1) {
        channel->currentW++;
        if (channel->currentW == channel->w) {
            channel->currentW = 0;
            channel->sign = !channel->sign;
        }
    }
    
    return;
}

void sign(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts, queue* channelLeftEndQueue, queue* channelRightEndQueue, queue* izqPositions, queue* derPositions, long* signBegin, struct timespec channelTimer) {
    clock_gettime(CLOCK_REALTIME, &channelTimer);
    long elapsedSignTime = channelTimer.tv_sec - *signBegin;

    if (elapsedSignTime >= channel->time) {
        channel->sign = !channel->sign;
        clock_gettime(CLOCK_REALTIME, &channelTimer);
        *signBegin = channelTimer.tv_sec;
    }

    switch (channel->sign) {
        case 0:
            schedulerHandler(channel, channelLeftQueue, currentChannelAnts, channelRightEndQueue, izqPositions);
            break;
        case 1:
            schedulerHandler(channel, channelRightQueue, currentChannelAnts, channelLeftEndQueue, derPositions);
            break;
    }
    
    return;
}

void tico(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, queue* currentChannelAnts, queue* channelLeftEndQueue, queue* channelRightEndQueue, queue* izqPositions, queue* derPositions) {
    if (channelLeftQueue->count == 0 && channelRightQueue->count > 0) {
        schedulerHandler(channel, channelRightQueue, currentChannelAnts, channelLeftEndQueue, derPositions);
    }
    else if (channelLeftQueue->count > 0 && channelRightQueue->count == 0) {
        schedulerHandler(channel, channelLeftQueue, currentChannelAnts, channelRightEndQueue, izqPositions);
    }
    else {
        int random = (rand() % 2);
        switch (random) {
            case 0:
                schedulerHandler(channel, channelLeftQueue, currentChannelAnts, channelRightEndQueue, izqPositions);
                break;
            case 1:
                schedulerHandler(channel, channelRightQueue, currentChannelAnts, channelLeftEndQueue, derPositions);
                break;
        }
    }

    return;
}

int schedulerHandler(channel_t* channel, queue* channelQueue, queue* currentChannelAnts, queue* channelEndQueue, queue* positions) {
    switch (channel->scheduler) {
        case 0:
            //rr
            break;
        case 1:
            return PR__SJF_FCFS_RT(channel, channelQueue, currentChannelAnts, channelEndQueue, positions);
            break;
        case 2:
            return PR__SJF_FCFS_RT(channel, channelQueue, currentChannelAnts, channelEndQueue, positions);
            break;
        case 3:
            return PR__SJF_FCFS_RT(channel, channelQueue, currentChannelAnts, channelEndQueue, positions);
            break;
        case 4:
            return PR__SJF_FCFS_RT(channel, channelQueue, currentChannelAnts, channelEndQueue, positions);
            break;
    }

    return 0;
}

int PR__SJF_FCFS_RT(channel_t* channel, queue* channelQueue, queue* currentChannelAnts, queue* channelEndQueue, queue* positions) {
    if (currentChannelAnts->count > 0) {
        ant_t* currentAnt = (ant_t*) currentChannelAnts->head->item;
        if (currentAnt->inChannel == 1) {
            return 0;
        }
        else {
            queueGetFirstItem(currentChannelAnts);
            queueAddItem(channelEndQueue, currentAnt);
        }
    }
    if (channelQueue->count > 0) {
        ant_t* ant = (ant_t*) channelQueue->head->item;
        if (ant->waiting == 1) { 
            queueGetFirstItem(channelQueue);

            if (currentChannelAnts->count > 0) {
                queueGetFirstItem(currentChannelAnts);
            }

            ant->inChannel = 1;

            queueAddItem(currentChannelAnts, ant);

            if (channelQueue->count > 0) {
                advanceAnts(channelQueue, positions);
            }

            return 1;
        }
    }
    return 0;
}

void advanceAnts(queue* channelQueue, queue* positions) {
    queueNode* antNode = (queueNode*) channelQueue->head;
    queueNode* positionNode = (queueNode*) positions->head;

    ant_t* ant;
    int position;

    while (antNode != NULL) {
        ant = (ant_t*) antNode->item;
        position = (int) (__intptr_t) positionNode->item;

        ant->finalDest = position;

        antNode = antNode->next;
        positionNode = positionNode->next;        
    }
    
    return;
}
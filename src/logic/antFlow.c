#include "antFlow.h"

void manageFlow(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, ant_t* currentChannelAnt, long elapsedSignTime, struct timespec signBegin, struct timespec signEnd);
void equity(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, ant_t* currentChannelAnt);
void equityAux(channel_t* channel, queue* channelQueue, ant_t* currentChannelAnt);
void sign(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, ant_t* currentChannelAnt, long elapsedSignTime, struct timespec signBegin, struct timespec signEnd);
void tico(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, ant_t* currentChannelAnt);
void signAndticoAux(channel_t* channel, queue* channelQueue, ant_t* currentChannelAnt);

ant_t* currentChannel1Ant = NULL;
ant_t* currentChannel2Ant = NULL;
ant_t* currentChannel3Ant = NULL;
long elapsedSign1Time = 0;
long elapsedSign2Time = 0;
long elapsedSign3Time = 0;

void moveAnts() {
    /*while (1) {
        CEmutex_lock(&channel1DataMutex);
        manageFlow(channel1, &channel1LeftQueue, &channel1RightQueue, currentChannel1Ant);
        CEmutex_unlock(&channel1DataMutex);

        CEmutex_lock(&channel2DataMutex);
        manageFlow(channel2, &channel2LeftQueue, &channel2RightQueue, currentChannel2Ant);
        CEmutex_unlock(&channel2DataMutex);

        CEmutex_lock(&channel3DataMutex);
        manageFlow(channel3, &channel3LeftQueue, &channel3RightQueue, currentChannel3Ant);
        CEmutex_unlock(&channel3DataMutex);

        CEthread_yield();
    }*/

    manageFlow(channel1, &channel1LeftQueue, &channel1RightQueue, currentChannel1Ant, elapsedSign1Time, sign1Begin, sign1End);
    manageFlow(channel2, &channel2LeftQueue, &channel2RightQueue, currentChannel2Ant, elapsedSign2Time, sign2Begin, sign2End);
    manageFlow(channel3, &channel3LeftQueue, &channel3RightQueue, currentChannel3Ant, elapsedSign3Time, sign3Begin, sign3End);

    return;
}

void manageFlow(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, ant_t* currentChannelAnt, long elapsedSignTime, struct timespec signBegin, struct timespec signEnd) {
    switch (channel->flow) {
        case 0:
            equity(channel, channelLeftQueue, channelRightQueue, currentChannelAnt);
            break;
        case 1:
            sign(channel, channelLeftQueue, channelRightQueue, currentChannelAnt, elapsedSignTime, signBegin, signEnd);
            break;
        case 2:
            tico(channel, channelLeftQueue, channelRightQueue, currentChannelAnt);
            break;
    }

    return;
}

void equity(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue, ant_t* currentChannelAnt) {
    if (channelLeftQueue->count == 0 && channelRightQueue->count > 0) {
        channel->currentW = 0;
        channel->sign = 1;
        equityAux(channel, channelRightQueue, currentChannelAnt);
    }
    else if (channelLeftQueue->count > 0 && channelRightQueue->count == 0) {
        channel->currentW = 0;
        channel->sign = 0;
        equityAux(channel, channelLeftQueue, currentChannelAnt);
    }
    else if (channelLeftQueue->count == 0 && channelRightQueue->count == 0) {
        return;
    }
    else {
        switch (channel->sign) {
            case 0:
                equityAux(channel, channelLeftQueue, currentChannelAnt);
                break;
            case 1:
                equityAux(channel, channelRightQueue, currentChannelAnt);
                break;
        }
    }
    
    return;
}

void equityAux(channel_t* channel, queue* channelQueue, ant_t* currentChannelAnt) {
    if (currentChannelAnt != NULL) {
        if (currentChannelAnt->inChannel == 1) {
            return;
        }
    }

    ant_t* ant = (ant_t*) queueGetFirstItem(channelQueue);
    ant->inChannel = 1;
    currentChannelAnt = ant;

    channel->currentW++;
    if (channel->currentW == channel->w) {
        channel->currentW = 0;
        channel->sign = !channel->sign;
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
#include "antFlow.h"

void manageFlow(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue);
void equity(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue);
void equityAux(channel_t* channel, queue* channelQueue);
void sign(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue);
void tico(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue);

void moveAnts() {
    manageFlow(channel1, &channel1LeftQueue, &channel1RightQueue);
    manageFlow(channel2, &channel2LeftQueue, &channel2RightQueue);
    manageFlow(channel3, &channel3LeftQueue, &channel3RightQueue);
    return;
}

void manageFlow(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue) {
    switch (channel->flow) {
        case 0:
            equity(channel, channelLeftQueue, channelRightQueue);
            break;
        case 1:
            
            break;
        case 2:
            
            break;
    }

    return;
}

void equity(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue) {
    if (channelLeftQueue->count == 0 && channelRightQueue->count > 0) {
        channel->currentW = 0;
        channel->sign = 1;
        equityAux(channel, channelRightQueue);
    }
    else if (channelLeftQueue->count > 0 && channelRightQueue->count == 0) {
        channel->currentW = 0;
        channel->sign = 0;
        equityAux(channel, channelLeftQueue);
    }
    else {
        switch (channel->sign) {
            case 0:
                equityAux(channel, channelLeftQueue);
                break;
            case 1:
                equityAux(channel, channelRightQueue);
                break;
        }
    }
    
    return;
}

void equityAux(channel_t* channel, queue* channelQueue) {
    if (channel->scheduler == 0) {
        //move rr
    }
    else if (channel->scheduler == 1 || channel->scheduler == 3) {
        //move p & fcfs
    }
    else if (channel->scheduler == 2) {
        //move sjf
    }
    else {
        //move tr
    }

    return;
}

void sign(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue) {


    return;
}

void tico(channel_t* channel, queue* channelLeftQueue, queue* channelRightQueue) {

    return;
}
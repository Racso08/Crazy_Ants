#ifndef __CHANNEL_H
#define __CHANNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../threads/queue.h"
#include "../threads/CEthread.h"

typedef struct {
    int id;
    int scheduler;
    int flow;
    int lenght;
    int trueLenght;
    int antAmount;
    int time;
    int w;
    int currentW;
    int sign;
    int previousAntSign;
    int scheduled;
} channel_t;

channel_t* channel1;
channel_t* channel2;
channel_t* channel3;

queue channel1LeftQueue, channel1RightQueue, currentChannel1Ants, currentChannel1ActiveAnt, channel1LeftEndQueue, channel1RightEndQueue;
queue channel2LeftQueue, channel2RightQueue, currentChannel2Ants, currentChannel2ActiveAnt, channel2LeftEndQueue, channel2RightEndQueue;
queue channel3LeftQueue, channel3RightQueue, currentChannel3Ants, currentChannel3ActiveAnt, channel3LeftEndQueue, channel3RightEndQueue;

queue allAnts;

void initializeChannels();

#endif
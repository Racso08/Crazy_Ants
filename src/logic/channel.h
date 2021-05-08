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
    int antAmount;
    int time;
    int w;
    int currentW;
    int sign;
} channel_t;

CEmutex_t channel1Mutex, channel1DataMutex;
CEmutex_t channel2Mutex, channel2DataMutex;
CEmutex_t channel3Mutex, channel3DataMutex;

channel_t* channel1;
channel_t* channel2;
channel_t* channel3;

queue channel1LeftQueue, channel1RightQueue;
queue channel2LeftQueue, channel2RightQueue;
queue channel3LeftQueue, channel3RightQueue;
queue allAnts;

void initializeChannels();

#endif
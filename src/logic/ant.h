#ifndef __ANT_H
#define __ANT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../threads/CEthread.h"
#include "../threads/queue.h"
#include "channel.h"

#define MAXANTS 1000

typedef struct {
    int posX;
    int posY;
    int vel;
    int currentChannelPosition;
    int channelLenght;
    int dest;
    int type;
    int channel;
    int time;
    int priority;
    int inChannel;
    int waiting;
    int finalDest;
    int path;
    CEthread_t* thread;
} ant_t;

queue izq1, izq2, izq3, der1, der2, der3;

void initializePositions();
int createAnt(int type, int channel, int dest, int extra);

#endif
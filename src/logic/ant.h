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
    int finalDest;
    int path;
    CEthread_t* thread;
    CEmutex_t* mutex;
} ant_t;

void createAnt(int posX, int posY, int channelLenght, int dest, int type, int channel, int time, int priority, int finalDest, int path);

#endif
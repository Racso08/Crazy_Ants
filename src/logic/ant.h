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
    int dest;
    int type;
    int channel;
    int time;
    int priority;
    CEthread_t* thread;
} ant_t;

void createAnt(int channel, int priority, int time, int dest, int type);

#endif
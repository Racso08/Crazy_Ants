#ifndef __ANTSCHEDULER_H
#define __ANTSCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../threads/queue.h"
#include "../threads/CEthread.h"
#include "channel.h"
#include "ant.h"

char *times;
char *priorities;

void scheduleAnts(int channel, int dest);

#endif
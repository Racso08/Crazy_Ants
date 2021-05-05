#ifndef __ANT_H
#define __ANT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../threads/CEthread.h"
#include "../threads/queue.h"
#include "channel.h"

void createAnt(int scheduler, int priority, int time, int dest, int type);

#endif
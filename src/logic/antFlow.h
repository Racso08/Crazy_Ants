#ifndef __ANTFLOW_H
#define __ANTFLOW_H

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../threads/queue.h"
#include "../threads/CEthread.h"
#include "channel.h"
#include "ant.h"

struct timespec channel1Timer, channel2Timer, channel3Timer;
long sign1Begin, sign2Begin, sign3Begin; 

void* moveAnts(void* arg);

#endif
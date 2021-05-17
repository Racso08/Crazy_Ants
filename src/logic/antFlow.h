#ifndef __ANTFLOW_H
#define __ANTFLOW_H

#define _POSIX_C_SOURCE 199309L
#define ANTQUANTUM 3

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../threads/queue.h"
#include "../threads/CEthread.h"
#include "channel.h"
#include "ant.h"

struct timespec channel1Timer, channel2Timer, channel3Timer;
struct timespec antTimer;
long sign1Begin, sign2Begin, sign3Begin; 

void moveAnts();

#endif
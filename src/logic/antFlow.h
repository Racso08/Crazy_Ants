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

struct timespec sign1Begin, sign1End, sign2Begin, sign2End, sign3Begin, sign3End;

void moveAnts();

#endif
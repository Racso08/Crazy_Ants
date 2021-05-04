#ifndef __CETHREAD_H
#define __CETHREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <ucontext.h>
#include "queue.h"

#define QUANTUM 10
#define RUNNING 0
#define DONE 1

typedef int CEthread_t;

void CEthread_init();
int CEthread_create(CEthread_t* thread, void *(*start_routine)(void *), void *arg);
void CEthread_end(void* returnValue);
int CEthread_yield(void);
int CEthread_join(CEthread_t thread, void **status);
int CEthread_detach(CEthread_t thread);

#endif
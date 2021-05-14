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

#define QUANTUM 1000
#define RUNNING 0
#define DONE 1

typedef struct {
    ucontext_t* context;
    int id;
    int join;
    int state;
    int detach;
    void* (*proc)(void*);
    void* arg;
    void* returnValue;
} cethread_t;

typedef int CEthread_t;
typedef queue CEmutex_t; 

int threadsIdCount;
queue readyQueue, finishQueue;
queueNode* currentNode;
sigset_t vtalrm;
static struct itimerval timer;

void CEthread_init();
int CEthread_create(CEthread_t* thread, void *(*start_routine)(void *), void *arg);
void CEthread_end(void* returnValue);
int CEthread_yield(void);
int CEthread_join(CEthread_t thread, void **status);
int CEthread_detach(CEthread_t thread);
int CEmutex_init(CEmutex_t* mutex);
int CEmutex_destroy(CEmutex_t* mutex);
int CEmutex_unlock(CEmutex_t* mutex);
int CEmutex_trylock(CEmutex_t* mutex);

#endif
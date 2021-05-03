#ifndef __CETHREAD_H
#define __CETHREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <ucontext.h>

#define CETHREAD_RUNNING 0
#define CETHREAD_CANCEL 1
#define CETHREAD_DONE 2

typedef struct{
    CEthread_t threadID;
    CEthread_t joining;
    int state;
    void* (*proc)(void*);
    void* arg;
    void* retval;
    ucontext_t* uc;
} CEthread_t;

#endif
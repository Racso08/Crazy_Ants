#define _XOPEN_SOURCE 700

#include "CEthread.h"

typedef struct{
    ucontext_t* context;
    int id;
    int join;
    int state;
    int detach;
    void* (*proc)(void*);
    void* arg;
    void* returnValue;
} cethread_t;

int threadsIdCount;
queue readyQueue, finishQueue;
cethread_t* currentThread;
sigset_t vtalrm;
static struct itimerval timer;

void initialize();
void start(void* (*startFunction)(void*), void* args);
cethread_t* findThread(CEthread_t thread);
void scheduler(int signal);

void CEthread_init() {
	struct sigaction act;

    threadsIdCount = 1;
    queueInit(&readyQueue);
    queueInit(&finishQueue);

    cethread_t* mainThread = (cethread_t*) malloc(sizeof(cethread_t));
    mainThread->id = threadsIdCount;
    mainThread->context = (ucontext_t*) malloc(sizeof(ucontext_t)); 
    memset(mainThread->context, '\0', sizeof(ucontext_t));
    mainThread->arg = NULL;
    mainThread->state = RUNNING;
    mainThread->join = 0;
    mainThread->detach = 0;
    threadsIdCount++;

    if (getcontext(mainThread->context) == -1) {
      perror("Error, no se logro conseguir el contexto del hilo principal\n");
      exit(EXIT_FAILURE);
    }

    currentThread = mainThread;
    
    sigemptyset(&vtalrm);
    sigaddset(&vtalrm, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);

    timer.it_interval.tv_usec = QUANTUM;
    timer.it_interval.tv_sec = 0;
    timer.it_value.tv_usec = QUANTUM;
    timer.it_value.tv_sec = 0; 
    
    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) < 0) {
        perror("Error, no se logro colocar el timer\n");
        exit(EXIT_FAILURE);
    }

    memset(&act, '\0', sizeof(act));
    act.sa_handler = &scheduler;
    if (sigaction(SIGVTALRM, &act, NULL) < 0) {
      perror ("Error, no se logro colocar el calendarizador\n");
      exit(EXIT_FAILURE);
    }
}

int CEthread_create(CEthread_t* thread, void *(*startFunction)(void *), void *arg) {
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    
    cethread_t* newThread = malloc(sizeof(cethread_t));
    *thread = threadsIdCount;
    newThread->id = threadsIdCount;
    newThread->state = RUNNING;
    newThread->proc = startFunction;
    newThread->arg = arg;
    newThread->context = (ucontext_t*) malloc(sizeof(ucontext_t));
    newThread->join = 0;
    newThread->detach = 0;
    memset(newThread->context, '\0', sizeof(ucontext_t));
    threadsIdCount++;

    if (getcontext(newThread->context) == -1) {
      perror("Error, no se logro conseguir el contexto del hilo\n");
      exit(EXIT_FAILURE);
    }

    newThread->context->uc_stack.ss_sp = malloc(SIGSTKSZ);
    newThread->context->uc_stack.ss_size = SIGSTKSZ;
    newThread->context->uc_stack.ss_flags = 0;
    newThread->context->uc_link = NULL;

    makecontext(newThread->context, (void (*)(void)) start, 2, startFunction, arg);
    queueAddItem(&readyQueue, newThread);

    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);   
    return 0; 
}

void CEthread_end(void* returnValue) {
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);

    if (readyQueue.count == 0) { 
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
        exit((long) returnValue);
    }

    if (currentThread->id == 1)
    {
        while (readyQueue.count != 0)
        {
            sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);  
            scheduler(SIGVTALRM);
            sigprocmask(SIG_BLOCK, &vtalrm, NULL);
        }
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);   
        exit((long) returnValue);
    }

    cethread_t* previousThread = currentThread; 
    currentThread = (cethread_t*) queueGetFirstItem(&readyQueue);
    currentThread->state = RUNNING; 

    free(previousThread->context->uc_stack.ss_sp); 
    free(previousThread->context);                
    previousThread->context = NULL;

    previousThread->state = DONE;
    previousThread->join = 0;
    previousThread->returnValue = NULL;

    if (previousThread->detach == 0) {
        previousThread->returnValue = returnValue;
    }

    queueAddItem(&finishQueue, previousThread);

    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    setcontext(currentThread->context);
}

int CEthread_yield(void) {
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    
    if (readyQueue.count == 0) {
        return 0;
    }

    cethread_t* nextThread = (cethread_t*) queueGetFirstItem(&readyQueue);
    cethread_t* previousThread = currentThread;
    queueAddItem(&readyQueue, currentThread);
    currentThread = nextThread;

    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    swapcontext(previousThread->context, currentThread->context); 

    return 0; 
}

int CEthread_join(CEthread_t thread, void **returnValue) {
    if (thread == currentThread->id) {
        printf("Error, no se puede hacer un join del hilo actual\n");
        return -1;
    }

    cethread_t* trueThread = findThread(thread);

    if (trueThread == NULL) {
        printf("Error, no se encontro el hilo\n");
        return -1;
    }

    if (trueThread->join == currentThread->id) {
        printf("Error, no se puede hacer un join del hilo actual\n");
        return -1;
    }

    if (trueThread->detach == 1) {
        printf("Error, no se puede hacer un join de un hilo detached\n");
        return -1;
    }

    currentThread->join = trueThread->id;

    while (trueThread->state == RUNNING) {
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
        scheduler(SIGVTALRM);
        sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    }

    if (returnValue == NULL) {
        return 0;
    }

    else if (trueThread->state == DONE) {
        *returnValue = trueThread->returnValue;
    }

    return 0;
}

int CEthread_detach(CEthread_t thread) {
    cethread_t* trueThread = findThread(thread);

    if (trueThread == NULL) {
        printf("Error, no se encontro el hilo\n");
        return -1;
    }

    if (trueThread->state == DONE) {
        printf("Error, el hilo ya termino su ejecucion\n");
        return -1;
    }

    trueThread->detach = 1;

    return 0;
}

int CEmutex_init(CEmutex_t* mutex) {
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);  
    queueInit(mutex);
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
    return 0;
}

int CEmutex_destroy(CEmutex_t* mutex) {
    sigprocmask(SIG_BLOCK, &vtalrm, NULL); 
    queueDestroy(mutex);
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    return 0; 
}

int CEmutex_unlock(CEmutex_t* mutex) {
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    if (mutex->count == 0) {
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
        return -1;
    }

    if ((int) (__intptr_t) mutex->head->item != currentThread->id) {
       sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
       return -1;
    }

    queueGetFirstItem(mutex);
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    return 0; 
}

int CEmutex_lock(CEmutex_t* mutex) {
    sigprocmask(SIG_BLOCK, &vtalrm, NULL); 

    if (mutex->count == 0) {
        queueAddItem(mutex, (void*) (__intptr_t) currentThread->id);  
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);   
        return 0;
    }

    if ((int) (__intptr_t) mutex->head->item == currentThread->id) {
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
        return 0;
    }

    queueAddItem(mutex, (void*) (__intptr_t) currentThread->id);
    while ((int) (__intptr_t) mutex->head->item != currentThread->id) {
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
        scheduler(SIGVTALRM);
        sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    }
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);  
    return 0; 
}

void start(void* (*startFunction)(void*), void* args) {
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);

    currentThread->returnValue = (*startFunction)(args);

    CEthread_end(currentThread->returnValue);
}

cethread_t* findThread(CEthread_t thread) {
    queueNode* tmp = readyQueue.head;   
    cethread_t* itemThread;

    while (tmp != NULL) {
        itemThread = (cethread_t*) tmp->item;
        if (itemThread->id == thread) {
            return itemThread;
        }
        tmp = tmp->next;
    }

    tmp = finishQueue.head;
    while (tmp != NULL) {
        itemThread = (cethread_t*) tmp->item;
        if (itemThread->id == thread) {
            return itemThread;
        }
        tmp = tmp->next;
    } 
    return NULL;
}

void scheduler(int signal) {
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);

    if (readyQueue.count == 0) {
        return;
    }

    cethread_t* nextThread = (cethread_t*) queueGetFirstItem(&readyQueue);

    cethread_t* previousThread = currentThread;
    queueAddItem(&readyQueue, currentThread);
    nextThread->state = RUNNING; 
    currentThread = nextThread;

    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    swapcontext(previousThread->context, currentThread->context);
}
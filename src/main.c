#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "threads/CEthread.h"
#include "logic/ant.h"
#include "logic/channel.h"
#include "logic/antScheduler.h"
#include "logic/antFlow.h"

int main() {
	//srand(time(0));
	// CEthread_init();
	// initializeChannels();
	// clock_gettime(CLOCK_REALTIME, &sign1Begin);
	// clock_gettime(CLOCK_REALTIME, &sign2Begin);
	// clock_gettime(CLOCK_REALTIME, &sign3Begin);

	//CEthread_t antMovementThread;
	//CEthread_create(&antMovementThread, , NULL);
	//CEthread_detach(antMovementThread);

	//createAnt(0, 0, 1, channel1->lenght, 4, 23, 1, 2);
	//scheduleAnts();
	//createAnt(0, 0, 1, channel1->lenght, 5, 12, 1, 1);
	//scheduleAnts();
	//createAnt(0, 0, 1, channel1->lenght, 5, 5, 1, 0);
	//scheduleAnts();

	//moveAnts();

	//while (1) {
		//printf("f3\n");
	//}
	queue channel1LeftQueue;
	queueInit(&channel1LeftQueue);
	ant_t* ant1=(ant_t*)malloc(sizeof(ant_t));
	ant_t* ant2=(ant_t*)malloc(sizeof(ant_t));
	ant_t* ant3=(ant_t*)malloc(sizeof(ant_t));
	ant_t* ant4=(ant_t*)malloc(sizeof(ant_t));
	ant_t* ant5=(ant_t*)malloc(sizeof(ant_t));
	ant_t* ant6=(ant_t*)malloc(sizeof(ant_t));
	ant_t* ant7=(ant_t*)malloc(sizeof(ant_t));
	ant_t* ant8=(ant_t*)malloc(sizeof(ant_t));
	ant_t* ant9=(ant_t*)malloc(sizeof(ant_t));
	ant_t* ant10=(ant_t*)malloc(sizeof(ant_t));
	
	ant1->priority=2;
	ant2->priority=3;
	ant3->priority=11;
	ant4->priority=1;
	ant5->priority=15;
	ant6->priority=4;
	ant7->priority=8;
	ant8->priority=10;
	ant9->priority=9;
	ant10->priority=6;

	ant1->time=3;
	ant2->time=3;
	ant3->time=11;
	ant4->time=5;
	ant5->time=20;
	ant6->time=22;
	ant7->time=8;
	ant8->time=10;
	ant9->time=14;
	ant10->time=1;

	// ant1->priority=15;
	// ant2->priority=10;
	// ant3->priority=7;
	// ant4->priority=8;
	// ant5->priority=5;
	// ant6->priority=11;
	// ant7->priority=2;
	// ant8->priority=8;
	// ant9->priority=3;
	// ant10->priority=9;
	queueAddItem(&channel1LeftQueue,ant1);
	queueAddItem(&channel1LeftQueue,ant2);
	queueAddItem(&channel1LeftQueue,ant3);
	queueAddItem(&channel1LeftQueue,ant4);
	queueAddItem(&channel1LeftQueue,ant5);
	queueAddItem(&channel1LeftQueue,ant6);
	queueAddItem(&channel1LeftQueue,ant7);
	queueAddItem(&channel1LeftQueue,ant8);
	queueAddItem(&channel1LeftQueue,ant9);
	queueAddItem(&channel1LeftQueue,ant10);
	printf("Lista tiempo\n");
	queuePrintTime(&channel1LeftQueue);
	printf("Lista prioridad\n");
	queuePrintPriority(&channel1LeftQueue);
	printf("Ordenada por tiempo\n");
	sjfSchedule(&channel1LeftQueue);
	queuePrintTime(&channel1LeftQueue);
	printf("Ordenada por prioridad\n");
	prioritySchedule(&channel1LeftQueue);
	queuePrintPriority(&channel1LeftQueue);
	
	return 0;
}
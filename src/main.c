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

/*CEmutex_t mutex;

void* test(void* arg) {
	printf("hola2\n");
	CEmutex_lock(&mutex);
	printf("hola3\n");

	int i = (int) (__intptr_t) arg;
	i += 1;

	for (int j = 0; j < 1000000000; j++) {
	//	printf("ah\n");
	}

	CEmutex_unlock(&mutex);
	return (void*) (__intptr_t) i;
}

int main() {
	CEthread_init();
	CEmutex_init(&mutex);

	CEthread_t thread1;
	CEthread_t thread2;
	void* returnValue1;
	void* returnValue2;
	int i = 1;
	int j = 1;

	printf("%d\n", i);
	CEthread_create(&thread1, test, (void*) (__intptr_t) i);
	CEthread_create(&thread2, test, (void*) (__intptr_t) j);
	//CEthread_detach(thread1);

	printf("hola1\n");

	for (int j = 0; j < 1000000000; j++) {
	//	printf("ah\n");
	}

	CEmutex_lock(&mutex);
	//CEmutex_unlock(&mutex);
	
	CEthread_join(thread1, &returnValue1);
	printf("%d\n", (int) (__intptr_t) returnValue1);
	CEthread_join(thread1, &returnValue2);
	printf("%d\n", (int) (__intptr_t) returnValue2);
	printf("hola4\n");

	printf("hola5\n");

	return 0;
}*/

void* worker(void* arg) {
	int i = (int) (__intptr_t) arg;
	
	while (1) {
		printf("hola %d\n", i);
	}
	

	return NULL;
}

int main() {
	srand(time(0));
	initializeChannels();
	initializePositions();
	clock_gettime(CLOCK_REALTIME, &sign1Begin);
	clock_gettime(CLOCK_REALTIME, &sign2Begin);
	clock_gettime(CLOCK_REALTIME, &sign3Begin);
	CEthread_init();

	//createAnt(posX, posY, channelLenght, dest, type, channel, channelTime, priority);
	/*createAnt(80, 440, channel1->lenght, 1, 0, 1, -1, -1);
	createAnt(80, 440, channel2->lenght, 1, 0, 2, -1, -1);
	createAnt(80, 440, channel3->lenght, 1, 0, 3, -1, -1);
	createAnt(80, 440, channel1->lenght, 1, 0, 1, -1, -1);
	createAnt(80, 440, channel2->lenght, 1, 0, 2, -1, -1);
	createAnt(80, 440, channel3->lenght, 1, 0, 3, -1, -1);

	createAnt(1100, 440, channel1->lenght, 0, 0, 1, -1, -1);
	createAnt(1100, 440, channel2->lenght, 0, 0, 2, -1, -1);
	createAnt(1100, 440, channel3->lenght, 0, 0, 3, -1, -1);
	createAnt(1100, 440, channel1->lenght, 0, 0, 1, -1, -1);
	createAnt(1100, 440, channel2->lenght, 0, 0, 2, -1, -1);
	createAnt(1100, 440, channel3->lenght, 0, 0, 3, -1, -1);*/

	// CEthread_t threads[7];

	// CEthread_create(&threads[0], &worker, (void*) (__intptr_t) 1);
	// CEthread_create(&threads[1], &worker, (void*) (__intptr_t) 2);
	// CEthread_create(&threads[2], &worker, (void*) (__intptr_t) 3);
	// CEthread_create(&threads[3], &worker, (void*) (__intptr_t) 4);
	// CEthread_create(&threads[4], &worker, (void*) (__intptr_t) 5);
	// CEthread_create(&threads[5], &worker, (void*) (__intptr_t) 6);
	// CEthread_create(&threads[6], &worker, (void*) (__intptr_t) 7);

	while (1) {
		//printf("hola 0\n");
	}

	return 0;
}
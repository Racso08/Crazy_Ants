#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "threads/CEthread.h"
#include "logic/ant.h"
#include "logic/channel.h"

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

int main() {
	CEthread_init();
	initializeChannels();

	createAnt(1, 4, 23, 1, 2);
	createAnt(1, 5, 12, 1, 1);
//	createAnt(1, 5, 5, 1, 0);


	while (1)
	{
		//printf("f3\n");
	}

	return 0;
}
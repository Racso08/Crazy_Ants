#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "threads/CEthread.h"

CEmutex_t mutex;

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
	void* returnValue;
	int i = 1;

	printf("%d\n", i);
	CEthread_create(&thread1, test, (void*) (__intptr_t) i);
	//CEthread_detach(thread1);

	printf("hola1\n");

	for (int j = 0; j < 1000000000; j++) {
	//	printf("ah\n");
	}

	CEmutex_lock(&mutex);
	//CEmutex_unlock(&mutex);
	
	CEthread_join(thread1, &returnValue);
	printf("%d\n", (int) (__intptr_t) returnValue);
	printf("hola4\n");

	printf("hola5\n");

	return 0;
}
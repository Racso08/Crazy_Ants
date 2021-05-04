#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "threads/CEthread.h"

const char* g_str = "Hello World\n";
const long g_ret = 0xF0F0F0F0;

void* test(void* arg) {
	printf("hola2\n");
	int i = (int) (__intptr_t) arg;
	i += 1;
	return (void*) (__intptr_t) i;
}

int main() {
	CEthread_init();

	CEthread_t thread1;
	void* returnValue;
	int i = 1;

	printf("%d\n", i);
	CEthread_create(&thread1, test, (void*) (__intptr_t) i);
	CEthread_detach(thread1);

	printf("hola1\n");

	for (int j = 0; j < 10000000; j++) {
	//	printf("ah\n");
	}
	
	
	CEthread_join(thread1, &returnValue);
	printf("%d\n", (int) (__intptr_t) returnValue);
	printf("hola3\n");

	printf("hola4\n");

	return 0;
}
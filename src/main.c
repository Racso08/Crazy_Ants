#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "threads/CEthread.h"

const char* g_str = "Hello World\n";
const long g_ret = 0xF0F0F0F0;

void* worker(void* arg) {
	printf("hola2\n");
	if (strcmp((char*) arg, g_str) != 0) {
		fprintf(stderr, 
				"!ERROR! Wrong argument! %p, %p\n",
				arg, g_str);
	}
	return (void*) g_ret;
}

int main() {
	CEthread_init();

	CEthread_t thread1;
	void* returnValue;

	CEthread_create(&thread1, worker, (void*) g_str);

	printf("hola1\n");
	CEthread_join(thread1, &returnValue);
	printf("hola3\n");

	if ((long) returnValue != g_ret) {
		fprintf(stderr, 
				"!ERROR! Wrong return! %ld, %ld\n",
				(long) returnValue, g_ret);
	}
	printf("hola4\n");

	return 0;
}
all: main clean

main: main.o CEthread.o queue.o
	gcc -o out/main main.o CEthread.o queue.o

main.o: src/main.c
	gcc -c src/main.c

CEthread: CEthread.o queue.o
	gcc -o out/CEthread CEthread.o queue.o

CEthread.o: src/threads/CEthread.c
	gcc -c src/threads/CEthread.c

queue.o: src/threads/queue.c
	gcc -c src/threads/queue.c

clean:
	rm *.o
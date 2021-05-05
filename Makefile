all: main clean

main: main.o CEthread.o queue.o ant.o channel.o
	gcc -o out/main main.o CEthread.o queue.o ant.o channel.o

main.o: src/main.c
	gcc -c src/main.c

ant.o: src/logic/ant.c src/logic/ant.h
	gcc -c src/logic/ant.c

channel.o: src/logic/channel.c src/logic/channel.h
	gcc -c src/logic/channel.c

CEthread.o: src/threads/CEthread.c src/threads/CEthread.h
	gcc -c src/threads/CEthread.c

queue.o: src/threads/queue.c src/threads/queue.h
	gcc -c src/threads/queue.c

clean:
	rm *.o
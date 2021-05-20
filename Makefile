all: gui clean

gui: gui.o CEthread.o queue.o ant.o channel.o antScheduler.o antFlow.o
	gcc -o out/gui gui.o CEthread.o queue.o ant.o channel.o antScheduler.o antFlow.o `sdl2-config --libs --cflags` -lSDL2_image

gui.o: src/gui/gui.c
	gcc -c src/gui/gui.c

main: main.o CEthread.o queue.o ant.o channel.o antScheduler.o antFlow.o
	gcc -o out/main main.o CEthread.o queue.o ant.o channel.o antScheduler.o antFlow.o

main.o: src/main.c
	gcc -c src/main.c

ant.o: src/logic/ant.c src/logic/ant.h
	gcc -c src/logic/ant.c

channel.o: src/logic/channel.c src/logic/channel.h
	gcc -c src/logic/channel.c

antScheduler.o: src/logic/antScheduler.c src/logic/antScheduler.h
	gcc -c src/logic/antScheduler.c

antFlow.o: src/logic/antFlow.c src/logic/antFlow.h
	gcc -c src/logic/antFlow.c

CEthread.o: src/threads/CEthread.c src/threads/CEthread.h
	gcc -c src/threads/CEthread.c

queue.o: src/threads/queue.c src/threads/queue.h
	gcc -c src/threads/queue.c

install:
	sudo apt install libsdl1.2-dev
	sudo apt-get install libsdl2-2.0
	sudo apt-get install libsdl2-dev
	sudo apt install libsdl2-image-dev

clean:
	rm *.o
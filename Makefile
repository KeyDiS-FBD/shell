#переменная CC указывает на компилятор
CC=gcc
#переменная CFLAGS хранит необходимые для компиляции флаги
CFLAGS=-Wall -lm -Iinclude

.PHONY: clean, all

all: bin main reading ui execute pidList link OBJ_RM
	echo Success compile

bin:
	mkdir bin -p

main: src/main.c
	$(CC) $(CFLAGS) src/main.c -c

ui: src/ui.c
	$(CC) $(CFLAGS) src/ui.c -c

reading: src/reading.c
	$(CC) $(CFLAGS) src/reading.c -c

execute: src/execute.c
	$(CC) $(CFLAGS) src/execute.c -c

pidList: src/pidList.c
	$(CC) $(CFLAGS) src/pidList.c -c

link: main.o reading.o ui.o execute.o pidList.o
	$(CC) $(CFLAGS) main.o reading.o ui.o execute.o pidList.o -o bin/shell

OBJ_RM: main.o reading.o ui.o execute.o pidList.o
	rm main.o pidList.o
	rm reading.o ui.o
	rm execute.o

clean:
	rm bin/shell
	#rm bin/reading
	rmdir bin
	#ls | grep -v "\." | grep -v Makefile | xargs rm

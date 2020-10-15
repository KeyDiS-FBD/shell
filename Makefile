#переменная CC указывает на компилятор
CC=gcc
#переменная CFLAGS хранит необходимые для компиляции флаги
CFLAGS=-Wall -Werror -lm

.PHONY: clean, all

all: bin bin/main bin/reading bin/ui link OBJ_RM
	echo Success compile

bin:
	mkdir bin -p

bin/main: src/main.c
	$(CC) $(CFLAGS) src/main.c -c

bin/ui: src/ui.c
	$(CC) $(CFLAGS) src/ui.c -c

bin/reading: src/reading.c
	$(CC) $(CFLAGS) src/reading.c -c

link: main.o reading.o ui.o
	$(CC) $(CFLAGS) main.o reading.o ui.o -o bin/shell

OBJ_RM: main.o reading.o ui.o
	rm main.o
	rm reading.o
	rm ui.o

clean:
	rm bin/main
	#rm bin/reading
	rmdir /bin
	#ls | grep -v "\." | grep -v Makefile | xargs rm

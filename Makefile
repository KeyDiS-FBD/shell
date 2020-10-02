#переменная CC указывает на компилятор
CC=gcc
#переменная CFLAGS хранит необходимые для компиляции флаги
CFLAGS=-Wall -Werror -lm 

all: bin bin/main
	echo OK

bin:
	mkdir bin -p

bin/main: src/main.c
	$(CC) $(CFLAGS) src/main.c -o bin/main
	#cpplint --filter=-legal/copyright main.c

.PHONY: clean

clean:
	rm bin/main
	rmdir /bin
	#ls | grep -v "\." | grep -v Makefile | xargs rm

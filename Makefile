#переменная CC указывает на компилятор
CC=gcc
#переменная CFLAGS хранит необходимые для компиляции флаги
CFLAGS=-Wall -Werror -lm

.PHONY: clean, all

all: bin bin/main
	echo Success compile

bin:
	mkdir bin -p

bin/main: src/main.c
	$(CC) $(CFLAGS) src/main.c src/reading.c -o bin/main
	#cpplint --filter=-legal/copyright main.c

bin/reading: src/reading.c
	$(CC) $(CFLAGS) src/reading.c



clean:
	rm bin/main
	#rm bin/reading
	rmdir /bin
	#ls | grep -v "\." | grep -v Makefile | xargs rm

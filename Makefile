CC = gcc
CFLAGS = -g -Wall -I include

bin/main:	src/main.c include/dit.h
	${CC} ${CFLAGS} -o bin/main src/main.c -lm

clean:
	rm bin/main

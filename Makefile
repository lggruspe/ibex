CC = gcc
CFLAGS = -g -Wall -I include

bin/main:	src/main.c include/dit.h
	${CC} ${CFLAGS} -o $@ $< -lm

clean:
	rm bin/main

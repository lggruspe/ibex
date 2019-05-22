CC = g++
CFLAGS = -g -Wall -std=c++1z

main:	build/main.o
	${CC} ${CFLAGS} -o main build/main.o

build/main.o:	main.cpp grammar.h
	${CC} ${CFLAGS} -c -o build/main.o main.cpp

clean:
	rm build/*


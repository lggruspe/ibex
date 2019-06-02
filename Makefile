CC = clang++-4.0 -I .
CFLAGS = -g -Wall -std=c++1z -O0

main:	build/main.o
	${CC} ${CFLAGS} -o main build/main.o

build/main.o:	main.cpp grammar.h lr1/parser.h lr1/item.h lr1/collection.h
	${CC} ${CFLAGS} -c -o build/main.o main.cpp

clean:
	rm build/*
	rm main

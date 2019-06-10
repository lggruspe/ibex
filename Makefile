CC = clang++ 
CFLAGS = -g -Wall -std=c++1z -I . -I ./include

bin/main:	build/main.o
	${CC} ${CFLAGS} -o bin/main build/main.o

build/main.o:	src/main.cpp include/grammar.h include/lr1/parser.h include/lr1/collection.h include/enumeration.h
	${CC} ${CFLAGS} -c -o build/main.o src/main.cpp

clean:
	rm build/*
	rm bin/*

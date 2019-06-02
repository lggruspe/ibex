CC = clang++-4.0 -I ..
CFLAGS = -g -Wall -std=c++1z -O0

test:	test.o
	${CC} ${CFLAGS} -o test test.o

test.o:	test.cpp test.h ../lr1/parser.h ../lr1/item.h ../lr1/collection.h
	${CC} ${CFLAGS} -c -o test.o test.cpp

clean:
	rm test.o
	rm test

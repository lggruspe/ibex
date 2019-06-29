CC = clang++
CFLAGS = -g -Wall --std=c++1z -I include

bin/main:	src/main.cpp $(shell find include -name "*.h")
	${CC} ${CFLAGS} -o bin/main src/main.cpp

clean:
	rm bin/main

example:	bin/main
	./bin/main

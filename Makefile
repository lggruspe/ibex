CC = clang++
CFLAGS = -g -Wall -std=c++1z -I ./include

bin/main:	build/main.o build/utilities.o build/generator.o build/gen2.o
	${CC} ${CFLAGS} -o bin/main build/main.o build/gen2.o build/utilities.o build/generator.o -lrnd

build/main.o:	./src/main.cpp
	${CC} ${CFLAGS} -c -o build/main.o ./src/main.cpp

build/gen2.o:	./src/gen2.cpp ./src/gen2.h
	${CC} ${CFLAGS} -c -o build/gen2.o ./src/gen2.cpp

build/generator.o:	./src/generator.cpp ./include/generator.h 
	${CC} ${CFLAGS} -c -o build/generator.o ./src/generator.cpp

build/utilities.o:	./src/utilities.cpp ./include/utilities.h
	${CC} ${CFLAGS} -c -o build/utilities.o ./src/utilities.cpp

clean:
	rm bin/*
	rm build/*

compare:	bin/main
	./bin/main > build/tmp.h
	diff --color examples/sample.h build/tmp.h

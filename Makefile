CC = clang++
CFLAGS = -g -Wall -std=c++1z -I ./include

bin/main:	build/main.o build/regex2.o build/dfa.o build/nfa.o build/utilities.o build/generator.o build/gen2.o
	${CC} ${CFLAGS} -o bin/main build/main.o build/regex2.o build/dfa.o build/nfa.o build/gen2.o build/utilities.o build/generator.o

build/main.o:	./src/main.cpp
	${CC} ${CFLAGS} -c -o build/main.o ./src/main.cpp

build/gen2.o:	./src/generate/gen2.cpp ./src/generate/gen2.h
	${CC} ${CFLAGS} -c -o build/gen2.o ./src/generate/gen2.cpp

build/generator.o:	./src/generate/generator.cpp ./include/generate/generator.h 
	${CC} ${CFLAGS} -c -o build/generator.o ./src/generate/generator.cpp

build/utilities.o:	./src/regex2/utilities.cpp ./include/regex2/utilities.h
	${CC} ${CFLAGS} -c -o build/utilities.o ./src/regex2/utilities.cpp

build/regex2.o:	./src/regex2/regex2.cpp ./include/regex2/regex2.h
	${CC} ${CFLAGS} -c -o build/regex2.o ./src/regex2/regex2.cpp

build/nfa.o:	./src/nfa.cpp ./include/nfa.h ./include/regex2/regex2.h
	${CC} ${CFLAGS} -c -o build/nfa.o ./src/nfa.cpp

build/dfa.o:	./src/dfa.cpp ./include/dfa.h ./include/nfa.h ./src/enumeration2.h ./src/partition.h
	${CC} ${CFLAGS} -c -o build/dfa.o ./src/dfa.cpp

clean:
	rm bin/*
	rm build/*

compare:	bin/main
	./bin/main > build/tmp.h
	diff --color examples/sample.h build/tmp.h

CC = clang++-4.0
CFLAGS = -g -Wall -std=c++1z -I ./include

bin/main:	build/main.o build/regex2.o build/hopcroft.o build/dfa.o build/nfa.o build/_nfa.o
	${CC} ${CFLAGS} -o bin/main build/main.o build/regex2.o build/hopcroft.o build/dfa.o build/nfa.o build/_nfa.o

build/main.o:	./src/main.cpp ./include/generate/generator.h ./include/regex2/utilities.h
	${CC} ${CFLAGS} -c -o build/main.o ./src/main.cpp

build/regex2.o:	./src/regex2/regex2.cpp ./include/regex2/regex2.h
	${CC} ${CFLAGS} -c -o build/regex2.o ./src/regex2/regex2.cpp

build/nfa.o:	./src/nfa/nfa.cpp ./include/nfa/nfa.h ./include/regex2/regex2.h
	${CC} ${CFLAGS} -c -o build/nfa.o ./src/nfa/nfa.cpp

build/_nfa.o:	./src/nfa/_nfa.cpp ./include/nfa/_nfa.h
	${CC} ${CFLAGS} -c -o build/_nfa.o ./src/nfa/_nfa.cpp

build/hopcroft.o:	./src/dfa/hopcroft.cpp ./include/dfa/hopcroft.h ./src/dfa/partition.h
	${CC} ${CFLAGS} -c -o build/hopcroft.o ./src/dfa/hopcroft.cpp

build/dfa.o:	./src/dfa/dfa.cpp ./include/dfa/dfa.h ./include/nfa/nfa.h ./src/dfa/enumeration2.h
	${CC} ${CFLAGS} -c -o build/dfa.o ./src/dfa/dfa.cpp

clean:
	rm bin/*
	rm build/*

compare:	bin/main
	./bin/main > build/tmp.h
	diff --color examples/sample.h build/tmp.h

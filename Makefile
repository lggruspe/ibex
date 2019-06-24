CC = clang++
CFLAGS = -g -Wall -std=c++1z -I ./include

sample/sample:	sample/sample.o build/regex.o build/dfa.o build/nfa.o
	${CC} ${CFLAGS} -o sample/sample sample/sample.o build/regex.o build/dfa.o build/nfa.o

sample/sample.o:	sample/sample.cpp
	${CC} ${CFLAGS} -c -o sample/sample.o sample/sample.cpp

build/regex.o:	./src/regex.cpp ./include/regex.h
	${CC} ${CFLAGS} -c -o build/regex.o ./src/regex.cpp

build/nfa.o:	./src/nfa.cpp ./include/nfa.h ./include/regex.h
	${CC} ${CFLAGS} -c -o build/nfa.o ./src/nfa.cpp

build/dfa.o:	./src/dfa.cpp ./include/dfa.h ./include/nfa.h ./src/enumeration2.h ./src/partition.h
	${CC} ${CFLAGS} -c -o build/dfa.o ./src/dfa.cpp

clean:
	rm sample
	rm build/*

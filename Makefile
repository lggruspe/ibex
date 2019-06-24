CC = clang++
CFLAGS = -g -Wall -std=c++1z -I ./include

build/librnd.a:	build/regex.o build/nfa.o build/dfa.o
	ar rcs build/librnd.a build/*.o

build/regex.o:	./src/regex.cpp ./include/regex.h
	${CC} ${CFLAGS} -c -o build/regex.o ./src/regex.cpp

build/nfa.o:	./src/nfa.cpp ./include/nfa.h ./include/regex.h
	${CC} ${CFLAGS} -c -o build/nfa.o ./src/nfa.cpp

build/dfa.o:	./src/dfa.cpp ./include/dfa.h ./include/nfa.h ./src/enumeration2.h ./src/partition.h
	${CC} ${CFLAGS} -c -o build/dfa.o ./src/dfa.cpp

clean:
	rm sample
	rm build/*

sample/sample:	sample/sample.o build/regex.o build/dfa.o build/nfa.o
	${CC} ${CFLAGS} -o sample/sample sample/sample.o build/regex.o build/dfa.o build/nfa.o

sample/sample.o:	sample/sample.cpp
	${CC} ${CFLAGS} -c -o sample/sample.o sample/sample.cpp

sample:	sample/sample
	./sample/sample

CC = g++
CFLAGS = -g -Wall -I ../regex -I . -I ../enumeration2

bin/test:	bin/test.o bin/nfa.o bin/regex.o
	${CC} ${CFLAGS} -o bin/test bin/test.o bin/nfa.o bin/regex.o

bin/regex.o: ../regex/regex.h ../regex/utilities.h
	${CC} ${CFLAGS} -c -o bin/regex.o ../regex/regex.cpp

bin/test.o:	test/test.cpp test/test.h bin/regex.o
	${CC} ${CFLAGS} -c -o bin/test.o test/test.cpp 

bin/nfa.o: nfa.cpp nfa.h
	${CC} ${CFLAGS} -c -o bin/nfa.o nfa.cpp

clean:
	rm bin/*

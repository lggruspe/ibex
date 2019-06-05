CC = g++
CFLAGS = -g -Wall -I ../categories -I .

bin/test:	bin/test.o bin/nfa.o bin/regex.o bin/_nfa.o
	${CC} ${CFLAGS} -o bin/test bin/test.o bin/nfa.o bin/regex.o bin/_nfa.o

bin/regex.o: ../categories/regex2.h ../categories/utilities.h
	${CC} ${CFLAGS} -c -o bin/regex.o ../categories/regex2.cpp

bin/test.o:	test/test.cpp bin/regex.o
	${CC} ${CFLAGS} -c -o bin/test.o test/test.cpp 

bin/nfa.o: nfa.cpp nfa.h
	${CC} ${CFLAGS} -c -o bin/nfa.o nfa.cpp

bin/_nfa.o:	_nfa.cpp _nfa.h
	${CC} ${CFLAGS} -c -o bin/_nfa.o _nfa.cpp

clean:
	rm bin/*

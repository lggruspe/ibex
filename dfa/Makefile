CC = clang++-4.0
CFLAGS = -g -Wall -std=c++1z -I ../nfa -I . -I ../regex

bin/test:	bin/test.o bin/hopcroft.o bin/regex.o bin/dfa.o bin/nfa.o
	${CC} ${CFLAGS} -o bin/test bin/test.o bin/hopcroft.o bin/regex.o bin/dfa.o bin/nfa.o

bin/test.o:	../regex/utilities.h test/test.cpp test/test.h
	${CC} ${CFLAGS} -c -o bin/test.o test/test.cpp

bin/hopcroft.o:	hopcroft.cpp hopcroft.h partition.h
	${CC} ${CFLAGS} -c -o bin/hopcroft.o hopcroft.cpp

bin/regex.o:	../regex/regex.h ../regex/regex.cpp
	${CC} ${CFLAGS} -c -o bin/regex.o ../regex/regex.cpp

bin/dfa.o:	dfa.cpp dfa.h enumeration.h
	${CC} ${CFLAGS} -c -o bin/dfa.o dfa.cpp

bin/nfa.o:	../nfa/nfa.cpp ../nfa/nfa.h 
	${CC} ${CFLAGS} -c -o bin/nfa.o ../nfa/nfa.cpp 

clean:
	rm bin/*

CC = clang++-4.0
CFLAGS = -g -Wall -std=c++1z -I ../nfa -I . -I ../enumeration2 -I ../categories

bin/test:	bin/test.o bin/hopcroft.o bin/regex.o bin/dfa.o bin/nfa.o bin/_nfa.o
	${CC} ${CFLAGS} -o bin/test bin/test.o bin/hopcroft.o bin/regex.o bin/dfa.o bin/nfa.o bin/_nfa.o

bin/test.o:	../categories/utilities.h test/test.cpp 
	${CC} ${CFLAGS} -c -o bin/test.o test/test.cpp

bin/hopcroft.o:	hopcroft.cpp hopcroft.h partition.h
	${CC} ${CFLAGS} -c -o bin/hopcroft.o hopcroft.cpp

bin/regex.o:	../categories/regex2.h ../categories/regex2.cpp
	${CC} ${CFLAGS} -c -o bin/regex.o ../categories/regex2.cpp

bin/dfa.o:	dfa.cpp dfa.h ../enumeration2/enumeration.h ../enumeration2/enumeration2.h
	${CC} ${CFLAGS} -c -o bin/dfa.o dfa.cpp

bin/_nfa.o:	../nfa/_nfa.cpp ../nfa/_nfa.h
	${CC} ${CFLAGS} -c -o bin/_nfa.o ../nfa/_nfa.cpp

bin/nfa.o:	../nfa/nfa.cpp ../nfa/nfa.h 
	${CC} ${CFLAGS} -c -o bin/nfa.o ../nfa/nfa.cpp 

clean:
	rm bin/*

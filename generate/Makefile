CC = clang++-4.0
CFLAGS = -g -Wall -std=c++1z -I . -I ../categories -I ../dfa -I ../enumeration2 -I ../nfa

main:	main.o dfa.o hopcroft.o nfa.o _nfa.o regex2.o cats.o
	${CC} ${CFLAGS} -o bin/main bin/main.o bin/dfa.o bin/hopcroft.o bin/nfa.o bin/_nfa.o bin/regex2.o bin/cats.o

main.o:	main.cpp ../categories/utilities.h gen2.h generator.h
	${CC} ${CFLAGS} -c -o bin/main.o main.cpp

dfa.o:	../dfa/dfa.cpp ../dfa/dfa.h ../enumeration2/enumeration2.h
	${CC} ${CFLAGS} -c -o bin/dfa.o ../dfa/dfa.cpp

hopcroft.o:	../dfa/hopcroft.cpp ../dfa/hopcroft.h ../dfa/partition.h
	${CC} ${CFLAGS} -c -o bin/hopcroft.o ../dfa/hopcroft.cpp

nfa.o:	../nfa/nfa.cpp ../nfa/nfa.h
	${CC} ${CFLAGS} -c -o bin/nfa.o ../nfa/nfa.cpp

_nfa.o:	../nfa/_nfa.cpp ../nfa/_nfa.h
	${CC} ${CFLAGS} -c -o bin/_nfa.o ../nfa/_nfa.cpp

regex2.o:	../categories/regex2.cpp ../categories/regex2.h
	${CC} ${CFLAGS} -c -o bin/regex2.o ../categories/regex2.cpp

cats.o:	../categories/cats.cpp ../categories/cats.h
	${CC} ${CFLAGS} -c -o bin/cats.o ../categories/cats.cpp

clean:
	rm bin/*

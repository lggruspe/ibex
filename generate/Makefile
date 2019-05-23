CC = clang++-4.0
CFLAGS = -g -Wall -std=c++1z -I ../dfa -I ../regex -I ../nfa -I . -I ./generator

bin/main:	bin/main.o bin/generate.o bin/dfa.o bin/regex.o bin/nfa.o bin/hopcroft.o
	${CC} ${CFLAGS} -o bin/main bin/main.o bin/generate.o bin/dfa.o bin/regex.o bin/nfa.o bin/hopcroft.o

bin/main.o:	main.cpp generator/generate_category.h ../regex/utilities.h
	${CC} ${CFLAGS} -c -o bin/main.o main.cpp

bin/generate.o:	generator/generate.h generator/generate.cpp
	${CC} ${CFLAGS} -c -o bin/generate.o generator/generate.cpp

bin/dfa.o: ../dfa/dfa.cpp ../dfa/dfa.h
	${CC} ${CFLAGS} -c -o bin/dfa.o ../dfa/dfa.cpp

bin/regex.o:	../regex/regex.cpp ../regex/regex.h
	${CC} ${CFLAGS} -c -o bin/regex.o ../regex/regex.cpp

bin/nfa.o:	../nfa/nfa.cpp ../nfa/nfa.h
	${CC} ${CFLAGS} -c -o bin/nfa.o ../nfa/nfa.cpp

bin/hopcroft.o:	../dfa/hopcroft.h ../dfa/hopcroft.cpp
	${CC} ${CFLAGS} -c -o bin/hopcroft.o ../dfa/hopcroft.cpp

clean:
	rm bin/*

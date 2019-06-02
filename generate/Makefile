CC = clang++-4.0
CFLAGS = -g -Wall -std=c++1z -I ../dfa -I ../regex -I ../nfa -I . -I ./generator

main:	bin/main.o bin/generator.o bin/dfa.o bin/regex.o bin/nfa.o bin/hopcroft.o
	${CC} ${CFLAGS} -o bin/main bin/main.o bin/generator.o bin/dfa.o bin/regex.o bin/nfa.o bin/hopcroft.o

bin/main.o:	main.cpp ../regex/utilities.h
	${CC} ${CFLAGS} -c -o bin/main.o main.cpp

bin/generator.o:	generator/generator.h generator/generator.cpp
	${CC} ${CFLAGS} -c -o bin/generator.o generator/generator.cpp

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

maketest:	main
	rm sample/test.h
	./bin/main >> sample/test.h
	${CC} sample/test.cpp -o bin/test -std=c++1z

test:	maketest
	./bin/test

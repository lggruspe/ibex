CC = clang++
CFLAGS = -g -Wall --std=c++1z

main:	main.cpp enumeration.h grammar.h item.h state.h automaton.h parsingtable.h parser.h scanner.h
	${CC} ${CFLAGS} -o main main.cpp

clean:
	rm main

CC = clang++-4.0
CFLAGS = -g -Wall -Werror -std=c++1z

main:	bin/main.o
	${CC} ${CFLAGS} -o main bin/main.o -licuuc

bin/main.o:	main.cpp cats.h regex2.h
	${CC} ${CFLAGS} -o bin/main.o -c main.cpp

clean:
	rm main.o
	rm main
	rm bin/*

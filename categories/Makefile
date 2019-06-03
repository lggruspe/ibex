CC = clang++-4.0
CFLAGS = -g -Wall -Werror -std=c++1z

main:	main.cpp bin/cats.o bin/regex2.o
	${CC} ${CFLAGS} -o main main.cpp bin/regex2.o bin/cats.o -licuuc

bin/regex2.o:	regex2.cpp regex2.h
	${CC} ${CFLAGS} -o bin/regex2.o -c regex2.cpp

bin/main.o:	main.cpp
	${CC} ${CFLAGS} -o bin/main.o -c main.cpp

bin/cats.o:	cats.cpp cats.h
	${CC} ${CFLAGS} -o bin/cats.o -c cats.cpp

clean:
	rm main
	rm bin/*

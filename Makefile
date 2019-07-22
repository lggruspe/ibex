CC = gcc
CFLAGS = -g -Wall -I include
CXX = g++
CXXFLAGS = -g -Wall -std=c++17 -I include

vpath %.h include

.PHONY:	all
all:	bin/dis

bin/dis:	test/dis.c dis.h test/test_lib.h
	${CC} ${CFLAGS} -o $@ $< -lm

bin/profile:	test/profile.cpp test/test_lib.h
	${CXX} -g -Wall -std=c++17 -I include -pg -o $@ $< -lm

.PHONY:	clean
clean:
	-rm -f bin/dis bin/profile vgcore.* gmon.out

.PHONY:	test
test:	all
	bin/dis

.PHONY:	profile
profile:	bin/profile
	bin/profile
	gprof -p -b bin/profile gmon.out > analysis.txt

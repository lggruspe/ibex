CC = gcc
CFLAGS = -g -Wall -I include
CXX = g++
CXXFLAGS = -g -Wall -std=c++17 -I include

TESTS = $(patsubst test/%.c,bin/%,$(wildcard test/*.c))

vpath %.h include
vpath %.hpp include

.PHONY:	all
all:	bin/dis bin/dis2 bin/distree

$(TESTS):	bin/%: test/%.c %.h test/test_lib.h
	${CC} ${CFLAGS} -o $@ $< -lm

bin/profile:	test/profile.cpp test/test_lib.h
	${CXX} -g -Wall -std=c++17 -I include -pg -o $@ $< -lm

.PHONY:	clean
clean:
	-rm -f $(TESTS) bin/profile vgcore.* gmon.out

.PHONY:	test
test:	all
	bin/dis
	bin/dis2
	bin/distree

.PHONY:	profile
profile:	bin/profile
	bin/profile
	gprof -p -b bin/profile gmon.out > analysis.txt


bin/distree:	test/distree.cpp distree.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

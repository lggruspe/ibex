CC = gcc
CFLAGS = -g -Wall -I include
CXX = g++
CXXFLAGS = -g -Wall -std=c++17 -I include

TESTS = $(patsubst test/%.c,bin/%,$(wildcard test/*.c))

vpath %.h include
vpath %.hpp include

.PHONY:	all
all:	bin/distree

#$(TESTS):	bin/%: test/%.c %.h
#	${CC} ${CFLAGS} -o $@ $< -lm

bin/distree:	test/distree.cpp src/distree.cpp distree.h
	$(CXX) $(CXXFLAGS) -o $@ $< src/distree.cpp

bin/profile:	test/profile.cpp
	${CXX} -g -Wall -std=c++17 -I include -pg -o $@ $< -lm

.PHONY:	clean
clean:
	-rm -f $(TESTS) vgcore.* gmon.out bin/distree

.PHONY:	test
test:	all
	bin/distree

.PHONY:	profile
profile:	bin/profile
	bin/profile
	gprof -p -b bin/profile gmon.out > analysis.txt

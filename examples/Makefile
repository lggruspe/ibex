CC = gcc
CFLAGS = -g -Wall -I../include
CXX = g++
CXXFLAGS = -g -Wall -I../src/rnd -I../src/distree/include -I../src/red-black-tree

vpath %.so ../lib

all:	example_cpp example_c

example_cpp:	example.cpp librnd.so
	$(CXX) $(CXXFLAGS) -o $@ $^

example_c:	example.c libcrnd.so
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm -f example_cpp example_c

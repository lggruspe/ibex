CC = gcc
CPPFLAGS = -I../include -L../lib
CFLAGS = -g -Wall -Wextra
CXX = g++
CXXFLAGS = -g -Wall -I../include -I../src

vpath %.so ../lib

all:	example_cpp example_c

example_cpp:	example.cpp librnd.so
	$(CXX) $(CXXFLAGS) -o $@ $^

example_c:	example.c libcrnd.so
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

clean:
	-rm -f example_cpp example_c

CC = gcc
CXX = g++
CFLAGS = -g -Wall -Wextra -Wpedantic
CXXFLAGS = -g -Wall -Wextra -Wpedantic -fpic -std=c++17 -Werror
CPPFLAGS = -I. -I../../../tools -I../../../sagl/include -L/usr/local/lib -L.

all:	main

regexp.o:	regexp.cxx regexp.h ../../../tools/handles.hpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

libregexp.so:	regexp.o
	$(CXX) -shared -o $@ $<

main:	main.c libregexp.so
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -lregexp

clean:
	-rm -rf *.o main *.so vgcore*

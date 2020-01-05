CC = gcc
CXX = g++
CFLAGS = -g -Wall -Wextra -Wpedantic
CXXFLAGS = -g -Wall -Wextra -Wpedantic -fpic -std=c++17 -Werror
CPPFLAGS = -I. -I../../../tools -I../../../sagl/include -L/usr/local/lib -L.

all:	main_cpp main_c

regexp.o:	regexp.cxx regexp.h ../../../tools/handles.hpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

libregexp.so:	regexp.o
	$(CXX) -shared -o $@ $<

main_c:	main.c libregexp.so
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -lregexp

main_cpp:	main.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	-rm -rf *.o main_c main_cpp *.so vgcore*

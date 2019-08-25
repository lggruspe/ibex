CC = gcc
CFLAGS = -g -Wall -Wextra
CPPFLAGS = -I../extra

CXX = g++
CXXFLAGS = -g -std=gnu++17 -Wall -Wextra

.PHONY:	all
all:	build build/example.c build/example.cpp

build:
	mkdir -p build

build/example.c:	c/template.c c/scanner.c
	python example.py template.c -d c > build/example.c

build/example.cpp:	cpp/template.cpp cpp/scanner.cpp
	python example.py template.cpp -d cpp > build/example.cpp

.PHONY:	clean
clean:
	-rm -rf build

.PHONY:	compile
compile:	all
	$(CC) $(CFLAGS) $(CPPFLAGS) -o build/example_c build/example.c
	$(CXX) $(CXXFLAGS) -o build/example_cpp build/example.cpp

CC = gcc
CFLAGS = -g -Wall -Wextra
CPPFLAGS = -I../extra

CXX = g++
CXXFLAGS = -g -std=gnu++17 -Wall -Wextra

.PHONY:	all
all:	build build/example.c build/example.cpp build/example.py

build:
	mkdir -p build

build/example.c:	c/template.c c/scanner.c
	python example.py template.c -d c > build/example.c

build/example.cpp:	cpp/template.cpp cpp/scanner.cpp
	python example.py template.cpp -d cpp > build/example.cpp

build/example.py:	py/template.py py/scanner.py
	python example.py template.py -d py > build/example.py

.PHONY:	clean
clean:
	-rm -rf build

.PHONY:	compile
compile:	all
	$(CC) $(CFLAGS) $(CPPFLAGS) -o build/example_c build/example.c
	$(CXX) $(CXXFLAGS) -o build/example_cpp build/example.cpp
	cp build/example.py build/example_py
	chmod +x build/example_py

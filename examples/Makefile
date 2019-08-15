CC = gcc
CFLAGS = -g -Wall -Wextra
CPPFLAGS = -I../extra

CXX = g++
CXXFLAGS = -g -std=gnu++17 -Wall -Wextra

.PHONY:	all
all:	build/example_c build/example_cpp build/example_py

build/example_c:	c/template.c c/scanner.c
	python example.py template.c -d c > build/example.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ build/example.c


build/example_cpp:	cpp/template.cpp cpp/scanner.cpp
	python example.py template.cpp -d cpp > build/example.cpp
	$(CXX) $(CXXFLAGS) -o $@ build/example.cpp

build/example_py:	python/template.py python/scanner.py
	python example.py template.py -d python > build/example_py
	chmod +x build/example_py

.PHONY:	clean
clean:
	-rm -f build/example*

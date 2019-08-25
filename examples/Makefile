CC = gcc
CFLAGS = -g -Wall -Wextra
CPPFLAGS = -I../extra

.PHONY:	all
all:	build build/example.c

build:
	mkdir -p build

build/example.c:	c/template.c c/scanner.c
	python example.py template.c -d c > build/example.c

.PHONY:	clean
clean:
	-rm -rf build

.PHONY:	compile
compile:	all
	$(CC) $(CFLAGS) $(CPPFLAGS) -o build/example_c build/example.c

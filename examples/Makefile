CC = gcc
CFLAGS = -g -Wall
CPPFLAGS = -I../extra

build/example_c:	c/template.c c/scanner.c
	python example.py template.c -d c > build/example.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ build/example.c


.PHONY:	clean
clean:
	rm build/*

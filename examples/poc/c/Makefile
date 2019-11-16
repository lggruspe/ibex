CC = gcc
CFLAGS = -g -Wall -Wextra
CPPFLAGS = -I../extra

.PHONY:	all
all:	output/scanners.h

output/scanners.h:	templates/template.c templates/scanner.c
	python ../example.py template.c -d templates > $@

.PHONY:	clean
clean:
	-rm -rf output/scanners.h a.out

.PHONY:	compile
compile:	all
	$(CC) $(CFLAGS) $(CPPFLAGS) test/run.c

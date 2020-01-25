CC = gcc
CFLAGS = -g -Wall -Wextra -Wpedantic

.PHONY:	all
all:	output/scanner.h output/scanner_utils.h output/scanner.py compile

output/scanner.h:	templates/template.c templates/recognizer.c
	python ../example.py template.c -d templates > $@

output/scanner_utils.h:	templates/scanner_utils.c
	python ../example.py scanner_utils.c -d templates > $@

output/scanner.py:	templates/scanner.py
	python ../example.py scanner.py -d templates > $@

.PHONY:	clean
clean:
	-rm -rf run_match

.PHONY:	compile
compile:	run_match

run_match:	run.c
	$(CC) $(CFLAGS) -o $@ $<

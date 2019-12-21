CC = gcc
CFLAGS = -g -Wall -Wextra -Wpedantic

.PHONY:	all
all:	output/scanners.h output/scanner_utils.h output/scanners.py compile

output/scanners.h:	templates/template.c templates/scanner.c
	python ../example.py template.c -d templates > $@

output/scanner_utils.h:	templates/scanner_utils.c
	python ../example.py scanner_utils.c -d templates > $@

output/scanners.py:	templates/scanners.py
	python ../example.py scanners.py -d templates > $@

.PHONY:	clean
clean:
	-rm -rf test/run_match

.PHONY:	compile
compile:	test/run_match

test/run_match:	test/run.c
	$(CC) $(CFLAGS) -o $@ $<

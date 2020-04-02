CC = gcc
CFLAGS = -g -Wall -Wextra -Wpedantic

run_match:	run.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY:	clean
clean:
	-rm -rf run_match scanner.h scanner.c scanner.py utils.h __pycache__

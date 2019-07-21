CC = gcc
CFLAGS = -g -Wall -I include

vpath %.h include

.PHONY:	all
all:	bin/dis

bin/dis:	test/dis.c dis.h test/test_lib.h
	${CC} ${CFLAGS} -o $@ $< -lm

.PHONY:	clean
clean:
	-rm -f bin/dis vgcore.*

.PHONY:	test
test:	all
	bin/dis

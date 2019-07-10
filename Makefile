CC = clang++

C++17 =
ifeq ($(CC),clang++)
	C++17=-std=c++1z
else
	C++17=-std=c++17
endif

CFLAGS = -g -Wall $(C++17) -I include
OBJECTS = build/main.o build/gen2.o build/generator.o build/utilities.o

vpath %.o build
vpath %.cpp src
vpath %.h include src
vpath -lrnd /usr/local/lib

bin/main:	$(OBJECTS) -lrnd
	$(CC) $(CFLAGS) -o $@ $^

$(OBJECTS):	build/%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

build/gen2.o build/generator.o build/utilities.o:	build/%.o: %.h

-lrnd:

clean:
	-rm -f bin/main $(OBJECTS) build/tmp.h

compare:	bin/main
	bin/main > build/tmp.h
	diff --color examples/sample.h build/tmp.h

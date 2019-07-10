CC = clang++

C++17 =
ifeq ($(CC),clang++)
	C++17=-std=c++1z
else
	C++17=-std=c++17
endif

CFLAGS = -g -Wall $(C++17) -I include
OBJECTS = build/regex.o build/nfa.o build/dfa.o

vpath %.cpp src
vpath %.o build
vpath %.h include src

build/librnd.a:	$(OBJECTS)
	ar rcs build/librnd.a $(OBJECTS)

$(OBJECTS):	build/%.o : %.cpp %.h
	$(CC) $(CFLAGS) -c -o $@ $<

build/nfa.o:	nfa.h

build/dfa.o:	nfa.h enumeration2.h partition.h

.PHONY:	clean
clean:
	-rm -f $(OBJECTS) sample/sample.o sample/sample build/*.a

sample/sample:	sample/sample.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

sample/sample.o:	sample/sample.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

sample:	sample/sample
	./sample/sample

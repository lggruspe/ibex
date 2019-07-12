AR = ar
CXX = clang++

C++17 =
ifeq ($(CXX),clang++)
	C++17=-std=c++1z
else
	C++17=-std=c++17
endif

CXXFLAGS = -g -Wall $(C++17) -I include
OBJECTS = build/regex.o build/nfa.o build/dfa.o
prefix = /usr/local
bindir = $(prefix)/bin
includedir = $(prefix)/include
libdir = $(prefix)/lib

vpath %.cpp src
vpath %.o build
vpath %.h include src

.PHONY:	all
all:	lib/librnd.a

lib/librnd.a:	$(OBJECTS)
	$(AR) rcs lib/librnd.a $(OBJECTS)

$(OBJECTS):	build/%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/nfa.o:	nfa.h

build/dfa.o:	nfa.h enumeration2.h partition.h

.PHONY:	clean
clean:
	-rm -f $(OBJECTS) sample/sample.o sample/sample

sample/sample:	sample/sample.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

sample/sample.o:	sample/sample.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

sample:	sample/sample
	./sample/sample

.PHONY:	install
install:	lib/librnd.a
	if [ ! -d "$(prefix)" ]; then \
		mkdir "$(prefix)"; \
	fi; \
	if [ ! -d $(includedir) ]; then \
		mkdir "$(includedir)"; \
	fi; \
	if [ ! -d $(libdir) ]; then \
		mkdir "$(libdir)"; \
	fi; \
	cp $< $(libdir); \
	cp -r include "$(includedir)/rnd";
	# TODO don't replace files if already exists, and is not from rnd library

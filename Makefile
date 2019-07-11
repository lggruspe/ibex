CXX = clang++

C++17 =
ifeq ($(CXX),clang++)
	C++17=-std=c++1z
else
	C++17=-std=c++17
endif

CXXFLAGS = -g -Wall $(C++17) -I include
OBJECTS = build/regex.o build/nfa.o build/dfa.o

vpath %.cpp src
vpath %.o build
vpath %.h include src

lib/librnd.a:	$(OBJECTS)
	ar rcs lib/librnd.a $(OBJECTS)

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

CXX = clang++

C++17 =
ifeq ($(CXX),clang++)
	C++17=--std=c++1z
else
	C++17=--std=c++17
endif

CPPFLAGS = -I include
CFLAGS = -g -Wall
CXXFLAGS = -g -Wall $(C++17)

vpath main.cpp src

bin/main:	main.cpp $(shell find include -name "*.hpp")
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.PHONY:	clean
clean:
	-rm -f bin/main

example:	bin/main
	./bin/main

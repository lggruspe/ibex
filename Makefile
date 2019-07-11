CXX = clang++

C++17 =
ifeq ($(CXX),clang++)
	C++17=--std=c++1z
else
	C++17=--std=c++17
endif

CFLAGS = -g -Wall $(C++17) -I include

vpath main.cpp src

bin/main:	main.cpp $(shell find include -name "*.hpp")
	$(CXX) $(CFLAGS) -o $@ $<

.PHONY:	clean
clean:
	-rm -f bin/main

example:	bin/main
	./bin/main

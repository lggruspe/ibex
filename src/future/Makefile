CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic -Werror -std=c++17 -fpic

targets = $(patsubst %.cpp,%,$(shell find -name "*.cpp"))

.PHONY:	all
all:	$(targets) libfuture.a libfuture.so example_c

$(targets):	%:	%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

libfuture.a:	wrapper.cxx rnd.h
	$(CXX) $(CXXFLAGS) -c -o _future_.o $<
	$(AR) rcs $@ _future_.o
	rm _future_.o

libfuture.so:	wrapper.cxx rnd.h
	$(CXX) $(CXXFLAGS) -c -o _future_.o $<
	$(CXX) -shared -o $@ _future_.o
	rm _future_.o

example_c:	example.c
	gcc -I. -L. $< -o $@ -lfuture 

.PHONY:	clean
clean:
	-rm -rf $(targets) vgcore* example_c

CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic -Werror -std=c++17 -fpic

targets = $(patsubst %.cpp,%,$(shell find -name "*.cpp"))

.PHONY:	all
all:	$(targets) libfuture.a libfuture.so example_c

$(targets):	%:	%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

libfuture.a:	wrapper.cxx rnd.h dfa.hpp
	$(CXX) $(CXXFLAGS) -c -o _future_.o $<
	$(AR) rcs $@ _future_.o
	rm _future_.o

libfuture.so:	wrapper.cxx rnd.h dfa.hpp
	$(CXX) $(CXXFLAGS) -c -o _future_.o $<
	$(CXX) -shared -o $@ _future_.o
	rm _future_.o

example_c:	example.c libfuture.so libfuture.a
	gcc -I. -L. $< -o $@ -lfuture 

.PHONY:	clean
clean:
	-rm -rf $(targets) vgcore* example_c *.pdf *.so *.a

.PHONY:	install
install:	libfuture.so
	cp rnd.h /usr/local/include
	cp libfuture.so /usr/local/lib/libcrnd.so
	cp libfuture.so /usr/local/lib/libcrnd.so.0.0.0

.PHONY:	uninstall
uninstall:
	rm /usr/local/include/rnd.h
	rm /usr/local/lib/libcrnd.so
	rm /usr/local/lib/libcrnd.so.0.0.0

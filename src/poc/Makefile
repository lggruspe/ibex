CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic -Werror -std=c++17 -fpic
TESTS = $(patsubst %.cpp,%,$(shell find -name "*.test.cpp"))

.PHONY:	all
all:	$(TESTS) libfuture.a libfuture.so example_c

$(TESTS):	%.test:	%.test.cpp %.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

libfuture.a:	wrapper.cxx rnd.h automaton.hpp nexpr.hpp zsymbols.hpp
	$(CXX) $(CXXFLAGS) -c -o _future_.o $<
	$(AR) rcs $@ _future_.o
	rm _future_.o

libfuture.so:	wrapper.cxx rnd.h automaton.hpp nexpr.hpp zsymbols.hpp
	$(CXX) $(CXXFLAGS) -c -o _future_.o $<
	$(CXX) -shared -o $@ _future_.o
	rm _future_.o

example_c:	example.c libfuture.so libfuture.a
	gcc -I. -L. $< -o $@ -lfuture

check:	all
	@for i in $(TESTS); \
	do \
		./$$i; \
	done

.PHONY:	clean
clean:
	-rm -rf $(TESTS) example_c *.so *.a

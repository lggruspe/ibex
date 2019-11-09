CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic -Werror -std=c++17

all:	zsymbols.test nexpr.test

zsymbols.test:	zsymbols.test.cpp zsymbols.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

nexpr.test:	nexpr.test.cpp nexpr.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	-rm -rf zsymbols.test nexpr.test

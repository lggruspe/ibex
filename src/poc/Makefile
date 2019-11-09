CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic -Werror -std=c++17

zsymbols.test:	zsymbols.test.cpp zsymbols.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	-rm -rf zsymbols.test

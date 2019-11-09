CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic -Werror -std=c++17
TESTS = zsymbols.test nexpr.test

all:	$(TESTS)

$(TESTS):	%.test:	%.test.cpp %.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	-rm -rf $(TESTS)

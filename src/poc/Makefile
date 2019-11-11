CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic -Werror -std=c++17
TESTS = $(patsubst %.cpp,%,$(shell find -name "*.test.cpp"))

all:	$(TESTS)

$(TESTS):	%.test:	%.test.cpp %.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

check:	all
	@for i in $(TESTS); \
	do \
		./$$i; \
	done

clean:
	-rm -rf $(TESTS)

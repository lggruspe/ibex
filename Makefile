CXX = g++
CXXFLAGS = -g -Wall --std=c++17


test:	test.cpp tree.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	-rm -f test

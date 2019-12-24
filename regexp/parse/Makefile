CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic -Werror -std=c++17
CPPFLAGS = -I. -I../../sagl/src/temp

main:	main.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	-rm -rf main

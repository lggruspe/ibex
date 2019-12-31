CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic -Werror -std=c++17
CPPFLAGS = -I. -I../../sagl/include

main:	main.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	-rm -rf main

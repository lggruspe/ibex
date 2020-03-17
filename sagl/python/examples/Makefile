CXX = g++
CXXFLAGS = -g -Wall -Wextra -Werror -Wpedantic -std=c++17

main:	main.cpp example.py
	LD_LIBRARY_PATH=../../build python3 example.py
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.PHONY:	clean
clean:
	rm -rf main parser.hpp scanner.hpp

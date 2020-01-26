CXX = g++
CXXFLAGS = -g -std=gnu++17 -Wall -Wextra -Wpedantic

run_match:	run.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY:	clean
clean:
	-rm -rf run_match scanner.hpp scanner.py scanner_utils.hpp __pycache__

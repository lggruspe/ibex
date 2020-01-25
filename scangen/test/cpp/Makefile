CXX = g++
CXXFLAGS = -g -std=gnu++17 -Wall -Wextra -Wpedantic

.PHONY:	all
all:	output/scanner.hpp output/scanner_utils.hpp compile output/scanner.py

output/scanner.hpp:	templates/template.cpp templates/recognizer.cpp
	python ../example.py template.cpp -d templates > $@

output/scanner_utils.hpp:	templates/scanner_utils.cpp
	python ../example.py scanner_utils.cpp -d templates > $@

output/scanner.py:	templates/scanner.py
	python ../example.py scanner.py -d templates > $@

.PHONY:	clean
clean:
	-rm -rf run_match

.PHONY:	compile
compile:	run_match

run_match:	run.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

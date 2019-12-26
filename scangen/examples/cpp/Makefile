CXX = g++
CXXFLAGS = -g -std=gnu++17 -Wall -Wextra -Wpedantic

.PHONY:	all
all:	output/scanners.hpp output/scanner_utils.hpp compile output/scanners.py

output/scanners.hpp:	templates/template.cpp templates/scanner.cpp
	python ../example.py template.cpp -d templates > $@

output/scanner_utils.hpp:	templates/scanner_utils.cpp
	python ../example.py scanner_utils.cpp -d templates > $@

output/scanners.py:	templates/scanners.py
	python ../example.py scanners.py -d templates > $@

.PHONY:	clean
clean:
	-rm -rf run_match

.PHONY:	compile
compile:	run_match

run_match:	run.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

CXX = g++
CXXFLAGS = -g -std=gnu++17 -Wall -Wextra -Wpedantic
CPPFLAGS = -I../extra

.PHONY:	all
all:	output/scanners.hpp output/scanner_utils.hpp

output/scanners.hpp:	templates/template.cpp templates/scanner.cpp
	python ../example.py template.cpp -d templates > $@

output/scanner_utils.hpp:	templates/scanner_utils.cpp
	python ../example.py scanner_utils.cpp -d templates > $@

.PHONY:	clean
clean:
	-rm -rf test/a.out

.PHONY:	compile
compile:	test/a.out

test/a.out:	test/run.cpp test/match.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

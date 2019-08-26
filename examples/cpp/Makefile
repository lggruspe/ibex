CXX = g++
CXXFLAGS = -g -std=gnu++17 -Wall -Wextra -Wpedantic
CPPFLAGS = -I../extra

.PHONY:	all
all:	output/scanners.hpp

output/scanners.hpp:	templates/template.cpp templates/scanner.cpp
	python ../example.py template.cpp -d templates > $@

.PHONY:	clean
clean:
	-rm -rf a.out

.PHONY:	compile
compile:	all
	$(CXX) $(CXXFLAGS) test/run.cpp

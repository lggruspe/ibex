test:	test.cpp tree.hpp
	g++ --std=c++17 test.cpp -o test

clean:
	-rm -f test

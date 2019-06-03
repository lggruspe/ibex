
bin/test:	bin/test.o bin/regex.o
	g++ -g -Wall bin/test.o bin/regex.o -o bin/test

bin/test.o:	test.cpp utilities.h
	g++ -c -g -Wall test.cpp -o bin/test.o

bin/regex.o:	regex.cpp regex.h
	g++ -c -g -Wall regex.cpp -o bin/regex.o

clean:
	rm bin/*

test:	bin/test
	./bin/test

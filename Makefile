AR = ar
CXX = g++

C++17 =
ifeq ($(CXX),clang++)
	C++17=-std=c++1z
else
	C++17=-std=c++17
endif

CXXFLAGS = -g -Wall -fpic $(C++17) -I./include -I./src -I./src/rnd -I./src/distree
OBJECTS = build/regex.o build/nfa.o build/dfa.o build/distree.o
prefix = /usr/local
bindir = $(prefix)/bin
includedir = $(prefix)/include
libdir = $(prefix)/lib
TESTS = bin/test_distree bin/test_redblack

vpath %.cpp src src/rnd src/distree/src src/distree
vpath %.o build
vpath %.h include src src/rnd src/distree
vpath %.hpp include src src/rnd

.PHONY:	all
all:	lib/librnd.a lib/librnd.so lib/libcrnd.so

lib/librnd.so:	$(OBJECTS)
	$(CXX) -shared -o $@ $^

lib/librnd.a:	$(OBJECTS)
	$(AR) rcs $@ $^

$(OBJECTS):	%:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/regex.o:	regex.cpp regex.h alphabet.hpp

build/nfa.o:	nfa.cpp nfa.h

build/dfa.o:	dfa.cpp dfa.h nfa.h rnd/enumeration2.h rnd/partition.h

build/distree.o:	distree/distree.cpp distree/distree.h redblack/tree.hpp

.PHONY:	clean
clean:
	-rm -f $(OBJECTS) vgcore.* build/*.o $(TESTS)

.PHONY:	install
install:	lib/libcrnd.so
	@if [ ! -d "$(prefix)" ]; then \
		mkdir "$(prefix)"; \
	fi; \
	if [ ! -d $(includedir) ]; then \
		mkdir "$(includedir)"; \
	fi; \
	if [ ! -d $(libdir) ]; then \
		mkdir "$(libdir)"; \
	fi; \
	cp $< $(libdir); \
	cp -r include/* "$(includedir)";

build/rnd.o:	rnd.cpp rnd.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lib/libcrnd.so:	build/rnd.o $(OBJECTS)
	$(CXX) -shared -o $@ $^

.PHONY:	docker-build
docker-build:
	docker rmi -f rnd-dev;\
	docker build -t rnd-dev .

.PHONY:	docker-rund
docker-rund:
	docker run -d -it -v $$(pwd):/home rnd-dev /bin/sh

.PHONY:	docker-run
docker-run:
	docker run -it -v $$(pwd):/home rnd-dev /bin/sh

## Tests

bin/test_distree:	test_distree.cpp distree.cpp distree.h test_runner.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(filter-out %.hpp,$(filter-out %.h,$^))
	./$@

bin/test_redblack:	redblack/test.cpp redblack/tree.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(filter-out %.hpp,$(filter-out %.h,$^))
	./$@

.PHONY:	test
test:	$(TESTS)


## profiling flags (examples)
# $(CXX) -g -Wall -std=c++17 -I./include -pg -o $@ $< -lm
#
# generating profile analysis:
# ./bin/profile
# gprof -p -b bin/profile gmon.out > analysis.txt

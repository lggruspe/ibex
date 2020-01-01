lib_LTLIBRARIES = lib/librndpoc.la
lib_librndpoc_la_SOURCES = \
	include/rnd.h \
	src/automaton.hpp \
	src/nexpr.hpp \
	src/wrapper.cxx \
	src/zsymbols.hpp \
	../tools/handles.hpp
lib_librndpoc_la_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_srcdir)/../tools
lib_librndpoc_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = include/rnd.h

ACLOCAL_AMFLAGS = -I m4

check_PROGRAMS = examples/example_c examples/example_cpp \
	src/automaton.test src/nexpr.test src/zsymbols.test

src_automaton_test_SOURCES = src/automaton.hpp src/automaton.test.cpp ../tools/handles.hpp

src_automaton_test_CPPFLAGS = -I$(top_srcdir)/../tools

src_nexpr_test_SOURCES = src/nexpr.hpp src/nexpr.test.cpp

src_zsymbols_test_SOURCES = src/zsymbols.hpp src/zsymbols.test.cpp

examples_example_c_SOURCES = include/rnd.h examples/example.c src/wrapper.cxx \
	src/automaton.hpp ../tools/handles.hpp src/nexpr.hpp src/zsymbols.hpp

examples_example_cpp_SOURCES = examples/example.cpp \
	src/automaton.hpp ../tools/handles.hpp src/nexpr.hpp src/zsymbols.hpp

examples_example_c_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_srcdir)/examples -I$(top_srcdir)/../tools

examples_example_cpp_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_srcdir)/examples -I$(top_srcdir)/../tools

TESTS = $(check_PROGRAMS)

#.PHONY:	docker-build
#docker-build:
#	docker rmi -f rnd-dev
#	docker build -t rnd-dev $(top_srcdir)
#
#.PHONY:	docker-rund
#docker-rund:
#	docker run -d -it -v $(abs_top_srcdir):/home rnd-dev /bin/sh
#
#.PHONY:	docker-run
#docker-run:
#	docker run -it -v $(abs_top_srcdir):/home rnd-dev /bin/sh

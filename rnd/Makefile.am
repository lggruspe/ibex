lib_LTLIBRARIES = lib/librndpoc.la
lib_librndpoc_la_SOURCES = \
	include/rnd.h \
	src/rnd/automaton.hpp \
	src/rnd/nexpr.hpp \
	src/rnd/wrapper.cxx \
	src/rnd/zsymbols.hpp \
	../tools/handles.hpp
lib_librndpoc_la_CPPFLAGS = \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../tools \
	-I$(top_srcdir)/src/rnd \
	-I$(top_srcdir)/src/lib \
	-I$(top_srcdir)/src/parse

lib_librndpoc_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = include/rnd.h

ACLOCAL_AMFLAGS = -I m4

check_PROGRAMS = examples/example_c examples/example_cpp \
	src/rnd/automaton.test src/rnd/nexpr.test src/rnd/zsymbols.test

src_rnd_automaton_test_SOURCES = \
	src/rnd/automaton.hpp \
	src/rnd/automaton.test.cpp \
	../tools/handles.hpp

src_rnd_automaton_test_CPPFLAGS = -I$(top_srcdir)/../tools -I$(top_srcdir)/src/rnd

src_rnd_nexpr_test_SOURCES = src/rnd/nexpr.hpp src/rnd/nexpr.test.cpp

src_rnd_zsymbols_test_SOURCES = src/rnd/zsymbols.hpp src/rnd/zsymbols.test.cpp

examples_example_c_SOURCES = include/rnd.h examples/example.c src/rnd/wrapper.cxx \
	src/rnd/automaton.hpp ../tools/handles.hpp src/rnd/nexpr.hpp src/rnd/zsymbols.hpp

examples_example_cpp_SOURCES = examples/example.cpp \
	src/rnd/automaton.hpp ../tools/handles.hpp src/rnd/nexpr.hpp src/rnd/zsymbols.hpp

examples_example_c_CPPFLAGS = \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	-I$(top_srcdir)/../tools \
	-I$(top_srcdir)/src/rnd

examples_example_cpp_CPPFLAGS = \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	-I$(top_srcdir)/../tools \
	-I$(top_srcdir)/src/rnd

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

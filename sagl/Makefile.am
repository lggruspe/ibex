bin_PROGRAMS = bin/main
bin_main_SOURCES = \
	src/main.cpp \
	src/parser.hpp \
	src/parsetree.hpp \
	src/scanner.hpp \
	include/sagl.hpp \
	../tools/handles.hpp

bin_main_CXXFLAGS = -g -Wall -Wextra -Wpedantic

bin_main_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/../tools

ACLOCAL_AMFLAGS = -I m4

TESTS = $(bin_PROGRAMS)

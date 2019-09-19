bin_PROGRAMS = bin/sagl
bin_sagl_SOURCES = \
	src/main.cpp \
	src/match.hpp \
	src/scanners.hpp \
	src/sagl/automaton.hpp \
	src/sagl/enumeration.hpp \
	include/grammar.hpp \
	include/parser.hpp \
	include/parsetable.hpp

bin_sagl_CXXFLAGS = -g -Wall -Wextra -Wpedantic
bin_sagl_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/sagl

ACLOCAL_AMFLAGS = -I m4

TESTS = $(bin_PROGRAMS)

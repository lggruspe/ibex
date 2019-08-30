bin_PROGRAMS = bin/sagl
bin_sagl_SOURCES = src/main.cpp src/match.hpp src/scanners.hpp \
			   include/automaton.hpp include/enumeration.hpp include/grammar.hpp \
			   include/item.hpp include/parser.hpp include/parsingtable.hpp \
			   include/state.hpp

bin_sagl_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src

ACLOCAL_AMFLAGS = -I m4

TESTS = $(bin_PROGRAMS)

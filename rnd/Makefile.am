lib_LTLIBRARIES = lib/libregexp.la

lib_libregexp_la_SOURCES = \
	include/regexp.h \
	src/rnd/automaton.hpp \
	src/rnd/automaton.cpp \
	src/rnd/nexpr.hpp \
	src/rnd/nexpr.cpp \
	src/rnd/zsymbols.hpp \
	src/rnd/zsymbols.cpp \
	../tools/handles.hpp \
	src/regexp/regexp.cxx \
	src/regexp/eval.hpp \
	src/regexp/parser.hpp \
	src/regexp/scanner.hpp \
	../sagl/include/sagl.hpp

lib_libregexp_la_CPPFLAGS = \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../tools \
	-I$(top_srcdir)/src/regexp \
	-I$(top_srcdir)/../sagl/include

lib_libregexp_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = include/regexp.h

ACLOCAL_AMFLAGS = -I m4

check_PROGRAMS = \
	src/rnd/nexpr.test src/rnd/zsymbols.test \
	src/regexp/regexp.test src/rnd/minauto.test

src_rnd_minauto_test_SOURCES = \
	src/rnd/minauto.test.cpp \
	src/rnd/automaton.hpp \
	src/rnd/automaton.cpp \
	src/rnd/minauto.hpp \
	src/rnd/minauto.cpp \
	src/rnd/nexpr.hpp \
	src/rnd/nexpr.cpp \
	src/rnd/zsymbols.hpp \
	src/rnd/zsymbols.cpp \
	../tools/handles.hpp

src_rnd_minauto_test_CPPFLAGS = -I$(top_srcdir)/../tools -I$(top_srcdir)/src

src_rnd_nexpr_test_SOURCES = \
	src/rnd/nexpr.hpp \
	src/rnd/nexpr.cpp \
	src/rnd/nexpr.test.cpp \
	src/rnd/zsymbols.hpp \
	src/rnd/zsymbols.cpp

src_rnd_nexpr_test_CPPFLAGS = -I$(top_srcdir)/src

src_rnd_zsymbols_test_SOURCES = \
	src/rnd/zsymbols.hpp \
	src/rnd/zsymbols.cpp \
	src/rnd/zsymbols.test.cpp

src_rnd_zsymbols_test_CPPFLAGS = -I$(top_srcdir)/src

src_regexp_regexp_test_SOURCES = src/regexp/regexp.test.c include/regexp.h \
	src/regexp/regexp.cxx src/regexp/eval.hpp src/regexp/parser.hpp \
	src/regexp/scanner.hpp ../sagl/include/sagl.hpp \
	src/rnd/automaton.cpp src/rnd/nexpr.cpp src/rnd/automaton.hpp src/rnd/nexpr.hpp \
	src/rnd/zsymbols.hpp src/rnd/zsymbols.cpp

src_regexp_regexp_test_CPPFLAGS = \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../tools \
	-I$(top_srcdir)/../sagl/include

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

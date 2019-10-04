lib_LTLIBRARIES = lib/libcrnd.la
lib_libcrnd_la_SOURCES = \
	src/rnd.cpp \
    include/rnd.h \
    src/rnd/regex/regex.cpp \
    src/rnd/regex/regex.h \
    src/rnd/alphabet/alphabet.hpp \
    src/rnd/nfa/nfa.cpp \
    src/rnd/nfa/nfa.h \
    src/rnd/dfa/dfa.cpp \
    src/rnd/dfa/dfa.h \
	src/rnd/dfa/uhs.hpp \
    src/rnd/dfa/partition.h \
    src/distree/distree.cpp \
    src/distree/distree.h \
    src/redblack/tree.hpp
lib_libcrnd_la_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include
lib_libcrnd_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = include/rnd.h

ACLOCAL_AMFLAGS = -I m4

check_PROGRAMS = bin/test_distree bin/test_redblack

bin_test_distree_SOURCES = \
	src/distree/test.cpp \
    src/distree/distree.cpp \
    src/distree/distree.h \
    src/distree/utils.hpp \
    src/test_runner.hpp
bin_test_redblack_SOURCES = \
	src/redblack/test.cpp \
    src/redblack/tree.hpp \
    src/test_runner.hpp

bin_test_distree_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include
bin_test_redblack_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include

TESTS = $(check_PROGRAMS)

.PHONY:	docker-build
docker-build:
	docker rmi -f rnd-dev
	docker build -t rnd-dev $(top_srcdir)

.PHONY:	docker-rund
docker-rund:
	docker run -d -it -v $(abs_top_srcdir):/home rnd-dev /bin/sh

.PHONY:	docker-run
docker-run:
	docker run -it -v $(abs_top_srcdir):/home rnd-dev /bin/sh

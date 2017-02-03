## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testcore

testcore_SOURCES = \
    abts.c testds.c testfsm.c testnetlib.c testthread.c testtlv.c \
    testaes.c testfile.c testlock.c testsha.c testtime.c testutil.c \
    testdir.c testfilecopy.c testmsgq.c testsleep.c testtimer.c \
    abts.h  abts_tests.h  testutil.h

testcore_LDADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
    -lpthread -lsctp

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include/arch/@OSDIR@ \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror @OSCFLAGS@ \
	-Wno-unused-function

TESTS = testcore

MAINTAINERCLEANFILES = Makefile.in 
CLEANFILES = -R data
MOSTLYCLEANFILES = core *.stackdump

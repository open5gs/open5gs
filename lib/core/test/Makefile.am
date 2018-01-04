## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testcore

testcore_SOURCES = \
    abts.c testds.c testfsm.c testsock.c testthread.c testtlv.c \
    testaes.c testfile.c testlock.c testatomic.c testsha.c testtime.c \
	testdir.c testfilecopy.c testmsgq.c testsleep.c testtimer.c \
	testpkbuf.c testmisc.c testhash.c test3gpp.c \
    abts.h  abts_tests.h testutil.c testutil.h

if !USRSCTP
testcore_SOURCES += testsctp.c
endif

testcore_LDADD = \
    $(top_srcdir)/lib/core/src/libcore.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror @OSCPPFLAGS@ \
	-Wno-unused-function -Wno-unused-variable

TESTS = testcore

MAINTAINERCLEANFILES = Makefile.in 
CLEANFILES = -R data
MOSTLYCLEANFILES = core *.stackdump

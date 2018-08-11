## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testcore

testcore_SOURCES = \
    abts.c testds.c testfsm.c testsock.c testthread.c testtlv.c \
    testaes.c testfile.c testlock.c testatomic.c testsha.c testtime.c \
	testdir.c testfilecopy.c testmsgq.c testsleep.c testtimer.c \
	testpkbuf.c testmisc.c testhash.c test3gpp.c \
    abts.h  abts_tests.h testutil.c testutil.h \
	$(NULL)

if !USRSCTP
testcore_SOURCES += testsctp.c
endif

testcore_LDADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(NULL)

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror @OSCPPFLAGS@ \
	-Wno-unused-function -Wno-unused-variable -Wno-restrict \
	-Wno-unknown-warning-option \
	$(NULL)

TESTS = testcore

MAINTAINERCLEANFILES = Makefile.in 
CLEANFILES = -R data
MOSTLYCLEANFILES = core *.stackdump

## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testasn

testasn_SOURCES = \
	abts.h abts_tests.h testutil.h \
	abts.c testutil.c \
	s1ap_test.c 

testasn_LDADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/asn/asn1c/libasn1c.la \
    $(top_srcdir)/lib/asn/src/libasn.la \
    -lpthread -lsctp

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/asn/asn1c \
    -I$(top_srcdir)/lib/asn/src

AM_CFLAGS = \
	-Wall -Werror @OSCFLAGS@ \
	-Wno-unused-function

TESTS = testasn

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = core *.stackdump

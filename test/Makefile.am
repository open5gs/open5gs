## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testcellwire

testcellwire_SOURCES = \
	abts.h abts_tests.h testutil.h \
	abts.c testutil.c \
	s1ap_enb_message.c \
	s1ap_message_test.c enb_setup_test.c

testcellwire_LDADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/asn/src/libasn.la \
    $(top_srcdir)/src/libcellwire.la \
    -lpthread -lsctp

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/asn/src/asn1c \
    -I$(top_srcdir)/lib/asn/src \
    -I$(top_srcdir)/src

AM_CFLAGS = \
	-Wall -Werror @OSCFLAGS@ \
	-Wno-unused-function -Wno-unused-variable

TESTS = testcellwire

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = core *.stackdump

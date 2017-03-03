## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testcellwire

testcellwire_SOURCES = \
	abts.h abts_tests.h testutil.h \
	abts.c testutil.c \
	s1ap_enb_build.c \
	s1ap_message_test.c nas_message_test.c \
	enb_setup_test.c

testcellwire_LDADD = \
    $(top_srcdir)/src/libcellwire.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/3gpp \
    -I$(top_srcdir)/lib/s1ap/asn1c \
    -I$(top_srcdir)/lib/s1ap \
    -I$(top_srcdir)/lib/nas \
    -I$(top_srcdir)/lib/s6a \
    -I$(top_srcdir)/src

AM_CFLAGS = \
	-Wall -Werror \
	-Wno-unused-function -Wno-unused-variable

TESTS = testcellwire

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = core *.stackdump

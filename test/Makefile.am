## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testcellwire

testcellwire_SOURCES = \
	abts.h abts_tests.h testutil.h \
	abts.c testutil.c tests1ap.h tests1ap.c \
	s1ap_message_test.c nas_message_test.c gtp_message_test.c \
	security_test.c \
	s1ap_sm_test.c nas_sm_test.c

testcellwire_LDADD = \
    $(top_srcdir)/src/libcellwire.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/3gpp \
    -I$(top_srcdir)/lib/s1ap/asn1c \
    -I$(top_srcdir)/lib/s1ap \
    -I$(top_srcdir)/lib/nas \
    -I$(top_srcdir)/lib/s6a \
    -I$(top_srcdir)/lib/gtp \
    -I$(top_srcdir)/src/mme \
    -I$(top_srcdir)/src/hss \
    -I$(top_srcdir)/src

AM_CFLAGS = \
	-Wall -Werror @TEST_CFLAGS@ \
	-Wno-unused-function -Wno-unused-variable

TESTS = testcellwire

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = core *.stackdump

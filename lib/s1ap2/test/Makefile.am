## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = tests1ap

tests1ap_SOURCES = \
    abts.c abts.h abts_tests.h testutil.c testutil.h s1ap_conv.c \
	s1ap_message_test.c

tests1ap_LDADD = \
    $(top_srcdir)/lib/s1ap2/libs1ap.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/s1ap2/asn1c \
    -I$(top_srcdir)/lib/s1ap2 \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror @OSCPPFLAGS@ \
	-Wno-unused-function -Wno-unused-variable

TESTS = tests1ap

MAINTAINERCLEANFILES = Makefile.in 
CLEANFILES = -R data
MOSTLYCLEANFILES = core *.stackdump

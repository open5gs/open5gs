## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = tests1ap

tests1ap_SOURCES = \
    abts.c abts.h abts_tests.h testutil.c testutil.h \
	s1ap_conv.h s1ap_conv.c s1ap_build.h s1ap_build.c \
	s1ap_message_test.c

tests1ap_LDADD = \
    $(top_srcdir)/lib/s1ap/libs1ap.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/s1ap/asn1c \
    -I$(top_srcdir)/lib/s1ap \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror @OSCPPFLAGS@ \
	-Wno-unused-function -Wno-unused-variable

TESTS = tests1ap

MAINTAINERCLEANFILES = Makefile.in 
CLEANFILES = -R data
MOSTLYCLEANFILES = core *.stackdump

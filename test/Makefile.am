## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testepc

testepc_SOURCES = \
	abts.h abts_tests.h testutil.h \
	abts.c testutil.c tests1ap.h tests1ap.c \
	base_test.c \
	s1ap_message_test.c nas_message_test.c gtp_message_test.c \
	security_test.c \
	s1ap_sm_test.c nas_sm_test.c

testepc_LDADD = \
    $(top_srcdir)/src/libepc.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/base \
    -I$(top_srcdir)/lib/s1ap/asn1c \
    -I$(top_srcdir)/lib/s1ap \
    -I$(top_srcdir)/lib/nas \
    -I$(top_srcdir)/lib/s6a \
    -I$(top_srcdir)/lib/gtp \
    -I$(top_srcdir)/src/mme \
    -I$(top_srcdir)/src/hss \
    -I$(top_srcdir)/src/sgw \
    -I$(top_srcdir)/src

AM_CFLAGS = \
	-Wall -Werror \
	-Wno-unused-function -Wno-unused-variable

TESTS = testepc

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = core *.stackdump

## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libcellwire.la

libcellwire_la_SOURCES = \
    cellwire.h event.h context.h s1ap_message.h s1ap_conv.h s1_path.h \
	sm.h 


nodist_libcellwire_la_SOURCES = \
    init.c event.c context.c s1ap_message.c s1ap_conv.c s1_path.c \
	mme_sm.c

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/asn/src/asn1c \
    -I$(top_srcdir)/lib/asn/src

AM_CFLAGS = \
	-Wall -Werror

DEFS = @DEFS@ -DSYSCONFDIR=\"$(sysconfdir)/\"

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

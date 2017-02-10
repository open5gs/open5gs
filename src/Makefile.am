## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libcellwire.la

libcellwire_la_SOURCES = \
    cellwire.h context.h s1ap_message.h s1ap_conv.h

nodist_libcellwire_la_SOURCES = \
    init.c context.c s1ap_message.c s1ap_conv.c

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

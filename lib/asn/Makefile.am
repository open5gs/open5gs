## Process this file with automake to produce Makefile.in

SUBDIRS = asn1c

noinst_LTLIBRARIES = libasn.la

libasn_la_SOURCES = \
	s1ap_ies_defs.h s1ap_common.h

nodist_libasn_la_SOURCES = \
	s1ap_encoder.c s1ap_decoder.c s1ap_xer_print.c \
	s1ap_common.c

libasn_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/core/src/@OSDIR@/libcore@OSDIR@.la

libasn_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/core/src/@OSDIR@/libcore@OSDIR@.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include/arch/@OSDIR@ \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/asn/asn1c

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

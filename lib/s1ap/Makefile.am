## Process this file with automake to produce Makefile.in

SUBDIRS = asn1c

noinst_LTLIBRARIES = libs1ap.la

libs1ap_la_SOURCES = \
	s1ap_ies_defs.h s1ap_codecs.h

nodist_libs1ap_la_SOURCES = \
	s1ap_encoder.c s1ap_decoder.c s1ap_free.c \
	s1ap_ies_encoder.c s1ap_ies_decoder.c s1ap_ies_free.c s1ap_ies_xer_print.c

libs1ap_la_DEPENDENCIES = \
    $(top_srcdir)/lib/3gpp/lib3gpp.la \
    $(top_srcdir)/lib/s1ap/asn1c/libs1apasn1c.la

libs1ap_la_LIBADD = \
    $(top_srcdir)/lib/3gpp/lib3gpp.la \
    $(top_srcdir)/lib/s1ap/asn1c/libs1apasn1c.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/3gpp \
    -I$(top_srcdir)/lib/s1ap/asn1c

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

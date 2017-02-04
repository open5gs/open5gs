## Process this file with automake to produce Makefile.in

SUBDIRS = asn1c

noinst_LTLIBRARIES = libasn.la

libasn_la_SOURCES = \
	s1ap_ies_defs.h s1ap_common.h

nodist_libasn_la_SOURCES = \
	s1ap_common.c \
	s1ap_encoder.c s1ap_decoder.c s1ap_xer_print.c \
	s1ap_mme_encoder.c s1ap_mme_decoder.c

libasn_la_DEPENDENCIES = \
    $(top_srcdir)/lib/asn/src/asn1c/libasn1c.la

libasn_la_LIBADD = \
    $(top_srcdir)/lib/asn/src/asn1c/libasn1c.la


AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/asn/src/asn1c

AM_CFLAGS = \
	-Wall -Werror -Wno-incompatible-pointer-types

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

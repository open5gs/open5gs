## Process this file with automake to produce Makefile.in

SUBDIRS = s1ap

noinst_LTLIBRARIES = libmessage.la

libmessage_la_SOURCES = \
	s1ap_ies_defs.h s1ap_codecs.h

nodist_libmessage_la_SOURCES = \
	s1ap_encoder.c s1ap_decoder.c s1ap_free.c \
	s1ap_ies_encoder.c s1ap_ies_decoder.c s1ap_ies_free.c s1ap_ies_xer_print.c

libmessage_la_DEPENDENCIES = \
    $(top_srcdir)/lib/message/src/s1ap/libmessages1ap.la

libmessage_la_LIBADD = \
    $(top_srcdir)/lib/message/src/s1ap/libmessages1ap.la


AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/message/src/s1ap

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

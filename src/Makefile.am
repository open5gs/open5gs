## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libcellwire.la

libcellwire_la_SOURCES = \
    cellwire.h event.h context.h \
	s1ap_build.h s1ap_conv.h s1ap_path.h \
	s6_path.h \
	sm.h 


nodist_libcellwire_la_SOURCES = \
    init.c event.c context.c \
	s1ap_build.c s1ap_conv.c s1ap_path.c \
	s6a_path.c \
	mme_sm.c enb_s1_sm.c

libcellwire_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la

libcellwire_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/message/s1ap/asn1c \
    -I$(top_srcdir)/lib/message/s1ap

AM_CFLAGS = \
	-Wall -Werror

DEFS = @DEFS@ -DSYSCONFDIR=\"$(sysconfdir)/\"

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libcellwire.la

libcellwire_la_SOURCES = \
    cellwire.h event.h context.h \
	s1ap_build.h s1ap_conv.h s1ap_path.h \
	s6_auth_info.h \
	sm.h 


nodist_libcellwire_la_SOURCES = \
    init.c event.c context.c \
	s1ap_build.c s1ap_conv.c s1ap_path.c \
	s6a_auth_info.c \
	mme_sm.c enb_s1_sm.c

libcellwire_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/lib/3gpp/lib3gpp.la \
    $(top_srcdir)/lib/s1ap/libs1ap.la \
    $(top_srcdir)/lib/nas/libnas.la \
    $(top_srcdir)/lib/s6a/libs6a.la

libcellwire_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/lib/3gpp/lib3gpp.la \
    $(top_srcdir)/lib/s1ap/libs1ap.la \
    $(top_srcdir)/lib/nas/libnas.la \
    $(top_srcdir)/lib/s6a/libs6a.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/lib/core/include \
	-I$(top_srcdir)/lib/logger \
    -I$(top_srcdir)/lib/3gpp \
    -I$(top_srcdir)/lib/s1ap/asn1c \
    -I$(top_srcdir)/lib/s1ap \
    -I$(top_srcdir)/lib/nas \
    -I$(top_srcdir)/lib/s6a

AM_CFLAGS = \
	-Wall -Werror

DEFS = @DEFS@ -DSYSCONFDIR=\"$(sysconfdir)/\"

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

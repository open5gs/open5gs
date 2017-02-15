## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libcellwire.la

libcellwire_la_SOURCES = \
    cellwire.h event.h context.h s1ap_message.h s1ap_conv.h s1ap_path.h \
	sm.h 


nodist_libcellwire_la_SOURCES = \
    init.c event.c context.c s1ap_message.c s1ap_conv.c s1ap_path.c \
	mme_sm.c enb_s1_sm.c

libcellwire_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la

libcellwire_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/message/src/s1ap \
    -I$(top_srcdir)/lib/message/src

AM_CFLAGS = \
	-Wall -Werror

DEFS = @DEFS@ -DSYSCONFDIR=\"$(sysconfdir)/\"

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

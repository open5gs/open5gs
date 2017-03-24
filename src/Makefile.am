## Process this file with automake to produce Makefile.in.

SUBDIRS = mme hss sgw

noinst_LTLIBRARIES = libcellwire.la

libcellwire_la_SOURCES = \
	cellwire.h 

nodist_libcellwire_la_SOURCES = \
    init.c 

libcellwire_la_DEPENDENCIES = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/mme/libmme.la \
    $(top_srcdir)/src/hss/libhss.la \
    $(top_srcdir)/src/sgw/libsgw.la

libcellwire_la_LIBADD = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/mme/libmme.la \
    $(top_srcdir)/src/hss/libhss.la \
    $(top_srcdir)/src/sgw/libsgw.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
	-I$(top_srcdir)/lib/logger

AM_CFLAGS = \
	-Wall -Werror

DEFS = @DEFS@ -DSYSCONFDIR=\"$(sysconfdir)/\"

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

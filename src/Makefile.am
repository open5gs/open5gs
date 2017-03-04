## Process this file with automake to produce Makefile.in.

SUBDIRS = mme

noinst_LTLIBRARIES = libcellwire.la

libcellwire_la_SOURCES = \
	cellwire.h 

nodist_libcellwire_la_SOURCES = \
    init.c 

libcellwire_la_DEPENDENCIES = \
    $(top_srcdir)/src/mme/libmme.la

libcellwire_la_LIBADD = \
    $(top_srcdir)/src/mme/libmme.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
	-I$(top_srcdir)/lib/logger \
	-I$(top_srcdir)/src/mme/include

AM_CFLAGS = \
	-Wall -Werror

DEFS = @DEFS@ -DSYSCONFDIR=\"$(sysconfdir)/\"

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

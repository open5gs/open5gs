## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libbase.la

libbase_la_SOURCES = \
	types.h context.h \
	types.c context.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
	@MONGOC_CFLAGS@

AM_CFLAGS = \
	-Wall -Werror

DEFS = @DEFS@ -DSYSCONFDIR=\"$(sysconfdir)/\"

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

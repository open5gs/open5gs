## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = libbase.la

libbase_la_SOURCES = \
	types.h context.h \
	types.c context.c

libbase_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/@OSDIR@/libcore@OSDIR@.la \
    $(top_srcdir)/lib/core/src/libcore.la

libbase_la_LIBADD = \
    $(top_srcdir)/lib/core/src/@OSDIR@/libcore@OSDIR@.la \
    $(top_srcdir)/lib/core/src/libcore.la \
	@MONGOC_LIBS@

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
	@MONGOC_CFLAGS@

AM_CFLAGS = \
	-Wall -Werror

DEFS = @DEFS@ -DSYSCONF_DIR=\"$(sysconfdir)/\"

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

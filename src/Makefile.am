## Process this file with automake to produce Makefile.in.

SUBDIRS = mme hss sgw pgw

noinst_LTLIBRARIES = libepc.la

libepc_la_SOURCES = \
	init.h 

nodist_libepc_la_SOURCES = \
    init.c 

libepc_la_DEPENDENCIES = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/mme/libmme.la \
    $(top_srcdir)/src/hss/libhss.la \
    $(top_srcdir)/src/sgw/libsgw.la \
    $(top_srcdir)/src/pgw/libpgw.la

libepc_la_LIBADD = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/mme/libmme.la \
    $(top_srcdir)/src/hss/libhss.la \
    $(top_srcdir)/src/sgw/libsgw.la \
    $(top_srcdir)/src/pgw/libpgw.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
	-I$(top_srcdir)/lib/logger

AM_CFLAGS = \
	-Wall -Werror

DEFS = @DEFS@ -DSYSCONFDIR=\"$(sysconfdir)/\"

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

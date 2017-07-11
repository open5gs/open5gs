## Process this file with automake to produce Makefile.in.

SUBDIRS = mme hss sgw pgw

noinst_LTLIBRARIES = libmme.la libhss.la libsgw.la libpgw.la libepc.la

COMMON_SOURCES = init.c
COMMON_INCLUDES = app.h

libmme_la_SOURCES = $(COMMON_INCLUDES)
nodist_libmme_la_SOURCES = mme.c $(COMMON_SOURCES)
libmme_la_DEPENDENCIES = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/mme/libmme.la
libmme_la_LIBADD = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/mme/libmme.la

libhss_la_SOURCES = $(COMMON_INCLUDES)
nodist_libhss_la_SOURCES = hss.c $(COMMON_SOURCES)
libhss_la_DEPENDENCIES = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/hss/libhss.la
libhss_la_LIBADD = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/hss/libhss.la

libsgw_la_SOURCES = $(COMMON_INCLUDES)
nodist_libsgw_la_SOURCES = sgw.c $(COMMON_SOURCES)
libsgw_la_DEPENDENCIES = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/sgw/libsgw.la
libsgw_la_LIBADD = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/sgw/libsgw.la

libpgw_la_SOURCES = $(COMMON_INCLUDES)
nodist_libpgw_la_SOURCES = pgw.c $(COMMON_SOURCES)
libpgw_la_DEPENDENCIES = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/pgw/libpgw.la
libpgw_la_LIBADD = \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/pgw/libpgw.la

libepc_la_SOURCES = $(COMMON_INCLUDES)
nodist_libepc_la_SOURCES = epc.c $(COMMON_SOURCES)
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

## Process this file with automake to produce Makefile.in.

SUBDIRS = mme hss sgw pgw pcrf

noinst_LTLIBRARIES = libmme.la libhss.la libsgw.la libpgw.la libpcrf.la libepc.la

COMMON_SOURCES = init.c
COMMON_INCLUDES = app.h

libmme_la_SOURCES = $(COMMON_INCLUDES) mme.c $(COMMON_SOURCES)
libmme_la_DEPENDENCIES = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/mme/libmme.la
libmme_la_LIBADD = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/mme/libmme.la

libhss_la_SOURCES = $(COMMON_INCLUDES) hss.c $(COMMON_SOURCES)
libhss_la_DEPENDENCIES = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/hss/libhss.la
libhss_la_LIBADD = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/hss/libhss.la

libsgw_la_SOURCES = $(COMMON_INCLUDES) sgw.c $(COMMON_SOURCES)
libsgw_la_DEPENDENCIES = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/sgw/libsgw.la
libsgw_la_LIBADD = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/sgw/libsgw.la

libpgw_la_SOURCES = $(COMMON_INCLUDES) pgw.c $(COMMON_SOURCES)
libpgw_la_DEPENDENCIES = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/pgw/libpgw.la
libpgw_la_LIBADD = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/pgw/libpgw.la

libpcrf_la_SOURCES = $(COMMON_INCLUDES) pcrf.c $(COMMON_SOURCES)
libpcrf_la_DEPENDENCIES = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/pcrf/libpcrf.la
libpcrf_la_LIBADD = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/pcrf/libpcrf.la

libepc_la_SOURCES = $(COMMON_INCLUDES) epc.c $(COMMON_SOURCES)
libepc_la_DEPENDENCIES = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/mme/libmme.la \
    $(top_srcdir)/src/hss/libhss.la \
    $(top_srcdir)/src/sgw/libsgw.la \
    $(top_srcdir)/src/pgw/libpgw.la \
    $(top_srcdir)/src/pcrf/libpcrf.la
libepc_la_LIBADD = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/src/mme/libmme.la \
    $(top_srcdir)/src/hss/libhss.la \
    $(top_srcdir)/src/sgw/libsgw.la \
    $(top_srcdir)/src/pgw/libpgw.la \
    $(top_srcdir)/src/pcrf/libpcrf.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
	-I$(top_srcdir)/lib/logger \
	-I$(top_srcdir)/lib/base

AM_CFLAGS = \
	-Wall -Werror

DEFS = @DEFS@ \
	   -DSYSCONF_DIR=\"$(sysconfdir)/\" \
	   -DLOCALSTATE_DIR=\"$(localstatedir)/\"

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

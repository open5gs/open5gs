## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libhss.la

libhss_la_SOURCES = \
    hss_ctx.h hss_sm.h

nodist_libhss_la_SOURCES = \
    init.c hss_ctx.c hss_sm.c

libhss_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/3gpp/lib3gpp.la \
    $(top_srcdir)/lib/s6a/libs6a.la

libhss_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/3gpp/lib3gpp.la \
    $(top_srcdir)/lib/s6a/libs6a.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/3gpp \
    -I$(top_srcdir)/lib/s6a

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

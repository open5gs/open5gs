## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libpgw.la

libpgw_la_SOURCES = \
    pgw_event.h pgw_ctx.h \
	pgw_path.h pgw_sm.h pgw_handler.h

nodist_libpgw_la_SOURCES = \
    pgw_init.c pgw_event.c pgw_ctx.c \
	pgw_path.c pgw_sm.c pgw_handler.c

libpgw_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/gtp/libgtp.la

libpgw_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/gtp/libgtp.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/3gpp \
    -I$(top_srcdir)/lib/gtp

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

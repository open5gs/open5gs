## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libpgw.la

libpgw_la_SOURCES = \
    event.h context.h \
	pgw_path.h sm.h s11_handler.h

nodist_libpgw_la_SOURCES = \
    init.c event.c context.c \
	pgw_path.c pgw_sm.c s5c_handler.c

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

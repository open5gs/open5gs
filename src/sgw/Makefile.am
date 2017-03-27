## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libsgw.la

libsgw_la_SOURCES = \
    event.h context.h \
	sgw_path.h \
	sm.h

nodist_libsgw_la_SOURCES = \
    init.c event.c context.c \
	sgw_path.c \
	sgw_sm.c s11_sm.c s5c_sm.c

libsgw_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/gtp/libgtp.la

libsgw_la_LIBADD = \
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

## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libsgw.la

libsgw_la_SOURCES = \
    sgw_event.h sgw_context.h \
	sgw_path.h sgw_sm.h sgw_handler.h

nodist_libsgw_la_SOURCES = \
    sgw_init.c sgw_event.c sgw_context.c \
	sgw_path.c sgw_sm.c sgw_handler.c

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

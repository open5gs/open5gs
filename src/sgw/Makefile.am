## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libsgw.la

SUBDIR = gtp

libsgw_la_SOURCES = \
    sgw-event.h sgw-context.h \
	sgw-gtp-path.h sgw-sm.h sgw-s11-handler.h sgw-s5c-handler.h \
    sgw-init.c sgw-event.c sgw-context.c \
	sgw-gtp-path.c sgw-sm.c sgw-s11-handler.c sgw-s5c-handler.c \
	$(NULL)

libsgw_la_DEPENDENCIES = \
    $(top_srcdir)/lib/gtp/libogsgtp.la \
	$(top_srcdir)/lib/app/libogsapp.la \
	$(NULL)

libsgw_la_LIBADD = \
    $(top_srcdir)/lib/gtp/libogsgtp.la \
	$(top_srcdir)/lib/app/libogsapp.la \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
    -I$(top_srcdir)/lib \
	-I$(top_srcdir)/src \
	@MONGOC_CFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror

## Process this file with automake to produce Makefile.in

extlib_LTLIBRARIES = dbg_msg_dumps.la

dbg_msg_dumps_la_SOURCES = dbg_msg_dumps.c

dbg_msg_dumps_la_DEPENDENCIES = \
    $(top_srcdir)/libfdcore/libfdcore.la \
    $(top_srcdir)/libfdproto/libfdproto.la \
	$(NULL)

dbg_msg_dumps_la_LIBADD = \
    $(top_srcdir)/libfdcore/libfdcore.la \
    $(top_srcdir)/libfdproto/libfdproto.la \
	$(NULL)

AM_LDFLAGS = \
	-module -avoid-version -shared \
	$(NULL)

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

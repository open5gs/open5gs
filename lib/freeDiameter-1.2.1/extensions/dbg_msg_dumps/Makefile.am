## Process this file with automake to produce Makefile.in

extlib_LTLIBRARIES = dbg_msg_dumps.la

dbg_msg_dumps_la_SOURCES = \
	dbg_msg_dumps.c

dbg_msg_dumps_la_DEPENDENCIES = \
    $(top_srcdir)/libfdcore/libfdcore.la \
    $(top_srcdir)/libfdproto/libfdproto.la

dbg_msg_dumps_la_LIBADD = \
    $(top_srcdir)/libfdcore/libfdcore.la \
    $(top_srcdir)/libfdproto/libfdproto.la

AM_LDFLAGS = \
	-module -avoid-version -shared

AM_CPPFLAGS = \
	-I$(top_srcdir)/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

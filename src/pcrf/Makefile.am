## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libpcrf.la

libpcrf_la_SOURCES = \
	pcrf_context.h pcrf_gx_handler.h \
	pcrf_init.c pcrf_context.c pcrf_gx_handler.c

libpcrf_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(top_srcdir)/lib/fd/gx/libfdgx.la \
    $(top_srcdir)/lib/fd/libfd.la

libpcrf_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(top_srcdir)/lib/fd/gx/libfdgx.la \
    $(top_srcdir)/lib/fd/libfd.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/base \
    -I$(top_srcdir)/lib/fd/gx \
    -I$(top_srcdir)/lib/fd \
	@MONGOC_CFLAGS@

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

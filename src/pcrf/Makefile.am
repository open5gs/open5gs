## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libpcrf.la

libpcrf_la_SOURCES = \
	pcrf_context.h pcrf_fd_path.h \
	pcrf_init.c pcrf_context.c pcrf_fd_path.c

libpcrf_la_DEPENDENCIES = \
	$(top_srcdir)/lib/fd/gx/libfdgx.la \
    $(top_srcdir)/lib/fd/libfd.la

libpcrf_la_LIBADD = \
	$(top_srcdir)/lib/fd/gx/libfdgx.la \
    $(top_srcdir)/lib/fd/libfd.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/base \
    -I$(top_srcdir)/lib/fd/gx \
    -I$(top_srcdir)/lib/fd \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include \
	@MONGOC_CFLAGS@

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

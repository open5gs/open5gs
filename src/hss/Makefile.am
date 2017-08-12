## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libhss.la

libhss_la_SOURCES = \
	milenage.h hss_kdf.h hss_context.h hss_fd.h hss_s6a_handler.h

nodist_libhss_la_SOURCES = \
	milenage.c hss_kdf.c hss_init.c hss_context.c hss_fd.c hss_s6a_handler.c

libhss_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(top_srcdir)/lib/fd/s6a/libfds6a.la \
    $(top_srcdir)/lib/fd/libfd.la

libhss_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(top_srcdir)/lib/fd/s6a/libfds6a.la \
    $(top_srcdir)/lib/fd/libfd.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/base \
    -I$(top_srcdir)/lib/fd/s6a \
    -I$(top_srcdir)/lib/fd \
	@MONGOC_CFLAGS@

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

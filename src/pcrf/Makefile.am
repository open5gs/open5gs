## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libpcrf.la

libpcrf_la_SOURCES = \
	pcrf_context.h pcrf_fd_path.h \
	pcrf_init.c pcrf_context.c pcrf_fd_path.c pcrf_gx_path.c pcrf_rx_path.c \
	$(NULL)

libpcrf_la_DEPENDENCIES = \
    $(top_srcdir)/lib/fd/libfd.la \
    $(top_srcdir)/src/common/libcommon.la \
	$(NULL)

libpcrf_la_LIBADD = \
    $(top_srcdir)/lib/fd/libfd.la \
    $(top_srcdir)/src/common/libcommon.la \
	$(NULL)

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include \
	@MONGOC_CFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

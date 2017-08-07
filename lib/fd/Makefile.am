## Process this file with automake to produce Makefile.in

SUBDIRS = s6a

noinst_LTLIBRARIES = libfd.la

libfd_la_SOURCES = \
	fd_context.h fd_logger.h fd_init.h

nodist_libfd_la_SOURCES = \
	libapp_sip.c fd_context.c fd_logger.c fd_init.c

libfd_la_DEPENDENCIES = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/fd/s6a/libfds6a.la

libfd_la_LIBADD = \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/fd/s6a/libfds6a.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/fd/s6a

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

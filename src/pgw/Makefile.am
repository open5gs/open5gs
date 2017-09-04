## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libpgw.la

libpgw_la_SOURCES = \
    pgw_event.h pgw_context.h pgw_sm.h \
	pgw_gtp_path.h pgw_s5c_build.h pgw_s5c_handler.h \
	pgw_fd_path.h pgw_gx_handler.h \
    pgw_init.c pgw_event.c pgw_context.c pgw_sm.c \
	pgw_gtp_path.c pgw_s5c_build.c pgw_s5c_handler.c \
	pgw_fd_path.c pgw_gx_handler.c

libpgw_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(top_srcdir)/lib/fd/gx/libfdgx.la \
    $(top_srcdir)/lib/fd/libfd.la \
    $(top_srcdir)/lib/gtp/libgtp.la

libpgw_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(top_srcdir)/lib/fd/gx/libfdgx.la \
    $(top_srcdir)/lib/fd/libfd.la \
    $(top_srcdir)/lib/gtp/libgtp.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/base \
    -I$(top_srcdir)/lib/fd/gx \
    -I$(top_srcdir)/lib/fd \
    -I$(top_srcdir)/lib/gtp

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

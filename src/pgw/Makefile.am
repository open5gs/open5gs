## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libpgw.la

libpgw_la_SOURCES = \
	pgw_ipfw.h \
    pgw_event.h pgw_context.h pgw_sm.h \
	pgw_gtp_path.h pgw_s5c_build.h pgw_s5c_handler.h \
	pgw_fd_path.h pgw_gx_handler.h \
	pgw_ipfw.c \
    pgw_init.c pgw_event.c pgw_context.c pgw_sm.c \
	pgw_gtp_path.c pgw_s5c_build.c pgw_s5c_handler.c \
	pgw_fd_path.c pgw_gx_handler.c \
	$(NULL)

libpgw_la_DEPENDENCIES = \
    $(top_srcdir)/lib/fd/libfd.la \
    $(top_srcdir)/lib/gtp/libgtp.la \
    $(top_srcdir)/lib/ipfw/libipfw.la \
    $(top_srcdir)/src/common/libcommon.la \
	$(NULL)

libpgw_la_LIBADD = \
    $(top_srcdir)/lib/fd/libfd.la \
    $(top_srcdir)/lib/gtp/libgtp.la \
    $(top_srcdir)/lib/ipfw/libipfw.la \
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
	-Wall -Werror \
	$(NULL)

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libgtp.la

libgtp_la_SOURCES = \
	gtp_message.h gtp_types.h gtp_path.h gtp_xact.h \
	gtp_message.c gtp_types.c gtp_path.c gtp_xact.c

libgtp_la_DEPENDENCIES = \
    $(top_srcdir)/lib/base/libbase.la

libgtp_la_LIBADD = \
    $(top_srcdir)/lib/base/libbase.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/base

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

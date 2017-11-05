## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = libfdgx.la

libfdgx_la_SOURCES = \
	gx_dict.h gx_message.h dict_init.c gx_message.c

libfdgx_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/@OSDIR@/libcore@OSDIR@.la \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la

libfdgx_la_LIBADD = \
    $(top_srcdir)/lib/core/src/@OSDIR@/libcore@OSDIR@.la \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
	-I$(top_srcdir)/lib/base \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

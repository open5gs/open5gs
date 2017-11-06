## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = subdir-objects

pkglib_LTLIBRARIES = libfd.la

libfd_la_SOURCES = \
	fd_message.h fd_logger.h fd_lib.h \
	libapp_sip.c fd_message.c fd_logger.c fd_init.c \
	\
	gx/gx_dict.h gx/gx_message.h \
	gx/dict_init.c gx/gx_message.c \
	\
	s6a/s6a_dict.h s6a/s6a_message.h \
	s6a/dict_init.c

libfd_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/base/libbase.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la

libfd_la_LIBADD = \
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

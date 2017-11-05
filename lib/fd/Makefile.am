## Process this file with automake to produce Makefile.in

SUBDIRS = extensions gx s6a

pkglib_LTLIBRARIES = libfd.la

libfd_la_SOURCES = \
	fd_message.h fd_logger.h fd_lib.h \
	libapp_sip.c fd_message.c fd_logger.c fd_init.c

libfd_la_DEPENDENCIES = \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la

libfd_la_LIBADD = \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

## Process this file with automake to produce Makefile.in

SUBDIRS = extensions gx s6a

noinst_LTLIBRARIES = libfd.la

libfd_la_SOURCES = \
	fd_message.h fd_logger.h fd_init.h fd_lib.h

nodist_libfd_la_SOURCES = \
	libapp_sip.c fd_message.c fd_logger.c fd_init.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

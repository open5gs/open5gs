## Process this file with automake to produce Makefile.in

SUBDIRS = freeDiameter

noinst_LTLIBRARIES = libmessages6a.la

libmessages6a_la_SOURCES = \
	s6a_message.h

nodist_libmessages6a_la_SOURCES = \
	s6a_init.c s6a_dict.c

libmessages6a_la_DEPENDENCIES = \
    $(top_srcdir)/lib/message/s6a/freeDiameter/libmessages6afd.la

libmessages6a_la_LIBADD = \
    $(top_srcdir)/lib/message/s6a/freeDiameter/libmessages6afd.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

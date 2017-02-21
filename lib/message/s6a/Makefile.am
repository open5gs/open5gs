## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libmessages6a.la

libmessages6a_la_SOURCES = \
	freeDiameter/extension.h

nodist_libmessages6a_la_SOURCES = \
	dict_nasreq.c dict_dcca.c dict_dcca_3gpp.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

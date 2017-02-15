## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libmessagenas.la

libmessagenas_la_SOURCES = \
	nas_message.h nas_ies.h

nodist_libmessagenas_la_SOURCES = \
	nas_message.c nas_ies.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

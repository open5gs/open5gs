## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libbase.la

libbase_la_SOURCES = \
	types.h context.h

nodist_libbase_la_SOURCES = \
	types.c context.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libfds6a.la

libfds6a_la_SOURCES = \
	extension.h

nodist_libfds6a_la_SOURCES = \
	dict_nas_mipv6.c dict_s6a.c s6a_init.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/fd/s6a \
    -I$(top_srcdir)/lib/fd

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

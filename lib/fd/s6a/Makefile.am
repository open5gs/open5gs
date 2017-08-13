## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libfds6a.la

nodist_libfds6a_la_SOURCES = \
	dict_init.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/fd/s6a

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libfdgx.la

nodist_libfdgx_la_SOURCES = \
	dict_init.c

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

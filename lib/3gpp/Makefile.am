## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = lib3gpp.la

lib3gpp_la_SOURCES = \
	3gpp_message.h 3gpp_conv.h

nodist_lib3gpp_la_SOURCES = \
	3gpp_conv.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

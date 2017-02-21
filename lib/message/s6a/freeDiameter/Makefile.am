## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libmessages6afd.la

libmessages6afd_la_SOURCES = \
	extension.h

nodist_libmessages6afd_la_SOURCES = \
	dict_nas_mipv6.c dict_s6a.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/message/s6a

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

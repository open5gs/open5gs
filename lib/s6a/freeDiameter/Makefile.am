## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libs6afreeDiameter.la

libs6afreeDiameter_la_SOURCES = \
	extension.h

nodist_libs6afreeDiameter_la_SOURCES = \
	dict_nas_mipv6.c dict_s6a.c libapp_sip.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/s6a

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = dict_rfc5777.la

dict_rfc5777_la_SOURCES = \
	dict_rfc5777.c

AM_LDFLAGS = \
	-module -avoid-version -shared

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

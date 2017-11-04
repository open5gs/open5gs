## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = dict_nas_mipv6.la

dict_nas_mipv6_la_SOURCES = \
	dict_nas_mipv6.c

AM_LDFLAGS = \
	-module -avoid-version -shared

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = dict_s6a.la

dict_s6a_la_SOURCES = \
	dict_s6a.c

AM_LDFLAGS = \
	-module -avoid-version -shared

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

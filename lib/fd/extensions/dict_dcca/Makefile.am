## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = dict_dcca.la

dict_dcca_la_SOURCES = \
	dict_dcca.c

AM_LDFLAGS = \
	-module -avoid-version -shared

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

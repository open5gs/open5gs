## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = dict_mip6i.la

dict_mip6i_la_SOURCES = \
	dict_mip6i.c

AM_LDFLAGS = \
	-module -avoid-version -shared

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

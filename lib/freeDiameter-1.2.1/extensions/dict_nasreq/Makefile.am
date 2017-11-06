## Process this file with automake to produce Makefile.in

extlib_LTLIBRARIES = dict_nasreq.la

dict_nasreq_la_SOURCES = \
	dict_nasreq.c

dict_nasreq_la_DEPENDENCIES = \
    $(top_srcdir)/libfdcore/libfdcore.la \
    $(top_srcdir)/libfdproto/libfdproto.la

dict_nasreq_la_LIBADD = \
    $(top_srcdir)/libfdcore/libfdcore.la \
    $(top_srcdir)/libfdproto/libfdproto.la

AM_LDFLAGS = \
	-module -avoid-version -shared

AM_CPPFLAGS = \
	-I$(top_srcdir)/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

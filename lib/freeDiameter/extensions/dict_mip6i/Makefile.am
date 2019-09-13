## Process this file with automake to produce Makefile.in

extlib_LTLIBRARIES = dict_mip6i.la

dict_mip6i_la_SOURCES = dict_mip6i.c

dict_mip6i_la_DEPENDENCIES = \
    $(top_srcdir)/libfdcore/libfdcore.la \
    $(top_srcdir)/libfdproto/libfdproto.la \
	$(NULL)

dict_mip6i_la_LIBADD = \
    $(top_srcdir)/libfdcore/libfdcore.la \
    $(top_srcdir)/libfdproto/libfdproto.la \
	$(NULL)

AM_LDFLAGS = \
	-module -avoid-version -shared \
	$(NULL)

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

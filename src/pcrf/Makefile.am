## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libpcrf.la

libpcrf_la_SOURCES = \
	pcrf-context.h pcrf-fd-path.h \
	pcrf-init.c pcrf-context.c pcrf-fd-path.c pcrf-gx-path.c pcrf-rx-path.c \
	$(NULL)

libpcrf_la_DEPENDENCIES = \
	$(top_srcdir)/lib/diameter/libogsdiameter.la \
	$(top_srcdir)/lib/dbi/libogsdbi.la \
	$(top_srcdir)/lib/app/libogsapp.la \
	$(NULL)

libpcrf_la_LIBADD = \
	$(top_srcdir)/lib/diameter/libogsdiameter.la \
	$(top_srcdir)/lib/dbi/libogsdbi.la \
	$(top_srcdir)/lib/app/libogsapp.la \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
    -I$(top_srcdir)/lib \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/lib/freeDiameter/include \
	@MONGOC_CFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror

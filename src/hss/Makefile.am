## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libhss.la

libhss_la_SOURCES = \
	hss-auc.h hss-context.h hss-fd-path.h \
	hss-auc.c hss-init.c hss-context.c hss-fd-path.c \
	$(NULL)

libhss_la_DEPENDENCIES = \
	$(top_srcdir)/lib/diameter/libogsdiameter.la \
	$(top_srcdir)/lib/dbi/libogsdbi.la \
	$(top_srcdir)/lib/app/libogsapp.la \
	$(NULL)

libhss_la_LIBADD = \
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
	-Wall -Werror \
	$(NULL)

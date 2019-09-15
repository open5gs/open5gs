## Process this file with automake to produce Makefile.in.

SUBDIRS = ipfw

noinst_LTLIBRARIES = libpgw.la

libpgw_la_SOURCES = \
	ogs-tun.h ogs-tun.c \
	pgw-ipfw.h \
    pgw-event.h pgw-context.h pgw-sm.h \
	pgw-gtp-path.h pgw-s5c-build.h pgw-s5c-handler.h \
	pgw-fd-path.h pgw-gx-handler.h \
	pgw-ipfw.c \
    pgw-init.c pgw-event.c pgw-context.c pgw-sm.c \
	pgw-gtp-path.c pgw-s5c-build.c pgw-s5c-handler.c \
	pgw-fd-path.c pgw-gx-handler.c \
	$(NULL)

libpgw_la_DEPENDENCIES = \
	$(top_srcdir)/lib/diameter/libogsdiameter.la \
    $(top_srcdir)/lib/gtp/libogsgtp.la \
	$(top_srcdir)/lib/app/libogsapp.la \
    $(top_srcdir)/src/pgw/ipfw/libipfw.la \
	$(NULL)

libpgw_la_LIBADD = \
	$(top_srcdir)/lib/diameter/libogsdiameter.la \
    $(top_srcdir)/lib/gtp/libogsgtp.la \
	$(top_srcdir)/lib/app/libogsapp.la \
    $(top_srcdir)/src/pgw/ipfw/libipfw.la \
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

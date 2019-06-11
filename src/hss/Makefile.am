## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libhss.la

libhss_la_SOURCES = \
	milenage.h hss-auc.h hss-context.h hss-fd-path.h \
	milenage.c hss-auc.c hss-init.c hss-context.c hss-fd-path.c \
	$(NULL)

libhss_la_DEPENDENCIES = \
    $(top_srcdir)/lib/fd/libfd.la \
	$(top_srcdir)/src/app/libapp.la \
	$(NULL)

libhss_la_LIBADD = \
    $(top_srcdir)/lib/fd/libfd.la \
	$(top_srcdir)/src/app/libapp.la \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
    -I$(top_srcdir)/lib \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include \
	@MONGOC_CFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

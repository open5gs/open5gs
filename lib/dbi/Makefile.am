## Process this file with automake to produce Makefile.in.

pkglib_LTLIBRARIES = libogsdbi.la

libogsdbi_la_SOURCES = \
	ogs-mongoc.h ogs-mongoc.c \
	$(NULL)

libogsdbi_la_LIBADD = \
	@OGSCORE_LIBS@ \
	@MONGOC_LIBS@ \
	$(NULL)
 
libogsdbi_la_DEPENDENCIES = \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
	@MONGOC_CFLAGS@ \
	-I$(top_srcdir)/lib \
	$(NULL)

AM_CFLAGS = \
	-DOGS_DBI_COMPILATION \
	-Wall -Werror \
	$(NULL)

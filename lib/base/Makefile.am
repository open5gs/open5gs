## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = libbase.la

libbase_la_SOURCES = \
	base.h \
	types.h types.c \
	context.h context.c \
	$(NULL)

libbase_la_DEPENDENCIES = \
	$(NULL)

libbase_la_LIBADD = \
	@OGSCORE_LIBS@ \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

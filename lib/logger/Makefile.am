## Process this file with automake to produce Makefile.in.

pkglib_LTLIBRARIES = liblogger.la

liblogger_la_SOURCES = \
	logger.h logger.c

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

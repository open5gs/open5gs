## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = dbg_msg_dumps.la

dbg_msg_dumps_la_SOURCES = \
	dbg_msg_dumps.c

AM_LDFLAGS = \
	-module -avoid-version -shared

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

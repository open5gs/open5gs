## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libfdgx.la

libfdgx_la_SOURCES = \
	gx_dict.h gx_message.h dict_init.c gx_message.c

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/base

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

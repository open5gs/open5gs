## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libfdproto.la

libfdproto_la_SOURCES = \
	fdproto-internal.h \
	dictionary.c dictionary_functions.c dispatch.c fifo.c init.c \
	lists.c log.c messages.c ostr.c portability.c rt_data.c \
	sessions.c utils.c version.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/include

AM_CFLAGS = \
	-Wall -Werror -Wno-unused-variable

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = *.stackdump

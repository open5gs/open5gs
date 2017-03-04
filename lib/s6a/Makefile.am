## Process this file with automake to produce Makefile.in

SUBDIRS = freeDiameter

noinst_LTLIBRARIES = libs6a.la

libs6a_la_SOURCES = \
	milenage.h kasme.h \
	s6a_lib.h

nodist_libs6a_la_SOURCES = \
	milenage.c kasme.c \
	s6a_init.c s6a_fd.c s6a_config.c s6a_dict.c

libs6a_la_DEPENDENCIES = \
    $(top_srcdir)/lib/s6a/freeDiameter/libs6afreeDiameter.la

libs6a_la_LIBADD = \
    $(top_srcdir)/lib/s6a/freeDiameter/libs6afreeDiameter.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror -DSYSCONFDIR=\"$(sysconfdir)/\"

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

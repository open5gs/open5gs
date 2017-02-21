## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libmessages6a.la

libmessages6a_la_SOURCES = \
	freeDiameter/extension.h \
	s6a_message.h

nodist_libmessages6a_la_SOURCES = \
	s6a_init.c s6a_dict.c \
	dict_nas_mipv6.c dict_s6a.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

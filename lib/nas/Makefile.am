## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libnas.la

libnas_la_SOURCES = \
	nas_message.h nas_ies.h nas_types.h

nodist_libnas_la_SOURCES = \
	nas_decoder.c nas_encoder.c nas_ies.c

libnas_la_DEPENDENCIES = \
    $(top_srcdir)/lib/3gpp/lib3gpp.la

libnas_la_LIBADD = \
    $(top_srcdir)/lib/3gpp/lib3gpp.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/3gpp

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libnas.la

libnas_la_SOURCES = \
	nas_message.h nas_ies.h

nodist_libnas_la_SOURCES = \
	nas_decoder.c nas_encoder.c nas_ies.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = libnas.la

libnas_la_SOURCES = \
	nas_message.h nas_ies.h nas_types.h \
	nas_decoder.c nas_encoder.c nas_ies.c nas_types.c \
	$(NULL)

libnas_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(NULL)

libnas_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(NULL)

AM_LDFLAGS = \
	-version-info @LIBVERSION@ \
	$(NULL)

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

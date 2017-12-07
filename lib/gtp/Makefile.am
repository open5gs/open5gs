## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = libgtp.la

libgtp_la_SOURCES = \
	gtp_message.h gtp_types.h gtp_conv.h gtp_node.h gtp_path.h gtp_xact.h \
	gtp_message.c gtp_types.c gtp_conv.c gtp_node.c gtp_path.c gtp_xact.c \
	$(NULL)

libgtp_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(NULL)

libgtp_la_LIBADD = \
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

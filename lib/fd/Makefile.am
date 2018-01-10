## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = libfd.la

libfd_la_SOURCES = \
	fd_message.h fd_logger.h fd_lib.h \
	libapp_sip.c fd_message.c fd_logger.c fd_config.c fd_init.c \
	\
	s6a/s6a_dict.h s6a/s6a_message.h \
	s6a/dict_init.c \
	\
	gx/gx_dict.h gx/gx_message.h \
	gx/dict_init.c gx/gx_message.c \
	\
	rx/rx_dict.h rx/rx_message.h \
	rx/dict_init.c rx/rx_message.c \
	$(NULL)

libfd_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la \
	$(NULL)

libfd_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la \
	$(NULL)

AM_LDFLAGS = \
	-version-info @LIBVERSION@ \
	$(NULL)

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

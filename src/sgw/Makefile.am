## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libsgw.la

libsgw_la_SOURCES = \
    sgw_event.h sgw_context.h \
	sgw_gtp_path.h sgw_sm.h sgw_s11_handler.h sgw_s5c_handler.h \
    sgw_init.c sgw_event.c sgw_context.c \
	sgw_gtp_path.c sgw_sm.c sgw_s11_handler.c sgw_s5c_handler.c \
	$(NULL)

libsgw_la_DEPENDENCIES = \
    $(top_srcdir)/lib/gtp/libgtp.la \
    $(top_srcdir)/src/common/libcommon.la \
	$(NULL)

libsgw_la_LIBADD = \
    $(top_srcdir)/lib/gtp/libgtp.la \
    $(top_srcdir)/src/common/libcommon.la \
	$(NULL)

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib \
	-I$(top_srcdir)/src \
	@MONGOC_CFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

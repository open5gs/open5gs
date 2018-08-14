## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = \
	context.h application.h yaml_helper.h \
	context.c application.c yaml_helper.c \
	$(NULL)

libcommon_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
	$(NULL)

libcommon_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
	@YAML_LIBS@ \
	@MONGOC_LIBS@ \
	$(NULL)

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
	@MONGOC_CFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

DEFS = \
   @DEFS@ \
   -DSYSCONF_DIR=\"$(sysconfdir)/\" \
   -DLOCALSTATE_DIR=\"$(localstatedir)/\" \
   $(NULL)


MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

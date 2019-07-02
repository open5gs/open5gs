## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libapp.la

libapp_la_SOURCES = \
	ogs-yaml.h ogs-yaml.c \
	context.h application.h context.c application.c \
	$(NULL)

libapp_la_LIBADD = \
	@OGSCORE_LIBS@ \
	@YAML_LIBS@ \
	@MONGOC_LIBS@ \
	$(NULL)
 
libapp_la_DEPENDENCIES = \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
	@YAML_CFLAGS@ \
	@MONGOC_CFLAGS@ \
	-I$(top_srcdir)/lib \
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

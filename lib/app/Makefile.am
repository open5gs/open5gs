## Process this file with automake to produce Makefile.in.

pkglib_LTLIBRARIES = libogsapp.la

libogsapp_la_SOURCES = \
	ogs-yaml.h ogs-yaml.c \
	ogs-config.h ogs-config.c \
	ogs-init.h ogs-init.c \
	$(NULL)

libogsapp_la_LIBADD = \
	@OGSCORE_LIBS@ \
	@YAML_LIBS@ \
	$(NULL)
 
libogsapp_la_DEPENDENCIES = \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
	@YAML_CFLAGS@ \
	-I$(top_srcdir)/lib \
	$(NULL)

AM_CFLAGS = \
	-DOGS_APP_COMPILATION \
	-Wall -Werror \
	$(NULL)

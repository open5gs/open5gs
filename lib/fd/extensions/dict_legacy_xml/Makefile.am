## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = dict_legacy_xml.la

dict_legacy_xml_la_SOURCES = \
	dict_lxml.tab.y lex.dict_lxml.l dict_lxml.c dict_lxml_xml.c

dict_legacy_xml_la_DEPENDENCIES = \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la

dict_legacy_xml_la_LIBADD = \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdcore/libfdcore.la \
    $(top_srcdir)/lib/@FREEDIAMETER_DIR@/libfdproto/libfdproto.la

AM_YFLAGS = -p dict_lxml -d

AM_LFLAGS = -p -Cem -Pdict_lxml

AM_LDFLAGS = \
	-module -avoid-version -shared

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/fd/gx \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include \
	@LIBXML2_CFLAGS@

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

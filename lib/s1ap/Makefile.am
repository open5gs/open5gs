## Process this file with automake to produce Makefile.in

SUBDIRS = asn1c

pkglib_LTLIBRARIES = libs1ap.la

libs1ap_la_SOURCES = \
	s1ap_decoder.c s1ap_encoder.c s1ap_free.c \
	$(NULL)

libs1ap_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/s1ap/asn1c/libs1apasn1c.la \
	$(NULL)

libs1ap_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/s1ap/asn1c/libs1apasn1c.la \
	$(NULL)

AM_LDFLAGS = \
	-version-info @LIBVERSION@

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/s1ap/asn1c \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

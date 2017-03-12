## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libgtp.la

libgtp_la_SOURCES = \
	asn_msg.h asn_tlv_desc.h

nodist_libgtp_la_SOURCES = \
	asn_msg.c asn_tlv_desc.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/3gpp

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

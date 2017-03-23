## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libgtp.la

libgtp_la_SOURCES = \
	gtpv2c_tlv.h gtp_path.h

nodist_libgtp_la_SOURCES = \
	gtpv2c_tlv.c gtp_path.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/3gpp

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

## Process this file with automake to produce Makefile.in.

SUBDIRS = common mme hss sgw pgw pcrf

noinst_LTLIBRARIES = libmme.la libhss.la libsgw.la libpgw.la libpcrf.la libepc.la

libmme_la_SOURCES = mme.c
libmme_la_DEPENDENCIES = $(top_srcdir)/src/mme/libmme.la 
libmme_la_LIBADD = $(top_srcdir)/src/mme/libmme.la

libhss_la_SOURCES = hss.c
libhss_la_DEPENDENCIES = $(top_srcdir)/src/hss/libhss.la
libhss_la_LIBADD = $(top_srcdir)/src/hss/libhss.la

libsgw_la_SOURCES = sgw.c
libsgw_la_DEPENDENCIES = $(top_srcdir)/src/sgw/libsgw.la
libsgw_la_LIBADD = $(top_srcdir)/src/sgw/libsgw.la 

libpgw_la_SOURCES = pgw.c
libpgw_la_DEPENDENCIES = $(top_srcdir)/src/pgw/libpgw.la
libpgw_la_LIBADD = $(top_srcdir)/src/pgw/libpgw.la

libpcrf_la_SOURCES = pcrf.c
libpcrf_la_DEPENDENCIES = $(top_srcdir)/src/pcrf/libpcrf.la 
libpcrf_la_LIBADD = $(top_srcdir)/src/pcrf/libpcrf.la 

libepc_la_SOURCES = epc.c
libepc_la_DEPENDENCIES = \
    $(top_srcdir)/src/mme/libmme.la \
    $(top_srcdir)/src/hss/libhss.la \
    $(top_srcdir)/src/sgw/libsgw.la \
    $(top_srcdir)/src/pgw/libpgw.la \
    $(top_srcdir)/src/pcrf/libpcrf.la \
	$(NULL)
libepc_la_LIBADD = \
    $(top_srcdir)/src/mme/libmme.la \
    $(top_srcdir)/src/hss/libhss.la \
    $(top_srcdir)/src/sgw/libsgw.la \
    $(top_srcdir)/src/pgw/libpgw.la \
    $(top_srcdir)/src/pcrf/libpcrf.la \
	$(NULL)

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
	-I$(top_srcdir)/src \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

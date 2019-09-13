## Process this file with automake to produce Makefile.in.

SUBDIRS = mme hss sgw pgw pcrf

noinst_LTLIBRARIES = libmme.la libhss.la libsgw.la libpgw.la libpcrf.la libepc.la

libmme_la_SOURCES = mme.c
libmme_la_DEPENDENCIES = \
	$(top_srcdir)/src/mme/libmme.la \
	$(NULL)
libmme_la_LIBADD = \
	$(top_srcdir)/src/mme/libmme.la \
	@OGSCRYPT_LIBS@ \
	$(NULL)

libhss_la_SOURCES = hss.c
libhss_la_DEPENDENCIES = \
	$(top_srcdir)/src/hss/libhss.la \
	$(NULL)
libhss_la_LIBADD = \
	$(top_srcdir)/src/hss/libhss.la \
	@OGSCRYPT_LIBS@ \
	$(NULL)

libsgw_la_SOURCES = sgw.c
libsgw_la_DEPENDENCIES = \
	$(top_srcdir)/src/sgw/libsgw.la \
	$(NULL)
libsgw_la_LIBADD = \
	$(top_srcdir)/src/sgw/libsgw.la \
	$(NULL)

libpgw_la_SOURCES = pgw.c
libpgw_la_DEPENDENCIES = \
	$(top_srcdir)/src/pgw/libpgw.la \
	$(NULL)
libpgw_la_LIBADD = \
	$(top_srcdir)/src/pgw/libpgw.la \
	$(NULL)

libpcrf_la_SOURCES = pcrf.c
libpcrf_la_DEPENDENCIES = \
	$(top_srcdir)/src/pcrf/libpcrf.la \
	$(NULL)
libpcrf_la_LIBADD = \
	$(top_srcdir)/src/pcrf/libpcrf.la \
	$(NULL)

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
	@OGSCRYPT_LIBS@ \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/lib \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = libfdcore.la

libfdcore_la_SOURCES = \
	fdcore-internal.h \
	apps.c cnxctx.h config.c core.c cnxctx.c endpoints.c events.c \
	extensions.c fifo_stats.c hooks.c dict_base_proto.c messages.c \
	queues.c peers.c p_ce.c p_cnx.c p_dw.c p_dp.c p_expiry.c \
	p_out.c p_psm.c p_sr.c routing_dispatch.c server.c tcp.c \
	fdd.tab.y lex.fdd.l version.c

libfdcore_la_DEPENDENCIES = \
	$(top_srcdir)/libfdproto/libfdproto.la

libfdcore_la_LIBADD = \
	$(top_srcdir)/libfdproto/libfdproto.la

if SCTP
libfdcore_la_SOURCES += sctp.c sctp3436.c
endif

AM_YFLAGS = -p fdd -d
AM_LFLAGS = -p -Cem -Pfdd

BUILT_SOURCES = fdd.tab.h

AM_LDFLAGS = \
	-version-info @LIBVERSION@

AM_CPPFLAGS = \
    -I$(top_srcdir)/include

AM_CFLAGS =

MAINTAINERCLEANFILES = version.h fdd.c Makefile.in
MOSTLYCLEANFILES = *.stackdump

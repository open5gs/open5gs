## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libfdcore.la

libfdcore_la_SOURCES = \
	apps.c cnxctx.c cnxctx.h config.c core.c dict_base_proto.c endpoints.c \
	events.c extensions.c fdcore-internal.h fdd.l fdd.y fifo_stats.c hooks.c \
	messages.c p_ce.c p_cnx.c p_dp.c p_dw.c p_expiry.c p_out.c p_psm.c p_sr.c \
	peers.c queues.c routing_dispatch.c sctp.c sctp3436.c server.c tcp.c \
	version.c

AM_CPPFLAGS = \
    -I$(top_srcdir)/include

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

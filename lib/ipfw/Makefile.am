## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libipfw.la

libipfw_la_SOURCES = \
	ipfw2.h \
	ipfw2.c dummynet.c ipv6.c tables.c \
	expand_number.c humanize_number.c \
	glue.c

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/ipfw/objs/include_e \
	-include glue.h \
	-DUSERSPACE -D__BSD_VISIBLE -DNEED_STRTONUM -DNEED_ROUNDUP2 \
	@IPFW_CPPFLAGS@

AM_CFLAGS = \
	-Wall -Werror -Wno-shift-negative-value \
	-Wno-unused-but-set-variable -Wno-unknown-warning-option

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

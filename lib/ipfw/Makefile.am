## Process this file with automake to produce Makefile.in.

pkglib_LTLIBRARIES = libipfw.la

EXTRA_DIST = \
	objs/include_e/timeconv.h \
	objs/include_e/net/if_var.h \
	objs/include_e/netinet/ip_fw.h \
	objs/include_e/netinet/ip_dummynet.h \
	objs/include_e/sys/sockio.h \
	objs/include_e/libutil.h \
	objs/include_e/timeconv.h \
	objs/include_e/net/if_var.h \
	objs/include_e/netinet/ip_fw.h \
	objs/include_e/netinet/ip_dummynet.h \
	objs/include_e/sys/sockio.h \
	objs/include_e/libutil.h \
	$(NULL)

libipfw_la_SOURCES = \
	ipfw2.h ipfw2.c dummynet.c ipv6.c tables.c \
	expand_number.c humanize_number.c \
	glue.h glue.c missing.h \
	$(NULL)

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/ipfw/objs/include_e \
	-include glue.h \
	-D_DEFAULT_SOURCE -D_BSD_SOURCE \
	-DUSERSPACE -D__BSD_VISIBLE -DNEED_STRTONUM -DNEED_ROUNDUP2 \
	@IPFW_CPPFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror -Wno-shift-negative-value \
	-Wno-unused-but-set-variable -Wno-unknown-warning-option \
	$(NULL)

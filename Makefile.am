## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = lib src test

bin_PROGRAMS = nextepc-mmed nextepc-hssd nextepc-sgwd nextepc-pgwd nextepc-pcrfd nextepc-epcd

dist_nextepc_mmed_SOURCES = main.c
nextepc_mmed_LDADD = $(top_srcdir)/src/libmme.la
nextepc_mmed_LDFLAGS = -export-dynamic

dist_nextepc_hssd_SOURCES = main.c
nextepc_hssd_LDADD = $(top_srcdir)/src/libhss.la
nextepc_hssd_LDFLAGS = -export-dynamic

dist_nextepc_sgwd_SOURCES = main.c
nextepc_sgwd_LDADD = $(top_srcdir)/src/libsgw.la
nextepc_sgwd_LDFLAGS = -export-dynamic

dist_nextepc_pgwd_SOURCES = main.c
nextepc_pgwd_LDADD = $(top_srcdir)/src/libpgw.la
nextepc_pgwd_LDFLAGS = -export-dynamic

dist_nextepc_pcrfd_SOURCES = main.c
nextepc_pcrfd_LDADD = $(top_srcdir)/src/libpcrf.la
nextepc_pcrfd_LDFLAGS = -export-dynamic

dist_nextepc_epcd_SOURCES = main.c
nextepc_epcd_LDADD = $(top_srcdir)/src/libepc.la
nextepc_epcd_LDFLAGS = -export-dynamic

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/src

sysconf_DATA = \
	support/nextepc.conf

freeDiameterdir = ${prefix}/etc/freeDiameter
freeDiameter_DATA = \
	support/freeDiameter/mme.conf \
	support/freeDiameter/hss.conf \
	support/freeDiameter/pgw.conf \
	support/freeDiameter/pcrf.conf
dist_freeDiameter_DATA = \
	support/freeDiameter/cacert.pem \
	support/freeDiameter/mme.key.pem \
	support/freeDiameter/mme.cert.pem \
	support/freeDiameter/hss.key.pem \
	support/freeDiameter/hss.cert.pem \
	support/freeDiameter/pgw.key.pem \
	support/freeDiameter/pgw.cert.pem \
	support/freeDiameter/pcrf.key.pem \
	support/freeDiameter/pcrf.cert.pem

install-data-hook:
	$(MKDIR_P) $(DESTDIR)/$(prefix)/var/log

CLEANFILES = symtbl.c
DISTCLEANFILES = $(DIST_ARCHIVES)
MAINTAINERCLEANFILES = \
    configure config.in \
    aclocal.m4 m4/ltsugar.m4 m4/libtool.m4 m4/ltversion.m4 \
    m4/lt~obsolete.m4 m4/ltoptions.m4 \
    build-aux/ar-lib build-aux/config.guess build-aux/depcomp \
	build-aux/ltmain.sh build-aux/test-driver build-aux/compile \
	build-aux/config.sub build-aux/missing build-aux/install-sh \
	.cscope.out .cscope.out.in .cscope.files .cscope.out.po \
    Makefile.in
MOSTLYCLEANFILES = core *.stackdump

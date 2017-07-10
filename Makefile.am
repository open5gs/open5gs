## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = lib src test

bin_PROGRAMS = nextepcd

dist_nextepcd_SOURCES = main.c 

nextepcd_LDADD = \
    $(top_srcdir)/src/libnextepc.la

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/src

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

## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = lib src test

bin_PROGRAMS = cellwired

dist_cellwired_SOURCES = main.c symtbl.h
nodist_cellwired_SOURCES = symtbl.c

cellwired_LDADD = \
    $(top_srcdir)/src/libcellwire.la \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/logger/liblogger.la \
    $(top_srcdir)/lib/message/src/libmessage.la \
    -lpthread -lm -lsctp @OSLIBS@

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/logger \
    -I$(top_srcdir)/lib/message/src/s1ap \
    -I$(top_srcdir)/lib/emssage/src \
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

EXTRA_DIST = symtbl.sh

NM_TMP_FILE = nm.tmp
.INTERMEDIATE: symtbl.c
symtbl.c: $(bsd_DEPENDENCIES)
	@for las in $<; do \
		lib=`echo $$las | sed -e "s|\(.*\)/lib\([^/].*\)\.la|\1/.libs/lib\2.a|g"`;  \
		echo $$lib; \
		nm $$lib >> $(NM_TMP_FILE); \
	done; \
	$(top_srcdir)/symtbl.sh $(NM_TMP_FILE) $@; \
	rm -f $(NM_TMP_FILE)

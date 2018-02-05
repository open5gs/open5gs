## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testepc testvolte

configdir = ${sysconfdir}/nextepc/test
config_DATA = sample.conf sample-volte.conf

testepc_SOURCES = \
	abts.h abts.c testpacket.h testpacket.c \
	testutil.h testutil.c \
	s1ap_message_test.c nas_message_test.c gtp_message_test.c \
	security_test.c s1setup_test.c attach_test.c volte_test.c handover_test.c \
	$(NULL)

testvolte_SOURCES = \
	abts.h abts.c testpacket.h testpacket.c \
	volte/testutil.h volte/testutil.c \
	volte/testapp.h volte/testapp.c volte/pcscf_fd_path.c \
	volte/volte_test.c \
	$(NULL)

LDADD = \
    $(top_srcdir)/src/libepc.la \
	$(NULL)

DEPENDENCIES = \
    $(top_srcdir)/src/libepc.la \
	$(NULL)

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib \
    -I$(top_srcdir)/src \
    -I$(top_srcdir)/lib/s1ap/asn1c \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include \
	@MONGOC_CFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	-Wno-unused-function -Wno-unused-variable \
	@OSCPPFLAGS@ \
	$(NULL)

DEFS = \
   @DEFS@ \
   -DFD_EXT_DIR=\"$(abs_top_builddir)/lib/@FREEDIAMETER_DIR@/extensions\" \
   $(NULL)


MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = core *.stackdump

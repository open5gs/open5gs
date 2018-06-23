## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testepc testvolte

configdir = ${sysconfdir}/nextepc/test
config_DATA = sample.conf sample-volte.conf

testepc_SOURCES = \
	common/abts.h common/abts.c \
	common/testpacket.h common/testpacket.c common/testutil.h \
	basic/testutil.c basic/s1ap_message_test.c basic/nas_message_test.c \
	basic/gtp_message_test.c basic/security_test.c basic/s1setup_test.c \
	basic/attach_test.c basic/volte_test.c basic/handover_test.c \
	$(NULL)

testvolte_SOURCES = \
	common/abts.h common/abts.c \
	common/testpacket.h common/testpacket.c common/testutil.h \
	volte/testutil.c volte/testapp.h volte/testapp.c volte/pcscf_fd_path.c \
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
	-I$(top_srcdir)/test/common \
	@MONGOC_CFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	-Wno-unused-function -Wno-unused-variable -Wno-deprecated-declarations \
	@OSCPPFLAGS@ \
	$(NULL)

DEFS = \
   @DEFS@ \
   -DFD_EXT_DIR=\"$(abs_top_builddir)/lib/@FREEDIAMETER_DIR@/extensions\" \
   $(NULL)

TESTSUITE = $(srcdir)/testsuite

check-local: atconfig $(TESTSUITE)
	$(SHELL) '$(TESTSUITE)' $(TESTSUITEFLAGS)

#installcheck-local: atconfig $(TESTSUITE)
#	$(SHELL) '$(TESTSUITE)' AUTOTEST_PATH='$(bindir)' \
#		$(TESTSUITEFLAGS)

clean-local:
	test ! -f '$(TESTSUITE)' || \
		$(SHELL) '$(TESTSUITE)' --clean

AUTOM4TE = $(SHELL) $(top_srcdir)/build-aux/missing --run autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest
$(TESTSUITE): $(srcdir)/testsuite.at $(srcdir)/package.m4
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $@.at
	mv $@.tmp $@

# The `:;' works around a Bash 3.2 bug when the output is not writeable.
$(srcdir)/package.m4: $(top_srcdir)/configure.ac
	:;{ \
               echo '# Signature of the current package.' && \
               echo 'm4_define([AT_PACKAGE_NAME],' && \
               echo '  [$(PACKAGE_NAME)])' && \
               echo 'm4_define([AT_PACKAGE_TARNAME],' && \
               echo '  [$(PACKAGE_TARNAME)])' && \
               echo 'm4_define([AT_PACKAGE_VERSION],' && \
               echo '  [$(PACKAGE_VERSION)])' && \
               echo 'm4_define([AT_PACKAGE_STRING],' && \
               echo '  [$(PACKAGE_STRING)])' && \
               echo 'm4_define([AT_PACKAGE_BUGREPORT],' && \
               echo '  [$(PACKAGE_BUGREPORT)])'; \
               echo 'm4_define([AT_PACKAGE_URL],' && \
               echo '  [$(PACKAGE_URL)])'; \
             } >'$(srcdir)/package.m4'

EXTRA_DIST = testsuite.at $(srcdir)/package.m4 $(TESTSUITE)

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = core *.stackdump

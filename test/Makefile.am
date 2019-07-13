## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testunit testsimple testcomplex testvolte

configdir = ${sysconfdir}/nextepc/test
config_DATA = sample.conf sample-simple.conf sample-volte.conf

testunit_SOURCES = \
	common/testpacket.h common/testpacket.c \
	unit/abts-main.c \
	unit/s1ap_message_test.c unit/nas_message_test.c \
	unit/gtp_message_test.c unit/security_test.c \
	$(NULL)
testunit_LDADD = $(top_srcdir)/src/libepc.la

if !WITH_USRSCTP
testunit_SOURCES += unit/sctp-test.c
endif

testsimple_SOURCES = \
	common/testpacket.h common/testpacket.c \
	simple/abts-main.c \
	simple/mnc3-test.c \
	$(NULL)
testsimple_LDADD = $(top_srcdir)/src/libepc.la

testcomplex_SOURCES = \
	common/testpacket.h common/testpacket.c \
	complex/abts-main.c \
	complex/s1setup_test.c \
	complex/attach_test.c complex/volte_test.c complex/handover_test.c \
	$(NULL)
testcomplex_LDADD = $(top_srcdir)/src/libepc.la

testvolte_SOURCES = \
	common/testpacket.h common/testpacket.c \
	volte/abts-main.c \
	volte/testapp.h volte/testapp.c \
	volte/pcscf_fd_path.h volte/pcscf_fd_path.c \
	volte/volte_test.c \
	$(NULL)
testvolte_LDADD = $(top_srcdir)/src/libepc.la

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
	@MONGOC_CFLAGS@ \
    -I$(top_srcdir)/lib \
    -I$(top_srcdir)/src \
    -I$(top_srcdir)/lib/asn1c/common \
    -I$(top_srcdir)/lib/asn1c/s1ap \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include \
	-I$(top_srcdir)/test/common \
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

AUTOM4TE = $(SHELL) $(top_srcdir)/build/missing --run autom4te
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

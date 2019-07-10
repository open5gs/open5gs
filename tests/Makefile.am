## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = testunit testsimple testcomplex testvolte testcsfb

configdir = ${sysconfdir}/nextepc/tests
config_DATA = sample.conf sample-simple.conf sample-volte.conf sample-csfb.conf

testunit_SOURCES = \
	common/test-packet.h common/test-packet.c \
	unit/abts-main.c \
	unit/s1ap-message-test.c unit/nas-message-test.c \
	unit/gtp-message-test.c unit/security-test.c \
	unit/sctp-test.c \
	$(NULL)
testunit_LDADD = $(top_srcdir)/src/libepc.la

testsimple_SOURCES = \
	common/test-packet.h common/test-packet.c \
	simple/abts-main.c \
	simple/mnc3-test.c \
	$(NULL)
testsimple_LDADD = $(top_srcdir)/src/libepc.la

testcomplex_SOURCES = \
	common/test-packet.h common/test-packet.c \
	complex/abts-main.c \
	complex/s1setup-test.c \
	complex/attach-test.c complex/volte-test.c complex/handover-test.c \
	complex/crash-test.c \
	$(NULL)
testcomplex_LDADD = $(top_srcdir)/src/libepc.la

testvolte_SOURCES = \
	common/test-packet.h common/test-packet.c \
	common/test-app.h \
	volte/abts-main.c volte/test-app.c \
	volte/pcscf-fd-path.h volte/pcscf-fd-path.c \
	volte/volte-test.c \
	$(NULL)
testvolte_LDADD = $(top_srcdir)/src/libepc.la

testcsfb_SOURCES = \
	common/test-packet.h common/test-packet.c \
	common/test-app.h \
	csfb/abts-main.c csfb/test-app.c \
	csfb/mo-idle-test.c csfb/mt-idle-test.c \
	csfb/mo-active-test.c \
	$(NULL)
testcsfb_LDADD = $(top_srcdir)/src/libepc.la

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
	@MONGOC_CFLAGS@ \
    -I$(top_srcdir)/lib \
    -I$(top_srcdir)/src \
    -I$(top_srcdir)/lib/asn1c/common \
    -I$(top_srcdir)/lib/asn1c/s1ap \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include \
	-I$(top_srcdir)/tests/common \
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

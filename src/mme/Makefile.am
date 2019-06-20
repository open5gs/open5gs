## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libmme.la

libmme_la_SOURCES = \
	mme-kdf.h kasumi.h snow-3g.h zuc.h \
    mme-event.h mme-context.h \
	ogs-sctp.h \
	s1ap-build.h s1ap-handler.h s1ap-conv.h s1ap-path.h \
	sgsap-build.h sgsap-conv.h sgsap-path.h \
	mme-fd-path.h mme-s6a-handler.h \
	nas-conv.h nas-security.h nas-path.h \
	emm-handler.h emm-build.h \
	esm-handler.h esm-build.h \
	mme-gtp-path.h mme-s11-build.h mme-s11-handler.h \
	mme-sm.h mme-path.h \
	mme-kdf.c kasumi.c snow-3g.c zuc.c \
    mme-init.c mme-event.c mme-context.c \
	ogs-sctp.c \
	s1ap-sm.c s1ap-build.c s1ap-handler.c s1ap-conv.c s1ap-path.c \
	sgsap-sm.c sgsap-build.c sgsap-conv.c sgsap-path.c \
	mme-fd-path.c mme-s6a-handler.c \
	nas-conv.c nas-security.c nas-path.c \
	emm-sm.c emm-handler.c emm-build.c \
	esm-sm.c esm-handler.c esm-build.c \
	mme-gtp-path.c mme-s11-build.c mme-s11-handler.c \
	mme-sm.c mme-path.c \
	sbc-message.h sbc-handler.h sbc-handler.c \
	$(NULL)

if WITH_USRSCTP
libmme_la_SOURCES += ogs-usrsctp.c s1ap-usrpath.c sgsap-usrpath.c
else
libmme_la_SOURCES += ogs-lksctp.c s1ap-lkpath.c sgsap-lkpath.c
endif

libmme_la_DEPENDENCIES = \
    $(top_srcdir)/lib/asn1c/libasn1c.la \
    $(top_srcdir)/lib/nas/libnas.la \
	$(top_srcdir)/lib/fd/libfd.la \
    $(top_srcdir)/lib/gtp/libgtp.la \
	$(top_srcdir)/src/app/libapp.la \
	$(NULL)

libmme_la_LIBADD = \
    $(top_srcdir)/lib/asn1c/libasn1c.la \
    $(top_srcdir)/lib/nas/libnas.la \
    $(top_srcdir)/lib/fd/libfd.la \
    $(top_srcdir)/lib/gtp/libgtp.la \
	$(top_srcdir)/src/app/libapp.la \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
    -I$(top_srcdir)/lib \
	-I$(top_srcdir)/src \
    -I$(top_srcdir)/lib/asn1c/common \
    -I$(top_srcdir)/lib/asn1c/s1ap \
	-I$(top_srcdir)/lib/@FREEDIAMETER_DIR@/include \
	@MONGOC_CFLAGS@ \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	$(NULL)

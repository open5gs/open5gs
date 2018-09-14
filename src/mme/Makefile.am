## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libmme.la

libmme_la_SOURCES = \
	mme_kdf.h kasumi.h snow_3g.h zuc.h \
    mme_event.h mme_context.h \
	s1ap_build.h s1ap_handler.h s1ap_conv.h s1ap_path.h \
	mme_fd_path.h mme_s6a_handler.h \
	nas_conv.h nas_security.h nas_path.h \
	emm_handler.h emm_build.h \
	esm_handler.h esm_build.h \
	mme_gtp_path.h mme_s11_build.h mme_s11_handler.h \
	mme_sm.h mme_path.h \
	mme_kdf.c kasumi.c snow_3g.c zuc.c \
    mme_init.c mme_event.c mme_context.c \
	s1ap_sm.c s1ap_build.c s1ap_handler.c s1ap_conv.c s1ap_path.c \
	mme_fd_path.c mme_s6a_handler.c \
	nas_conv.c nas_security.c nas_path.c \
	emm_sm.c emm_handler.c emm_build.c \
	esm_sm.c esm_handler.c esm_build.c \
	mme_gtp_path.c mme_s11_build.c mme_s11_handler.c \
	mme_sm.c mme_path.c \
	sbc_message.h sbc_handler.h sbc_handler.c \
	$(NULL)

if USRSCTP
libmme_la_SOURCES += s1ap_usrsctp.c
else
libmme_la_SOURCES += s1ap_sctp.c
endif

libmme_la_DEPENDENCIES = \
    $(top_srcdir)/lib/s1ap/libs1ap.la \
    $(top_srcdir)/lib/nas/libnas.la \
	$(top_srcdir)/lib/fd/libfd.la \
    $(top_srcdir)/lib/gtp/libgtp.la \
    $(top_srcdir)/src/common/libcommon.la \
	$(NULL)

libmme_la_LIBADD = \
    $(top_srcdir)/lib/s1ap/libs1ap.la \
    $(top_srcdir)/lib/nas/libnas.la \
    $(top_srcdir)/lib/fd/libfd.la \
    $(top_srcdir)/lib/gtp/libgtp.la \
    $(top_srcdir)/src/common/libcommon.la \
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
	$(NULL)

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

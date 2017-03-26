## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libmme.la

libmme_la_SOURCES = \
	kdf.h kasumi.h snow_3g.h zuc.h \
    event.h context.h \
	s1ap_build.h s1ap_handler.h s1ap_conv.h s1ap_path.h \
	nas_conv.h nas_security.h emm_handler.h \
	s11_path.h \
	sm.h s6a_sm.h

nodist_libmme_la_SOURCES = \
	kdf.c kasumi.c snow_3g.c zuc.c \
    init.c event.c context.c \
	s1ap_build.c s1ap_handler.c s1ap_conv.c s1ap_path.c \
	nas_conv.c nas_security.c emm_handler.c \
	s11_path.c \
	mme_sm.c enb_s1ap_sm.c ue_emm_sm.c s6a_sm.c 

libmme_la_DEPENDENCIES = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/s1ap/libs1ap.la \
    $(top_srcdir)/lib/nas/libnas.la \
	$(top_srcdir)/lib/s6a/libs6a.la \
    $(top_srcdir)/lib/gtp/libgtp.la

libmme_la_LIBADD = \
    $(top_srcdir)/lib/core/src/libcore.la \
    $(top_srcdir)/lib/s1ap/libs1ap.la \
    $(top_srcdir)/lib/nas/libnas.la \
    $(top_srcdir)/lib/s6a/libs6a.la \
    $(top_srcdir)/lib/gtp/libgtp.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/core/include \
    -I$(top_srcdir)/lib/3gpp \
    -I$(top_srcdir)/lib/s1ap/asn1c \
    -I$(top_srcdir)/lib/s1ap \
    -I$(top_srcdir)/lib/nas \
    -I$(top_srcdir)/lib/s6a \
    -I$(top_srcdir)/lib/gtp

AM_CFLAGS = \
	-Wall -Werror

MAINTAINERCLEANFILES = Makefile.in
MOSTLYCLEANFILES = core *.stackdump

EXTRA_DIST = .libs $(noinst_LTLIBRARIES)

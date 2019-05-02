## Process this file with automake to produce Makefile.in

SUBDIRS = @FREEDIAMETER_DIR@ ipfw
DIST_SUBDIRS = @FREEDIAMETER_DIR@ ipfw

if WITH_SRCLIB
SUBDIRS += ogslib
DIST_SUBDIRS += ogslib
endif

SUBDIRS += base asn1c nas fd gtp
DIST_SUBDIRS += base asn1c nas fd gtp

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

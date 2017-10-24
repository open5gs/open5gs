unitfiles = \
	nextepc-mmed.service \
	nextepc-sgwd.service \
	nextepc-pgwd.service \
	nextepc-pcrfd.service \
	nextepc-hssd.service \
	$(NULL)

unitdir = ${systemd_unit_dir}

unit_DATA = $(unitfiles)

EXTRA_DIST = $(unitfiles)

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

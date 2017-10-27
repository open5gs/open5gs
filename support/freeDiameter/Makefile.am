configfiles = \
	mme.conf \
	hss.conf \
	pgw.conf \
	pcrf.conf \
    $(NULL)

pemfiles = \
	cacert.pem \
	mme.key.pem \
	mme.cert.pem \
	hss.key.pem \
	hss.cert.pem \
	pgw.key.pem \
	pgw.cert.pem \
	pcrf.key.pem \
	pcrf.cert.pem \
    $(NULL)

freeDiameterdir = ${sysconfdir}/nextepc/freeDiameter

freeDiameter_DATA = ${configfiles} ${pemfiles}

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = *.stackdump

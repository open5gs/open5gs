#!/usr/bin/make -s
#
# This file is inspired from freeDiameter's contrib/ca_script and
# improved to handle multiple CA in a hierarchical fashion.
# Warning: the directory structure is flat, does not reflect the CA hierarchy

SCRIPT_DIR = .
DATA_DIR = ./ca_data

CONFIG = -config $(SCRIPT_DIR)/openssl.cnf
REMAKE = $(MAKE) -f $(SCRIPT_DIR)/Makefile

#Disable "make destroy" -- overwrite on command line
force = 

#RSA key sizes, can be overwritten on command line
cakeysize = 2048
keysize = 1024

# Save current date
DATE=`date +%Y%m%d-%H%M%S`

# Default: print the help
all:	help

# Help message
help:
	@echo "\n\
Available commands:\n\
   make init topca=name\n\
       Creates the initial top-level CA structure\n\
   make newca name=newcaname ca=parentca\n\
       Creates a new sub-CA that can be used for certificates later.\n\
   make newcert name=foo ca=parentca\n\
       Create private key and csr, then issue the certificate (named foo.*)\n\
   make p12 name=foo ca=parentca\n\
       Same as newcert, but additionnaly creates a pkcs12 file to ship client certificate to Windows or Mac\n\
   make ship name=foo ca=parentca\n\
       Create an archive with the data for the client (useful for freeDiameter peers)\n\
   make revoke name=foo ca=parentca\n\
       Revokes the certificate foo.cert issued by parentca and regenerates the CRL.\n\
   make gencrl ca=caname\n\
       Regenerates the CRL of CA caname. Should be run periodically.\n\
\n\
";

# Destroy the CA hierarchy completely. Use with care.
destroy:
	@if [ -z "$(force)" ]; then echo "Destroy disabled, use: make destroy force=y"; exit 1; fi
	@if [ ! -d $(SCRIPT_DIR) ]; then echo "Error in setup"; exit 1; fi
	@echo "Removing everything (for debug purpose)..."
	@rm -rf $(DATA_DIR)/*

# Initialize the CA structure
structure:
	@if [ -z "$(caname)" ]; then echo "Internal error: caname is missing"; exit 1; fi
	@if [ -d $(DATA_DIR)/$(caname) ]; then echo "CA $(caname) already exists."; exit 1; fi
	# Creating CA structure
	@mkdir -p $(DATA_DIR)/$(caname)
	@mkdir $(DATA_DIR)/$(caname)/public
	@mkdir $(DATA_DIR)/$(caname)/public/crl
	@mkdir $(DATA_DIR)/$(caname)/private
	@chmod 700 $(DATA_DIR)/$(caname)/private
	@mkdir $(DATA_DIR)/$(caname)/clients
	@echo "01" > $(DATA_DIR)/$(caname)/serial
	@echo "01" > $(DATA_DIR)/$(caname)/crlnumber
	@touch $(DATA_DIR)/$(caname)/index.txt

# Initialize the top-level CA structure and keys.
init:
	@if [ -z "$(topca)" ]; then echo "Please specify the name of the root CA. Ex: make init topca=rootca.testbed.aaa"; exit 1; fi
	# Create the folder hierarchy
	@$(REMAKE) structure caname=$(topca)
	# Generate the self-signed certificate
	@CA_ROOT_DIR=$(DATA_DIR)/$(topca) openssl req $(CONFIG) -new -batch -x509 -days 3650 -nodes -newkey rsa:$(cakeysize) -out $(DATA_DIR)/$(topca)/public/cacert.pem \
		-keyout $(DATA_DIR)/$(topca)/private/cakey.pem -extensions ca_cert -subj /CN=$(topca)
	@ln -s cacert.pem $(DATA_DIR)/$(topca)/public/`openssl x509 -noout -hash < $(DATA_DIR)/$(topca)/public/cacert.pem`.0
	@touch $(DATA_DIR)/$(topca)/public/cachain.pem
	@ln -s ../../$(topca)/public/cacert.pem $(DATA_DIR)/$(topca)/public/caroot.pem 
	@$(REMAKE) gencrl ca=$(topca)

# Create a secondary CA
newca:
	@if [ -z "$(name)" -o -z "$(ca)" ]; then echo "Missing parameter. Ex: make newca name=subca.testbed.aaa ca=rootca.testbed.aaa"; exit 1; fi
	@if [ ! -e $(DATA_DIR)/$(ca)/private/cakey.pem ]; then echo "The parent CA $(ca) does not exist."; exit 1; fi
	@if [ ! -d $(DATA_DIR)/$(name) ]; then $(REMAKE) structure caname=$(name); fi
	# Generate the private key and CSR for the new CA if needed
	@if [ ! -e $(DATA_DIR)/$(name)/private/cakey.pem ]; then \
		openssl genrsa -out $(DATA_DIR)/$(name)/private/cakey.pem $(cakeysize) ; fi
	@if [ ! -e $(DATA_DIR)/$(name)/private/cacsr.pem ]; then \
		CA_ROOT_DIR=$(DATA_DIR)/$(name) openssl req $(CONFIG) -new -batch -out $(DATA_DIR)/$(name)/private/cacsr.pem \
			-key $(DATA_DIR)/$(name)/private/cakey.pem \
			-subj /CN=$(name) -reqexts v3_req_ca; fi
	# Revoke a previous certificate for this CA if any
	@if [ -e $(DATA_DIR)/$(name)/public/cacert.pem ]; then \
		echo "Revoking previous certificate ..."; \
		$(REMAKE) revoke name=$(name) ca=$(ca); \
		mv $(DATA_DIR)/$(name)/public/cacert.pem $(DATA_DIR)/$(name)/public/cacert-$(DATE).pem; fi
	# Issue the new CA certificate
	@CA_ROOT_DIR=$(DATA_DIR)/$(ca) openssl ca $(CONFIG) -in $(DATA_DIR)/$(name)/private/cacsr.pem \
		-out $(DATA_DIR)/$(name)/public/cacert.pem \
		-batch -extensions ca_cert
	# Hash and link to parent
	@ln -s cacert.pem $(DATA_DIR)/$(ca)/public/`openssl x509 -noout -hash < $(DATA_DIR)/$(name)/public/cacert.pem`.0
	@rm -f $(DATA_DIR)/$(name)/parent
	@ln -s ../$(ca) $(DATA_DIR)/$(name)/parent
	@cat $(DATA_DIR)/$(name)/public/cacert.pem $(DATA_DIR)/$(ca)/public/cachain.pem > $(DATA_DIR)/$(name)/public/cachain.pem
	@ln -s ../../$(ca)/public/caroot.pem $(DATA_DIR)/$(name)/public/caroot.pem
	@for CRLFILE in `cd $(DATA_DIR)/$(ca)/public/crl && ls -1`; do ln -sf ../../../$(ca)/public/crl/$$CRLFILE $(DATA_DIR)/$(name)/public/crl/$$CRLFILE; done
	@$(REMAKE) gencrl ca=$(name)

# Create a new certificate for use in TLS communications and other terminal usages
newcert:
	@if [ -z "$(name)" -o -z "$(ca)" ]; then echo "Missing parameter. Ex: make newcert name=service.testbed.aaa ca=ca.testbed.aaa"; exit 1; fi
	@if [ ! -e $(DATA_DIR)/$(ca)/private/cakey.pem ]; then echo "The parent CA $(ca) does not exist."; exit 1; fi
	@if [ ! -d $(DATA_DIR)/$(ca)/clients/$(name) ]; then mkdir $(DATA_DIR)/$(ca)/clients/$(name); fi
	# Create a private key if needed
	@if [ ! -e $(DATA_DIR)/$(ca)/clients/$(name)/privkey.pem ]; then \
		openssl genrsa -out $(DATA_DIR)/$(ca)/clients/$(name)/privkey.pem $(keysize); fi
	# Create a CSR if needed
	@if [ ! -e $(DATA_DIR)/$(ca)/clients/$(name)/csr.pem ]; then \
		CA_ROOT_DIR=$(DATA_DIR)/$(ca) openssl req $(CONFIG) -new -batch -out $(DATA_DIR)/$(ca)/clients/$(name)/csr.pem \
			-key $(DATA_DIR)/$(ca)/clients/$(name)/privkey.pem \
			-subj /CN=$(name); fi
	# Revoke a previous certificate if any
	@if [ -e $(DATA_DIR)/$(ca)/clients/$(name)/cert.pem ]; then \
		$(REMAKE) revoke name=$(name) ca=$(ca); \
		mv $(DATA_DIR)/$(ca)/clients/$(name)/cert.pem $(DATA_DIR)/$(ca)/clients/$(name)/cert-$(DATE).pem; fi
	# Now sign the new certificate with the CA key
	@CA_ROOT_DIR=$(DATA_DIR)/$(ca) openssl ca $(CONFIG) -in $(DATA_DIR)/$(ca)/clients/$(name)/csr.pem \
		-out $(DATA_DIR)/$(ca)/clients/$(name)/cert.pem \
		-batch
	# Hash
	@ln -sf `cat $(DATA_DIR)/$(ca)/serial.old`.pem $(DATA_DIR)/$(ca)/public/`openssl x509 -noout -hash < $(DATA_DIR)/$(ca)/clients/$(name)/cert.pem`.0
	# Compiled informations for the client
	@cat $(DATA_DIR)/$(ca)/clients/$(name)/cert.pem $(DATA_DIR)/$(ca)/public/cachain.pem > $(DATA_DIR)/$(ca)/clients/$(name)/certchain.pem
	@ln -sf ../../public/crl $(DATA_DIR)/$(ca)/clients/$(name)/crl
	@ln -sf ../../public/caroot.pem $(DATA_DIR)/$(ca)/clients/$(name)/ca.pem

# Create a PKCS#12 file containing the client's information
p12:	newcert
	# Create the PKCS#12 file
	@cat 	$(DATA_DIR)/$(ca)/clients/$(name)/privkey.pem \
		$(DATA_DIR)/$(ca)/clients/$(name)/certchain.pem \
		$(DATA_DIR)/$(ca)/clients/$(name)/ca.pem \
		| openssl pkcs12 -export -out $(DATA_DIR)/$(ca)/clients/$(name)/$(name).p12
	@echo "Client certificate is created in $(DATA_DIR)/$(ca)/clients/$(name)/$(name).p12"

# Create an archive to send the data to the client node
ship:
	@if [ -z "$(name)" -o -z "$(ca)" ]; then echo "Missing parameter. Ex: make ship name=service.testbed.aaa ca=ca.testbed.aaa"; exit 1; fi
	@if [ ! -e $(DATA_DIR)/$(ca)/private/cakey.pem ]; then echo "The parent CA $(ca) does not exist."; exit 1; fi
	@if [ ! -e  $(DATA_DIR)/$(ca)/clients/$(name)/privkey.pem ]; then echo "The client $(name) does not exist, use 'make newcert' first."; exit 1; fi
	# Ship the data
	@tar -c -C $(DATA_DIR)/$(ca)/clients/$(name) -z -f $(ca)_$(name).tar.gz -h .
	@echo "The files have been packaged into archive: $(ca)_$(name).tar.gz"

# Revoke a certificate
revoke:
	@if [ -z "$(name)" -o -z "$(ca)" ]; then echo "Missing parameter. Ex: make revoke name=service.testbed.aaa ca=ca.testbed.aaa"; exit 1; fi
	@if [ ! -e $(DATA_DIR)/$(ca)/private/cakey.pem ]; then echo "The parent CA $(ca) does not exist."; exit 1; fi
	@if [ ! -e $(DATA_DIR)/$(ca)/clients/$(name)/cert.pem ]; \
		then echo "$(DATA_DIR)/$(ca)/clients/$(name)/cert.pem not found"; \
		exit 1; \
		fi;
	# Revoke the certificate
	@CA_ROOT_DIR=$(DATA_DIR)/$(ca) openssl ca $(CONFIG) -revoke $(DATA_DIR)/$(ca)/clients/$(name)/cert.pem;
	@$(REMAKE) gencrl ca=$(ca)

# Regenerate the Certificate Revocation List.
gencrl:
	@if [ -z "$(ca)" ]; then echo "Missing parameter. Ex: make gencrl ca=ca.testbed.aaa"; exit 1; fi
	# Create the CRL
	@CA_ROOT_DIR=$(DATA_DIR)/$(ca) openssl ca $(CONFIG) -gencrl -out $(DATA_DIR)/$(ca)/public/crl/$(ca).pem
	@ln -s crl/$(ca).pem $(DATA_DIR)/$(ca)/public/local.pem
	@ln -s local.pem $(DATA_DIR)/$(ca)/public/`openssl crl -noout -hash < $(DATA_DIR)/$(ca)/public/crl/$(ca).pem`.r0

# End of file...

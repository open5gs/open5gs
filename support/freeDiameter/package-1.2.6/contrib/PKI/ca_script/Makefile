#!/usr/bin/make -s
#
# This file is designed to automatize the CA tasks such as:
#  -> init  : create the initial CA tree and the CA root certificate.
#  -> newcsr: create a new private key and csr. $name and $email must be set. C, ST, L, O, OU may be overwitten (example: make newcsr C=FR)
#  -> cert  : sign a pending CSR and generate the certificate. $name must be provided.
#  -> revoke: revoke a certificate. $name must be provided.
#  -> gencrl: update/create the CRL.
#
# The file should be located in the directory STATIC_DIR as defined below.
# The DIR directory will contain the data of the CA. It might be placed in /var.
# The DIR should also be configured in openssl.cnf file under [ CA_default ]->dir.
#
# Here are the steps to install the CA scripts in default environment:
## mkdir /etc/openssl-ca.static
## cp Makefile openssl.cnf /etc/openssl-ca.static
# ( configure the default parameters of your CA in /etc/openssl-ca/openssl.cnf ) ##
## mkdir /etc/openssl-ca
## make -f /etc/openssl-ca.static/Makefile destroy force=y
## cd /etc/openssl-ca
## make init
## make help

DIR = /home/thedoc/testbed.aaa/ca
STATIC_DIR = /home/thedoc/testbed.aaa/ca
CONFIG = -config $(DIR)/openssl.cnf

#Defaults for new CSR
C = JP
ST = Tokyo
L = Koganei
O = WIDE
OU = "AAA WG"

#Default lifetime
DAYS = 365

#Values for the CA
CA_CN = mgr.testbed.aaa
CA_mail = sdecugis@freediameter.net

#Disable "make destroy"
force = 


# Default: print the help
all:	help

# Help message
help:
	@echo "\n\
Default values (can be overwritten on command-line):\n\
   [C=$(C)] [ST=$(ST)] [L=$(L)] [O=$(O)] [OU=$(OU)]\n\
   [CA_CN=$(CA_CN)] [CA_mail=$(CA_mail)]\n\n\
Available commands:\n\
   make init\n\
       Creates the initial CA structure in $(DIR)\n\
   make gencrl\n\
       Regenerates the CRL. Should be run at least once a month.\n\
   make newcsr name=foo email=b@r [type=ca]\n\
       Create private key and csr in clients subdir (named foo.*)\n\
   make cert name=foo\n\
       Signs the CSR foo.csr and creates the certificate foo.cert.\n\
   make revoke name=foo\n\
       Revokes the certificate foo.cert and regenerates the CRL.\n\
\n\
Notes:\n\
   Content from public-www should be available from Internet. \n\
   The URL to CRL should be set in openssl.cnf.\n\
   A cron job should execute make gencrl once a month.\n\
";

# Destroy the CA completely. Use with care.
destroy:
	@if [ -z "$(force)" ]; then echo "Restart disabled, use: make destroy force=y"; exit 1; fi
	@if [ ! -d $(STATIC_DIR) ]; then echo "Error in setup"; exit 1; fi
	@echo "Removing everything (for debug purpose)..."
	@rm -rf $(DIR)/*
	@ln -sf $(STATIC_DIR)/Makefile $(DIR)
	@ln -sf $(STATIC_DIR)/openssl.cnf $(DIR)

# Initialize the CA structure and keys.
init:
	@if [ -d $(DIR)/private ]; then echo "CA already initialized."; exit 1; fi
	@echo "Creating CA structure..."
	@mkdir $(DIR)/crl
	@mkdir $(DIR)/certs
	@mkdir $(DIR)/newcerts
	@mkdir $(DIR)/public-www
	@mkdir $(DIR)/private
	@chmod 700 $(DIR)/private
	@mkdir $(DIR)/clients
	@mkdir $(DIR)/clients/privkeys
	@mkdir $(DIR)/clients/csr
	@mkdir $(DIR)/clients/certs
	@echo "01" > $(DIR)/serial
	@touch $(DIR)/index.txt
	@openssl req $(CONFIG) -new -batch -x509 -days 3650 -nodes -newkey rsa:2048 -out $(DIR)/public-www/cacert.pem \
		-keyout $(DIR)/private/cakey.pem -subj /C=$(C)/ST=$(ST)/L=$(L)/O=$(O)/OU=$(OU)/CN=$(CA_CN)/emailAddress=$(CA_mail)
	@ln -s $(DIR)/public-www/cacert.pem $(DIR)/certs/`openssl x509 -noout -hash < $(DIR)/public-www/cacert.pem`.0
	@$(MAKE) -f $(DIR)/Makefile gencrl

# Regenerate the Certificate Revocation List.
# This list should be available publicly
gencrl:
	@openssl ca $(CONFIG) -gencrl -out $(DIR)/public-www/crl.pem
	@ln -sf $(DIR)/public-www/crl.pem $(DIR)/crl/`openssl crl -noout -hash < $(DIR)/public-www/crl.pem`.r0

# Create a new private key and a CSR, in case the client does not provide the CSR by another mean.
# Usage is: make newcsr name=peer.client.fqdn email=admin@client.fqdn
newcsr:
	@if [ -z "$(name)" -o -z "$(email)" ]; then echo "Please provide certificate name and email address: make newcsr name=mn.nautilus.org email=you@mail.com"; exit 1; fi
	@if [ -e $(DIR)/clients/csr/$(name).csr ]; then echo "There is already a pending csr for this name."; exit 1; fi
	@if [ ! -e $(DIR)/clients/privkeys/$(name).key.pem ]; \
		then echo "Generating a private key for $(name) ..."; \
		openssl genrsa -out $(DIR)/clients/privkeys/$(name).key.pem 1024; \
		fi;
	@echo "Creating the CSR in $(DIR)/clients/csr/$(name).csr";
	@openssl req $(CONFIG) -new -batch -out $(DIR)/clients/csr/$(name).csr \
		-key $(DIR)/clients/privkeys/$(name).key.pem \
		-subj /C=$(C)/ST=$(ST)/L=$(L)/O=$(O)/OU=$(OU)/CN=$(name)/emailAddress=$(email)

# Process a CSR to create a x509 certificate. The certificate is valid for 1 year. 
# It should be sent to the client by any mean.
cert:
	@if [ -z "$(name)" ]; then echo "name must be provided: make cert name=mn.n6.org"; exit 1; fi
	@if [ ! -e $(DIR)/clients/csr/$(name).csr ]; then echo "Could not find CSR in $(DIR)/clients/csr/$(name).csr."; exit 1; fi
	@if [ -e $(DIR)/clients/certs/$(name).cert ]; \
		then echo "Revoking old certificate..."; \
		$(MAKE) revoke name=$(name); \
		fi;
	@openssl ca $(CONFIG) -in $(DIR)/clients/csr/$(name).csr \
		-out $(DIR)/clients/certs/$(name).cert \
		-days $(DAYS) \
		-batch
	@ln -s $(DIR)/clients/certs/$(name).cert $(DIR)/certs/`openssl x509 -noout -hash < $(DIR)/clients/certs/$(name).cert`.0

# Revoke a certificate.
revoke:
	@if [ -z "$(name)" ]; then echo "name must be provided: make revoke name=mn.n6.org"; exit 1; fi
	@if [ ! -e $(DIR)/clients/certs/$(name).cert ]; \
		then echo "$(DIR)/clients/certs/$(name).cert not found"; \
		exit 1; \
		fi;
	@openssl ca $(CONFIG) -revoke $(DIR)/clients/certs/$(name).cert;
	@rm -f $(DIR)/certs/`openssl x509 -noout -hash < $(DIR)/clients/certs/$(name).cert`.0
	@$(MAKE) gencrl

# End of file...

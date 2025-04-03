#!/bin/bash

# Remove any existing CA setup
rm -rf demoCA
mkdir demoCA
echo 01 > demoCA/serial
touch demoCA/index.txt.attr
touch demoCA/index.txt

# Generate .rnd if it does not exist
openssl rand -out /root/.rnd -hex 256

# Define certificate paths
TLS_DIR="/etc/open5gs/tls"
mkdir -p $TLS_DIR

# Step 1: Generate CA Certificate
echo "Generating CA Certificate..."
openssl req -new -batch -x509 -days 3650 -nodes -newkey rsa:2048 \
    -out $TLS_DIR/cacert.pem -keyout cakey.pem \
    -subj "/CN=ca.epc.mnc230.mcc334.3gppnetwork.org/C=KO/ST=Seoul/L=Nowon/O=Open5GS/OU=Tests"

# Function to generate certificates for network elements
generate_cert() {
    local ENTITY=$1
    local DOMAIN=$2
    echo "Generating certificate for $ENTITY..."

    openssl genrsa -out $TLS_DIR/$ENTITY.key.pem 2048
    openssl req -new -batch -out $ENTITY.csr.pem -key $TLS_DIR/$ENTITY.key.pem \
        -subj "/CN=$ENTITY.$DOMAIN/C=KO/ST=Seoul/L=Nowon/O=Open5GS/OU=Tests"
    openssl ca -cert $TLS_DIR/cacert.pem -days 3650 -keyfile cakey.pem \
        -in $ENTITY.csr.pem -out $TLS_DIR/$ENTITY.cert.pem -outdir . -batch
}

# Step 2: Generate Certificates for MME and SMF
generate_cert "mme" "epc.mnc001.mcc001.3gppnetwork.org"
generate_cert "smf" "epc.mnc001.mcc001.3gppnetwork.org"

# Step 3: Cleanup
rm -rf demoCA
rm -f cakey.pem
rm -f mme.csr.pem smf.csr.pem

echo "Certificates have been generated and stored in $TLS_DIR"
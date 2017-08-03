#!/bin/sh

if [ 1 -ne $# ]
then
    echo You must specify output directory : ./make_cert ../out

    exit;
fi

rm -rf demoCA
mkdir demoCA
echo 01 > demoCA/serial
touch demoCA/index.txt

# CA self certificate
openssl req  -new -batch -x509 -days 3650 -nodes -newkey rsa:1024 -out $1/cacert.pem -keyout cakey.pem -subj /CN=ca.localdomain/C=FR/ST=BdR/L=Aix/O=fD/OU=Tests

#mme
openssl genrsa -out $1/mme.key.pem 1024
openssl req -new -batch -out mme.csr.pem -key $1/mme.key.pem -subj /CN=mme.localdomain/C=FR/ST=BdR/L=Aix/O=fD/OU=Tests
openssl ca -cert $1/cacert.pem -keyfile cakey.pem -in mme.csr.pem -out $1/mme.cert.pem -outdir . -batch

#hss
openssl genrsa -out $1/hss.key.pem 1024
openssl req -new -batch -out hss.csr.pem -key $1/hss.key.pem -subj /CN=hss.localdomain/C=FR/ST=BdR/L=Aix/O=fD/OU=Tests
openssl ca -cert $1/cacert.pem -keyfile cakey.pem -in hss.csr.pem -out $1/hss.cert.pem -outdir . -batch

rm -rf demoCA
rm -f 01.pem 02.pem
rm -f cakey.pem
rm -f hss.csr.pem mme.csr.pem

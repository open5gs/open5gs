#!/bin/sh

if [ 1 -ne $# ]
then
    echo You must specify output directory : ./make_certs.sh ./freeDiameter

    exit;
fi

rm -rf demoCA
mkdir demoCA
echo 01 > demoCA/serial
touch demoCA/index.txt

# CA self certificate
openssl req  -new -batch -x509 -days 3650 -nodes -newkey rsa:1024 -out $1/cacert.pem -keyout cakey.pem -subj /CN=ca.localdomain/C=KO/ST=Seoul/L=Nowon/O=NextEPC/OU=Tests

#mme
openssl genrsa -out $1/mme.key.pem 1024
openssl req -new -batch -out mme.csr.pem -key $1/mme.key.pem -subj /CN=mme.localdomain/C=KO/ST=Seoul/L=Nowon/O=NextEPC/OU=Tests
openssl ca -cert $1/cacert.pem -days 3650 -keyfile cakey.pem -in mme.csr.pem -out $1/mme.cert.pem -outdir . -batch

#hss
openssl genrsa -out $1/hss.key.pem 1024
openssl req -new -batch -out hss.csr.pem -key $1/hss.key.pem -subj /CN=hss.localdomain/C=KO/ST=Seoul/L=Nowon/O=NextEPC/OU=Tests
openssl ca -cert $1/cacert.pem -days 3650 -keyfile cakey.pem -in hss.csr.pem -out $1/hss.cert.pem -outdir . -batch

#pgw
openssl genrsa -out $1/pgw.key.pem 1024
openssl req -new -batch -out pgw.csr.pem -key $1/pgw.key.pem -subj /CN=pgw.localdomain/C=KO/ST=Seoul/L=Nowon/O=NextEPC/OU=Tests
openssl ca -cert $1/cacert.pem -days 3650 -keyfile cakey.pem -in pgw.csr.pem -out $1/pgw.cert.pem -outdir . -batch

#pcrf
openssl genrsa -out $1/pcrf.key.pem 1024
openssl req -new -batch -out pcrf.csr.pem -key $1/pcrf.key.pem -subj /CN=pcrf.localdomain/C=KO/ST=Seoul/L=Nowon/O=NextEPC/OU=Tests
openssl ca -cert $1/cacert.pem -days 3650 -keyfile cakey.pem -in pcrf.csr.pem -out $1/pcrf.cert.pem -outdir . -batch

rm -rf demoCA
rm -f 01.pem 02.pem 03.pem 04.pem
rm -f cakey.pem
rm -f mme.csr.pem hss.csr.pem pgw.csr.pem pcrf.csr.pem

#!/bin/sh

if [ 1 -ne $# ]
then
    echo You must specify output directory : ./make-certs.sh ../config/open5gs/tls
    exit;
fi

rm -rf demoCA
mkdir demoCA
echo 01 > demoCA/serial
touch demoCA/index.txt

# CA self certificate
openssl req -new -x509 -days 3650 -newkey rsa:2048 -nodes -keyout $1/ca.key -out $1/ca.crt \
    -subj /CN=ca.localdomain/C=KO/ST=Seoul/O=NeoPlane

for i in amf ausf bsf hss mme nrf scp sepp1 sepp2 sepp3 nssf pcf pcrf smf udm udr
do
    openssl genpkey -algorithm rsa -pkeyopt rsa_keygen_bits:2048 \
        -out $1/$i.key
    openssl req -new -key $1/$i.key -out $1/$i.csr \
        -subj /CN=$i.localdomain/C=KO/ST=Seoul/O=NeoPlane
    openssl ca -batch -notext -days 3650 \
        -keyfile $1/ca.key -cert $1/ca.crt \
        -in $1/$i.csr -out $1/$i.crt -outdir .
done

rm -rf demoCA
rm -f *.pem

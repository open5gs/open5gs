rm -rf demoCA
mkdir demoCA
echo 01 > demoCA/serial
touch demoCA/index.txt



# CA self certificate
openssl req  -new -batch -x509 -days 3650 -nodes -newkey rsa:1024 -out cacert.pem -keyout cakey.pem -subj /CN=ca.localdomain/C=FR/ST=BdR/L=Aix/O=fD/OU=Tests

#peer1
openssl genrsa -out peer1.key.pem 1024
openssl req -new -batch -out peer1.csr.pem -key peer1.key.pem -subj /CN=peer1.localdomain/C=FR/ST=BdR/L=Aix/O=fD/OU=Tests
openssl ca -cert cacert.pem -keyfile cakey.pem -in peer1.csr.pem -out peer1.cert.pem -outdir . -batch

#peer2
openssl genrsa -out peer2.key.pem 1024
openssl req -new -batch -out peer2.csr.pem -key peer2.key.pem -subj /CN=peer2.localdomain/C=FR/ST=BdR/L=Aix/O=fD/OU=Tests
openssl ca -cert cacert.pem -keyfile cakey.pem -in peer2.csr.pem -out peer2.cert.pem -outdir . -batch


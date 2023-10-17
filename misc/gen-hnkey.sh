#!/bin/sh

if [ $# -lt 1 -o $# -gt 2 ]
then
    echo You must specify output directory : ./gen-hnkey.sh ../config/open5gs/hnet [numberOfKey]
    exit;
fi

limit=3

if [ $# -eq 2 ]
then
    limit=$2
fi

limit=$((limit * 2))

i=1; while [ $i -le $limit ]; do
    openssl genpkey -algorithm X25519 -out $1/curve25519-$i.key
    openssl ecparam -name prime256v1 -genkey -conv_form compressed -out $1/secp256r1-$(($i+1)).key

    i=$(($i + 2))
done

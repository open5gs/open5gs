ARG dist=fedora
ARG tag=latest
FROM ${dist}:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN dnf -y install \
        autoconf \
        libtool \
        gcc \
        make \
        flex \
        bison \
        git \
        lksctp-tools-devel \
        libidn-devel \
        gnutls-devel \
        libgcrypt-devel \
        mongo-c-driver-devel \
        openssl-devel \
        cyrus-sasl-devel \
        snappy-devel \
        libyaml-devel \
        iproute

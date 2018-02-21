ARG dist=centos
ARG tag=latest
FROM ${dist}:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN yum -y install \
        autoconf \
        libtool \
        gcc \
        flex \
        bison \
        git \
        lksctp-tools-devel \
        libidn-devel \
        gnutls-devel \
        libgcrypt-devel \
        openssl-devel \
        cyrus-sasl-devel \
        libyaml-devel \
        iproute

RUN yum -y install epel-release && \
    yum -y install mongo-c-driver-devel

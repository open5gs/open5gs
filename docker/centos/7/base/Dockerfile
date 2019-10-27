ARG dist=centos
ARG tag=latest
FROM ${dist}:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN yum -y install epel-release
RUN yum -y install https://centos7.iuscommunity.org/ius-release.rpm
RUN yum -y update

RUN yum -y install \
        python3 \
        ninja-build \
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
        iproute \
        mongo-c-driver-devel

RUN pip3 install --upgrade pip && pip install meson

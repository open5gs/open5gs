ARG dist=fedora
ARG tag=latest
FROM ${dist}:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN dnf -y install \
        python3 \
        ninja-build \
        gcc \
        gcc-c++ \
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
        mongo-c-driver-devel \
        libmicrohttpd-devel \
        libcurl-devel \
        libnghttp2-devel \
        libtalloc-devel \
        iproute

RUN dnf -y install meson

ARG tag=latest
FROM ubuntu:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN	apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
        autoconf \
        libtool \
        gcc \
        flex \
        bison \
        git \
        debhelper \
        devscripts \
        dh-autoreconf \
        dh-systemd \
        libsctp-dev \
        libgnutls28-dev \
        libgcrypt-dev \
        libssl-dev \
        libmongoc-dev \
        libbson-dev \
        libyaml-dev \
        build-essential \
        ca-certificates \
        netbase \
        ifupdown \
        pkg-config && \
    apt-get clean

COPY setup.sh /root

ARG dist=ubuntu
ARG tag=latest
FROM ${dist}:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
        python3-pip \
        python3-setuptools \
        ninja-build \
        build-essential \
        flex \
        bison \
        git \
        libsctp-dev \
        libgnutls28-dev \
        libgcrypt-dev \
        libssl-dev \
        libidn11-dev \
        libmongoc-dev \
        libbson-dev \
        libyaml-dev \
        iproute2 \
        ca-certificates \
        netbase \
        pkg-config && \
    apt-get clean
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install meson

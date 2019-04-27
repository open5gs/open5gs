ARG dist=ubuntu
ARG tag=latest
FROM ${dist}:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN	apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
        autoconf \
        automake \
        libtool \
        gcc \
        flex \
        bison \
        git \
        libsctp-dev \
        libgnutls28-dev \
        libgcrypt-dev \
        libssl-dev \
        libidn11-dev \
        libyaml-dev \
        build-essential \
        iproute2 \
        ca-certificates \
        netbase \
        curl \
        pkg-config && \
    apt-get clean

WORKDIR /root
RUN curl -SLO "https://github.com/mongodb/mongo-c-driver/releases/download/1.9.2/mongo-c-driver-1.9.2.tar.gz"; \
    tar xzf mongo-c-driver-1.9.2.tar.gz; \
    cd mongo-c-driver-1.9.2; \
    ./configure --disable-automatic-init-and-cleanup; \
    make -j `nproc` install; \
    ldconfig;

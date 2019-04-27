ARG dist=debian
ARG tag=stable
FROM ${dist}:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

ARG tag=stable
RUN apt-get update && \
    apt-get install -y netselect-apt && \
    apt-get clean && \
    if [ "x$tag" = "xlatest" ]; then \
        netselect-apt -o /etc/apt/sources.list; \
    else \
        netselect-apt ${tag} -o /etc/apt/sources.list; \
    fi

RUN apt-get update && \
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
        libmongoc-dev \
        libbson-dev \
        libyaml-dev \
        build-essential \
        iproute2 \
        ca-certificates \
        netbase \
        pkg-config && \
    apt-get clean

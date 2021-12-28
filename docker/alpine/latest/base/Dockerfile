ARG dist=alpine
ARG tag=latest
FROM ${dist}:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN apk update && \
    apk add --no-cache \
        alpine-sdk \
        bison \
        flex \
        git \
        meson \
        bash \
        linux-headers \
        bsd-compat-headers \
        yaml-dev \
        lksctp-tools-dev \
        gnutls-dev \
        libgcrypt-dev \
        libidn-dev \
        mongo-c-driver-dev \
        libmicrohttpd-dev \
        curl-dev \
        nghttp2-dev \
        talloc-dev \
        iproute2

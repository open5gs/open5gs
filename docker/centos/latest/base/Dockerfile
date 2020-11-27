ARG dist=centos
ARG tag=latest
FROM ${dist}:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN dnf -y install epel-release && \
        dnf -y install 'dnf-command(config-manager)' && \
        dnf config-manager --set-enabled PowerTools && \
        dnf -y update

RUN dnf -y install \
        https://www.elrepo.org/elrepo-release-8.el8.elrepo.noarch.rpm && \
        dnf config-manager --set-enabled elrepo-testing && \
        dnf -y update

RUN dnf -y install \
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
        mongo-c-driver-devel \
        libmicrohttpd-devel \
        libcurl-devel \
        libnghttp2-devel \
        iproute

RUN dnf -y install meson

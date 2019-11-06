ARG dist=centos
ARG tag=latest
FROM ${dist}:${tag}

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN dnf -y install epel-release && \
        dnf -y install 'dnf-command(config-manager)' && \
        dnf config-manager --set-enabled PowerTools && \
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
        iproute \
        mongo-c-driver-devel

RUN pip3 install --upgrade pip && pip install meson

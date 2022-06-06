ARG dist=ubuntu
ARG tag=latest
ARG username=acetcom
FROM ${username}/${dist}-${tag}-open5gs-base

MAINTAINER Sukchan Lee <acetcom@gmail.com>

WORKDIR /open5gs
COPY docker/build/setup.sh /root
COPY ./ /open5gs

RUN meson build && ninja -C build install

ARG dist=ubuntu
ARG tag=latest
ARG username=acetcom
FROM ${username}/${dist}-${tag}-base

MAINTAINER Sukchan Lee <acetcom@gmail.com>

WORKDIR /tmp

RUN	git clone https://github.com/acetcom/nextepc
RUN	cd nextepc && \
    autoreconf -iv && \
    ./configure && \
    make -j `nproc` && \
    make install

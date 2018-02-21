ARG dist=ubuntu
ARG tag=latest
ARG username=acetcom
FROM ${username}/${dist}-${tag}-base

MAINTAINER Sukchan Lee <acetcom@gmail.com>

WORKDIR /tmp

ARG USER=acetcom
ARG REPO=nextepc
ARG BRANCH=master
RUN	git clone https://github.com/$USER/$REPO
ADD https://api.github.com/repos/$USER/$REPO/git/refs/heads/$BRANCH /tmp/version.json

RUN	cd nextepc && \
	git fetch && git checkout -f -B master origin/master && \
    autoreconf -f -i && \
    ./configure \
        --prefix=/usr \
        --sysconfdir=/etc \
        --localstatedir=/var && \
    make -j `nproc` install

WORKDIR /root

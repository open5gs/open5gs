ARG dist=ubuntu
ARG tag=latest
ARG username=acetcom
FROM ${username}/${dist}-${tag}-open5gs-base

MAINTAINER Sukchan Lee <acetcom@gmail.com>

WORKDIR /root
COPY setup.sh /root

ARG USER=open5gs
ARG REPO=open5gs
ARG BRANCH=main
RUN git clone https://github.com/$USER/$REPO
ADD https://api.github.com/repos/$USER/$REPO/git/refs/heads/$BRANCH /root/open5gs-ver.json

RUN cd $REPO && \
    git fetch && git checkout -f -B $BRANCH origin/$BRANCH && \
    meson build && ninja -C build install

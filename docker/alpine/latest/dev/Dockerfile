ARG dist=alpine
ARG tag=latest
ARG username=acetcom
FROM ${username}/${dist}-${tag}-open5gs-base

MAINTAINER Sukchan Lee <acetcom@gmail.com>

COPY setup.sh /root

RUN apk update && \
    apk add --no-cache \
        vim \
        sudo

ARG username=acetcom
RUN adduser -u 1000 acetcom -D && \
    echo "${username} ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/${username} && \
    chmod 0440 /etc/sudoers.d/${username}

WORKDIR /home/${username}

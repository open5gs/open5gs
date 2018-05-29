ARG dist=ubuntu
ARG tag=latest
ARG username=acetcom
FROM ${username}/${dist}-${tag}-nextepc-base

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
        debhelper \
        devscripts \
        dput \
        dh-autoreconf \
        dh-systemd \
        manpages-dev \
        fakeroot \
        cscope \
        vim \
        sudo \
        iputils-ping \
        net-tools && \
    apt-get clean

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y wireshark

COPY setup.sh /root

ARG username=acetcom
RUN useradd -m --uid=1000 ${username} && \
    echo "${username} ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/${username} && \
    chmod 0440 /etc/sudoers.d/${username}

WORKDIR /home/${username}

ARG dist=ubuntu
ARG tag=latest
ARG username=acetcom
FROM ${username}/${dist}-${tag}-open5gs-base

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN apt-get update && \
    apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        git-buildpackage \
        debhelper \
        devscripts \
        osc \
        dput \
        manpages-dev \
        fakeroot \
        cscope \
        vim \
        sudo \
        wireshark \
        iputils-ping \
        net-tools && \
    apt-get clean

COPY setup.sh /root

ARG username=acetcom
RUN useradd -m --uid=1000 ${username} && \
    echo "${username} ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/${username} && \
    chmod 0440 /etc/sudoers.d/${username}

WORKDIR /home/${username}

ARG tag=latest
ARG username=acetcom
FROM ${username}/ubuntu-${tag}-base

MAINTAINER Sukchan Lee <acetcom@gmail.com>

RUN	apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
        cscope \
        vim \
        sudo \
        iproute2 \
        iputils-ping \
        net-tools && \
    apt-get clean

ARG username=acetcom
RUN	useradd -m --uid=1000 ${username} && \
    echo "${username} ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/${username} && \
    chmod 0440 /etc/sudoers.d/${username}

WORKDIR /home/${username}

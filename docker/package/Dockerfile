ARG tag=latest
FROM ubuntu:${tag}

RUN apt-get update && \
    apt-get -y install software-properties-common && \
    add-apt-repository ppa:acetcom/open5gs && \
    apt-get update && \
    apt-get install -y open5gs

WORKDIR /root

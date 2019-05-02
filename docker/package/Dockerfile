ARG tag=latest
FROM ubuntu:${tag}

RUN apt-get update && \
    apt-get -y install software-properties-common && \
    add-apt-repository ppa:acetcom/nextepc && \
    apt-get update && \
    apt-get install -y nextepc

WORKDIR /root

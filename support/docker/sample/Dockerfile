FROM ubuntu:16.04

RUN \
  apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv F26B05AF && \
  echo "deb http://ppa.launchpad.net/acetcom/nextepc/ubuntu xenial main" > /etc/apt/sources.list.d/nextepc.list && \
  apt-get update && \
  apt-get install -y nextepc

WORKDIR /root

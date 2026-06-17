FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        python3-pip python3-setuptools python3-wheel \
        ninja-build build-essential flex bison git cmake meson \
        libsctp-dev libgnutls28-dev libgcrypt-dev libssl-dev \
        libmongoc-dev libbson-dev libyaml-dev libmicrohttpd-dev \
        libcurl4-gnutls-dev libnghttp2-dev libtins-dev libtalloc-dev \
        iproute2 iputils-ping tcpdump curl ca-certificates netbase pkg-config \
        wondershaper procps net-tools iptables iperf3 traceroute && \
    if apt-cache show libidn-dev >/dev/null 2>&1; then \
        apt-get install -y --no-install-recommends libidn-dev; \
    else \
        apt-get install -y --no-install-recommends libidn11-dev; \
    fi && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /open5gs
COPY . /open5gs

RUN test -f subprojects/prometheus-client-c/README.md || \
    printf 'prometheus-client-c\n' > subprojects/prometheus-client-c/README.md

RUN rm -rf build && \
    meson setup build --prefix=/open5gs/install --buildtype=release && \
    ninja -C build install && \
    mkdir -p /var/log/open5gs /opt/open5gs/bin

WORKDIR /open5gs

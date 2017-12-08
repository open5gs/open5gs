#!/bin/sh

SYSTEM=`uname`;

if [ "$SYSTEM" = "Linux" ]; then
    if ! grep "pgwtun" /proc/net/dev > /dev/null; then
        ip tuntap add name pgwtun mode tun
    fi
    ip addr add 45.45.0.1/16 dev pgwtun
    ip link set pgwtun up
    ip addr add fec0::2 dev lo
    ip addr add fec0::3 dev lo
    ip addr add fec0::4 dev lo
    ip addr add fec0::5 dev lo
else
    ifconfig lo0 alias 127.0.0.2 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.3 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.4 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.5 netmask 255.255.255.255
    ifconfig lo0 inet6 fec0::2 prefixlen 128
    ifconfig lo0 inet6 fec0::3 prefixlen 128
    ifconfig lo0 inet6 fec0::4 prefixlen 128
    ifconfig lo0 inet6 fec0::5 prefixlen 128
fi

#!/bin/sh

SYSTEM=`uname`;

if [ "$SYSTEM" = "Linux" ]; then
    if ! grep "pgwtun" /proc/net/dev > /dev/null; then
        ip tuntap add name pgwtun mode tun
        ip addr add 45.45.0.1/16 dev pgwtun
        ip link set pgwtun up
    fi
else
    ifconfig lo0 alias 127.76.0.1 netmask 255.255.255.255
    ifconfig lo0 alias 127.76.0.2 netmask 255.255.255.255
    ifconfig lo0 alias 127.76.0.3 netmask 255.255.255.255
    ifconfig lo0 alias 127.76.0.4 netmask 255.255.255.255
    ifconfig lo0 alias 127.76.0.5 netmask 255.255.255.255
fi

#!/bin/sh

SYSTEM=`uname`;

if [ "$SYSTEM" = "Linux" ]; then
    if ! grep "ogstun" /proc/net/dev > /dev/null; then
        ip tuntap add name ogstun mode tun
    fi
    ip addr del 10.45.0.1/16 dev ogstun 2> /dev/null
    ip addr add 10.45.0.1/16 dev ogstun
    ip addr del cafe::1/64 dev ogstun 2> /dev/null
    ip addr add cafe::1/64 dev ogstun
    ip link set ogstun up
    ip addr del fe80::2 dev lo 2> /dev/null
    ip addr del fe80::3 dev lo 2> /dev/null
    ip addr del fe80::4 dev lo 2> /dev/null
    ip addr del fe80::5 dev lo 2> /dev/null
    ip addr add fe80::2 dev lo
    ip addr add fe80::3 dev lo
    ip addr add fe80::4 dev lo
    ip addr add fe80::5 dev lo
else
    ifconfig lo0 alias 127.0.0.2 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.3 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.4 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.5 netmask 255.255.255.255
    ifconfig lo0 inet6 delete fe80::2 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fe80::3 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fe80::4 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fe80::5 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 add fe80::2 prefixlen 128
    ifconfig lo0 inet6 add fe80::3 prefixlen 128
    ifconfig lo0 inet6 add fe80::4 prefixlen 128
    ifconfig lo0 inet6 add fe80::5 prefixlen 128
fi

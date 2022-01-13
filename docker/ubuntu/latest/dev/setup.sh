#!/bin/sh

if ! grep "ogstun" /proc/net/dev > /dev/null; then
    ip tuntap add name ogstun mode tun
fi
ip addr del 10.45.0.1/16 dev ogstun 2> /dev/null
ip addr add 10.45.0.1/16 dev ogstun
ip addr del 2001:db8:cafe::1/48 dev ogstun 2> /dev/null
ip addr add 2001:db8:cafe::1/48 dev ogstun
ip link set ogstun up

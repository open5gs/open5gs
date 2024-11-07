#!/bin/sh

SYSTEM=`uname`;

if [ "$SYSTEM" = "Linux" ]; then
    if ! grep "ogstun" /proc/net/dev > /dev/null; then
        ip tuntap add name ogstun mode tun
    fi
    if test "x`sysctl -n net.ipv6.conf.ogstun.disable_ipv6`" = x1; then
        echo "net.ipv6.conf.ogstun.disable_ipv6=0" > /etc/sysctl.d/30-open5gs.conf
        sysctl -p /etc/sysctl.d/30-open5gs.conf
    fi
    ip addr del 10.45.0.1/16 dev ogstun 2> /dev/null
    ip addr add 10.45.0.1/16 dev ogstun
    ip addr del 2001:db8:cafe::1/48 dev ogstun 2> /dev/null
    ip addr add 2001:db8:cafe::1/48 dev ogstun
    ip addr del 10.46.0.1/16 dev ogstun2 2> /dev/null
    ip addr add 10.46.0.1/16 dev ogstun2
    ip addr del 2001:db8:babe::1/48 dev ogstun2 2> /dev/null
    ip addr add 2001:db8:babe::1/48 dev ogstun2
    ip addr del 10.47.0.1/16 dev ogstun3 2> /dev/null
    ip addr add 10.47.0.1/16 dev ogstun3
    ip addr del 2001:db8:face::1/48 dev ogstun3 2> /dev/null
    ip addr add 2001:db8:face::1/48 dev ogstun3
    ip link set ogstun up
else
    sysctl -w net.inet.ip.forwarding=1
    sysctl -w net.inet6.ip6.forwarding=1
    ifconfig lo0 alias 127.0.0.2 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.3 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.4 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.5 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.6 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.7 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.8 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.9 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.10 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.11 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.12 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.13 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.14 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.15 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.16 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.17 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.18 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.19 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.20 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.50 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.200 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.201 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.202 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.250 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.251 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.252 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.2 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.3 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.4 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.5 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.6 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.7 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.8 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.9 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.10 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.11 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.12 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.13 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.14 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.15 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.16 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.17 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.18 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.19 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.20 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.50 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.200 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.201 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.202 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.250 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.251 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.1.252 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.2 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.3 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.4 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.5 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.6 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.7 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.8 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.9 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.10 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.11 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.12 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.13 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.14 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.15 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.16 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.17 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.18 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.19 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.20 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.50 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.200 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.201 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.202 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.250 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.251 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.2.252 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.2 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.3 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.4 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.5 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.6 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.7 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.8 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.9 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.10 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.11 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.12 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.13 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.14 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.15 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.16 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.17 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.18 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.19 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.20 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.50 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.200 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.201 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.202 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.250 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.251 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.252 netmask 255.255.255.255
    if [ "$SYSTEM" = "Darwin" ]; then
        if ! test -f /etc/pf.anchors/org.open5gs; then
            sudo sh -c "echo 'nat on {en0} from 10.45.0.0/16 to any -> {en0}' > /etc/pf.anchors/org.open5gs"
            sudo sh -c "echo 'nat on {en0} from 2001:db8:cafe::1/48 to any -> {en0}' > /etc/pf.anchors/org.open5gs"
            sudo sh -c "echo 'nat on {en0} from 10.46.0.0/16 to any -> {en0}' > /etc/pf.anchors/org.open5gs"
            sudo sh -c "echo 'nat on {en0} from 2001:db8:babe::1/48 to any -> {en0}' > /etc/pf.anchors/org.open5gs"
            sudo sh -c "echo 'nat on {en0} from 10.47.0.0/16 to any -> {en0}' > /etc/pf.anchors/org.open5gs"
            sudo sh -c "echo 'nat on {en0} from 2001:db8:face::1/48 to any -> {en0}' > /etc/pf.anchors/org.open5gs"
        fi
        pfctl -e -f /etc/pf.anchors/org.open5gs
    fi
fi

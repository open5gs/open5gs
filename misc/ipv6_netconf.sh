#!/bin/sh

SYSTEM=`uname`;

if [ "$SYSTEM" = "Linux" ]; then
    if ! grep "ogstun" /proc/net/dev > /dev/null; then
        ip tuntap add name ogstun mode tun
    fi
    ip addr del 10.45.0.1/16 dev ogstun 2> /dev/null
    ip addr add 10.45.0.1/16 dev ogstun
    ip addr del 2001:db8:cafe::1/48 dev ogstun 2> /dev/null
    ip addr add 2001:db8:cafe::1/48 dev ogstun
    ip link set ogstun up
    ip addr del fd69:f21d:873c:fa::1 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::2 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::3 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::4 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::5 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::6 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::7 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::8 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::9 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::10 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::11 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::12 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::13 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::14 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::15 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::16 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::17 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::18 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::19 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::20 dev lo 2> /dev/null
    ip addr add fd69:f21d:873c:fa::1 dev lo
    ip addr add fd69:f21d:873c:fa::2 dev lo
    ip addr add fd69:f21d:873c:fa::3 dev lo
    ip addr add fd69:f21d:873c:fa::4 dev lo
    ip addr add fd69:f21d:873c:fa::5 dev lo
    ip addr add fd69:f21d:873c:fa::6 dev lo
    ip addr add fd69:f21d:873c:fa::7 dev lo
    ip addr add fd69:f21d:873c:fa::8 dev lo
    ip addr add fd69:f21d:873c:fa::9 dev lo
    ip addr add fd69:f21d:873c:fa::10 dev lo
    ip addr add fd69:f21d:873c:fa::11 dev lo
    ip addr add fd69:f21d:873c:fa::12 dev lo
    ip addr add fd69:f21d:873c:fa::13 dev lo
    ip addr add fd69:f21d:873c:fa::14 dev lo
    ip addr add fd69:f21d:873c:fa::15 dev lo
    ip addr add fd69:f21d:873c:fa::16 dev lo
    ip addr add fd69:f21d:873c:fa::17 dev lo
    ip addr add fd69:f21d:873c:fa::18 dev lo
    ip addr add fd69:f21d:873c:fa::19 dev lo
    ip addr add fd69:f21d:873c:fa::20 dev lo
else
    ifconfig lo0 alias 127.0.0.2 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.3 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.4 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.5 netmask 255.255.255.255
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
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::1 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::2 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::3 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::4 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::5 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::6 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::7 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::8 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::9 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::10 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::11 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::12 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::13 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::14 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::15 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::16 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::17 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::18 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::19 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::20 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::1 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::2 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::3 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::4 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::5 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::6 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::7 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::8 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::9 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::10 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::11 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::12 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::13 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::14 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::15 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::16 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::17 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::18 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::19 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::20 prefixlen 128
    if [ "$SYSTEM" = "Darwin" ]; then
        if ! test -f /etc/pf.anchors/org.open5gs; then
            sudo sh -c "echo 'nat on {en0} from 10.45.0.0/16 to any -> {en0}' > /etc/pf.anchors/org.open5gs"
            sudo sh -c "echo 'nat on {en0} from 2001:db8:cafe::1/48 to any -> {en0}' > /etc/pf.anchors/org.open5gs"
        fi
        pfctl -e -f /etc/pf.anchors/org.open5gs
    fi
fi

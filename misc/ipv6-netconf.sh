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
    ip addr del 10.46.0.1/16 dev ogstun2 2> /dev/null
    ip addr add 10.46.0.1/16 dev ogstun2
    ip addr del 2001:db8:babe::1/48 dev ogstun2 2> /dev/null
    ip addr add 2001:db8:babe::1/48 dev ogstun2
    ip addr del 10.47.0.1/16 dev ogstun3 2> /dev/null
    ip addr add 10.47.0.1/16 dev ogstun3
    ip addr del 2001:db8:face::1/48 dev ogstun3 2> /dev/null
    ip addr add 2001:db8:face::1/48 dev ogstun3
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
    ip addr del fd69:f21d:873c:fa::200 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::201 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::202 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::250 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::251 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fa::252 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::1 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::2 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::3 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::4 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::5 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::6 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::7 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::8 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::9 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::10 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::11 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::12 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::13 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::14 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::15 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::16 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::17 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::18 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::19 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::20 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::200 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::201 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::202 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::250 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::251 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fb::252 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::1 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::2 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::3 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::4 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::5 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::6 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::7 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::8 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::9 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::10 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::11 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::12 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::13 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::14 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::15 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::16 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::17 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::18 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::19 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::20 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::200 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::201 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::202 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::250 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::251 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fc::252 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::1 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::2 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::3 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::4 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::5 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::6 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::7 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::8 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::9 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::10 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::11 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::12 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::13 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::14 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::15 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::16 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::17 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::18 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::19 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::20 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::200 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::201 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::202 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::250 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::251 dev lo 2> /dev/null
    ip addr del fd69:f21d:873c:fd::252 dev lo 2> /dev/null
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
    ifconfig lo0 alias 127.0.3.200 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.201 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.202 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.250 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.251 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.3.252 netmask 255.255.255.255
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
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::200 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::201 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::202 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::250 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::251 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fa::252 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::1 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::2 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::3 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::4 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::5 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::6 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::7 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::8 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::9 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::10 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::11 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::12 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::13 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::14 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::15 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::16 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::17 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::18 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::19 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::20 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::200 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::201 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::202 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::250 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::251 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fb::252 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::1 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::2 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::3 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::4 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::5 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::6 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::7 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::8 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::9 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::10 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::11 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::12 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::13 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::14 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::15 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::16 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::17 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::18 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::19 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::20 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::200 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::201 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::202 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::250 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::251 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fc::252 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::1 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::2 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::3 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::4 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::5 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::6 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::7 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::8 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::9 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::10 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::11 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::12 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::13 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::14 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::15 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::16 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::17 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::18 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::19 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::20 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::200 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::201 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::202 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::250 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::251 prefixlen 128 2> /dev/null
    ifconfig lo0 inet6 delete fd69:f21d:873c:fd::252 prefixlen 128 2> /dev/null
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
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::200 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::201 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::202 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::250 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::251 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fa::252 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::1 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::2 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::3 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::4 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::5 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::6 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::7 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::8 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::9 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::10 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::11 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::12 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::13 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::14 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::15 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::16 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::17 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::18 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::19 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::20 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::200 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::201 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::202 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::250 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::251 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fb::252 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::1 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::2 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::3 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::4 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::5 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::6 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::7 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::8 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::9 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::10 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::11 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::12 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::13 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::14 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::15 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::16 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::17 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::18 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::19 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::20 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::200 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::201 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::202 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::250 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::251 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fc::252 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::1 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::2 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::3 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::4 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::5 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::6 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::7 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::8 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::9 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::10 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::11 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::12 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::13 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::14 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::15 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::16 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::17 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::18 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::19 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::20 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::200 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::201 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::202 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::250 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::251 prefixlen 128
    ifconfig lo0 inet6 add fd69:f21d:873c:fd::252 prefixlen 128
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

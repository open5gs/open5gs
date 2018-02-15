#!/bin/sh

SYSTEM=`uname`;

if [ "$SYSTEM" = "Linux" ]; then
    if ! grep "pgwtun" /proc/net/dev > /dev/null; then
        ip tuntap add name pgwtun mode tun
    fi
	if test "x`sysctl -n net.ipv6.conf.pgwtun.disable_ipv6`" = x1; then
		echo "net.ipv6.conf.pgwtun.disable_ipv6=0" > /etc/sysctl.d/30-nextepc.conf
		sysctl -p /etc/sysctl.d/30-nextepc.conf
	fi
    ip addr del 45.45.0.1/16 dev pgwtun 2> /dev/null
    ip addr add 45.45.0.1/16 dev pgwtun
    ip addr del cafe::1/64 dev pgwtun 2> /dev/null
    ip addr add cafe::1/64 dev pgwtun
    ip link set pgwtun up
else
    sysctl -w net.inet.ip.forwarding=1
    ifconfig lo0 alias 127.0.0.2 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.3 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.4 netmask 255.255.255.255
    ifconfig lo0 alias 127.0.0.5 netmask 255.255.255.255
    if [ "$SYSTEM" = "Darwin" ]; then
        if ! test -f /etc/pf.anchors/org.nextepc; then
            sudo sh -c "echo 'nat on {en0} from 45.45.0.0/16 to any -> {en0}' > /etc/pf.anchors/org.nextepc"
        fi
        pfctl -e -f /etc/pf.anchors/org.nextepc
    fi
fi

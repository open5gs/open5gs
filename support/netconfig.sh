#!/bin/sh

SYSTEM=`uname`;

if [ "$SYSTEM" = "Linux" ]; then
    ifconfig lo:mme 127.76.0.1 netmask 255.255.255.255 up
    ifconfig lo:sgw 127.76.0.2 netmask 255.255.255.255 up
    ifconfig lo:pgw 127.76.0.3 netmask 255.255.255.255 up
    ifconfig lo:hss 127.76.0.4 netmask 255.255.255.255 up
    ifconfig lo:pcrf 127.76.0.5 netmask 255.255.255.255 up
else
    ifconfig lo0 alias 127.76.0.1 netmask 255.255.255.255
    ifconfig lo0 alias 127.76.0.2 netmask 255.255.255.255
    ifconfig lo0 alias 127.76.0.3 netmask 255.255.255.255
    ifconfig lo0 alias 127.76.0.4 netmask 255.255.255.255
    ifconfig lo0 alias 127.76.0.5 netmask 255.255.255.255
fi

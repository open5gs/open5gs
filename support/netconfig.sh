#!/bin/sh

if [ 1 -ne $# ]
then
    echo You must specify interface name : ./netconfig.sh eth0

    exit;
fi

SYSTEM=`uname`;

if [ "$SYSTEM" = "Linux" ]; then
    ifconfig $1:hss 10.1.35.214/32 up
    ifconfig $1:mme 10.1.35.215/32 up
    ifconfig $1:sgw 10.1.35.216/32 up
    ifconfig $1:pcrf 10.1.35.218/32 up
    ifconfig $1:pgw 10.1.35.219/32 up
else
    ifconfig $1 alias 10.1.35.214 netmask 255.255.255.255
    ifconfig $1 alias 10.1.35.215 netmask 255.255.255.255
    ifconfig $1 alias 10.1.35.216 netmask 255.255.255.255
    ifconfig $1 alias 10.1.35.218 netmask 255.255.255.255
    ifconfig $1 alias 10.1.35.219 netmask 255.255.255.255
fi

#!/bin/sh

SYSTEM=`uname`;

if [ "$SYSTEM" = "Linux" ]; then
    ifconfig lo:hss 10.1.35.214 netmask 255.255.255.255 up
    ifconfig lo:mme 10.1.35.215 netmask 255.255.255.255 up
    ifconfig lo:sgw 10.1.35.216 netmask 255.255.255.255 up
    ifconfig lo:pcrf 10.1.35.218 netmask 255.255.255.255 up
    ifconfig lo:pgw 10.1.35.219 netmask 255.255.255.255 up
elif [ "$SYSTEM" = "Darwin" ]; then
    ifconfig lo0 alias 10.1.35.214 netmask 255.255.255.255
    ifconfig lo0 alias 10.1.35.215 netmask 255.255.255.255
    ifconfig lo0 alias 10.1.35.216 netmask 255.255.255.255
    ifconfig lo0 alias 10.1.35.218 netmask 255.255.255.255
    ifconfig lo0 alias 10.1.35.219 netmask 255.255.255.255
else
    if [ 1 -ne $# ]
    then
        echo You must specify output directory : ./netconfig.sh em0

        exit;
    fi
    ifconfig $1 alias 10.1.35.214 netmask 255.255.255.255
    ifconfig $1 alias 10.1.35.215 netmask 255.255.255.255
    ifconfig $1 alias 10.1.35.216 netmask 255.255.255.255
    ifconfig $1 alias 10.1.35.218 netmask 255.255.255.255
    ifconfig $1 alias 10.1.35.219 netmask 255.255.255.255
fi

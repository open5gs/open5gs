#!/bin/sh

ifconfig eth1:hss 10.1.35.214/24 up
ifconfig eth1:mme 10.1.35.215/24 up
ifconfig eth1:sgw 10.1.35.216/24 up
ifconfig eth1:pcrf 10.1.35.218/24 up
ifconfig eth1:pgw 10.1.35.219/24 up
ifconfig pgwtun 45.45.0.1/16 up


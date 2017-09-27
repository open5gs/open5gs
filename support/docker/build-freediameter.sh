#!/bin/sh
mkdir -p /root/build-freediameter/freeDiameter
wget -q -O - http://www.freediameter.net/hg/freeDiameter/archive/2cb8d71a405d.tar.bz2 | tar xj -C build-freediameter/freeDiameter --strip-components=1
cd /root/build-freediameter/freeDiameter
ln -s contrib/debian .
fakeroot dh binary

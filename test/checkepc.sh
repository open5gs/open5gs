#!/bin/sh

mkdir -p /tmp/nextepc/etc/freeDiameter /tmp/nextepc/var/log
sed 's?@SYSCONF_DIR@?/tmp/nextepc/etc?g;s?@LOCALSTATE_DIR@?/tmp/nextepc/var?g' `pwd`/../support/nextepc.conf.in > /tmp/nextepc/etc/nextepc.conf
for i in `cd ../support/freeDiameter && find . -name '*.conf.in' ! -name 'dict_legacy_xml.conf.in' -printf "%f\n" | sed 's/.in$//g'`
do
sed 's?@SYSCONF_DIR@?'`pwd`'/../support?g;s?@LIB_DIR@/nextepc/\([_a-z0-9]*\)\.so?'`pwd`'/../lib/fd/extensions/\1/.libs/\1.so?g' `pwd`/../support/freeDiameter/$i.in > /tmp/nextepc/etc/freeDiameter/$i
done

./testepc -f /tmp/nextepc/etc/nextepc.conf || exit;

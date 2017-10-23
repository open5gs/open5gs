#!/bin/sh

mkdir -p `pwd`/../test/etc/freeDiameter `pwd`/../test/var/log
sed 's?@SYSCONF_DIR@?'`pwd`'/etc?g;s?@LOCALSTATE_DIR@?'`pwd`'/var?g' `pwd`/../support/nextepc.conf.in > `pwd`/etc/nextepc.conf
for i in `cd ../support/freeDiameter && find . -name '*.conf.in' ! -name 'dict_legacy_xml.conf.in' -printf "%f\n" | sed 's/.in$//g'`
do
sed 's?@SYSCONF_DIR@?'`pwd`'/../support?g;s?@LIB_DIR@/nextepc/\([_a-z0-9]*\)\.so?'`pwd`'/../lib/fd/extensions/\1/.libs/\1.so?g' `pwd`/../support/freeDiameter/$i.in > `pwd`/etc/freeDiameter/$i
done

./testepc -f ./etc/nextepc.conf || exit;

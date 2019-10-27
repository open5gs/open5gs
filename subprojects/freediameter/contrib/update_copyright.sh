#!/bin/bash -x

# This script will search all copyrights dates from source files, and update these if 
# the file has been modified at a later date.

if [ ! -f include/freeDiameter/libfdcore.h ];
then echo "This script must be run from the source top directory"
exit 1;
fi;

# Create a clean working copy
TMPDIR=`mktemp -d up_cop.XXXXXXX` || exit 1
hg clone . $TMPDIR/fD || exit 1
pushd $TMPDIR/fD

# Now, for each file with a copyright
for SRC_FILE in `find . -name .hg -prune -or -type f -exec grep -q 'Copyright (c) 20.., WIDE Project and NICT' {} \; -print`;
do 
HG_YEAR=`hg log --template '{date|shortdate}' $SRC_FILE | tr - ' ' | awk '{print \$1}'`
CPY_YEAR=`grep 'Copyright (c) 20.., WIDE Project and NICT' $SRC_FILE | awk '{print substr(\$4, 1, 4) }'`
if [ $HG_YEAR -gt $CPY_YEAR ];
then
echo "Updating copyright $CPY_YEAR -> $HG_YEAR in $SRC_FILE";
sed -i -e "s/Copyright (c) $CPY_YEAR, WIDE Project and NICT/Copyright (c) $HG_YEAR, WIDE Project and NICT/" $SRC_FILE
fi;
done

hg commit -m"Updated copyright information"
hg push
popd
rm -rf $TMPDIR
hg update


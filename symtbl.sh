#!/bin/sh

SYMTBL_TMP="symtbl.tmp"

grep -e " B " -e " C " -e " D " -e " T " $1 | grep -v "\." > $SYMTBL_TMP

echo '/* Symbol table generated automatically by symtbl.sh */' > $2
echo '#include "symtbl.h"' >> $2
awk '/ [BCD] / { print "extern int "$3";"}' $SYMTBL_TMP >> $2
awk '/ T / { print "extern int "$3"();"}' $SYMTBL_TMP >> $2
echo "symbol_tbl_t st[] = {" >> $2
awk '/ B / { print "{ST_BSS,(void*)&"$3",\""$3"\"}," }' $SYMTBL_TMP >> $2
awk '/ C / { print "{ST_COMMON,(void*)&"$3",\""$3"\"}," }' $SYMTBL_TMP >> $2
awk '/ D / { print "{ST_DATA,(void*)&"$3",\""$3"\"}," }' $SYMTBL_TMP >> $2
awk '/ T / { print "{ST_TEXT,(void*)&"$3",\""$3"\"}," }' $SYMTBL_TMP >> $2
echo "{0,NULL,NULL}};" >> $2
echo "char g_compile_time[] = __DATE__ \" \" __TIME__;" >> $2

rm -f $SYMTBL_TMP

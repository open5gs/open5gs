Modify 36413-e40.txt to 36413-e40.asn
===========================================
user@host ~/Documents/git/nextepc/lib/s1ap/support/r14.4.0$ \
    diff 36413-e40.txt 36413-e40.asn

Use brchiu git's fork for asn1c
===========================================
user@host ~/Documents/git$ \
    git clone https://github.com/velichkov/asn1c.git asn1c.velichkov
user@host ~/Documents/git$ \
    git checkout s1ap
user@host Documents/git/asn1c$ \
    autoreconf -fi;./configure;make

ASN.1 encoder/decoder
===========================================
user@host ~/Documents/git/nextepc/lib/s1ap/asn1c$ \
    ASN1C_PREFIX=S1AP_ ../../../../asn1c.velichkov/asn1c/asn1c -pdu=all \
    -fcompound-names -findirect-choice -fno-include-deps \
    ../support/r14.4.0/36413-e40.asn

Update Files
===========================================
user@host ~/Documents/git/nextepc/lib/s1ap/s1ap/asn1c$ \
    diff Makefile.am Makefile.am.libasncodec

user@host ~/Documents/git/nextepc/lib/s1ap/asn1c$ \
    git diff asn_internal.h

Remove unnecessary files
===========================================
user@host ~/Documents/git/nextepc/lib/s1ap/s1ap/asn1c$ \
    rm -f S1AP_E-RAB-IE-ContainerPairList.c \
    S1AP_ProtocolError-IE-ContainerList.c \
    S1AP_ProtocolIE-ContainerPair.c S1AP_ProtocolIE-FieldPair.c \
    S1AP_ProtocolIE-ContainerPairList.c

user@host ~/Documents/git/nextepc/lib/s1ap/asn1c$ \
    rm -f Makefile.am.asn1convert Makefile.am.libasncodec

user@host ~/Documents/git/nextepc/lib/s1ap/asn1c$ \
    rm -f converter-example.mk converter-example.c pdu_collection.c


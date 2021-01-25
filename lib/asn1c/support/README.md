Use mounse07410(vlm_master) git's fork for asn1c

commit dcf963c0e43196057a97feac16421fe79dc7d943 (HEAD -> vlm_master, origin/vlm_master, origin/HEAD)
Merge: b33a84f9 c35ebd33
Author: Mouse <mouse008@gmail.com>
Date:   Sun Nov 1 08:58:12 2020 -0500

===========================================
user@host ~/Documents/git/my$ \
    git clone https://github.com/mouse07410/asn1c.git
user@host ~/Documents/git/my$ \
    git checkout dcf963c0e43196057a97feac16421fe79dc7d943

OR

user@host ~/Documents/git/my$ \
    git clone https://github.com/open5gs/asn1c.git

user@host Documents/git/my/asn1c$ \
    autoreconf -fi;./configure;make -j4

Modify 36413-g40.txt to 36413-g40.asn
===========================================
user@host ~/documents/git/open5gs/lib/asn1c/support/s1ap-r16.4.0$ \
    diff 36413-g40.txt 36413-g40.asn

ASN.1 encoder/decoder
===========================================
user@host ~/documents/git/open5gs/lib/asn1c/s1ap$ \
    ASN1C_PREFIX=S1AP_ ../../../../my/asn1c/asn1c/asn1c -pdu=all \
    -fcompound-names -findirect-choice -fno-include-deps \
    -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-UPER \
    ../support/s1ap-r16.4.0/36413-g40.asn

user@host ~/Documents/git/open5gs/lib/asn1c/ngap$ \
    ASN1C_PREFIX=NGAP_ ../../../../my/asn1c/asn1c/asn1c -pdu=all \
    -fcompound-names -findirect-choice -fno-include-deps \
    -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-UPER \
    ../support/ngap-r16.4.0/38413-g40.asn

Fix NGAP_ProtocolExtensionField.c
===========================================
$ diff --git a/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c b/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
index 2d0d96c7..d2e6b114 100644
--- a/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
+++ b/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
@@ -31124,11 +31124,7 @@ static asn_TYPE_member_t asn_MBR_NGAP_extensionValue_396[] = {
                        0,
 #endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
 #if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
+#if 0 /* modified by acetcom */
                        &asn_PER_memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constr_17,
+#else
+            0,
+#endif
 #endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
                        memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constraint_396
                },

Check common file
===========================================
user@host ~/Documents/git/open5gs/lib/asn1c/common$ \
    git diff asn_internal.h

Check meson.build
===========================================
user@host ~/Documents/git/open5gs/lib/asn1c/s1ap$ \
user@host ~/Documents/git/open5gs/lib/asn1c/ngap$ \
    cat Makefile.am.libasncodec

Remove unnecessary files
===========================================
user@host ~/Documents/git/open5gs/lib/asn1c/s1ap$ \
    rm -f S1AP_E-RAB-IE-ContainerPairList.c \
    S1AP_ProtocolError-IE-ContainerList.c \
    S1AP_ProtocolIE-ContainerPair.c S1AP_ProtocolIE-FieldPair.c \
    S1AP_ProtocolIE-ContainerPairList.c

user@host ~/Documents/git/open5gs/lib/asn1c/ngap$ \
    rm -f NGAP_ProtocolIE-ContainerPair.c NGAP_ProtocolIE-FieldPair.c \
    NGAP_ProtocolIE-ContainerPairList.c \
    NGAP_ProtocolIE-ContainerPair.h NGAP_ProtocolIE-ContainerPairList.h \
    NGAP_ProtocolIE-FieldPair.h

user@host ~/Documents/git/open5gs/lib/asn1c/s1ap$ \
    rm -f Makefile.am.asn1convert Makefile.am.libasncodec

user@host ~/Documents/git/open5gs/lib/asn1c/s1ap$ \
    rm -f converter-example.mk converter-example.c pdu_collection.c

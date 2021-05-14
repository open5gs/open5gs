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

Fix aper_support.c
===========================================
diff --git a/lib/asn1c/common/aper_support.c b/lib/asn1c/common/aper_support.c
index 67ad9db5..1adbdde6 100644
--- a/lib/asn1c/common/aper_support.c
+++ b/lib/asn1c/common/aper_support.c
@@ -22,7 +22,20 @@ aper_get_length(asn_per_data_t *pd, int range, int ebits, int *repeat) {

        *repeat = 0;

-       if (range <= 65536 && range >= 0)
+    /*
+     * ITU-T X.691(08/2015)
+     * #11.9.4.2
+     *
+     * If the length determinant "n" to be encoded is a normally small length,
+     * or a constrained whole number with "ub" greater than or equal to 64K,
+     * or is a semi-constrained whole number, then "n" shall be encoded
+     * as specified in 11.9.3.4 to 11.9.3.8.4.
+     *
+     * NOTE – Thus, if "ub" is greater than or equal to 64K,
+     * the encoding of the length determinant is the same as it would be
+     * if the length were unconstrained.
+     */
+       if (range <= 65535 && range >= 0)
                return aper_get_nsnnwn(pd, range);

        if (aper_get_align(pd) < 0)
@@ -32,14 +45,14 @@ aper_get_length(asn_per_data_t *pd, int range, int ebits, int *repeat) {

        value = per_get_few_bits(pd, 8);
        if(value < 0) return -1;
-       if((value & 128) == 0)  /* #10.9.3.6 */
+       if((value & 128) == 0)  /* #11.9.3.6 */
                return (value & 0x7F);
-       if((value & 64) == 0) { /* #10.9.3.7 */
+       if((value & 64) == 0) { /* #11.9.3.7 */
                value = ((value & 63) << 8) | per_get_few_bits(pd, 8);
                if(value < 0) return -1;
                return value;
        }
-       value &= 63;    /* this is "m" from X.691, #10.9.3.8 */
+       value &= 63;    /* this is "m" from X.691, #11.9.3.8 */
        if(value < 1 || value > 4)
                return -1;
        *repeat = 1;
@@ -162,18 +175,18 @@ aper_put_length(asn_per_outp_t *po, int range, size_t length, int *need_eom) {

        ASN_DEBUG("APER put length %zu with range %d", length, range);

-       /* 10.9 X.691 Note 2 */
+       /* 11.9 X.691 Note 2 */
        if (range <= 65536 && range >= 0)
                return aper_put_nsnnwn(po, range, length);

        if (aper_put_align(po) < 0)
                return -1;

-       if(length <= 127)          /* #10.9.3.6 */{
+       if(length <= 127)          /* #11.9.3.6 */{
                return per_put_few_bits(po, length, 8)
                ? -1 : (ssize_t)length;
        }
-       else if(length < 16384) /* #10.9.3.7 */
+       else if(length < 16384) /* #11.9.3.7 */
                return per_put_few_bits(po, length|0x8000, 16)
                ? -1 : (ssize_t)length;

@@ -193,7 +206,7 @@ int
 aper_put_nslength(asn_per_outp_t *po, size_t length) {

        if(length <= 64) {
-               /* #10.9.3.4 */
+               /* #11.9.3.4 */
                if(length == 0) return -1;
                return per_put_few_bits(po, length-1, 7) ? -1 : 0;
        } else {

Fix NGAP_RANNodeNameUTF8String.c/NGAP_AMFNameUTF8String.c
===========================================
diff --git a/lib/asn1c/ngap/NGAP_RANNodeNameUTF8String.c b/lib/asn1c/ngap/NGAP_RANNodeNameUTF8String.c
index 9e469f7f..79ebd028 100644
--- a/lib/asn1c/ngap/NGAP_RANNodeNameUTF8String.c
+++ b/lib/asn1c/ngap/NGAP_RANNodeNameUTF8String.c
@@ -46,7 +46,11 @@ NGAP_RANNodeNameUTF8String_constraint(const asn_TYPE_descriptor_t *td, const voi
 #if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
 asn_per_constraints_t asn_PER_type_NGAP_RANNodeNameUTF8String_constr_1 CC_NOTUSED = {
        { APC_UNCONSTRAINED,    -1, -1,  0,  0 },
+#if 0 /* modified by acetcom */
        { APC_UNCONSTRAINED,    -1, -1,  0,  0 },
+#else
+       { APC_CONSTRAINED | APC_EXTENSIBLE,  8,  8,  1,  150 }  /* (SIZE(1..150,...)) */,
+#endif
        0, 0    /* No PER value map */
 };
 #endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
diff --git a/lib/asn1c/ngap/NGAP_AMFNameUTF8String.c b/lib/asn1c/ngap/NGAP_AMFNameUTF8String.c
index 1df33a4d..a74f97ea 100644
--- a/lib/asn1c/ngap/NGAP_AMFNameUTF8String.c
+++ b/lib/asn1c/ngap/NGAP_AMFNameUTF8String.c
@@ -46,7 +46,11 @@ NGAP_AMFNameUTF8String_constraint(const asn_TYPE_descriptor_t *td, const void *s
 #if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
 asn_per_constraints_t asn_PER_type_NGAP_AMFNameUTF8String_constr_1 CC_NOTUSED = {
        { APC_UNCONSTRAINED,    -1, -1,  0,  0 },
+#if 0 /* modified by acetcom */
        { APC_UNCONSTRAINED,    -1, -1,  0,  0 },
+#else
+       { APC_CONSTRAINED | APC_EXTENSIBLE,  8,  8,  1,  150 }  /* (SIZE(1..150,...)) */,
+#endif
        0, 0    /* No PER value map */
 };
 #endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */

Fix NGAP_ProtocolExtensionField.c
===========================================
diff --git a/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c b/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
index 4a23d705..3afbede8 100644
--- a/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
+++ b/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
@@ -48809,11 +48809,7 @@ static asn_TYPE_member_t asn_MBR_NGAP_extensionValue_648[] = {
                        0,
 #endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
 #if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
+#if 0 /* modified by acetcom */
                        &asn_PER_memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constr_43,
+#else
+            0,
+#endif
 #endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
                        memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constraint_648
                },

Check common file
===========================================
user@host ~/Documents/git/open5gs/lib/asn1c/common$ \
    git diff asn_internal.h
diff --git a/lib/asn1c/common/asn_internal.h b/lib/asn1c/common/asn_internal.h
index 71397a62..0b673a46 100644
--- a/lib/asn1c/common/asn_internal.h
+++ b/lib/asn1c/common/asn_internal.h
@@ -34,18 +34,11 @@ extern "C" {
 #define        ASN1C_ENVIRONMENT_VERSION       923     /* Compile-time version */
 int get_asn1c_environment_version(void);       /* Run-time version */

+#if 0 /* modified by acetcom */
+#define        CALLOC(nmemb, size)     calloc(nmemb, size)
+#define        MALLOC(size)            malloc(size)
+#define        REALLOC(oldptr, size)   realloc(oldptr, size)
+#define        FREEMEM(ptr)            free(ptr)
+#else
 #include "ogs-core.h"
 #define        CALLOC(nmemb, size)     ogs_calloc(nmemb, size)
 #define        MALLOC(size)            ogs_malloc(size)
 #define        REALLOC(oldptr, size)   ogs_realloc(oldptr, size)
 #define        FREEMEM(ptr)            ogs_free(ptr)
+#endif

 #define        asn_debug_indent        0
 #define ASN_DEBUG_INDENT_ADD(i) do{}while(0)

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

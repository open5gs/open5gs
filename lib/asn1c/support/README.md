Use mounse07410(vlm_master) git's fork for asn1c

commit e0c4a3eafa9f1861dd7dac1000953eb2850e47e4 (HEAD -> vlm_master, origin/vlm_master, origin/HEAD)
Merge: ce10a57a 665a2278
Author: Mouse <mouse07410@users.noreply.github.com>
Date:   Sun Feb 11 17:13:22 2024 -0500

    Merge pull request #177 from v0-e/integer-2-int64

    INTEGER to 64-bit native integer methods

commit 665a227804e18c3fa6b46ab55aeedd7379f1ed29
Author: v0-e <xyz.vieira@gmail.com>
Date:   Wed Feb 7 15:29:18 2024 +0000

    INTEGER <-> (u)int64_t tests

commit 81cc4e47a665c51faff2d2c2fb2ba39066009356
Author: v0-e <xyz.vieira@gmail.com>
Date:   Wed Feb 7 15:28:09 2024 +0000

    INTEGER to (u)int64_t

===========================================
user@host ~/Documents/git/my$ \
    git clone https://github.com/mouse07410/asn1c.git
user@host ~/Documents/git/my$ \
    git checkout e0c4a3eafa9f1861dd7dac1000953eb2850e47e4

OR

user@host ~/Documents/git/my$ \
    git clone https://github.com/open5gs/asn1c.git

user@host Documents/git/my/asn1c$ \
    autoreconf -fi;./configure;make -j8

Modify 36413-h30.txt to 36413-h30.asn
===========================================
user@host ~/documents/git/open5gs/lib/asn1c/support/s1ap-r17.3.0$ \
    diff 36413-h30.txt 36413-h30.asn

Modify 38413-h30.txt to 38413-h30.asn
===========================================
user@host ~/documents/git/open5gs/lib/asn1c/support/ngap-r17.3.0$ \
    diff 38413-h30.txt 38413-h30.asn

ASN.1 encoder/decoder
===========================================
user@host ~/documents/git/open5gs/lib/asn1c/s1ap$ \
    ASN1C_PREFIX=S1AP_ ../../../../my/asn1c/asn1c/asn1c -pdu=all \
    -fcompound-names -findirect-choice -fno-include-deps \
    -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-UPER -no-gen-JER \
    ../support/s1ap-r17.3.0/36413-h30.asn

user@host ~/Documents/git/open5gs/lib/asn1c/ngap$ \
    ASN1C_PREFIX=NGAP_ ../../../../my/asn1c/asn1c/asn1c -pdu=all \
    -fcompound-names -findirect-choice -fno-include-deps \
    -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-UPER -no-gen-JER \
    ../support/ngap-r17.3.0/38413-h30.asn

Fix NGAP_RANNodeNameUTF8String.c (Issues #994 - APC_EXTENSIBLE)
===============================================================
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

Fix NGAP_AMFNameUTF8String.c (Issues #994 - APC_EXTENSIBLE)
===============================================================
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

Check ExtIEs in ProtocolExtensionContainer
==========================================
diff --git a/src/smf/ngap-build.c b/src/smf/ngap-build.c
index 9554be276..051cb6e8a 100644
--- a/src/smf/ngap-build.c
+++ b/src/smf/ngap-build.c
@@ -180,13 +180,13 @@ ogs_pkbuf_t *ngap_build_pdu_session_resource_setup_request_transfer(

             if (smf_self()->security_indication.
                     maximum_integrity_protected_data_rate_downlink) {
-                NGAP_ProtocolExtensionContainer_9625P229_t *extContainer = NULL;
+                NGAP_ProtocolExtensionContainer_11905P297_t *extContainer = NULL;
                 NGAP_SecurityIndication_ExtIEs_t *extIe = NULL;
                 NGAP_MaximumIntegrityProtectedDataRate_t
                     *MaximumIntegrityProtectedDataRate = NULL;

                 extContainer = CALLOC(1,
-                        sizeof(NGAP_ProtocolExtensionContainer_9625P229_t));
+                        sizeof(NGAP_ProtocolExtensionContainer_11905P297_t));
                 ogs_assert(extContainer);
                 SecurityIndication->iE_Extensions =
                     (struct NGAP_ProtocolExtensionContainer *)extContainer;

Check common file
===========================================
user@host ~/Documents/git/open5gs/lib/asn1c/common$ \
    git diff asn_internal.h
diff --git a/lib/asn1c/common/asn_internal.h b/lib/asn1c/common/asn_internal.h
diff -u asn_internal.h ~/asn_internal.h
--- asn_internal.h	2022-02-26 15:48:33.431509100 +0900
+++ /home/acetcom/asn_internal.h	2022-02-26 15:43:00.890972555 +0900
@@ -34,10 +34,53 @@
 #define	ASN1C_ENVIRONMENT_VERSION	923	/* Compile-time version */
 int get_asn1c_environment_version(void);	/* Run-time version */

+#if 0 /* modified by acetcom */
 #define	CALLOC(nmemb, size)	calloc(nmemb, size)
 #define	MALLOC(size)		malloc(size)
 #define	REALLOC(oldptr, size)	realloc(oldptr, size)
 #define	FREEMEM(ptr)		free(ptr)
+#else
+#include "ogs-core.h"
+
+static ogs_inline void *ogs_asn_malloc(size_t size, const char *file_line)
+{
+    void *ptr = ogs_malloc(size);
+    if (!ptr) {
+        ogs_fatal("asn_malloc() failed in `%s`", file_line);
+        ogs_assert_if_reached();
+    }
+
+    return ptr;
+}
+static ogs_inline void *ogs_asn_calloc(
+        size_t nmemb, size_t size, const char *file_line)
+{
+    void *ptr = ogs_calloc(nmemb, size);
+    if (!ptr) {
+        ogs_fatal("asn_calloc() failed in `%s`", file_line);
+        ogs_assert_if_reached();
+    }
+
+    return ptr;
+}
+static ogs_inline void *ogs_asn_realloc(
+        void *oldptr, size_t size, const char *file_line)
+{
+    void *ptr = ogs_realloc(oldptr, size);
+    if (!ptr) {
+        ogs_fatal("asn_realloc() failed in `%s`", file_line);
+        ogs_assert_if_reached();
+    }
+
+    return ptr;
+}
+
+#define CALLOC(nmemb, size) ogs_asn_calloc(nmemb, size, OGS_FILE_LINE)
+#define MALLOC(size) ogs_asn_malloc(size, OGS_FILE_LINE)
+#define REALLOC(oldptr, size) ogs_asn_realloc(oldptr, size, OGS_FILE_LINE)
+#define FREEMEM(ptr) ogs_free(ptr)
+
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

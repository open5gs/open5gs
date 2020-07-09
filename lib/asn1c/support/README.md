Use mounse07410(vlm_master) git's fork for asn1c
===========================================
user@host ~/Documents/git/my$ \
    git clone https://github.com/mouse07410/asn1c.git
user@host ~/Documents/git/my$ \
    git checkout ca3d87a0a3e27ce71b3800210f58d2a3c759f573

OR

user@host ~/Documents/git/my$ \
    git clone https://github.com/open5gs/asn1c.git

user@host Documents/git/my/asn1c$ \
    autoreconf -fi;./configure;make -j4

Modify 36413-g10.txt to 36413-g10.asn
===========================================
user@host ~/documents/git/open5gs/lib/asn1c/support/s1ap-r16.1.0$ \
    diff 36413-g40.txt 36413-g10.asn

ASN.1 encoder/decoder
===========================================
user@host ~/documents/git/open5gs/lib/asn1c/s1ap$ \
    ASN1C_PREFIX=S1AP_ ../../../../my/asn1c/asn1c/asn1c -pdu=all \
    -fcompound-names -findirect-choice -fno-include-deps \
    ../support/s1ap-r16.1.0/36413-g10.asn

user@host ~/Documents/git/open5gs/lib/asn1c/ngap$ \
    ASN1C_PREFIX=NGAP_ ../../../../my/asn1c/asn1c/asn1c -pdu=all \
    -fcompound-names -findirect-choice -fno-include-deps \
    ../support/ngap-r16.1.0/38413-g10.asn

Fix S1AP_E-RABUsageReportItem.c
===========================================
diff --git a/lib/asn1c/s1ap/S1AP_E-RABUsageReportItem.c b/lib/asn1c/s1ap/S1AP_E-RABUsageReportItem.c
index c8e6d551..73aae4fd 100644
--- a/lib/asn1c/s1ap/S1AP_E-RABUsageReportItem.c
+++ b/lib/asn1c/s1ap/S1AP_E-RABUsageReportItem.c
@@ -80,7 +80,7 @@ memb_S1AP_usageCountUL_constraint_1(const asn_TYPE_descriptor_t *td, const void
                return -1;
        }

-       if((value >= 0 && value <= 18446744073709551615)) {
+       if((value >= 0 && value <= 18446744073709551615U)) {
                /* Constraint check succeeded */
                return 0;
        } else {
@@ -111,7 +111,7 @@ memb_S1AP_usageCountDL_constraint_1(const asn_TYPE_descriptor_t *td, const void
                return -1;
        }

-       if((value >= 0 && value <= 18446744073709551615)) {
+       if((value >= 0 && value <= 18446744073709551615U)) {
                /* Constraint check succeeded */
                return 0;
        } else {
@@ -142,7 +142,7 @@ static asn_oer_constraints_t asn_OER_memb_S1AP_usageCountUL_constr_4 CC_NOTUSED
        { 8, 1 }        /* (0..18446744073709551615) */,
        -1};
 static asn_per_constraints_t asn_PER_memb_S1AP_usageCountUL_constr_4 CC_NOTUSED = {
-       { APC_CONSTRAINED,       64, -1,  0,  18446744073709551615 }    /* (0..18446744073709551615) */,
+       { APC_CONSTRAINED,       64, -1,  0,  18446744073709551615U }   /* (0..18446744073709551615) */,
        { APC_UNCONSTRAINED,    -1, -1,  0,  0 },
        0, 0    /* No PER value map */
 };
@@ -150,7 +150,7 @@ static asn_oer_constraints_t asn_OER_memb_S1AP_usageCountDL_constr_5 CC_NOTUSED
        { 8, 1 }        /* (0..18446744073709551615) */,
        -1};
 static asn_per_constraints_t asn_PER_memb_S1AP_usageCountDL_constr_5 CC_NOTUSED = {
-       { APC_CONSTRAINED,       64, -1,  0,  18446744073709551615 }    /* (0..18446744073709551615) */,
+       { APC_CONSTRAINED,       64, -1,  0,  18446744073709551615U }   /* (0..18446744073709551615) */,
        { APC_UNCONSTRAINED,    -1, -1,  0,  0 },

Fix NGAP_ProtocolExtensionField.c
===========================================
diff --git a/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c b/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
index 26da1b34..940db758 100644
--- a/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
+++ b/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
@@ -27671,7 +27671,11 @@ static asn_TYPE_member_t asn_MBR_NGAP_extensionValue_396[] = {
                0,
                &asn_DEF_OCTET_STRING,
                0,
+#if 0 /* modified by acetcom */
                { &asn_OER_memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constr_17, &asn_PER_memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constr_17,  memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constraint_396 },
+#else
+               { 0, 0,  memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constraint_396 },
+#endif
                0, 0, /* No default value */
                "OCTET STRING (CONTAINING PDUSessionResourceReleaseResponseTransfer)"
                },

--- a/lib/asn1c/ngap/NGAP_VolumeTimedReport-Item.c
+++ b/lib/asn1c/ngap/NGAP_VolumeTimedReport-Item.c
@@ -80,7 +80,7 @@ memb_NGAP_usageCountUL_constraint_1(const asn_TYPE_descriptor_t *td, const void
                return -1;
        }
        
-       if((value >= 0 && value <= 18446744073709551615)) {
+       if((value >= 0 && value <= 18446744073709551615U)) {
                /* Constraint check succeeded */
                return 0;
        } else {
@@ -111,7 +111,7 @@ memb_NGAP_usageCountDL_constraint_1(const asn_TYPE_descriptor_t *td, const void
                return -1;
        }
        
-       if((value >= 0 && value <= 18446744073709551615)) {
+       if((value >= 0 && value <= 18446744073709551615U)) {
                /* Constraint check succeeded */
                return 0;
        } else {
@@ -142,7 +142,7 @@ static asn_oer_constraints_t asn_OER_memb_NGAP_usageCountUL_constr_4 CC_NOTUSED
        { 8, 1 }        /* (0..18446744073709551615) */,
        -1};
 static asn_per_constraints_t asn_PER_memb_NGAP_usageCountUL_constr_4 CC_NOTUSED = {
-       { APC_CONSTRAINED,       64, -1,  0,  18446744073709551615 }    /* (0..18446744073709551615) */,
+       { APC_CONSTRAINED,       64, -1,  0,  18446744073709551615U }   /* (0..18446744073709551615) */,
        { APC_UNCONSTRAINED,    -1, -1,  0,  0 },
        0, 0    /* No PER value map */
 };
@@ -150,7 +150,7 @@ static asn_oer_constraints_t asn_OER_memb_NGAP_usageCountDL_constr_5 CC_NOTUSED
        { 8, 1 }        /* (0..18446744073709551615) */,
        -1};
 static asn_per_constraints_t asn_PER_memb_NGAP_usageCountDL_constr_5 CC_NOTUSED = {
-       { APC_CONSTRAINED,       64, -1,  0,  18446744073709551615 }    /* (0..18446744073709551615) */,
+       { APC_CONSTRAINED,       64, -1,  0,  18446744073709551615U }   /* (0..18446744073709551615) */,
        { APC_UNCONSTRAINED,    -1, -1,  0,  0 },
        0, 0    /* No PER value map */

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

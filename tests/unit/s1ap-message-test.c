/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-s1ap.h"
#include "core/abts.h"

static void s1ap_message_test1(abts_case *tc, void *data)
{
    /* S1SetupRequest */
    const char *payload = 
        "0011002d000004003b00090000f11040"
        "54f64010003c400903004a4c542d3632"
        "3100400007000c0e4000f11000894001"
        "00";

    ogs_s1ap_message_t message;
    ogs_pkbuf_t *pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 49);

    result = ogs_s1ap_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    ogs_s1ap_free(&message);

    ogs_pkbuf_free(pkbuf);
}

static void s1ap_message_test2(abts_case *tc, void *data)
{
    /* InitialUE(Attach Request) */
    const char *payload = 
        "000c406f000006000800020001001a00"
        "3c3b17df675aa8050741020bf600f110"
        "000201030003e605f070000010000502"
        "15d011d15200f11030395c0a003103e5"
        "e0349011035758a65d0100e0c1004300"
        "060000f1103039006440080000f1108c"
        "3378200086400130004b00070000f110"
        "000201";

    ogs_s1ap_message_t message;
    ogs_pkbuf_t *pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 115);

    result = ogs_s1ap_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    ogs_s1ap_free(&message);

    ogs_pkbuf_free(pkbuf);
}

static void s1ap_message_test3(abts_case *tc, void *data)
{
    /* initial context setup response */
    const char *payload = 
        "2009002500000300004005c0020000bf"
        "0008400200010033400f000032400a0a"
        "1f0a0123c601000908";

    ogs_s1ap_message_t message;
    ogs_pkbuf_t *pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 41);

    result = ogs_s1ap_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    ogs_s1ap_free(&message);

    ogs_pkbuf_free(pkbuf);
}

static void s1ap_message_test4(abts_case *tc, void *data)
{
    /* InitialUE(Service Request) */
    const char *payload = 
        "000c402d000005000800020071001a00 0504c706b410004300060013f1890001"
        "006440080013f189400bb75000864001 40006440080013f189400bb750004340"
        "060013f18900014300060013f1890001 006440080013f189400db09000864001"
        "30000000000000000000000000000000 00000000000000000000000000000000";

    ogs_s1ap_message_t message;
    ogs_pkbuf_t *pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 128);

    result = ogs_s1ap_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    ogs_s1ap_free(&message);

    ogs_pkbuf_free(pkbuf);
}

static void s1ap_message_test5(abts_case *tc, void *data)
{
    /* ENBDirectInformationTransferRAN-INFORMATION-REQUEST */
    const char *payload =
        "0025004a000001007900432036715489 0164f0000100010002548f0264f00000"
        "010064f000400000002057974b81054c 84000000204f81005581014d860064f0"
        "00000280094064f0000100010002";

    ogs_s1ap_message_t message;
    ogs_pkbuf_t *pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 78);

    result = ogs_s1ap_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    ogs_s1ap_free(&message);

    ogs_pkbuf_free(pkbuf);
}

static void s1ap_message_test6(abts_case *tc, void *data)
{
    /* ENBDirectInformationTransferRAN-INFORMATION-REQUEST */
    const char *payload =
        "0025004a000001007900432036715489 0164f0000100010001548f0264f00000"
        "010064f000400000002057974b81054c 840000001f4f81005581014d860064f0"
        "00000180094064f0000100010001";

    ogs_s1ap_message_t message;
    ogs_pkbuf_t *pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 78);

    result = ogs_s1ap_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    ogs_s1ap_free(&message);

    ogs_pkbuf_free(pkbuf);
}

static void s1ap_message_test7(abts_case *tc, void *data)
{
    /* ENBDirectInformationTransferRAN-INFORMATION-REQUEST */
    const char *payload =
        "0025004a000001007900432038715489 0064f000170001002d548f0254f42100"
        "010054f421400000002057994b81014c 84000000014f81045581014d8864f000"
        "170001002d0064f000170001002d";

    ogs_s1ap_message_t message;
    ogs_pkbuf_t *pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 78);

    result = ogs_s1ap_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    ogs_s1ap_free(&message);

    ogs_pkbuf_free(pkbuf);
}

static void s1ap_message_test8(abts_case *tc, void *data)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_InitialContextSetupRequest_t *InitialContextSetupRequest = NULL;

    S1AP_InitialContextSetupRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_UEAggregateMaximumBitrate_t *UEAggregateMaximumBitrate = NULL;
    S1AP_E_RABToBeSetupListCtxtSUReq_t *E_RABToBeSetupListCtxtSUReq = NULL;
    S1AP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    S1AP_SecurityKey_t *SecurityKey = NULL;
    S1AP_Masked_IMEISV_t *Masked_IMEISV = NULL;
    S1AP_NRUESecurityCapabilities_t *NRUESecurityCapabilities = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_InitialContextSetupRequest;

    InitialContextSetupRequest =
        &initiatingMessage->value.choice.InitialContextSetupRequest;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_uEaggregateMaximumBitrate;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitrate;

    UEAggregateMaximumBitrate = &ie->value.choice.UEAggregateMaximumBitrate;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupListCtxtSUReq;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
    S1AP_InitialContextSetupRequestIEs__value_PR_E_RABToBeSetupListCtxtSUReq;

    E_RABToBeSetupListCtxtSUReq = &ie->value.choice.E_RABToBeSetupListCtxtSUReq;

    *MME_UE_S1AP_ID = 215;
    *ENB_UE_S1AP_ID = 248;

    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateUL,
            1073741824);
    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateDL,
            1073741824);

    {
        S1AP_E_RABToBeSetupItemCtxtSUReqIEs_t *item = NULL;
        S1AP_E_RABToBeSetupItemCtxtSUReq_t *e_rab = NULL;

        item = CALLOC(1, sizeof(S1AP_E_RABToBeSetupItemCtxtSUReqIEs_t));
        ASN_SEQUENCE_ADD(&E_RABToBeSetupListCtxtSUReq->list, item);

        item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupItemCtxtSUReq;
        item->criticality = S1AP_Criticality_reject;
        item->value.present = S1AP_E_RABToBeSetupItemCtxtSUReqIEs__value_PR_E_RABToBeSetupItemCtxtSUReq;

        e_rab = &item->value.choice.E_RABToBeSetupItemCtxtSUReq;

        e_rab->e_RAB_ID = 5;
        e_rab->e_RABlevelQoSParameters.qCI = 9;

        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            priorityLevel = 8;
        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            pre_emptionCapability = 1;
        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            pre_emptionVulnerability = 1;

        ogs_ip_t sgw_s1u_ip;
        memset(&sgw_s1u_ip, 0, sizeof(sgw_s1u_ip));
        sgw_s1u_ip.ipv4 = 1;
        sgw_s1u_ip.addr = inet_addr("10.11.2.6");

        rv = ogs_asn_ip_to_BIT_STRING(
                &sgw_s1u_ip, &e_rab->transportLayerAddress);
        ogs_assert(rv == OGS_OK);
        ogs_asn_uint32_to_OCTET_STRING(
                4458, &e_rab->gTP_TEID);
    }

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    UESecurityCapabilities->encryptionAlgorithms.size = 2;
    UESecurityCapabilities->encryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->encryptionAlgorithms.size,
                    sizeof(uint8_t));
    UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->encryptionAlgorithms.buf[0] = 0xe0;

    UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] = 0xe0;

    {
        const char *kenb_string =
            "46c29ed95831e77a cd7becb5b57d02a7"
            "0b542ba0fbfd88a7 36f37f1b4f4b1a6d";
        uint8_t kenb[32];

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_SecurityKey;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_SecurityKey;

        SecurityKey = &ie->value.choice.SecurityKey;

        SecurityKey->size = 32;
        SecurityKey->buf = CALLOC(SecurityKey->size, sizeof(uint8_t));
        SecurityKey->bits_unused = 0;
        OGS_HEX(kenb_string, strlen(kenb_string), kenb);
        memcpy(SecurityKey->buf, kenb, SecurityKey->size);
    }

    {
        /* Set UeRadioCapability if exists */
        S1AP_UERadioCapability_t *UERadioCapability = NULL;
        const char *radio_string =
#define TEST8_RADIO_SIZE 2149
            "50400242 40002b40 060010fd 0031b400"
            "6d400100 002e400b 00002f40 060099f9"
            "100004e3 bfff8eff fe3bfff8 efffe3bf"
            "ff8efffe 3bfff8ef ffe3bfff 8efffe3b"
            "fffbf06e c4f00141 bc08d480 0000002a"
            "77c00000 00bff002 10030030 02801200"
            "60060060 84300304 018200c1 00608030"
            "4218210c 10860843 04218210 c1006080"
            "30421821 0c100608 03042182 10904018"
            "200c1006 08030420 82104108 08200c10"
            "06080304 018210c1 08608410 42082104"
            "10820803 04018200 c1006084 30421821"
            "0c108608 43042182 00c10060 84300304"
            "21801821 0c108608 43003042 18018400"
            "86100218 200c1006 10021840 08608030"
            "40184100 61040182 00c20803 08200c20"
            "80308010 c2004308 010c2080 308200c2"
            "08030801 0c200430 8010c208 41082104"
            "20003080 00c20003 08010c20 04308010"
            "c2004308 010c2004 308000c2 00030800"
            "0c200430 8010c200 4308010c 20043080"
            "10c20043 08010c20 04308010 c2004308"
            "010c0096 87d54674 040fe061 0002081b"
            "60008006 98002000 be1000c7 00040030"
            "0001040e 80004103 9c001040 c4000400"
            "31800104 0d100040 03480010 00e50004"
            "103a4001 040ea000 41070800 10508208"
            "18400092 0ed80020 21041056 c2061000"
            "2083a600 08084104 15308184 000820af"
            "841c2000 4107a000 10508208 2d010708"
            "00104167 08384000 820f2800 20a10410"
            "59420e10 0020838e 00080841 04147081"
            "84000820 e300020a 10410518 20e10002"
            "08388000 80841041 44081840 00820e10"
            "0020a004 1050820e 00002083 b6000808"
            "e10015b0 818e0008 00ed8002 02304105"
            "6c206300 02083b60 00808010 415b0818"
            "0000820a f841d300 04007940 01054c20"
            "02ca1074 c001001d 30004047 0800a984"
            "0c700040 074c0010 1182082a 61031800"
            "1041d300 04040082 0a9840c0 00041057"
            "c20e3000 2082be10 70000104 1e800041"
            "470800b4 041c7000 40079400 1051c200"
            "2ca1071c 001001c7 00040460 820a3840"
            "c6000410 71c00101 0020828e 10300001"
            "041e8000 41400820 b4041c00 00410794"
            "00105002 082ca107 00001041 c6000414"
            "00820a30 41c00004 10710001 01002082"
            "88103000 01041c00 00414008 20600002"
            "483d0000 82d01040 e8000490 79c00105"
            "9c2083ce 000828c1 04167083 8c000820"
            "73800248 38c00082 8c1040c6 000490b6"
            "c0010108 20a10410 96c20610 0020a104"
            "1076c001 01082482 b6103080 01242d30"
            "00404208 28410425 30818400 08284104"
            "1d300040 420920a9 840c2000 49097c20"
            "e9800202 1041097c 20a9840c 20004105"
            "7c206100 02485d00 00828e10 1082084d"
            "01071c00 10108208 4d01051c 20610002"
            "085d0000 80841050 02084d01 03080010"
            "5002084d 01010820 e0000208 5ca00082"
            "8e101082 084ca107 1c001010 82084ca1"
            "051c2061 0002085c a0008084 10500208"
            "4ca10308 00105002 084ca101 0820e000"
            "02084be1 07080010 5002004b e1050820"
            "e0000200 5b600080 8e101082 084b6103"
            "1c001010 82084b61 011c2061 0002085b"
            "6000808c 10508208 4b610318 00105082"
            "084b6101 1820e100 02085b60 00808410"
            "5002084b 61030800 10500208 4b610108"
            "20e00002 085a6000 808e1010 82084a61"
            "031c0010 1082084a 61011c20 61000208"
            "5a600080 8c105082 084a6103 18001050"
            "82084a61 011820e1 0002085a 60008084"
            "10500208 4a610308 00105002 084a6101"
            "0820e000 020858e0 00808c10 50820848"
            "e1031800 10508208 48e10118 20e10002"
            "0858e000 80841050 820848e1 03080010"
            "50820858 e0008084 10500208 18402093"
            "fc77fff1 dfffc77f ff1dfffc 77fff1df"
            "ffc77fff 1dfffc77 fff1dfff c77fff1d"
            "fffc77ff f1dfffc7 7fff1dff fc77fff1"
            "dfffc77f ff1dfffc 77fff1df ffc77fff"
            "1dfffc77 fff1dfff c77fff1d fffc77ff"
            "f1dfffc7 7fff1dff fc77fff1 dfffc77f"
            "ff1dfffc 77fff1df ffc77fff 1dfffc77"
            "fff1dfff c77fff1d fffc77ff f1dfffc7"
            "7fff1dff fc77fff1 dfffc77f ff1dfffc"
            "77fff1df ffc77fff 1dfffc77 fff1dfff"
            "c77fff1d fffc77ff f1dfffc7 7fff1dff"
            "fc77fff1 dfffc77f ff1dfffc 77fff1df"
            "ffc77fff 1dfffc77 fff1dfff c77fff1d"
            "fffc77ff f1dfffc7 7fff1dff fc77fff1"
            "dfffc77f ff1dfffc 77fff1df ffc77fff"
            "1dfffc77 fff1dfff c77fff1d fffc77ff"
            "f1dfffc7 7fff1dff fc77fff1 dfffc77f"
            "ff1dfffc 77fff1df ffc77fff 1dfffc77"
            "fff1dfff c77fff1d fffc77ff f1dfffc7"
            "7fff1dff fc77fff1 dfffc77f ff1dfffc"
            "77fff1df ffc77fff 1dfffc77 fff1dfff"
            "c77fff1d fffc77ff f1dfffc7 7fff1dff"
            "fdf3a000 023fe000 22e87878 78743c3c"
            "391fc7f0 f0f27e4f c3c390f0 f0f0f0e4"
            "5e2f178b 843c3917 8bc5e2f0 f0e43c3c"
            "3c3a1e1e 3f8721e3 f8b80000 000021e1"
            "e1c45e2f 1787878f cfc10048 00f1979a"
            "040149df ffffffe6 94bbc700 e8005820"
            "01149000 02000510 4a9e0600 00100803"
            "00e09836 1284e140 982689c2 00100082"
            "1800c0c0 00020100 601c1306 c2509c28"
            "1304d138 4583e0e9 a0100007 4f5a1302"
            "00020140 4824c580 0260d40c 13601873"
            "e618f285 81c13008 0c46032a 0d89dac1"
            "18434102 0f94022c b64c550d 0e7c471e"
            "819f604f 8c73e618 f28581c0 e1e0b800"
            "07800e00 01e00038 01c0a836 26eb0461"
            "0d04083e 5008b2d9 31543439 f11c7a06"
            "7d813e31 cf9863ca 16070387 82e000be"
            "0038002f 8000e407 001b2800 02800002"
            "a0d899ac 11843410 20f94022 cb64c550"
            "d0e7c471 e819f604 f8c73e61 8f28581c"
            "0e1e0b80 027800e0 009e0003 801c0e83"
            "614606b0 4610d040 83e5008b 2d931543"
            "439f11c7 a067d813 e31cf986 3ca16070"
            "38782e00 0be00380 02f8000e 00702a0d"
            "84fac118 4341020f 94022cb6 4c550d0e"
            "7c471e81 9f604f8c 73e618f2 8581c0e1"
            "e138002f 800e000b e0003801 c0a83612"
            "eb04610d 04083e50 08b2d931 543439f1"
            "1c7a067d 813e31cf 9863ca16 07038784"
            "e0008000 38002000 00e00702 a0f837ac"
            "11843410 20f94022 4b64c550 50e7c471"
            "e31cf986 3ca16070 387813fa 000000fe"
            "80000003 801c0a83 e09eb046 10d04083"
            "e500892d 93";
        uint8_t radio[TEST8_RADIO_SIZE];

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_UERadioCapability;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

        UERadioCapability = &ie->value.choice.UERadioCapability;

        ogs_assert(UERadioCapability);
        OGS_HEX(radio_string, strlen(radio_string), radio);
        ogs_s1ap_buffer_to_OCTET_STRING(
                radio, TEST8_RADIO_SIZE, UERadioCapability);
    }

    {
        const char *masked_string =
            "86740905 00ffff03";
        uint8_t masked[8];

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_Masked_IMEISV;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_Masked_IMEISV;

        Masked_IMEISV = &ie->value.choice.Masked_IMEISV;

        Masked_IMEISV->size = 8;
        Masked_IMEISV->buf = CALLOC(Masked_IMEISV->size, sizeof(uint8_t));
        Masked_IMEISV->bits_unused = 0;
        OGS_HEX(masked_string, strlen(masked_string), masked);
        memcpy(Masked_IMEISV->buf, masked, Masked_IMEISV->size);
    }
    {
        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_NRUESecurityCapabilities;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1AP_InitialContextSetupRequestIEs__value_PR_NRUESecurityCapabilities;

        NRUESecurityCapabilities = &ie->value.choice.NRUESecurityCapabilities;

        NRUESecurityCapabilities->nRencryptionAlgorithms.size = 2;
        NRUESecurityCapabilities->nRencryptionAlgorithms.buf =
            CALLOC(NRUESecurityCapabilities->nRencryptionAlgorithms.size,
                        sizeof(uint8_t));
        NRUESecurityCapabilities->nRencryptionAlgorithms.bits_unused = 0;
        NRUESecurityCapabilities->nRencryptionAlgorithms.buf[0] = 0xe0;

        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.size = 2;
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.buf =
            CALLOC(NRUESecurityCapabilities->
                    nRintegrityProtectionAlgorithms.size, sizeof(uint8_t));
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.
            bits_unused = 0;
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.buf[0] =
            0xe0;
    }

    s1apbuf = ogs_s1ap_encode(&pdu);
    ogs_assert(s1apbuf);
    ogs_pkbuf_free(s1apbuf);
}

static void s1ap_message_test9(abts_case *tc, void *data)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_InitialContextSetupRequest_t *InitialContextSetupRequest = NULL;

    S1AP_InitialContextSetupRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_UEAggregateMaximumBitrate_t *UEAggregateMaximumBitrate = NULL;
    S1AP_E_RABToBeSetupListCtxtSUReq_t *E_RABToBeSetupListCtxtSUReq = NULL;
    S1AP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    S1AP_SecurityKey_t *SecurityKey = NULL;
    S1AP_Masked_IMEISV_t *Masked_IMEISV = NULL;
    S1AP_NRUESecurityCapabilities_t *NRUESecurityCapabilities = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_InitialContextSetupRequest;

    InitialContextSetupRequest =
        &initiatingMessage->value.choice.InitialContextSetupRequest;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_uEaggregateMaximumBitrate;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitrate;

    UEAggregateMaximumBitrate = &ie->value.choice.UEAggregateMaximumBitrate;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupListCtxtSUReq;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
    S1AP_InitialContextSetupRequestIEs__value_PR_E_RABToBeSetupListCtxtSUReq;

    E_RABToBeSetupListCtxtSUReq = &ie->value.choice.E_RABToBeSetupListCtxtSUReq;

    *MME_UE_S1AP_ID = 205;
    *ENB_UE_S1AP_ID = 5888;

    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateUL,
            1073741824);
    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateDL,
            1073741824);

    {
        S1AP_E_RABToBeSetupItemCtxtSUReqIEs_t *item = NULL;
        S1AP_E_RABToBeSetupItemCtxtSUReq_t *e_rab = NULL;

        item = CALLOC(1, sizeof(S1AP_E_RABToBeSetupItemCtxtSUReqIEs_t));
        ASN_SEQUENCE_ADD(&E_RABToBeSetupListCtxtSUReq->list, item);

        item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupItemCtxtSUReq;
        item->criticality = S1AP_Criticality_reject;
        item->value.present = S1AP_E_RABToBeSetupItemCtxtSUReqIEs__value_PR_E_RABToBeSetupItemCtxtSUReq;

        e_rab = &item->value.choice.E_RABToBeSetupItemCtxtSUReq;

        e_rab->e_RAB_ID = 5;
        e_rab->e_RABlevelQoSParameters.qCI = 9;

        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            priorityLevel = 8;
        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            pre_emptionCapability = 1;
        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            pre_emptionVulnerability = 1;

        ogs_ip_t sgw_s1u_ip;
        memset(&sgw_s1u_ip, 0, sizeof(sgw_s1u_ip));
        sgw_s1u_ip.ipv4 = 1;
        sgw_s1u_ip.addr = inet_addr("10.11.2.6");

        rv = ogs_asn_ip_to_BIT_STRING(
                &sgw_s1u_ip, &e_rab->transportLayerAddress);
        ogs_assert(rv == OGS_OK);
        ogs_asn_uint32_to_OCTET_STRING(
                4400, &e_rab->gTP_TEID);
    }

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    UESecurityCapabilities->encryptionAlgorithms.size = 2;
    UESecurityCapabilities->encryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->encryptionAlgorithms.size,
                    sizeof(uint8_t));
    UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->encryptionAlgorithms.buf[0] = 0xe0;

    UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] = 0xe0;

    {
        const char *kenb_string =
            "1dacef4b 77ca2953 0b9f2076 1125181b"
            "11596aba 6a8c38be 2a4e0ead 9fdf2c67";
        uint8_t kenb[32];

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_SecurityKey;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_SecurityKey;

        SecurityKey = &ie->value.choice.SecurityKey;

        SecurityKey->size = 32;
        SecurityKey->buf = CALLOC(SecurityKey->size, sizeof(uint8_t));
        SecurityKey->bits_unused = 0;
        OGS_HEX(kenb_string, strlen(kenb_string), kenb);
        memcpy(SecurityKey->buf, kenb, SecurityKey->size);
    }

    {
        /* Set UeRadioCapability if exists */
        S1AP_UERadioCapability_t *UERadioCapability = NULL;
        const char *radio_string =
#define TEST9_RADIO_SIZE 2149
            "04537803 086e5ddb 8050e6c1 073e9c02"
            "80818892 26954aa1 c3bfff8e fffe3bff"
            "f8efffe3 bfff8eff fe3bfff8 efffe3bf"
            "ff8efffe 3bfff8ef ffe3bfff 8efffe3b"
            "fffbf06e c4f00141 bc08cc80 0000002a"
            "77c00000 00bfe801 20060060 05002200"
            "60060060 80304012 08030401 8200c100"
            "60843003 04018200 c1086084 30421821"
            "0c108608 43040182 00c10860 84304018"
            "200c1086 08430420 82104108 20840410"
            "06080304 018200c1 08208430 4218210c"
            "10860843 00304218 210c1086 08430401"
            "8200c108 60843040 18200c10 86006084"
            "30030401 8200c108 60060841 04208400"
            "86100218 200c1006 10001840 00610001"
            "84000610 00184000 61002184 00861002"
            "18400861 00218400 86080304 01840086"
            "10021840 08610021 84008610 02184008"
            "60803040 18200c20 80308200 c2080308"
            "010c2004 308010c2 08030820 0c208030"
            "8010c200 4308010c 20803082 00c20841"
            "08210420 00308000 c2000308 010c2004"
            "30025a07 d5467208 0fe06d80 02001840"
            "00820638 002000be 1000e700 04103000"
            "01040e80 00410310 001000c6 00041034"
            "4001000d 20004003 4c001000 e5000410"
            "3a400104 0ea00041 076c0010 1082082b"
            "61030800 1041db00 04047080 0ad840c7"
            "00040076 c0010118 2082b610 31800104"
            "1db00040 400820ad 840c0000 41070800"
            "10508208 18400092 0e380020 21041051"
            "c2061000 2082be10 70800104 1e800041"
            "420820b4 041c2000 41059c20 e1000208"
            "3ca00082 84104165 08384000 820e9800"
            "20210410 54c20610 0020838c 00082841"
            "04146083 84000820 e2000202 10410510"
            "20610002 08384000 82801041 42083800"
            "00820f40 0020a384 005a020e 38002003"
            "ca000828 e1001650 838e0008 00e98002"
            "02384005 4c206380 020038e0 00808c10"
            "41470818 c000820e 38002020 041051c2"
            "06000020 82be1074 c0010015 f0838c00"
            "0820af84 1c000041 079c0010 59c2083c"
            "e000828c 10416708 38c00082 07380024"
            "83d00008 28010416 80838000 0820f280"
            "020a0041 059420e0 0002083a 60008080"
            "10415308 18000082 0e300020 a0041051"
            "820e0000 20838800 08080104 14408180"
            "000820e0 00020a00 41030000 1241e800"
            "04168082 07400024 83a60008 08c10415"
            "30818c00 0820e300 020a3041 03180012"
            "41e50004 1530800b 2841d300 0400b6c0"
            "01010820 a1041096 c2061000 20a10410"
            "76c00101 082482b6 10308001 242db000"
            "40470808 410425b0 818e0008 08410425"
            "b0808e10 30800104 2db00040 47080801"
            "0425b081 8e000808 010425b0 808e1030"
            "0001042d b0004046 08284104 25b0818c"
            "00082841 0425b080 8c107080 01042db0"
            "00404608 28010425 b0818c00 08280104"
            "25b0808c 10700001 041db000 40460920"
            "ad840c60 00490b6c 00101082 0a004109"
            "6c206100 020a0041 096c2021 041c0000"
            "410b6c00 1010020a 0041096c 20600002"
            "0a00410b 1c001010 820a1041 091c2061"
            "00020a10 41071c00 10108248 28e10308"
            "0012415f 08184000 92174000 20a38404"
            "20821340 41c70004 04208213 40414708"
            "18400082 17400020 21041400 82134040"
            "c2000414 00821340 40420838 00008217"
            "280020a3 84042082 132841c7 00040420"
            "82132841 47081840 00821728 00202104"
            "14008213 2840c200 04140082 13284042"
            "08380000 8212f841 d3000404 208212f8"
            "41530818 40008212 f841c200 04140080"
            "12f84142 08380000 80169800 20238404"
            "20821298 40c70004 04208212 98404708"
            "18400082 16980020 23041420 82129840"
            "c6000414 20820610 0824ff1d fffc77ff"
            "f1dfffc7 7fff1dff fc77fff1 dfffc77f"
            "ff1dfffc 77fff1df ffc77fff 1dfffc77"
            "fff1dfff c77fff1d fffc77ff f1dfffc7"
            "7fff1dff fc77fff1 dfffc77f ff1dfffc"
            "77fff1df ffc77fff 1dfffc77 fff1dfff"
            "c77fff1d fffc77ff f1dfffc7 7fff1dff"
            "fc77fff1 dfffc77f ff1dfffc 77fff1df"
            "ffc77fff 1dfffc77 fff1dfff c77fff1d"
            "fffc77ff f1dfffc7 7fff1dff fc77fff1"
            "dfffc77f ff1dfffc 77fff1df ffc77fff"
            "1dfffc77 fff1dfff c77fff1d fffc77ff"
            "f1dfffc7 7fff1dff fc77fff1 dfffc77f"
            "ff1dfffc 77fff1df ffc77fff 1dfffc77"
            "fff1dfff c77fff1d fffc77ff f1dfffc7"
            "7fff1dff fc77fff1 dfffc77f ff1dfffc"
            "77fff1df ffc77fff 1dfffc77 fff1dfff"
            "c77fff1d fffc77ff f1dfffc7 7fff1dff"
            "fc77fff1 dfffc77f ff1dfffc 77fff1df"
            "ffc77fff 1dfffc77 fff1dfff c77fff1d"
            "fffc77ff f1dfffc7 7fff1dff fc77fff1"
            "dfffc77f ff1dfffc 77fff1df ffc77fff"
            "1dfffc77 fff1dfff c77fff7c e800008f"
            "f8000878 721e1e1e 1e2e8fe3 f43c3c39"
            "0f0f0f0f 27e4fc3c 3c3c3917 8bc5e2f1"
            "78bc390e 43c3c384 3c3c3c3a 1e1e3f8b"
            "c5e3f8b8 0008bc5e 2f0f0e08 78700000"
            "21e1cfc1 004800f9 95e68100 5277ffff"
            "fff9a52e f1c03a00 16080045 24000080"
            "014412e7 81800004 0200c038 260d84a1"
            "38502609 a273c080 04100000 00a32000"
            "00008004 00208600 30300000 80401807"
            "04c1b094 270a04c1 344e1005 8345e9a0"
            "1000074f 5a130200 02014048 24c58002"
            "60d40c13 601873e6 18f28581 c130080c"
            "47032a0d 89dac118 4341020f 94022cb6"
            "4c550d0e 7c471e81 9f604f8c 73e618f2"
            "8581c0e1 e0b80007 800e0001 e0003801"
            "c0a83626 eb04610d 04083e50 08b2d931"
            "543439f1 1c7a067d 813e31cf 9863ca16"
            "07038782 e000be00 38002f80 00e40700"
            "1b280002 800002a0 d899ac11 84341020"
            "f94022cb 64c550d0 e7c471e8 19f604f8"
            "c73e618f 28581c0e 1e0b8002 7800e000"
            "9e000380 1c0e8361 4606b046 10d04083"
            "e5008b2d 93154343 9f11c7a0 67d813e3"
            "1cf9863c a1607038 782e000b e0038002"
            "f8000e00 702a0d84 fac11843 41020f94"
            "022cb64c 550d0e7c 471e819f 604f8c73"
            "e618f285 81c0e1e1 38002f80 0e000be0"
            "003801c0 a83612eb 04610d04 083e5008"
            "b2d93154 3439f11c 7a067d81 3e31cf98"
            "63ca1607 038784e0 00800038 00200000"
            "e00702a0 f837ac11 84341020 f940224b"
            "64c55050 e7c471e3 1cf9863c a1607038"
            "7813fa00 0000fe80 00000380 1c0a83e0"
            "9eb04610 d0";
        uint8_t radio[TEST9_RADIO_SIZE];

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_UERadioCapability;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

        UERadioCapability = &ie->value.choice.UERadioCapability;

        ogs_assert(UERadioCapability);
        OGS_HEX(radio_string, strlen(radio_string), radio);
        ogs_s1ap_buffer_to_OCTET_STRING(
                radio, TEST9_RADIO_SIZE, UERadioCapability);
    }

    {
        const char *masked_string =
            "86740905 00ffff03";
        uint8_t masked[8];

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_Masked_IMEISV;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_Masked_IMEISV;

        Masked_IMEISV = &ie->value.choice.Masked_IMEISV;

        Masked_IMEISV->size = 8;
        Masked_IMEISV->buf = CALLOC(Masked_IMEISV->size, sizeof(uint8_t));
        Masked_IMEISV->bits_unused = 0;
        OGS_HEX(masked_string, strlen(masked_string), masked);
        memcpy(Masked_IMEISV->buf, masked, Masked_IMEISV->size);
    }
    {
        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_NRUESecurityCapabilities;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1AP_InitialContextSetupRequestIEs__value_PR_NRUESecurityCapabilities;

        NRUESecurityCapabilities = &ie->value.choice.NRUESecurityCapabilities;

        NRUESecurityCapabilities->nRencryptionAlgorithms.size = 2;
        NRUESecurityCapabilities->nRencryptionAlgorithms.buf =
            CALLOC(NRUESecurityCapabilities->nRencryptionAlgorithms.size,
                        sizeof(uint8_t));
        NRUESecurityCapabilities->nRencryptionAlgorithms.bits_unused = 0;
        NRUESecurityCapabilities->nRencryptionAlgorithms.buf[0] = 0xe0;

        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.size = 2;
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.buf =
            CALLOC(NRUESecurityCapabilities->
                    nRintegrityProtectionAlgorithms.size, sizeof(uint8_t));
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.
            bits_unused = 0;
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.buf[0] =
            0xe0;
    }

    s1apbuf = ogs_s1ap_encode(&pdu);
    ogs_assert(s1apbuf);
    ogs_pkbuf_free(s1apbuf);
}

static void s1ap_message_test10(abts_case *tc, void *data)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_InitialContextSetupRequest_t *InitialContextSetupRequest = NULL;

    S1AP_InitialContextSetupRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_UEAggregateMaximumBitrate_t *UEAggregateMaximumBitrate = NULL;
    S1AP_E_RABToBeSetupListCtxtSUReq_t *E_RABToBeSetupListCtxtSUReq = NULL;
    S1AP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    S1AP_SecurityKey_t *SecurityKey = NULL;
    S1AP_Masked_IMEISV_t *Masked_IMEISV = NULL;
    S1AP_NRUESecurityCapabilities_t *NRUESecurityCapabilities = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_InitialContextSetupRequest;

    InitialContextSetupRequest =
        &initiatingMessage->value.choice.InitialContextSetupRequest;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_uEaggregateMaximumBitrate;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitrate;

    UEAggregateMaximumBitrate = &ie->value.choice.UEAggregateMaximumBitrate;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupListCtxtSUReq;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
    S1AP_InitialContextSetupRequestIEs__value_PR_E_RABToBeSetupListCtxtSUReq;

    E_RABToBeSetupListCtxtSUReq = &ie->value.choice.E_RABToBeSetupListCtxtSUReq;

    *MME_UE_S1AP_ID = 205;
    *ENB_UE_S1AP_ID = 5888;

    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateUL,
            1073741824);
    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateDL,
            1073741824);

    {
        S1AP_E_RABToBeSetupItemCtxtSUReqIEs_t *item = NULL;
        S1AP_E_RABToBeSetupItemCtxtSUReq_t *e_rab = NULL;

        item = CALLOC(1, sizeof(S1AP_E_RABToBeSetupItemCtxtSUReqIEs_t));
        ASN_SEQUENCE_ADD(&E_RABToBeSetupListCtxtSUReq->list, item);

        item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupItemCtxtSUReq;
        item->criticality = S1AP_Criticality_reject;
        item->value.present = S1AP_E_RABToBeSetupItemCtxtSUReqIEs__value_PR_E_RABToBeSetupItemCtxtSUReq;

        e_rab = &item->value.choice.E_RABToBeSetupItemCtxtSUReq;

        e_rab->e_RAB_ID = 5;
        e_rab->e_RABlevelQoSParameters.qCI = 9;

        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            priorityLevel = 8;
        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            pre_emptionCapability = 1;
        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            pre_emptionVulnerability = 1;

        ogs_ip_t sgw_s1u_ip;
        memset(&sgw_s1u_ip, 0, sizeof(sgw_s1u_ip));
        sgw_s1u_ip.ipv4 = 1;
        sgw_s1u_ip.addr = inet_addr("10.11.2.6");

        rv = ogs_asn_ip_to_BIT_STRING(
                &sgw_s1u_ip, &e_rab->transportLayerAddress);
        ogs_assert(rv == OGS_OK);
        ogs_asn_uint32_to_OCTET_STRING(
                4400, &e_rab->gTP_TEID);
    }

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    UESecurityCapabilities->encryptionAlgorithms.size = 2;
    UESecurityCapabilities->encryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->encryptionAlgorithms.size,
                    sizeof(uint8_t));
    UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->encryptionAlgorithms.buf[0] = 0xe0;

    UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] = 0xe0;

    {
        const char *kenb_string =
            "e9cb2fcf 24cf962c c04ead05 519eed52"
            "bf2279a2 0b63f2fa fb515915 82b1e774";
        uint8_t kenb[32];

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_SecurityKey;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_SecurityKey;

        SecurityKey = &ie->value.choice.SecurityKey;

        SecurityKey->size = 32;
        SecurityKey->buf = CALLOC(SecurityKey->size, sizeof(uint8_t));
        SecurityKey->bits_unused = 0;
        OGS_HEX(kenb_string, strlen(kenb_string), kenb);
        memcpy(SecurityKey->buf, kenb, SecurityKey->size);
    }

    {
        /* Set UeRadioCapability if exists */
        S1AP_UERadioCapability_t *UERadioCapability = NULL;
        const char *radio_string =
#define TEST10_RADIO_SIZE 2674
            "04537803 086e5ddb 8050e6c1 073e9c02"
            "80818892 26954aa1 c3bfff8e fffe3bff"
            "f8efffe3 bfff8eff fe3bfff8 efffe3bf"
            "ff8efffe 3bfff8ef ffe3bfff 8efffe3b"
            "fffbf06e c4f00141 bc08cc80 0000002a"
            "77c00000 00bfe801 20060060 05002200"
            "60060060 80304012 08030401 8200c100"
            "60843003 04018200 c1086084 30421821"
            "0c108608 43040182 00c10860 84304018"
            "200c1086 08430420 82104108 20840410"
            "06080304 018200c1 08208430 4218210c"
            "10860843 00304218 210c1086 08430401"
            "8200c108 60843040 18200c10 86006084"
            "30030401 8200c108 60060841 04208400"
            "86100218 200c1006 10001840 00610001"
            "84000610 00184000 61002184 00861002"
            "18400861 00218400 86080304 01840086"
            "10021840 08610021 84008610 02184008"
            "60803040 18200c20 80308200 c2080308"
            "010c2004 308010c2 08030820 0c208030"
            "8010c200 4308010c 20803082 00c20841"
            "08210420 00308000 c2000308 010c2004"
            "30025a07 d5467208 0fe06d80 02001840"
            "00820638 002000be 1000e700 04103000"
            "01040e80 00410310 001000c6 00041034"
            "4001000d 20004003 4c001000 e5000410"
            "3a400104 0ea00041 076c0010 1082082b"
            "61030800 1041db00 04047080 0ad840c7"
            "00040076 c0010118 2082b610 31800104"
            "1db00040 400820ad 840c0000 41070800"
            "10508208 18400092 0e380020 21041051"
            "c2061000 2082be10 70800104 1e800041"
            "420820b4 041c2000 41059c20 e1000208"
            "3ca00082 84104165 08384000 820e9800"
            "20210410 54c20610 0020838c 00082841"
            "04146083 84000820 e2000202 10410510"
            "20610002 08384000 82801041 42083800"
            "00820f40 0020a384 005a020e 38002003"
            "ca000828 e1001650 838e0008 00e98002"
            "02384005 4c206380 020038e0 00808c10"
            "41470818 c000820e 38002020 041051c2"
            "06000020 82be1074 c0010015 f0838c00"
            "0820af84 1c000041 079c0010 59c2083c"
            "e000828c 10416708 38c00082 07380024"
            "83d00008 28010416 80838000 0820f280"
            "020a0041 059420e0 0002083a 60008080"
            "10415308 18000082 0e300020 a0041051"
            "820e0000 20838800 08080104 14408180"
            "000820e0 00020a00 41030000 1241e800"
            "04168082 07400024 83a60008 08c10415"
            "30818c00 0820e300 020a3041 03180012"
            "41e50004 1530800b 2841d300 0400b6c0"
            "01010820 a1041096 c2061000 20a10410"
            "76c00101 082482b6 10308001 242db000"
            "40470808 410425b0 818e0008 08410425"
            "b0808e10 30800104 2db00040 47080801"
            "0425b081 8e000808 010425b0 808e1030"
            "0001042d b0004046 08284104 25b0818c"
            "00082841 0425b080 8c107080 01042db0"
            "00404608 28010425 b0818c00 08280104"
            "25b0808c 10700001 041db000 40460920"
            "ad840c60 00490b6c 00101082 0a004109"
            "6c206100 020a0041 096c2021 041c0000"
            "410b6c00 1010020a 0041096c 20600002"
            "0a00410b 1c001010 820a1041 091c2061"
            "00020a10 41071c00 10108248 28e10308"
            "0012415f 08184000 92174000 20a38404"
            "20821340 41c70004 04208213 40414708"
            "18400082 17400020 21041400 82134040"
            "c2000414 00821340 40420838 00008217"
            "280020a3 84042082 132841c7 00040420"
            "82132841 47081840 00821728 00202104"
            "14008213 2840c200 04140082 13284042"
            "08380000 8212f841 d3000404 208212f8"
            "41530818 40008212 f841c200 04140080"
            "12f84142 08380000 80169800 20238404"
            "20821298 40c70004 04208212 98404708"
            "18400082 16980020 23041420 82129840"
            "c6000414 20820610 0824ff1d fffc77ff"
            "f1dfffc7 7fff1dff fc77fff1 dfffc77f"
            "ff1dfffc 77fff1df ffc77fff 1dfffc77"
            "fff1dfff c77fff1d fffc77ff f1dfffc7"
            "7fff1dff fc77fff1 dfffc77f ff1dfffc"
            "77fff1df ffc77fff 1dfffc77 fff1dfff"
            "c77fff1d fffc77ff f1dfffc7 7fff1dff"
            "fc77fff1 dfffc77f ff1dfffc 77fff1df"
            "ffc77fff 1dfffc77 fff1dfff c77fff1d"
            "fffc77ff f1dfffc7 7fff1dff fc77fff1"
            "dfffc77f ff1dfffc 77fff1df ffc77fff"
            "1dfffc77 fff1dfff c77fff1d fffc77ff"
            "f1dfffc7 7fff1dff fc77fff1 dfffc77f"
            "ff1dfffc 77fff1df ffc77fff 1dfffc77"
            "fff1dfff c77fff1d fffc77ff f1dfffc7"
            "7fff1dff fc77fff1 dfffc77f ff1dfffc"
            "77fff1df ffc77fff 1dfffc77 fff1dfff"
            "c77fff1d fffc77ff f1dfffc7 7fff1dff"
            "fc77fff1 dfffc77f ff1dfffc 77fff1df"
            "ffc77fff 1dfffc77 fff1dfff c77fff1d"
            "fffc77ff f1dfffc7 7fff1dff fc77fff1"
            "dfffc77f ff1dfffc 77fff1df ffc77fff"
            "1dfffc77 fff1dfff c77fff7c e800008f"
            "f8000878 721e1e1e 1e2e8fe3 f43c3c39"
            "0f0f0f0f 27e4fc3c 3c3c3917 8bc5e2f1"
            "78bc390e 43c3c384 3c3c3c3a 1e1e3f8b"
            "c5e3f8b8 0008bc5e 2f0f0e08 78700000"
            "21e1cfc1 004800f9 95e68100 5277ffff"
            "fff9a52e f1c03a00 16080045 24000080"
            "014412e7 81800004 0200c038 260d84a1"
            "38502609 a273c080 04100000 00a32000"
            "00008004 00208600 30300000 80401807"
            "04c1b094 270a04c1 344e1005 8345e9a0"
            "1000074f 5a130200 02014048 24c58002"
            "60d40c13 601873e6 18f28581 c130080c"
            "47032a0d 89dac118 4341020f 94022cb6"
            "4c550d0e 7c471e81 9f604f8c 73e618f2"
            "8581c0e1 e0b80007 800e0001 e0003801"
            "c0a83626 eb04610d 04083e50 08b2d931"
            "543439f1 1c7a067d 813e31cf 9863ca16"
            "07038782 e000be00 38002f80 00e40700"
            "1b280002 800002a0 d899ac11 84341020"
            "f94022cb 64c550d0 e7c471e8 19f604f8"
            "c73e618f 28581c0e 1e0b8002 7800e000"
            "9e000380 1c0e8361 4606b046 10d04083"
            "e5008b2d 93154343 9f11c7a0 67d813e3"
            "1cf9863c a1607038 782e000b e0038002"
            "f8000e00 702a0d84 fac11843 41020f94"
            "022cb64c 550d0e7c 471e819f 604f8c73"
            "e618f285 81c0e1e1 38002f80 0e000be0"
            "003801c0 a83612eb 04610d04 083e5008"
            "b2d93154 3439f11c 7a067d81 3e31cf98"
            "63ca1607 038784e0 00800038 00200000"
            "e00702a0 f837ac11 84341020 f940224b"
            "64c55050 e7c471e3 1cf9863c a1607038"
            "7813fa00 0000fe80 00000380 1c0a83e0"
            "9eb04610 d0";
        uint8_t radio[TEST10_RADIO_SIZE];

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_UERadioCapability;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

        UERadioCapability = &ie->value.choice.UERadioCapability;

        ogs_assert(UERadioCapability);
        OGS_HEX(radio_string, strlen(radio_string), radio);
        ogs_s1ap_buffer_to_OCTET_STRING(
                radio, TEST10_RADIO_SIZE, UERadioCapability);
    }

    {
        const char *masked_string =
            "35514310 00ffff07";
        uint8_t masked[8];

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_Masked_IMEISV;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_Masked_IMEISV;

        Masked_IMEISV = &ie->value.choice.Masked_IMEISV;

        Masked_IMEISV->size = 8;
        Masked_IMEISV->buf = CALLOC(Masked_IMEISV->size, sizeof(uint8_t));
        Masked_IMEISV->bits_unused = 0;
        OGS_HEX(masked_string, strlen(masked_string), masked);
        memcpy(Masked_IMEISV->buf, masked, Masked_IMEISV->size);
    }
    {
        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_NRUESecurityCapabilities;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1AP_InitialContextSetupRequestIEs__value_PR_NRUESecurityCapabilities;

        NRUESecurityCapabilities = &ie->value.choice.NRUESecurityCapabilities;

        NRUESecurityCapabilities->nRencryptionAlgorithms.size = 2;
        NRUESecurityCapabilities->nRencryptionAlgorithms.buf =
            CALLOC(NRUESecurityCapabilities->nRencryptionAlgorithms.size,
                        sizeof(uint8_t));
        NRUESecurityCapabilities->nRencryptionAlgorithms.bits_unused = 0;
        NRUESecurityCapabilities->nRencryptionAlgorithms.buf[0] = 0xe0;

        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.size = 2;
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.buf =
            CALLOC(NRUESecurityCapabilities->
                    nRintegrityProtectionAlgorithms.size, sizeof(uint8_t));
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.
            bits_unused = 0;
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.buf[0] =
            0xe0;
    }

    s1apbuf = ogs_s1ap_encode(&pdu);
    ogs_assert(s1apbuf);
    ogs_pkbuf_free(s1apbuf);
}

abts_suite *test_s1ap_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    ogs_log_install_domain(&__ogs_s1ap_domain, "s1ap", OGS_LOG_ERROR);

    abts_run_test(suite, s1ap_message_test1, NULL);
    abts_run_test(suite, s1ap_message_test2, NULL);
    abts_run_test(suite, s1ap_message_test3, NULL);
    abts_run_test(suite, s1ap_message_test4, NULL);
    abts_run_test(suite, s1ap_message_test5, NULL);
    abts_run_test(suite, s1ap_message_test6, NULL);
    abts_run_test(suite, s1ap_message_test7, NULL);
    abts_run_test(suite, s1ap_message_test8, NULL);
    abts_run_test(suite, s1ap_message_test9, NULL);
    abts_run_test(suite, s1ap_message_test10, NULL);

    return suite;
}

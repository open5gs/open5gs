#define TRACE_MODULE _testpacket

#include "core_debug.h"
#include "core_lib.h"

#include "mme/s1ap_build.h"
#include "mme/s1ap_conv.h"
#include "mme/s1ap_path.h"
#include "mme/snow_3g.h"

#include "gtp/gtp_message.h"
#include "gtp/gtp_conv.h"
#include "gtp/gtp_node.h"
#include "gtp/gtp_path.h"

#include "common/context.h"

extern int test_only_control_plane;

#define TEST_ENB1_ADDR "127.0.0.5"
#define TEST_ENB2_ADDR "127.0.0.4"
#if LINUX == 1
#define TEST_ENB1_ADDR6 "fe80::1%lo"
#else
#define TEST_ENB1_ADDR6 "fe80::1%lo0"
#endif
static c_sockaddr_t *test_enb1_addr = NULL;
static c_sockaddr_t *test_enb1_addr6 = NULL;
static c_sockaddr_t *test_enb2_addr = NULL;
static c_sockaddr_t *test_enb2_addr6 = NULL;

static list_t s1ap_list;
static list_t s1ap_list6;

status_t testpacket_init()
{
    status_t rv;

    rv = core_getaddrinfo(&test_enb1_addr,
            AF_INET, TEST_ENB1_ADDR, GTPV1_U_UDP_PORT, 0);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

/* There is no default link-local address,
 * If you want to test it, you need set the IPv6 address in some interface */
#if LINUX != 1 
    rv = core_getaddrinfo(&test_enb1_addr6,
            AF_INET6, TEST_ENB1_ADDR6, GTPV1_U_UDP_PORT, 0);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
#endif

    rv = core_getaddrinfo(&test_enb2_addr,
            AF_INET, TEST_ENB2_ADDR, GTPV1_U_UDP_PORT, 0);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    list_init(&s1ap_list);
    list_init(&s1ap_list6);

    rv = sock_probe_node(&s1ap_list, &s1ap_list6, NULL, 36412);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t testpacket_final()
{
    if (test_enb1_addr)
    {
        core_freeaddrinfo(test_enb1_addr);
        test_enb1_addr = NULL;
    }

    if (test_enb1_addr6)
    {
        core_freeaddrinfo(test_enb1_addr6);
        test_enb1_addr6 = NULL;
    }

    if (test_enb2_addr)
    {
        core_freeaddrinfo(test_enb2_addr);
        test_enb2_addr = NULL;
    }

    sock_remove_all_nodes(&s1ap_list);
    sock_remove_all_nodes(&s1ap_list6);

    return CORE_OK;
}

status_t tests1ap_enb_connect(sock_id *new)
{
    status_t rv;
    sock_node_t *snode = NULL;

    snode = list_first(&s1ap_list);
    if (!snode) snode = list_first(&s1ap_list6);
    d_assert(snode, return CORE_ERROR,);

    rv = sctp_client(new, SOCK_STREAM, snode->list);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t tests1ap_enb_close(sock_id id)
{
    return s1ap_delete(id);
}

status_t tests1ap_enb_read(sock_id id, pkbuf_t *recvbuf)
{
    return s1ap_recv(id, recvbuf);
}

status_t tests1ap_enb_send(sock_id id, pkbuf_t *sendbuf)
{
    return s1ap_send(id, sendbuf, NULL, 0);
}

status_t tests1ap_build_setup_req(
        pkbuf_t **pkbuf, S1AP_ENB_ID_PR present, c_uint32_t enb_id)
{
    status_t rv;

    int tac = 12345;
    plmn_id_t plmn_id;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_S1SetupRequest_t *S1SetupRequest = NULL;

    S1AP_S1SetupRequestIEs_t *ie = NULL;
    S1AP_Global_ENB_ID_t *Global_ENB_ID = NULL;
    S1AP_SupportedTAs_t *SupportedTAs = NULL;
    S1AP_SupportedTAs_Item_t *SupportedTAs_Item = NULL;
    S1AP_PLMNidentity_t *PLMNidentity = NULL;
    S1AP_PagingDRX_t *PagingDRX = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = 
        core_calloc(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_S1Setup;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_S1SetupRequest;

    S1SetupRequest = &initiatingMessage->value.choice.S1SetupRequest;

    ie = core_calloc(1, sizeof(S1AP_S1SetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&S1SetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Global_ENB_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_Global_ENB_ID;

    Global_ENB_ID = &ie->value.choice.Global_ENB_ID;

    ie = core_calloc(1, sizeof(S1AP_S1SetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&S1SetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SupportedTAs;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_SupportedTAs;

    SupportedTAs = &ie->value.choice.SupportedTAs;

    ie = core_calloc(1, sizeof(S1AP_S1SetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&S1SetupRequest->protocolIEs, ie);
    
    ie->id = S1AP_ProtocolIE_ID_id_DefaultPagingDRX;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_PagingDRX;

    PagingDRX = &ie->value.choice.PagingDRX;

    plmn_id_build(&plmn_id, 1, 1, 2);

    s1ap_uint32_to_ENB_ID(present, enb_id, &Global_ENB_ID->eNB_ID);
    s1ap_buffer_to_OCTET_STRING(
            &plmn_id, PLMN_ID_LEN, &Global_ENB_ID->pLMNidentity);

    SupportedTAs_Item = (S1AP_SupportedTAs_Item_t *)
        core_calloc(1, sizeof(S1AP_SupportedTAs_Item_t));
    s1ap_uint16_to_OCTET_STRING(tac, &SupportedTAs_Item->tAC);
    PLMNidentity = (S1AP_PLMNidentity_t *)
        core_calloc(1, sizeof(S1AP_PLMNidentity_t));
    s1ap_buffer_to_OCTET_STRING(
            &plmn_id, PLMN_ID_LEN, PLMNidentity);
    ASN_SEQUENCE_ADD(&SupportedTAs_Item->broadcastPLMNs.list, PLMNidentity);

    ASN_SEQUENCE_ADD(&SupportedTAs->list, SupportedTAs_Item);

    *PagingDRX = S1AP_PagingDRX_v64;

    rv = s1ap_encode_pdu(pkbuf, &pdu);
    s1ap_free_pdu(&pdu);

    if (rv != CORE_OK)
    {
        d_error("s1ap_encode_pdu() failed");
        return CORE_ERROR;
    }

    return CORE_OK;
}

#define TESTS1AP_MAX_MESSAGE 32

status_t tests1ap_build_initial_ue_msg(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000c405800000500 0800020001001a00 302f177ca0b38802 0741020809101010"
        "3254869104e060c0 4000050221d031d1 5c0a003103e5e034 9011035758a65d01"
        "00004300060000f1 1030390064400800 00f1101079baf000 86400130",

        "000c40809b000005 000800020002001a 00737217400466af 070741020bf600f1"
        "10000201fd001710 05e060c040010021 0221d011d1271a80 8021100100001081"
        "0600000000830600 000000000d00000a 005255f50110225c 0a003103e5c03e13"
        "55f501aaaa110357 58a6200b601404ef 65233b8878d29040 080402600400021f"
        "025d0107e0004300 060000f110303900 6440080055f50100 19d0100086400130",

        "000c40809b00"
        "0005000800020003 001a007372178c 3e3cff070741020b f600f11000020100"
        "00000905e060c040 0100210204d011d1 271a808021100100 0010810600000000"
        "830600000000000d 00000a005255f501 10225c0a003103e5 c03e1355f501aaaa"
        "11035758a6200b60 1404ef65233b8878 d290400804026004 00021f025d0107e0"
        "004300060000f110 3039006440080055 f5010019d0100086 400130",

        
        "000c406800000500 080002001f001a00 403f074172080910 10103254866202e0"
        "600021023cd011d1 271a808021100100 0010810600000000 830600000000000d"
        "00000a005c0a0090 11034f18a6f15d01 00004300060000f1 1030390064400800"
        "00f110002343d000 86400130",

        "000c405200000500 0800020011001a 002a2917acba67c4 8207410108091010"
        "103254866205f0f0 000000000e023cd0 11d1270780000a00 000d00c100430006"
        "0000f11030390064 40080000f1109d67 aa500086400130",

        "000c405200000500 0800020021001a 002a2917bcba67c4 8207410108091010"
        "000000003005f0f0 000000000e023cd0 11d1270780000a00 000d00c100430006"
        "0000f11030390064 40080000f1109d67 aa500086400130",


        "000c"
        "404c000005000800 020002001a002423 0741710809101010 3254767905f0f000"
        "0000000e0201d011 d1270780000a0000 0d00c10043000600 00f1103039006440"
        "080000f11054f640 100086400130",
        "",
        "000c"
        "4070000005000800 020001001a004847 0741720809101000 0000002004f07000"
        "000023020ed03127 1d80802110010000 1081060000000083 0600000000000d00"
        "000300000a005c0a 003103e5e0349011 035758a65d010000 4300060000f11030"
        "39006440080000f1 100019b010008640 0130",


        "000c"
        "4068000005000800 020001001a00403f 0741720809101010 3254861002e06000"
        "210207d011d1271a 8080211001000010 8106000000008306 00000000000d0000"
        "0a005c0a00901103 4f18a6f15d010000 4300060000f11030 39006440080000f1"
        "1004615380008640 0130",

        "000c"
        "4080830000060008 0002000c001a0050 4f1744b524490507 41020bf600f11000"
        "0201010003e802e0 6000210204d011d1 271a808021100100 0010810600000000"
        "830600000000000d 00000a005200f110 30395c0a00901103 4f18a6f15d0100e0"
        "004300060000f110 3039006440080000 f110001f20a00086 400130004b000700"
        "00f110000201",
        "",


        "000c"
        "4080810000050008 000340072d001a00 5857074172080910 10103254967305e0"
        "60c0401100270209 d031d12720808021 1001000010810600 0000008306000000"
        "00000d0000030000 0a000010005c0a00 3103e5e02e901103 5758a64008040260"
        "0000021f005d0103 c1004300060000f1 1030390064400800 00f1100787b80000"
        "86400130",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        92,
        160,
        160,

        108,
        86,
        86,

        80,
        0,
        116,

        108,
        136,
        0,

        134,
        0,
        0,

    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_identity_response(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "",
        "",
        "000d"
        "403e000005000000 05c00100009f0008 00020003001a00 121117d7e3248b57"
        "0756080910101032 5486510064400800 00f11054f6401000 4340060000f1105b"
        "a0",

        "",
        "",
        "",

        "",
        "",
        "",
        
        "",
        "000d"
        "403e000005000000 05c0020000c40008 0002000c001a0012 1117c07eb5f50607"
        "5608091010103254 8651006440080000 f110001f20a00043 40060000f1103039",
    };

    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        0,
        0,
        66,

        0,
        0,
        0,

        0,
        0,
        0,

        0,
        66,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_authentication_response(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000d403e00000500 000005c00100009d 000800020001001a 001211177c0bca9d"
        "030753086a91970e 838fd07900644008 0000f1101079baf0 004340060000f110"
        "5ba0",
        "",
        "",

        "000d403500000500 0000020001000800 02001f001a000c0b 07530831c964f076"
        "1378760064400800 00f110002343d000 4340060000f11030 39",
        "",
        "",

        "000d"
        "4038000005000000 05c0020000c80008 00020002001a000c 0b0753087dc78e7c"
        "421f9eb900644008 0000f11054f64010 004340060000f110 2b67",
        "",
        "000d"
        "4035000005000000 0200010008000200 01001a000c0b0753 08d8b388ddacaa1f"
        "36006440080000f1 100019b010004340 060000f1100001",

        
        "000d"
        "4038000005000000 05c0010001da0008 00020001001a000c 0b0753084ce11ef1"
        "24b1854500644008 0000f11004615380 004340060000f110 5ba0",

        "000d"
        "403e000005000000 05c0020000c40008 0002000c001a0012 1117deff957c0707"
        "5308e3b925330735 7093006440080000 f110001f20a00043 40060000f1103039",
        "",


        "000d"
        "4036000005000000 0200020008000340 072d001a000c0b07 5308e2b7921bc6fe"
        "b4c9006440080000 f1100787b8000043 40060000f1103039",
        "",
        "",
    };

    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        66,
        0,
        0,

        57,
        0,
        0,

        60,
        0,
        57,

        60,
        66,
        0,

        58,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}


status_t tests1ap_build_authentication_failure(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "",
        "",
        "",

        "000d"
        "403d000005000000 0200030008000200 21001a001413075c 15300e61640edcfb"
        "605d25911423ee1f 9e006440080000f1 100019b010004340 060000f1100001",
        "000d"
        "402d000005000000 0200030008000200 21001a00040307 5c14006440080000"
        "f1101a2d10100043 40060000f1100001",
        "",


        "",
        "",
        "",

        "",
        "",
        "",

    };

    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        0,
        0,
        0,

        65,
        49,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_security_mode_complete(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000d403500000500 000005c00100009d 000800020001001a 000908476b8f5f64"
        "00075e0064400800 00f1101079baf000 4340060000f1105b a0",
        "",
        "",

        "000d403200000500 0000020001000800 02001f001a000908 473c0c819e00075e"
        "006440080000f110 002343d000434006 0000f1103039",
        "",
        "",

        "000d"
        "4035000005000000 05c0020000c80008 00020002001a0009 0847c0eb1eb80007"
        "5e006440080000f1 1054f64010004340 060000f1102b67",
        "",
        "000d"
        "4032000005000000 0200010008000200 01001a00090847c8 38f8d700075e0064"
        "40080000f1100019 b010004340060000 f1100001",


        "000d"
        "4035000005000000 05c0010001da0008 00020001001a0009 0847d3b0ef030007"
        "5e006440080000f1 1004615380004340 060000f1105ba0",

        "000d"
        "4035000005000000 05c0020000c40008 0002000c001a0009 084790282df90007"
        "5e006440080000f1 10001f20a0004340 060000f1103039",
        "",

        "000d"
        "4033000005000000 0200020008000340 072d001a00090847 aaaf94cf00075e00"
        "6440080000f11007 87b8000043400600 00f1103039",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        57,
        0,
        0,

        54,
        0,
        0,

        57,
        0,
        54,

        57,
        57,
        0,

        55,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_esm_information_response(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] =  {
        "000d40808d000005 00000005c0010000 9d00080002000100 1a006160275c0667"
        "58010221da280908 696e7465726e6574 274a80c223150100 001510c09a2626c0"
        "9a2626c09a2626c0 9a2626c223150200 0015103d3dda5c72 4cc497354ae64653"
        "45a8088021100100 0010810600000000 830600000000000d 00000a0000644008"
        "0000f1101079baf0 004340060000f110 5ba0",
        "000d40808d000005 00000005c0010000 9e00080002000200 1a00616027c963b5"
        "1b010221da280908 696e7465726e6574 274a80c223150100 001510c09a2626c0"
        "9a2626c09a2626c0 9a2626c223150200 0015103d3dda5c72 4cc497354ae64653"
        "45a8088021100100 0010810600000000 830600000000000d 00000a0000644008"
        "0000f1101079baf0 004340060000f110 5ba0",
        "",

        "000d403e00000500 0000020001000800 02001f001a001514 27505a0b5301023c"
        "da280908696e7465 726e657400644008 0000f110002343d0 004340060000f110"
        "3039",
        "000d403e00000500 0000020002000800 020011001a001514 27b531c64f01023c"
        "da280908696e7465 726e657400644008 0000f110002343d0 004340060000f110"
        "3039",
        "",

        "000d"
        "4041000005000000 05c0020000c80008 00020002001a0015 142793b2bedc0102"
        "01da280908696e74 65726e6574006440 080000f11054f640 10004340060000f1"
        "102b67",
        "",
        "",


        "000d"
        "4041000005000000 05c0010001da0008 00020001001a0015 14279a2476c80102"
        "07da280908696e74 65726e6574006440 080000f110046153 80004340060000f1"
        "105ba0",

        "000d"
        "4041000005000000 05c0020000c40008 0002000c001a0015 14274f280bff0102"
        "04da280908696e74 65726e6574006440 080000f110001f20 a0004340060000f1"
        "103039",
        "",

        "000d"
        "403f000005000000 0200020008000340 072d001a00151427 c4a524d8010209da"
        "280908696e746572 6e65740064400800 00f1100787b80000 4340060000f11030"
        "39",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        146,
        146,
        0,

        66,
        66,
        0,

        69,
        0,
        0,

        69,
        69,
        0,

        67,
        0,
        0,

    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_ue_capability_info_indication(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "0016"
        "40809c0000030000 0005c00100009d00 0800020001004a40 8085808304040003"
        "04ec598007000820 81839b4e1e3ff8ff f1ffc7ff8ffe3ffc 7ff1ffe3ff8fff1f"
        "fc7ff8ffe3ffc7ff 1ffdfd3ffa7a2060 090e194e9525c8c2 fd80000000e03fe7"
        "ff5f60000000384f e9ffd3d800000002 1033035758a66014 042f6513b8800d2f"
        "0831c1a53432b259 ef989007000cdd9c 6331200e0019a332 c662401c003200",
        "",
        "",

        "0016402d00000300 0000020001000800 02001f004a401a19 00b801014c598080"
        "9c000bf06ec4d001 40302c0000000000 000000",
        "",
        "",

        "0016"
        "403f000003000000 05c0020000c80008 00020002004a4029 28013001023cd980"
        "00bc000ff06ec4d0 0141b82c00000000 07d404000ef08020 00012a0300008800"
        "00",

        "0016"
        "403f000003000000 05c0020000c70008 00020004004a4029 28013001023cd980"
        "00bc000ff06ec4d0 0141b82c00000000 07d404000ef08020 00012a0300008800"
        "00",
        "",


        "0016"
        "4030000003000000 05c0010001da0008 00020001004a401a 1900b801014c5980"
        "809c000bf06ec4d0 0140302c00000000 0000",

        "0016"
        "4030000003000000 05c0020000c40008 0002000c004a401a 1900b801014c5980"
        "0018000bf06ec4d0 0100302c00000000 0000",
        "",

        "0016"
        "4080800000030000 0002000200080003 40072d004a406c6b 034803042c998001"
        "08201838bfc1c5fd bd3ffa8a20400902 3a54930382c60000 00001c4083840008"
        "0881001c40004042 080060e2ff0717f0 0040000203fa0000 0007e8000000020f"
        "33035758a6601404 e09c100125600031 218d3432b2386200 40008d17960c4008"
        "001100",
        "",
        "",
    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        161,
        0,
        0,

        51,
        0,
        0,

        67,
        67,
        0,

        52,
        52,
        0,

        133,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_initial_context_setup_response(
        pkbuf_t **pkbuf, 
        c_uint32_t mme_ue_s1ap_id, c_uint32_t enb_ue_s1ap_id,
        c_uint8_t ebi, c_uint32_t teid)
{
    status_t rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_InitialContextSetupResponse_t *InitialContextSetupResponse = NULL;

    S1AP_InitialContextSetupResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABSetupListCtxtSURes_t *E_RABSetupListCtxtSURes = NULL;

    S1AP_E_RABSetupItemCtxtSUResIEs_t *item = NULL;
    S1AP_E_RABSetupItemCtxtSURes_t *e_rab = NULL;

    gtp_f_teid_t f_teid;
    ip_t ip;
    int len;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = 
        core_calloc(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_InitialContextSetupResponse;

    InitialContextSetupResponse = 
        &successfulOutcome->value.choice.InitialContextSetupResponse;

    ie = core_calloc(1, sizeof(S1AP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = core_calloc(1, sizeof(S1AP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = core_calloc(1, sizeof(S1AP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABSetupListCtxtSURes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_E_RABSetupListCtxtSURes;

    E_RABSetupListCtxtSURes = &ie->value.choice.E_RABSetupListCtxtSURes;

    *MME_UE_S1AP_ID = mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;

    item = core_calloc(1, sizeof(S1AP_E_RABSetupItemCtxtSUResIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupListCtxtSURes->list, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABSetupItemCtxtSURes;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present =
        S1AP_E_RABSetupItemCtxtSUResIEs__value_PR_E_RABSetupItemCtxtSURes;

    e_rab = &item->value.choice.E_RABSetupItemCtxtSURes;

    e_rab->e_RAB_ID = ebi;

    rv = gtp_sockaddr_to_f_teid(test_enb1_addr, test_enb1_addr6, &f_teid, &len);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_f_teid_to_ip(&f_teid, &ip);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = s1ap_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    s1ap_uint32_to_OCTET_STRING(teid, &e_rab->gTP_TEID);

    rv = s1ap_encode_pdu(pkbuf, &pdu);
    s1ap_free_pdu(&pdu);

    if (rv != CORE_OK)
    {
        d_error("s1ap_encode_pdu() failed");
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t tests1ap_build_initial_context_setup_failure(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "4009 0015000003000040 0200020008400200 01000240020000",

        "",
        "",
    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        25,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_attach_complete(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000d"
        "403a000005000000 05c00100009d0008 00020001001a000e 0d27c183eb950207"
        "4300035200c20064 40080000f1101079 baf0004340060000 f1105ba0",

        "000d"
        "403a000005000000 05c00100009e0008 00020002001a000e 0d27e8a2a3f10207"
        "4300035200c20064 40080000f1101079 baf0004340060000 f1105ba0",
        "",


        "000d"
        "403a000005000000 05c0000000010008 0002001f001a000e 0d27127c47860207"
        "4300035200c20064 40080000f1101079 baf0004340060000 f1105ba0",
        "000d"
        "403a000005000000 05c0010000020008 00020011001a000e 0d27225d92bb0207"
        "4300035200c20064 40080000f1101079 baf0004340060000 f1105ba0",
        "",

        "000d"
        "403a000005000000 05c0020000c80008 00020002001a000e 0d27f190fc2b0207"
        "4300035200c20064 40080000f11054f6 4010004340060000 f1102b67",
        "",
        "",

        "000d"
        "403a000005000000 05c0010001da0008 00020001001a000e 0d272e3456f70207"
        "4300035200c20064 40080000f1100461 5380004340060000 f1105ba0",

        "000d"
        "403a000005000000 05c0020000c40008 0002000c001a000e 0d276a543e560207"
        "4300035200c20064 40080000f110001f 20a0004340060000 f1103039",
        "",

        "000d"
        "4038000005000000 0200020008000340 072d001a000e0d27 600bbd3802074300"
        "035200c200644008 0000f1100787b800 004340060000f110 3039",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        62,
        62,
        0,

        62,
        62,
        0,

        62,
        0,
        0,

        62,
        62,
        0,

        60,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_emm_status(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "",
        "",
        "",

        "",
        "",
        "",

        "",
        "",
        "000d403300000500 0000020001000800 020001001a000a09 276941dec8020760"
        "65006440080000f1 10002343d0004340 060000f1103039",
    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        0,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        55,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_detach_request(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000c404800000600 0800020002001a 001615172ba435db 040745090bf600f1"
        "10000201020003e6 004300060000f110 2b67006440080000 f1109d67aa500086"
        "4001300060000600 40020003e6",
        "000c404800000600 0800020001001a 00161517ba258500 040745090bf600f1"
        "10000201020003e6 004300060000f110 2b67006440080000 f1109d67aa500086"
        "4001300060000600 40020003e6",
        "",

        ""
        ""
        ""

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        76,
        76,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_ue_context_release_request(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "0012"
        "4018000003000000 05c00100009d0008 0002000100024002 0280",
        "",
        "",

        "",
        "",
        "",

        "0012"
        "4018000003000000 05c0020000c80008 0002000200024002 0280",

        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        28,
        0,
        0,

        0,
        0,
        0,

        28,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_ue_context_release_complete(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "2017001300000200 004005c00100009e 00084003400002",
        "2017001300000200 004005c000000001 00084003400001",
        "2017001200000200 004005c00100009f 000840020003",

        "2017001300000200 004005c00100009d 00084003400001",
        "",
        "2017001300000200 004005c000000003 00084003400021",

        "2017"
        "0012000002000040 05c0020000c80008 40020002",
        "2017"
        "0012000002000040 05c0020000cb0008 40020026",
        "2017001200000200 004005c000000001 00084002001f",

        "2017"
        "0012000002000040 05c0020000c40008 4002000c",
        "2017"
        "0012000002000040 05c0020000c50008 40020008",
        "2017"
        "0012000002000040 05c0020000c70008 40020008",

        "2017"
        "0010000002000040 0200010008400340 072c",
        "2017"
        "0010000002000040 0200030008400340 072e",
        0,
    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        23,
        23,
        22,

        23,
        0,
        23,

        22,
        22,
        22,

        22,
        22,
        22,

        20,
        20,
        0,
    };
    char hexbuf[MAX_SDU_LEN];

    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_service_request(pkbuf_t **pkbuf,
        c_uint32_t enb_ue_s1ap_id, c_uint8_t seq,
        c_uint16_t mac, c_uint32_t m_tmsi)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000c"
        "4037000006000800 020004001a0005 04c7049551004300 060000f110303900"
        "6440080000f11007 87b8000086400140 0060000600400000 0001",

        "000c"
        "4038000006000800 03400700001a0005 04c7049551004300 060000f110303900"
        "6440080000f11007 87b8000086400140 0060000600400000 0001",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        59,
        60,
    };
    char hexbuf[MAX_SDU_LEN];
    int i = 0;

    if (enb_ue_s1ap_id & 0x400000) i = 1;
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    enb_ue_s1ap_id = htonl(enb_ue_s1ap_id << 8);
    memcpy((*pkbuf)->payload + 11, &enb_ue_s1ap_id, 2+i);
    mac = htons(mac);
    memcpy((*pkbuf)->payload + 19+i, &seq, 1);
    memcpy((*pkbuf)->payload + 20+i, &mac, 2);
    m_tmsi = htonl(m_tmsi);
    memcpy((*pkbuf)->payload + 55+i, &m_tmsi, 4);

    return CORE_OK;
}

status_t tests1ap_build_tau_request(pkbuf_t **pkbuf, int i,
    c_uint32_t mme_ue_s1ap_id, c_uint32_t enb_ue_s1ap_id, c_uint8_t active_flag,
    c_uint32_t m_tmsi, c_uint8_t seq, c_uint32_t mac, c_uint8_t *knas_int)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        /* Initial UE Message */
        "000c"
        "406d000006000800 020035001a003b3a 1797c955d80a0748 010bf600f1100002"
        "01d900a79e5805f0 f0c040005200f110 30395c1004570220 003103e561249011"
        "033358a25d0103d0 e0c1004300060000 f110303900644008 0000f11054f64010"
        "0086400130006000 060040d900a79e",
        /* Uplink NAS Transport */
        "000d" 
        "4063000005000000 0200030008000200 01001a003a39178a c93785030748010b"
        "f600f110000201d6 004ae05805f070c0 40185200f110303a 5c0a005702200031"
        "03e5e03490110357 58a65d0100e0c100 6440080000f11000 02cf900043400600"
        "00f1103039",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        113,
        103,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    if (i == 0)
    {
        enb_ue_s1ap_id = htonl(enb_ue_s1ap_id << 8);
        memcpy((*pkbuf)->payload + 11, &enb_ue_s1ap_id, 3);
        if (active_flag)
        {
            char *active_buf = (*pkbuf)->payload + 26;
            *active_buf |= 0x08;
        }
        mac = htonl(mac);
        memcpy((*pkbuf)->payload + 19, &mac, 4);
        memcpy((*pkbuf)->payload + 23, &seq, 1);
        m_tmsi = htonl(m_tmsi);
        memcpy((*pkbuf)->payload + 109, &m_tmsi, 4);
    }
    else if (i == 1)
    {
        d_assert(knas_int, return CORE_ERROR,);

        mme_ue_s1ap_id = htonl(mme_ue_s1ap_id << 8);
        memcpy((*pkbuf)->payload + 11, &mme_ue_s1ap_id, 3);
        enb_ue_s1ap_id = htonl(enb_ue_s1ap_id << 8);
        memcpy((*pkbuf)->payload + 17, &enb_ue_s1ap_id, 3);

        memcpy((*pkbuf)->payload + 29, &seq, 1);

        m_tmsi = htonl(m_tmsi);
        memcpy((*pkbuf)->payload + 41, &m_tmsi, 4);

        snow_3g_f9(knas_int, seq, (0 << 27), 0,
                (*pkbuf)->payload + 29, (52 << 3),
                (*pkbuf)->payload + 25);
    }
    else
        d_assert(0,,);

    return CORE_OK;
}

status_t tests1ap_build_pdn_connectivity_request(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000d40640000"
        "0500000002 0001000800020001 001a003b3a277c 81dab50a0205d011"
        "281208696e746572 6e6574036e673204 6d6e6574271a8080 2110010000108106"
        "0000000083060000 0000000d00000a00 006440080055f501 0019d01000434006"
        "0055f5011022",
        "000d40640000"
        "0500000002 0001000800020001 001a003b3a2710 3fdafa0a0209d011"
        "281208706e746572 6e6574036e673204 6d6e6574271a8080 2110010000108106"
        "0000000083060000 0000000d00000a00 006440080055f501 0019d01000434006"
        "0055f5011022",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        108,
        108,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_pdn_disconnectivity_request(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000d40370000"
        "0500000005c08000 0108000800020001 001a000b0a2732 423c53040206d206"
        "006440080055f501 0019d01000434006 0055f5011022",
        "000d40370000"
        "0500000005c08000 0108000800020001 001a000b0a2777 693066040206d206"
        "006440080055f501 0019d01000434006 0055f5011022",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        59,
        59,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_e_rab_setup_response(
        pkbuf_t **pkbuf, 
        c_uint32_t mme_ue_s1ap_id, c_uint32_t enb_ue_s1ap_id,
        c_uint8_t ebi, c_uint32_t teid)
{
    status_t rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_E_RABSetupResponse_t *E_RABSetupResponse = NULL;

    S1AP_E_RABSetupResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABSetupListBearerSURes_t *E_RABSetupListBearerSURes = NULL;

    S1AP_E_RABSetupItemBearerSUResIEs_t *item = NULL;
    S1AP_E_RABSetupItemBearerSURes_t *e_rab = NULL;

    gtp_f_teid_t f_teid;
    ip_t ip;
    int len;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = 
        core_calloc(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_E_RABSetup;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_E_RABSetupResponse;

    E_RABSetupResponse = &successfulOutcome->value.choice.E_RABSetupResponse;

    ie = core_calloc(1, sizeof(S1AP_E_RABSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = core_calloc(1, sizeof(S1AP_E_RABSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = core_calloc(1, sizeof(S1AP_E_RABSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABSetupListBearerSURes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABSetupResponseIEs__value_PR_E_RABSetupListBearerSURes;

    E_RABSetupListBearerSURes = &ie->value.choice.E_RABSetupListBearerSURes;

    *MME_UE_S1AP_ID = mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;

    item = core_calloc(1, sizeof(S1AP_E_RABSetupItemBearerSUResIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupListBearerSURes->list, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABSetupItemBearerSURes;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present =
        S1AP_E_RABSetupItemBearerSUResIEs__value_PR_E_RABSetupItemBearerSURes;

    e_rab = &item->value.choice.E_RABSetupItemBearerSURes;
    e_rab->e_RAB_ID = ebi;
 
    rv = gtp_sockaddr_to_f_teid(test_enb1_addr, test_enb1_addr6, &f_teid, &len);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_f_teid_to_ip(&f_teid, &ip);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = s1ap_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    s1ap_uint32_to_OCTET_STRING(teid, &e_rab->gTP_TEID);

    rv = s1ap_encode_pdu(pkbuf, &pdu);
    s1ap_free_pdu(&pdu);

    if (rv != CORE_OK)
    {
        d_error("s1ap_encode_pdu() failed");
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t tests1ap_build_e_rab_modify_response(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "2006"
        "001b000003000040 0480000001000840 020001001f400600 002500010e",
        "",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        31,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_e_rab_release_response(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "200700190000"
        "0300004002 0001000840020001 0045400600000f 40010c",
        "200700190000"
        "0300004002 0001000840020001 0045400600000f 40010e",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        29,
        29,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_activate_default_bearer_accept(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000d40360000"
        "0500000005c08000 0107000800020001 001a000a0927e7 f5bb400b6200c200"
        "6440080055f50100 19d0100043400600 55f5011022",
        "",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        58,
        0,
        0,

        0,
        0,
        0,

        59,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_activate_dedicated_bearer_accept(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000d403600000500"
        "000005c000000001 00080002000100 1a000a0927078a5f 34037200c6006440"
        "080000f1109d67aa 50004340060000f1 102b67",
        "000d403600000500"
        "000005c000000001 00080002000100 1a000a0927c035da 96036200c6006440"
        "080000f1109d67aa 50004340060000f1 102b67",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

        "000d403600000500"
        "000005c00200003c 00080002000100 1a000a0927a27f49 d6036200c6006440"
        "080000f1109d67aa 50004340060000f1 102b67",

        "000d"
        "4036000005000000 05c0020000c40008 0002000c001a000a 0927e3c77e000362"
        "00c6006440080000 f110001f20a00043 40060000f1103039",
    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        58,
        58,
        0,

        0,
        0,
        0,

        0,
        0,
        0,

        58,
        58,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_modify_bearer_accept(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000d"
        "4035000005000000 0480000001000800 020001001a000a09 27a5c0d564067200"
        "ca006440080064f0 430020a000004340 060064f043020a",
        "",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

        "",
        "",
        "",
    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        57,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_deactivate_bearer_accept(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000d40330000"
        "0500000002 0001000800020001 001a000a09274c b2ebbd056200ce00"
        "6440080055f50100 19d0100043400600 55f5011022",
        "000d40330000"
        "0500000002 0001000800020001 001a000a0927a9 a1b2bd057200ce00"
        "6440080055f50100 19d0100043400600 55f5011022",
        "000d40330000"
        "0500000002 0001000800020001 001a000a09277d 7f1f80056200ce00"
        "6440080055f50100 19d0100043400600 55f5011022",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        59,
        59,
        59,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_path_switch_request(
        pkbuf_t **pkbuf, int target,
        c_uint32_t mme_ue_s1ap_id, c_uint32_t enb_ue_s1ap_id,
        int num_of_bearer, c_uint8_t ebi, c_uint32_t teid)
{
    status_t rv;
    int i;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_PathSwitchRequest_t *PathSwitchRequest = NULL;

    S1AP_PathSwitchRequestIEs_t *ie = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABToBeSwitchedDLList_t *E_RABToBeSwitchedDLList = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_TAI_t *TAI = NULL;
    S1AP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;

    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = 
        core_calloc(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_PathSwitchRequest;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_PathSwitchRequest;

    PathSwitchRequest = &initiatingMessage->value.choice.PathSwitchRequest;

    ie = core_calloc(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = core_calloc(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedDLList;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_PathSwitchRequestIEs__value_PR_E_RABToBeSwitchedDLList;

    E_RABToBeSwitchedDLList = &ie->value.choice.E_RABToBeSwitchedDLList;

    ie = core_calloc(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SourceMME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = core_calloc(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_EUTRAN_CGI;

    EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;

    ie = core_calloc(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_TAI;

    TAI = &ie->value.choice.TAI;

    ie = core_calloc(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_PathSwitchRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;
    *MME_UE_S1AP_ID = mme_ue_s1ap_id;

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(mme_ue_s1ap_id);
    d_assert(enb_ue, return CORE_ERROR,);
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, return CORE_ERROR,);

    for (i = 0; i < num_of_bearer; i++)
    {
        S1AP_E_RABToBeSwitchedDLItemIEs_t *item = NULL;
        S1AP_E_RABToBeSwitchedDLItem_t *e_rab = NULL;

        gtp_f_teid_t f_teid;
        ip_t ip;
        int len;

        item = core_calloc( 1, sizeof(S1AP_E_RABToBeSwitchedDLItemIEs_t));
        ASN_SEQUENCE_ADD(&E_RABToBeSwitchedDLList->list, item);

        item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedDLItem;
        item->criticality = S1AP_Criticality_reject;
        item->value.present = 
            S1AP_E_RABToBeSwitchedDLItemIEs__value_PR_E_RABToBeSwitchedDLItem;

        e_rab = &item->value.choice.E_RABToBeSwitchedDLItem;

        e_rab->e_RAB_ID = ebi+i;

        if (target == 0)
            rv = gtp_sockaddr_to_f_teid(
                    test_enb1_addr, test_enb1_addr6, &f_teid, &len);
        else
            rv = gtp_sockaddr_to_f_teid(
                    test_enb2_addr, NULL, &f_teid, &len);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
        rv = gtp_f_teid_to_ip(&f_teid, &ip);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        rv = s1ap_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
        s1ap_uint32_to_OCTET_STRING(teid+i, &e_rab->gTP_TEID);
    }

    s1ap_buffer_to_OCTET_STRING(
            &mme_ue->e_cgi.plmn_id, PLMN_ID_LEN, &EUTRAN_CGI->pLMNidentity);
    EUTRAN_CGI->cell_ID.size = 4;
    EUTRAN_CGI->cell_ID.buf =  core_calloc(
         EUTRAN_CGI->cell_ID.size, sizeof(c_uint8_t));
    d_assert(EUTRAN_CGI->cell_ID.buf, return CORE_ERROR,);
    EUTRAN_CGI->cell_ID.buf[0] = (mme_ue->e_cgi.cell_id >> 24);
    EUTRAN_CGI->cell_ID.buf[1] = (mme_ue->e_cgi.cell_id >> 16);
    EUTRAN_CGI->cell_ID.buf[2] = (mme_ue->e_cgi.cell_id >> 8);
    EUTRAN_CGI->cell_ID.buf[3] = (mme_ue->e_cgi.cell_id);
    EUTRAN_CGI->cell_ID.bits_unused = 4;

    s1ap_uint16_to_OCTET_STRING(
            mme_ue->tai.tac, &TAI->tAC);
    s1ap_buffer_to_OCTET_STRING(
            &mme_ue->tai.plmn_id, PLMN_ID_LEN, &TAI->pLMNidentity);

    UESecurityCapabilities->encryptionAlgorithms.size = 2;
    UESecurityCapabilities->encryptionAlgorithms.buf = 
        core_calloc(UESecurityCapabilities->encryptionAlgorithms.size, 
                    sizeof(c_uint8_t));
    UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->encryptionAlgorithms.buf[0] = 
        (mme_ue->ue_network_capability.eea << 1);

    UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf =
        core_calloc(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(c_uint8_t));
    UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] =
        (mme_ue->ue_network_capability.eia << 1);

    rv = s1ap_encode_pdu(pkbuf, &pdu);
    s1ap_free_pdu(&pdu);

    if (rv != CORE_OK)
    {
        d_error("s1ap_encode_pdu() failed");
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t tests1ap_build_handover_required(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "0000"
        "0081030000060000 0005c0020000c400 080002000c000100 0100000240020220"
        "0004000d0000f110 0000043000f11030 3900680080d080ce 4080ab0f1014c598"
        "00018000bf06ec4d 00100302c0000000 000015a800140672 0af00348f10013d0"
        "3bc58414903bc588 09401de2c2100440 13f212249010127d ad808fd50398381c"
        "08fd503983805f50 37868baa010c846f b057874162d62e82 1f42f2e159f821d0"
        "9c0009e28aa1a001 2d888026d0600040 8008066072003e41 5250d46210208184"
        "3423c135002214d6 dfb0a0b06502c402 000004008177faab 4677235334a0e867"
        "213e804030000100 4e40024500004e40 0246000000f11000 043a000000f11000"
        "1f20a0000022",

        "0000"
        "00810e0000060000 0005c0020000c500 0800020008000100 0100000240020220"
        "0004000d0000f110 00001f2000f11030 3900680080db80d9 4080ab0f1014c598"
        "00018000bf06ec4d 00100302c0000000 000015a800140672 0af003e8f10013d0"
        "3bc58414903bc588 09401de2c2100440 13f212249010127d ad808fd50398381c"
        "08fd503983805f50 37868baa010c846f b057874162d62e82 1f42f2e159f821d0"
        "9c0009e28aa1a001 2d888050d1bc0040 8008066072000874 1250d46210208184"
        "3423c135002214d6 dfb0a0b06502c402 000004008177faab 4677235334a0e867"
        "2134804030000100 4e40024500004e40 0246000000f11000 1f20a10000f11000"
        "043a000000350000 f110001f20a00000 22",

        "0000"
        "0081190000060000 0005c0020000c600 080002000d000100 0100000240020220"
        "0004000d0000f110 0000043000f11030 3900680080e680e4 4080ab0f1014c598"
        "00018000bf06ec4d 00100302c0000000 000015a800140672 0af00348f10013d0"
        "3bc58414903bc588 09401de2c2100440 13f212249010127d ad808fd50398381c"
        "08fd503983805f50 37868baa010c846f b057874162d62e82 1f42f2e159f821d0"
        "9c0009e28aa1a001 2d888050514c0040 8008066072003e41 5250d46210208184"
        "3423c135002214d6 dfb0a0b06502c402 000004008177faab 4677235334a0e867"
        "213e804030000100 4e40024500004e40 0246000000f11000 043a020000f11000"
        "1f20a00000340000 f11000043a000000 350000f110001f20 a0000022",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        264,
        275,
        286,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_handover_failure(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "4001 0012000002000040 05c0020000c5 000240020000",

        "",
        "",
    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        22,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

CORE_DECLARE(status_t) tests1ap_build_handover_request_ack(
        pkbuf_t **pkbuf, int target,
        c_uint32_t mme_ue_s1ap_id, c_uint32_t enb_ue_s1ap_id,
        int num_of_bearer, c_uint8_t ebi, c_uint32_t teid)
{
    status_t rv;
    int i;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_HandoverRequestAcknowledge_t *HandoverRequestAcknowledge = NULL;

    S1AP_HandoverRequestAcknowledgeIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABAdmittedList_t *E_RABAdmittedList = NULL;
    S1AP_E_RABFailedtoSetupListHOReqAck_t
        *E_RABFailedtoSetupListHOReqAck = NULL;
    S1AP_Target_ToSource_TransparentContainer_t
        *Target_ToSource_TransparentContainer = NULL;

    char hexbuf[MAX_SDU_LEN];
    char *payload =
        "00 80810bf900d8af40 00a0339057801f47 88009e81de2c20a4"
        "81de2c404a00ef16 2000010044013f21 2249008093efd243 3914cd2aa0a0142f"
        "f2214d6dfb82c194 0b10080000020040 bbfd55aeab41ad80 8fd50398381c08fd"
        "503983804d037868 baa016423342bc3a 18b58fa084ca833f a17970acfc10e84e"
        "0004f14550d00096 c88900";

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = 
        core_calloc(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_HandoverResourceAllocation;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_HandoverRequestAcknowledge;

    HandoverRequestAcknowledge =
        &successfulOutcome->value.choice.HandoverRequestAcknowledge;

    ie = core_calloc(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = core_calloc(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = core_calloc(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABAdmittedList;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_E_RABAdmittedList;

    E_RABAdmittedList = &ie->value.choice.E_RABAdmittedList;

    ie = core_calloc(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Target_ToSource_TransparentContainer;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverRequestAcknowledgeIEs__value_PR_Target_ToSource_TransparentContainer;

    Target_ToSource_TransparentContainer =
        &ie->value.choice.Target_ToSource_TransparentContainer;

    *MME_UE_S1AP_ID = mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;

    for (i = 0; i < num_of_bearer; i++)
    {
        S1AP_E_RABAdmittedItemIEs_t *item = NULL;
        S1AP_E_RABAdmittedItem_t *e_rab = NULL;

        gtp_f_teid_t f_teid;
        ip_t ip;
        int len;

        item = core_calloc(1, sizeof(S1AP_E_RABAdmittedItemIEs_t));
        ASN_SEQUENCE_ADD(&E_RABAdmittedList->list, item);

        item->id = S1AP_ProtocolIE_ID_id_E_RABAdmittedItem;
        item->criticality = S1AP_Criticality_ignore;
        item->value.present =
            S1AP_E_RABAdmittedItemIEs__value_PR_E_RABAdmittedItem;

        e_rab = &item->value.choice.E_RABAdmittedItem;

        e_rab->e_RAB_ID = ebi+i;

        if (target == 0)
            rv = gtp_sockaddr_to_f_teid(
                    test_enb1_addr, test_enb1_addr6, &f_teid, &len);
        else
            rv = gtp_sockaddr_to_f_teid(
                    test_enb2_addr, NULL, &f_teid, &len);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
        rv = gtp_f_teid_to_ip(&f_teid, &ip);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        rv = s1ap_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
        s1ap_uint32_to_OCTET_STRING(teid+i, &e_rab->gTP_TEID);

        e_rab->dL_transportLayerAddress =
            (S1AP_TransportLayerAddress_t *)
            core_calloc(1, sizeof(S1AP_TransportLayerAddress_t));
        rv = s1ap_ip_to_BIT_STRING(&ip, e_rab->dL_transportLayerAddress);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
        e_rab->dL_gTP_TEID = (S1AP_GTP_TEID_t *)
            core_calloc(1, sizeof(S1AP_GTP_TEID_t));
        s1ap_uint32_to_OCTET_STRING(teid+i+10, e_rab->dL_gTP_TEID);

        e_rab->uL_TransportLayerAddress =
            (S1AP_TransportLayerAddress_t *)
            core_calloc(1, sizeof(S1AP_TransportLayerAddress_t));
        rv = s1ap_ip_to_BIT_STRING(&ip, e_rab->uL_TransportLayerAddress);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
        e_rab->uL_GTP_TEID = (S1AP_GTP_TEID_t *)
            core_calloc(1, sizeof(S1AP_GTP_TEID_t));
        s1ap_uint32_to_OCTET_STRING(teid+i+20, e_rab->uL_GTP_TEID);
    }

    s1ap_buffer_to_OCTET_STRING(
            CORE_HEX(payload, strlen(payload), hexbuf), 132,
            Target_ToSource_TransparentContainer);

    rv = s1ap_encode_pdu(pkbuf, &pdu);
    s1ap_free_pdu(&pdu);

    if (rv != CORE_OK)
    {
        d_error("s1ap_encode_pdu() failed");
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t tests1ap_build_handover_request_ack_static(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "2001"
        "0080e80000040000 4005c0020000c500 0840020008001240 47010014401f78a1"
        "f07f000001010007 080f807f00000101 0007090f807f0000 010100070a001440"
        "1f78c1f07f000001 010007100f807f00 0001010007110f80 7f00000101000712"
        "007b008086808400 80810bf900d8af40 00a0339057801f47 88009e81de2c20a4"
        "81de2c404a00ef16 2000010044013f21 2249008093efd243 3914cd2aa0a0142f"
        "f2214d6dfb82c194 0b10080000020040 bbfd55aeab41ad80 8fd50398381c08fd"
        "503983804d037868 baa016423342bc3a 18b58fa084ca833f a17970acfc10e84e"
        "0004f14550d00096 c88900",

        "2001"
        "0080e80000040000 4005c0020000c600 084002000d001240 47010014401f78a1"
        "f00a01237b010009 080f800a01237b01 0009090f800a0123 7b0100090a001440"
        "1f78c1f00a01237b 010009100f800a01 237b010009110f80 0a01237b01000912"
        "007b008086808400 80810bf900d8af40 00a0339057801a47 88009e81de2c20a4"
        "81de2c404a00ef16 2000010044013f21 2249008093effa43 3914cd2aa0a0140f"
        "f2214d6dfb82c194 0b10080000020040 bbfd55aeab01ad80 8fd50398381c08fd"
        "503983804d037868 baa016423342bc3a 18b58fa084ca833f a17970acfc10e84e"
        "0004f14550d00096 c88980",

        "2001"
        "0080e80000040000 4005c0020000c700 0840020009001240 47010014401f78a1"
        "f00a01237c010008 080f800a01237c01 0008090f800a0123 7c0100080a001440"
        "1f78c1f00a01237c 010008100f800a01 237c010008110f80 0a01237c01000812"
        "007b008086808400 80810bf900d8af40 00a0339057801f47 88009e81de2c20a4"
        "81de2c404a00ef16 2000010044013f21 2249008093efd243 3914cd2aa0a0144f"
        "f2214d6dfb82c194 0b10080000020040 bbfd55aeab81ad80 8fd50398381c08fd"
        "503983804d037868 baa016423342bc3a 18b58fa084ca833f a17970acfc10e84e"
        "0004f14550d00096 c88a00",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        237,
        237,
        237,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_enb_status_transfer(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "0018"
        "4027000003000000 05c0020000c40008 0002000c005a0011 00000059400b0500"
        "004b000000001100 00",
        "0018"
        "4027000003000000 05c0020000c50008 00020008005a0011 00000059400b0500"
        "00a8000000004100 00",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        43,
        43,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_enb_configuration_transfer(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "0028"
        "403b000001008140 3440049699000004 3004969900020004 969900001f200496"
        "9900020000000098 401341f0ac110e02 0000009940070200 f8ac110e02",
        "0028"
        "403b000001008140 344004969900001f 2004969900020004 9699000004300496"
        "9900020000000098 401341f0ac110e02 0000009940070200 f8ac110e02",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        63,
        63,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_handover_notify(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "0002"
        "4028000004000000 05c0020000c50008 0002000800644008 0000f11000043a00"
        "004340060000f110 3039",
        "0002"
        "4028000004000000 05c0020000c60008 0002000d00644008 0000f110001f20a0"
        "004340060000f110 3039",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        44,
        44,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_handover_cancel(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "0004"
        "0018000003000000 05c0020000c60008 0002000d00024002 0120",
        "",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        28,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_s1_reset(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000e001500000200 0240020060005c00 084000005b000220 04",
        "",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        25,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t testgtpu_enb_connect(sock_id *new)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    c_sockaddr_t *addr = NULL;
    int family = AF_UNSPEC;

    if (test_only_control_plane) return CORE_OK;

    family = AF_INET6;
    if (context_self()->parameter.no_ipv6) family = AF_INET;
    else if (context_self()->parameter.prefer_ipv4) family = AF_INET;
    else if (test_enb1_addr6 == NULL) family = AF_INET;

    rv = udp_socket(new, family);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    if (family == AF_INET) addr = test_enb1_addr;
    else if (family == AF_INET6) addr = test_enb1_addr6;
    else
        d_assert(0, return CORE_ERROR,);

    d_assert(addr, return CORE_ERROR,);
    rv = sock_bind(*new, addr);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t testgtpu_enb2_connect(sock_id *new)
{
    status_t rv;
    int family = AF_UNSPEC;

    if (test_only_control_plane) return CORE_OK;

    family = AF_INET;

    rv = udp_socket(new, family);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    d_assert(test_enb2_addr, return CORE_ERROR,);
    rv = sock_bind(*new, test_enb2_addr);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t testgtpu_enb_close(sock_id sock)
{
    if (test_only_control_plane) return CORE_OK;

    return sock_delete(sock);
}

status_t testgtpu_enb_send(pkbuf_t *sendbuf)
{
    status_t rv;
    sock_id sock = 0;
    hash_index_t *hi = NULL;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    c_sockaddr_t sgw;
    ssize_t sent;

    if (test_only_control_plane) return 0;

    hi = mme_ue_first();
    d_assert(hi, return -1,);
    mme_ue = mme_ue_this(hi);
    d_assert(mme_ue, return -1,);
    sess = mme_sess_first(mme_ue);
    d_assert(sess, return -1,);
    bearer = mme_bearer_first(sess);
    d_assert(bearer, return -1,);

    memset(&sgw, 0, sizeof(c_sockaddr_t));
    sgw.c_sa_port = htons(GTPV1_U_UDP_PORT);
    if (bearer->sgw_s1u_ip.ipv6)
    {
        sgw.c_sa_family = AF_INET6;
        if (bearer->sgw_s1u_ip.ipv4)
            memcpy(sgw.sin6.sin6_addr.s6_addr,
                    bearer->sgw_s1u_ip.both.addr6, IPV6_LEN);
        else
            memcpy(sgw.sin6.sin6_addr.s6_addr,
                    bearer->sgw_s1u_ip.addr6, IPV6_LEN);
        rv = sock_fill_scope_id_in_local(&sgw);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }
    else
    {
        sgw.c_sa_family = AF_INET;
        sgw.sin.sin_addr.s_addr = bearer->sgw_s1u_ip.addr;
    }

    rv = udp_client(&sock, &sgw);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    sent = core_send(sock, sendbuf->payload, sendbuf->len, 0);
    pkbuf_free(sendbuf);
    if (sent < 0 || sent != sendbuf->len)
        return CORE_ERROR;

    sock_delete(sock);

    return CORE_OK;
}

status_t testgtpu_enb_read(sock_id sock, pkbuf_t *recvbuf)
{
    int rc = 0;

    if (test_only_control_plane) return 0;

    while(1)
    {
        rc = core_recv(sock, recvbuf->payload, recvbuf->len, 0);
        if (rc == -2) 
        {
            continue;
        }
        else if (rc <= 0)
        {
            if (errno == EAGAIN)
            {
                continue;
            }
            break;
        }
        else
        {
            break;
        }
    }
    recvbuf->len = rc;

    return CORE_OK;
}

c_uint16_t in_cksum(c_uint16_t *addr, int len); /* from pgw_gtp_path.c */

status_t testgtpu_build_ping(
        pkbuf_t **sendbuf, const char *src_ip, const char *dst_ip)
{
    status_t rv;
    pkbuf_t *pkbuf = NULL;
    gtp_header_t *gtp_h = NULL;
    ipsubnet_t src_ipsub, dst_ipsub;

    if (test_only_control_plane) return CORE_OK;

    d_assert(src_ip, return CORE_ERROR,);
    d_assert(dst_ip, return CORE_ERROR,);
    rv = core_ipsubnet(&src_ipsub, src_ip, NULL);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = core_ipsubnet(&dst_ipsub, dst_ip, NULL);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    pkbuf = pkbuf_alloc(0, 200 /* enough for ICMP; use smaller buffer */);
    d_assert(pkbuf, return CORE_ERROR,);
    memset(pkbuf->payload, 0, pkbuf->len);
    
    gtp_h = (gtp_header_t *)pkbuf->payload;
    gtp_h->flags = 0x30;
    gtp_h->type = GTPU_MSGTYPE_GPDU;
    gtp_h->teid = htonl(1);

    if (dst_ipsub.family == AF_INET)
    {
        struct ip *ip_h = NULL;
        struct icmp *icmp_h = NULL;

        gtp_h->length = htons(sizeof *ip_h + ICMP_MINLEN);

        ip_h = (struct ip *)(pkbuf->payload + GTPV1U_HEADER_LEN);
        icmp_h = (struct icmp *)((c_uint8_t *)ip_h + sizeof *ip_h);

        ip_h->ip_v = 4;
        ip_h->ip_hl = 5;
        ip_h->ip_tos = 0;
        ip_h->ip_id = rand();
        ip_h->ip_off = 0;
        ip_h->ip_ttl = 255;
        ip_h->ip_p = IPPROTO_ICMP;
        ip_h->ip_len = gtp_h->length;
        ip_h->ip_src.s_addr = src_ipsub.sub[0];
        ip_h->ip_dst.s_addr = dst_ipsub.sub[0];
        ip_h->ip_sum = in_cksum((c_uint16_t *)ip_h, sizeof *ip_h);
        
        icmp_h->icmp_type = 8;
        icmp_h->icmp_seq = rand();
        icmp_h->icmp_id = rand();
        icmp_h->icmp_cksum = in_cksum((c_uint16_t *)icmp_h, ICMP_MINLEN);
    }
    else if (dst_ipsub.family == AF_INET6)
    {
        struct ip6_hdr *ip6_h = NULL;
        struct icmp6_hdr *icmp6_h = NULL;
        c_uint16_t plen = 0;
        c_uint8_t nxt = 0;
        c_uint8_t *p = NULL;

        gtp_h->length = htons(sizeof *ip6_h + sizeof *icmp6_h);
        plen =  htons(sizeof *icmp6_h);
        nxt = IPPROTO_ICMPV6;

        p = (c_uint8_t *)pkbuf->payload + GTPV1U_HEADER_LEN;
        ip6_h = (struct ip6_hdr *)p;
        icmp6_h = (struct icmp6_hdr *)((c_uint8_t *)ip6_h + sizeof *ip6_h);

        memcpy(p, src_ipsub.sub, sizeof src_ipsub.sub);
        p += sizeof src_ipsub.sub;
        memcpy(p, dst_ipsub.sub, sizeof dst_ipsub.sub);
        p += sizeof dst_ipsub.sub;
        p += 2; memcpy(p, &plen, 2); p += 2;
        p += 3; *p = nxt; p += 1;

        icmp6_h->icmp6_type = ICMP6_ECHO_REQUEST;
        icmp6_h->icmp6_seq = rand();
        icmp6_h->icmp6_id = rand();

        icmp6_h->icmp6_cksum = in_cksum(
                (c_uint16_t *)ip6_h, sizeof *ip6_h + sizeof *icmp6_h);

        ip6_h->ip6_flow = htonl(0x60000001);
        ip6_h->ip6_plen = plen;
        ip6_h->ip6_nxt = nxt;;
        ip6_h->ip6_hlim = 0xff;
        memcpy(ip6_h->ip6_src.s6_addr, src_ipsub.sub, sizeof src_ipsub.sub);
        memcpy(ip6_h->ip6_dst.s6_addr, dst_ipsub.sub, sizeof dst_ipsub.sub);
    }
    else
        d_assert(0, return CORE_ERROR,);

    *sendbuf = pkbuf;

    return CORE_OK;
}

status_t testgtpu_build_slacc_rs(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "32ff003400000001 00000000 6000000000083aff fe80000000000000 0000000000000002"
        "ff02000000000000 0000000000000002 85007d3500000000",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        60,
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}


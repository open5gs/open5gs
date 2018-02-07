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

#include "app/context.h"

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
    return s1ap_send(id, sendbuf, NULL);
}

status_t tests1ap_build_setup_req(
        pkbuf_t **pkbuf, S1ap_ENB_ID_PR present, c_uint32_t enb_id)
{
    int erval = -1;
    int tac = 12345;
    plmn_id_t plmn_id;

    s1ap_message_t message;
    S1ap_S1SetupRequestIEs_t *ies;
    S1ap_PLMNidentity_t *plmnIdentity;
    S1ap_SupportedTAs_Item_t *supportedTA;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies = &message.s1ap_S1SetupRequestIEs;

    plmn_id_build(&plmn_id, 1, 1, 2);

    s1ap_uint32_to_ENB_ID(present, enb_id, &ies->global_ENB_ID.eNB_ID);
    s1ap_buffer_to_OCTET_STRING(
            &plmn_id, PLMN_ID_LEN, &ies->global_ENB_ID.pLMNidentity);

    supportedTA = (S1ap_SupportedTAs_Item_t *)
        core_calloc(1, sizeof(S1ap_SupportedTAs_Item_t));
    s1ap_uint16_to_OCTET_STRING(tac, &supportedTA->tAC);
    plmnIdentity = (S1ap_PLMNidentity_t *)
        core_calloc(1, sizeof(S1ap_PLMNidentity_t));
    s1ap_buffer_to_OCTET_STRING(
            &plmn_id, PLMN_ID_LEN, plmnIdentity);
    ASN_SEQUENCE_ADD(&supportedTA->broadcastPLMNs, plmnIdentity);

    ASN_SEQUENCE_ADD(&ies->supportedTAs, supportedTA);

    ies->defaultPagingDRX = S1ap_PagingDRX_v64;

    message.direction = S1AP_PDU_PR_initiatingMessage;
    message.procedureCode = S1ap_ProcedureCode_id_S1Setup;

    erval = s1ap_encode_pdu(pkbuf, &message);
    s1ap_free_pdu(&message);

    if (erval < 0)
    {
        d_error("s1ap_encode_error : (%d)", erval);
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

        "000c40809c00"
        "0005000800030002 00001a0073721711 a73a12070741020b f600f11000020100"
        "00000105e060c040 0100210221d011d1 271a808021100100 0010810600000000"
        "830600000000000d 00000a005255f501 10225c0a003103e5 c03e1355f501aaaa"
        "11035758a6200b60 1404ef65233b8878 d290400804026004 00021f025d0107e0"
        "004300060000f110 3039006440080055 f5010019d0100086 400130",

        "000c40809c00"
        "0005000800030003 00001a007372178c 3e3cff070741020b f600f11000020100"
        "00000905e060c040 0100210204d011d1 271a808021100100 0010810600000000"
        "830600000000000d 00000a005255f501 10225c0a003103e5 c03e1355f501aaaa"
        "11035758a6200b60 1404ef65233b8878 d290400804026004 00021f025d0107e0"
        "004300060000f110 3039006440080055 f5010019d0100086 400130",

        
        "000c406800000500 080002001f001a00 403f074172080910 10103254866202e0"
        "600021023cd011d1 271a808021100100 0010810600000000 830600000000000d"
        "00000a005c0a0090 11034f18a6f15d01 00004300060000f1 1030390064400800"
        "00f110002343d000 86400130",

        "000c405300000500 080003001100001a 002a2917acba67c4 8207410108091010"
        "103254866205f0f0 000000000e023cd0 11d1270780000a00 000d00c100430006"
        "0000f11030390064 40080000f1109d67 aa500086400130",

        "000c405300000500 080003002100001a 002a2917bcba67c4 8207410108091010"
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
        161,
        161,

        108,
        87,
        87,

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
        "403f000005000000 05c00100009f0008 0003000300001a00 121117d7e3248b57"
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
        67,

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
        "402e000005000000 0200030008000300 2100001a00040307 5c14006440080000"
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
        50,
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
    int erval = -1;

    status_t rv;
    gtp_f_teid_t f_teid;
    ip_t ip;
    int len;

    s1ap_message_t message;
    S1ap_InitialContextSetupResponseIEs_t *ies = NULL;
    S1ap_E_RABSetupItemCtxtSURes_t *e_rab = NULL;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies = &message.s1ap_InitialContextSetupResponseIEs;

    message.direction = S1AP_PDU_PR_successfulOutcome;
    message.procedureCode = S1ap_ProcedureCode_id_InitialContextSetup;

    ies->mme_ue_s1ap_id = mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue_s1ap_id;

    e_rab = (S1ap_E_RABSetupItemCtxtSURes_t *)
        core_calloc(1, sizeof(S1ap_E_RABSetupItemCtxtSURes_t));
    e_rab->e_RAB_ID = ebi;

    rv = gtp_sockaddr_to_f_teid(test_enb1_addr, test_enb1_addr6, &f_teid, &len);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_f_teid_to_ip(&f_teid, &ip);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = s1ap_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    s1ap_uint32_to_OCTET_STRING(teid, &e_rab->gTP_TEID);

    ASN_SEQUENCE_ADD(&ies->e_RABSetupListCtxtSURes, e_rab);

    erval = s1ap_encode_pdu(pkbuf, &message);
    s1ap_free_pdu(&message);

    if (erval < 0)
    {
        d_error("s1ap_encode_error : (%d)", erval);
        return CORE_ERROR;
    }

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
        "000c404900000600 080003000200001a 001615172ba435db 040745090bf600f1"
        "10000201020003e6 004300060000f110 2b67006440080000 f1109d67aa500086"
        "4001300060000600 40020003e6",
        "000c404900000600 080003000100001a 00161517ba258500 040745090bf600f1"
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
        77,
        77,
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
        "4038000006000800 03400700001a0005 04c7049551004300 060000f110303900"
        "6440080000f11007 87b8000086400140 0060000600400000 0001",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        60,
    };
    char hexbuf[MAX_SDU_LEN];
    int i = 0;
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    enb_ue_s1ap_id = htonl(enb_ue_s1ap_id << 8);
    memcpy((*pkbuf)->payload + 11, &enb_ue_s1ap_id, 3);
    mac = htons(mac);
    memcpy((*pkbuf)->payload + 20, &seq, 1);
    memcpy((*pkbuf)->payload + 21, &mac, 2);
    m_tmsi = htonl(m_tmsi);
    memcpy((*pkbuf)->payload + 56, &m_tmsi, 4);

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
        "000d40680000"
        "0500000005c00000 0001000800030001 00001a003b3a277c 81dab50a0205d011"
        "281208696e746572 6e6574036e673204 6d6e6574271a8080 2110010000108106"
        "0000000083060000 0000000d00000a00 006440080055f501 0019d01000434006"
        "0055f5011022",
        "000d40680000"
        "0500000005c00000 0001000800030001 00001a003b3a2710 3fdafa0a0209d011"
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
        "000d40380000"
        "0500000005c08000 0108000800030001 00001a000b0a2732 423c53040206d206"
        "006440080055f501 0019d01000434006 0055f5011022",
        "000d40380000"
        "0500000005c08000 0108000800030001 00001a000b0a2777 693066040206d206"
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
        60,
        60,
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
    int erval = -1;

    status_t rv;
    gtp_f_teid_t f_teid;
    ip_t ip;
    int len;

    s1ap_message_t message;
    S1ap_E_RABSetupResponseIEs_t *ies = NULL;
    S1ap_E_RABSetupItemBearerSURes_t *e_rab = NULL;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies = &message.s1ap_E_RABSetupResponseIEs;

    message.direction = S1AP_PDU_PR_successfulOutcome;
    message.procedureCode = S1ap_ProcedureCode_id_E_RABSetup;

    ies->mme_ue_s1ap_id = mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue_s1ap_id;

    e_rab = (S1ap_E_RABSetupItemBearerSURes_t *)
        core_calloc(1, sizeof(S1ap_E_RABSetupItemBearerSURes_t));
    e_rab->e_RAB_ID = ebi;
 
    rv = gtp_sockaddr_to_f_teid(test_enb1_addr, test_enb1_addr6, &f_teid, &len);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_f_teid_to_ip(&f_teid, &ip);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = s1ap_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    s1ap_uint32_to_OCTET_STRING(teid, &e_rab->gTP_TEID);

    ies->presenceMask |= 
           S1AP_E_RABSETUPRESPONSEIES_E_RABSETUPLISTBEARERSURES_PRESENT;
    ASN_SEQUENCE_ADD(&ies->e_RABSetupListBearerSURes, e_rab);

    erval = s1ap_encode_pdu(pkbuf, &message);
    s1ap_free_pdu(&message);

    if (erval < 0)
    {
        d_error("s1ap_encode_error : (%d)", erval);
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
        "2007001d0000"
        "0300004005c00000 0001000840030001 000045400600000f 40010c",
        "2007001d0000"
        "0300004005c00000 0001000840030001 000045400600000f 40010e",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        33,
        33,
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
        "000d40370000"
        "0500000005c08000 0107000800030001 00001a000a0927e7 f5bb400b6200c200"
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
        59,
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
        "000d403700000500"
        "000005c000000001 0008000300010000 1a000a0927078a5f 34037200c6006440"
        "080000f1109d67aa 50004340060000f1 102b67",
        "000d403700000500"
        "000005c000000001 0008000300010000 1a000a0927c035da 96036200c6006440"
        "080000f1109d67aa 50004340060000f1 102b67",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

        "000d403700000500"
        "000005c00200003c 0008000300010000 1a000a0927a27f49 d6036200c6006440"
        "080000f1109d67aa 50004340060000f1 102b67",

        "000d"
        "4036000005000000 05c0020000c40008 0002000c001a000a 0927e3c77e000362"
        "00c6006440080000 f110001f20a00043 40060000f1103039",
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

        59,
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
        "000d40370000"
        "0500000005c00000 0001000800030001 00001a000a09274c b2ebbd056200ce00"
        "6440080055f50100 19d0100043400600 55f5011022",
        "000d40370000"
        "0500000005c00000 0001000800030001 00001a000a0927a9 a1b2bd057200ce00"
        "6440080055f50100 19d0100043400600 55f5011022",
        "000d40370000"
        "0500000005c00000 0001000800030001 00001a000a09277d 7f1f80056200ce00"
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
    int erval = -1;

    status_t rv;
    int i;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    s1ap_message_t message;
    S1ap_PathSwitchRequestIEs_t *ies =
            &message.s1ap_PathSwitchRequestIEs;
    S1ap_E_RABToBeSwitchedDLItem_t *e_rab = NULL;
    S1ap_EUTRAN_CGI_t *eutran_cgi = &ies->eutran_cgi;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->eNB_UE_S1AP_ID = enb_ue_s1ap_id;
    ies->sourceMME_UE_S1AP_ID = mme_ue_s1ap_id;

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(mme_ue_s1ap_id);
    d_assert(enb_ue, return CORE_ERROR,);
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, return CORE_ERROR,);

    for (i = 0; i < num_of_bearer; i++)
    {
        gtp_f_teid_t f_teid;
        ip_t ip;
        int len;

        e_rab = (S1ap_E_RABToBeSwitchedDLItem_t *)
            core_calloc(1, sizeof(S1ap_E_RABToBeSwitchedDLItem_t));
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

        ASN_SEQUENCE_ADD(&ies->e_RABToBeSwitchedDLList, e_rab);
    }

    s1ap_buffer_to_OCTET_STRING(
            &mme_ue->e_cgi.plmn_id, PLMN_ID_LEN, &ies->eutran_cgi.pLMNidentity);
    eutran_cgi->cell_ID.size = 4;
    eutran_cgi->cell_ID.buf =  core_calloc(
         eutran_cgi->cell_ID.size, sizeof(c_uint8_t));
    d_assert(eutran_cgi->cell_ID.buf, return CORE_ERROR,);
    eutran_cgi->cell_ID.buf[0] = (mme_ue->e_cgi.cell_id >> 24);
    eutran_cgi->cell_ID.buf[1] = (mme_ue->e_cgi.cell_id >> 16);
    eutran_cgi->cell_ID.buf[2] = (mme_ue->e_cgi.cell_id >> 8);
    eutran_cgi->cell_ID.buf[3] = (mme_ue->e_cgi.cell_id);
    eutran_cgi->cell_ID.bits_unused = 4;

    s1ap_uint16_to_OCTET_STRING(
            mme_ue->tai.tac, &ies->tai.tAC);
    s1ap_buffer_to_OCTET_STRING(
            &mme_ue->tai.plmn_id, PLMN_ID_LEN, &ies->tai.pLMNidentity);

    ies->ueSecurityCapabilities.encryptionAlgorithms.size = 2;
    ies->ueSecurityCapabilities.encryptionAlgorithms.buf = 
        core_calloc(ies->ueSecurityCapabilities.encryptionAlgorithms.size, 
                    sizeof(c_uint8_t));
    ies->ueSecurityCapabilities.encryptionAlgorithms.bits_unused = 0;
    ies->ueSecurityCapabilities.encryptionAlgorithms.buf[0] = 
        (mme_ue->ue_network_capability.eea << 1);

    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.size = 2;
    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.buf =
        core_calloc(ies->ueSecurityCapabilities.
                        integrityProtectionAlgorithms.size, sizeof(c_uint8_t));
    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.bits_unused = 0;
    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.buf[0] =
        (mme_ue->ue_network_capability.eia << 1);

    message.procedureCode = S1ap_ProcedureCode_id_PathSwitchRequest;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    erval = s1ap_encode_pdu(pkbuf, &message);
    s1ap_free_pdu(&message);

    if (erval < 0)
    {
        d_error("s1ap_encode_error : (%d)", erval);
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

CORE_DECLARE(status_t) tests1ap_build_handover_request_ack(
        pkbuf_t **pkbuf, int target,
        c_uint32_t mme_ue_s1ap_id, c_uint32_t enb_ue_s1ap_id,
        int num_of_bearer, c_uint8_t ebi, c_uint32_t teid)
{
    char hexbuf[MAX_SDU_LEN];
    char *payload =
        "00 80810bf900d8af40 00a0339057801f47 88009e81de2c20a4"
        "81de2c404a00ef16 2000010044013f21 2249008093efd243 3914cd2aa0a0142f"
        "f2214d6dfb82c194 0b10080000020040 bbfd55aeab41ad80 8fd50398381c08fd"
        "503983804d037868 baa016423342bc3a 18b58fa084ca833f a17970acfc10e84e"
        "0004f14550d00096 c88900";
    int erval = -1;

    status_t rv;
    int i;

    s1ap_message_t message;
    S1ap_HandoverRequestAcknowledgeIEs_t *ies =
            &message.s1ap_HandoverRequestAcknowledgeIEs;
    S1ap_E_RABAdmittedItem_t *e_rab = NULL;
    S1ap_Target_ToSource_TransparentContainer_t *container =
        &ies->target_ToSource_TransparentContainer;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->eNB_UE_S1AP_ID = enb_ue_s1ap_id;
    ies->mme_ue_s1ap_id = mme_ue_s1ap_id;

   for (i = 0; i < num_of_bearer; i++)
    {
        gtp_f_teid_t f_teid;
        ip_t ip;
        int len;

        e_rab = (S1ap_E_RABAdmittedItem_t *)
            core_calloc(1, sizeof(S1ap_E_RABAdmittedItem_t));
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
            (S1ap_TransportLayerAddress_t *)
            core_calloc(1, sizeof(S1ap_TransportLayerAddress_t));
        rv = s1ap_ip_to_BIT_STRING(&ip, e_rab->dL_transportLayerAddress);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
        e_rab->dL_gTP_TEID = (S1ap_GTP_TEID_t *)
            core_calloc(1, sizeof(S1ap_GTP_TEID_t));
        s1ap_uint32_to_OCTET_STRING(teid+i+10, e_rab->dL_gTP_TEID);

        e_rab->uL_S1ap_TransportLayerAddress =
            (S1ap_TransportLayerAddress_t *)
            core_calloc(1, sizeof(S1ap_TransportLayerAddress_t));
        rv = s1ap_ip_to_BIT_STRING(&ip, e_rab->uL_S1ap_TransportLayerAddress);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
        e_rab->uL_S1ap_GTP_TEID = (S1ap_GTP_TEID_t *)
            core_calloc(1, sizeof(S1ap_GTP_TEID_t));
        s1ap_uint32_to_OCTET_STRING(teid+i+20, e_rab->uL_S1ap_GTP_TEID);

        ASN_SEQUENCE_ADD(&ies->e_RABAdmittedList, e_rab);
    }

    s1ap_buffer_to_OCTET_STRING(
            CORE_HEX(payload, strlen(payload), hexbuf), 132, container);

    message.procedureCode = S1ap_ProcedureCode_id_HandoverResourceAllocation;
    message.direction = S1AP_PDU_PR_successfulOutcome;

    erval = s1ap_encode_pdu(pkbuf, &message);
    s1ap_free_pdu(&message);

    if (erval < 0)
    {
        d_error("s1ap_encode_error : (%d)", erval);
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


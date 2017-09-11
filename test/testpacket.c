#define TRACE_MODULE _s1enbmsg

#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"

#include "mme_context.h"
#include "s1ap_build.h"
#include "s1ap_conv.h"
#include "s1ap_path.h"

net_sock_t *tests1ap_enb_connect(void)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    mme_context_t *mme = mme_self();
    net_sock_t *sock = NULL;

    if (!mme) return NULL;

    rv = net_open_ext(&sock, mme->s1ap_addr, 
            INET_NTOP(&mme->s1ap_addr, buf), 0, mme->s1ap_port, 
            SOCK_SEQPACKET, IPPROTO_SCTP, SCTP_S1AP_PPID, 0);
    if (rv != CORE_OK) return NULL;

    return sock;
}

status_t tests1ap_enb_close(net_sock_t *sock)
{
    return net_close(sock);
}

int tests1ap_enb_send(net_sock_t *sock, pkbuf_t *sendbuf)
{
    return s1ap_send(sock, sendbuf);
}

int tests1ap_enb_read(net_sock_t *sock, pkbuf_t *recvbuf)
{
    int rc = 0;

    while(1)
    {
        rc = net_read(sock, recvbuf->payload, recvbuf->len, 0);
        if (rc == -2) 
        {
            continue;
        }
        else if (rc <= 0)
        {
            if (sock->sndrcv_errno == EAGAIN)
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

    return rc;
}


status_t tests1ap_build_setup_req(
        pkbuf_t **pkbuf, S1ap_ENB_ID_PR present, c_uint32_t enb_id)
{
    int erval = -1;

    s1ap_message_t message;
    S1ap_S1SetupRequestIEs_t *ies;
    S1ap_PLMNidentity_t *plmnIdentity;
    S1ap_SupportedTAs_Item_t *supportedTA;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies = &message.s1ap_S1SetupRequestIEs;

    s1ap_uint32_to_ENB_ID(present, enb_id, &ies->global_ENB_ID.eNB_ID);
    s1ap_buffer_to_OCTET_STRING(&mme_self()->served_tai[0].plmn_id,
            PLMN_ID_LEN, &ies->global_ENB_ID.pLMNidentity);

    supportedTA = (S1ap_SupportedTAs_Item_t *)
        core_calloc(1, sizeof(S1ap_SupportedTAs_Item_t));
    s1ap_uint16_to_OCTET_STRING(
            mme_self()->served_tai[0].tac, &supportedTA->tAC);
    plmnIdentity = (S1ap_PLMNidentity_t *)
        core_calloc(1, sizeof(S1ap_PLMNidentity_t));
    s1ap_buffer_to_OCTET_STRING(&mme_self()->served_tai[0].plmn_id,
            PLMN_ID_LEN, plmnIdentity);
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

#define TESTS1AP_MAX_MESSAGE 10

status_t tests1ap_build_initial_ue_msg(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000c405800000500 0800020001001a00 302f177ca0b38802 0741020809101010"
        "3254869104e060c0 4000050221d011d1 5c0a003103e5e034 9011035758a65d01"
        "00004300060000f1 105ba00064400800 00f1101079baf000 86400130",

        "000c40809c00"
        "0005000800030002 00001a0073721711 a73a12070741020b f600f11000020100"
        "00000105e060c040 0100210221d011d1 271a808021100100 0010810600000000"
        "830600000000000d 00000a005255f501 10225c0a003103e5 c03e1355f501aaaa"
        "11035758a6200b60 1404ef65233b8878 d290400804026004 00021f025d0107e0"
        "004300060055f501 1022006440080055 f5010019d0100086 400130",

        "000c40809c00"
        "0005000800030001 00001a007372178c 3e3cff070741020b f600f11000020100"
        "00000905e060c040 0100210204d011d1 271a808021100100 0010810600000000"
        "830600000000000d 00000a005255f501 10225c0a003103e5 c03e1355f501aaaa"
        "11035758a6200b60 1404ef65233b8878 d290400804026004 00021f025d0107e0"
        "004300060055f501 1022006440080055 f5010019d0100086 400130",

        
        "000c406800000500 080002001f001a00 403f074172080910 10103254866202e0"
        "600021023cd011d1 271a808021100100 0010810600000000 830600000000000d"
        "00000a005c0a0090 11034f18a6f15d01 00004300060000f1 1030390064400800"
        "00f110002343d000 86400130",

        "000c405300000500 080003001100001a 002a2917acba67c4 8207410108091010"
        "103254866205f0f0 000000000e023cd0 11d1270780000a00 000d00c100430006"
        "0000f1102b670064 40080000f1109d67 aa500086400130",

        "000c405300000500 080003002100001a 002a2917bcba67c4 8207410108091010"
        "103254866205f0f0 000000000e023cd0 11d1270780000a00 000d00c100430006"
        "0000f1102b670064 40080000f1109d67 aa500086400130",


        "000c"
        "404c000005000800 020002001a002423 0741710809101010 3254767905f0f000"
        "0000000e0201d011 d1270780000a0000 0d00c10043000600 00f1102b67006440"
        "080000f11054f640 100086400130",
        "",
        "",


        "000c"
        "4068000005000800 020001001a00403f 0741720809101010 3254861002e06000"
        "210207d011d1271a 8080211001000010 8106000000008306 00000000000d0000"
        "0a005c0a00901103 4f18a6f15d010000 4300060000f1105b a0006440080000f1"
        "1004615380008640 0130",
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
        0,

        108,
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
        "403f000005000000 05c00100009f0008 0003000100001a00 121117d7e3248b57"
        "0756080910101032 5486510064400800 00f11054f6401000 4340060000f1105b"
        "a0",

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
        67,

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
        "",

        "000d"
        "4038000005000000 05c0010001da0008 00020001001a000c 0b0753084ce11ef1"
        "24b1854500644008 0000f11004615380 004340060000f110 5ba0",
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
        0,

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
        "",

        "000d"
        "4035000005000000 05c0010001da0008 00020001001a0009 0847d3b0ef030007"
        "5e006440080000f1 1004615380004340 060000f1105ba0",

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
        0,

        57,
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
        "",
        "",

        "000d403e00000500 0000020001000800 02001f001a001514 27505a0b5301023c"
        "da280908696e7465 726e657400644008 0000f110002343d0 004340060000f110"
        "3039",
        "",
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
    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        146,
        0,
        0,

        66,
        0,
        0,

        69,
        0,
        0,

        69,
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
    };
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = len[i];
    memcpy((*pkbuf)->payload, CORE_HEX(payload[i], strlen(payload[i]), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_initial_context_setup_response(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "2009"
        "0025000003000040 05c00100009d0008 400200010033400f 000032400a0a1f0a"
        "012d2801000008",
        "",
        "",

        "2009002200000300 0040020001000840 02001f0033400f00 0032400a0a1f0a01"
        "23c501000508",
        "",
        "",

        "2009"
        "0025000003000040 05c0020000c80008 400200020033400f 000032400a0a1f0a"
        "0123a701000008",

        "2009"
        "0025000003000040 05c0020000c70008 400200040033400f 000032400a0a1f0a"
        "0123a701000208",

        "",


        "2009"
        "0025000003000040 05c0010001da0008 400200010033400f 000032400a0a1f0a"
        "012d8301000008",
    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        41,
        0,
        0,

        38,
        0,
        0,

        41,
        41,
        0,

        41,
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


        "",
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

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        62,
        62,
        0,

        0,
        62,
        0,

        62,
        0,
        0,

        62,
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

        "000d403300000500 0000020001000800 02001f001a000a09 27574292cc020760"
        "65006440080000f1 10002343d0004340 060000f1103039",

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

        55,
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

status_t tests1ap_build_detach_request(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000c404900000600 080003000100001a 001615172ba435db 040745090bf600f1"
        "10000201020003e6 004300060000f110 2b67006440080000 f1109d67aa500086"
        "4001300060000600 40020003e6",

        "",
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

status_t tests1ap_build_ue_context_release_request(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "",
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
        0,
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
        "2017001300000200 004005c00100009d 00084003400527",
        "",
        "2017"
        "0012000002000040 05c00100009f0008 40020001",

        "",
        "",
        "",

        "2017"
        "0012000002000040 05c0020000c80008 40020002",
        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        23,
        0,
        23,

        0,
        0,
        0,

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

status_t tests1ap_build_service_request(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "",
        "",
        "",

        "",
        "",
        "",

        "000c"
        "4037000006000800 020004001a000504 c704d4b800430006 0000f1102b670064"
        "40080000f11054f6 4010008640014000 6000060040000000 03",

        "",
        "",

    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        0,
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

status_t tests1ap_build_e_rab_setup_response(pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "2005002600000300 004005c00200003c 0008400300010000 1c400f000027400a"
        "0c1f0a012da50100 b410",
        "2005002600000300 004005c00200003c 0008400300010000 1c400f000027400a"
        "0e1f0a012da50100 b410",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

        "2005002600000300 004005c00200003c 0008400300010000 1c400f000027400a"
        "0c1f0a012da50100 b410",
    };
    c_uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        42,
        42,
        0,

        0,
        0,
        0,

        0,
        0,
        0,

        42,
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
        "0300004005c08000 0108000840030001 000045400600000f 40010c",
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
        33,
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
        "000005c00200003c 0008000300010000 1a000a0927078a5f 34037200c6006440"
        "080000f1109d67aa 50004340060000f1 102b67",
        "000d403700000500"
        "000005c00200003c 0008000300010000 1a000a0927c035da 96036200c6006440"
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
        "0500000005c08000 0108000800030001 00001a000a09274c b2ebbd056200ce00"
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

status_t tests1ap_build_pdn_connectivity_request(
        pkbuf_t **pkbuf, int i)
{
    char *payload[TESTS1AP_MAX_MESSAGE] = { 
        "000d40680000"
        "0500000005c08000 0107000800030001 00001a003b3a277c 81dab50a0205d011"
        "281208696e746572 6e6574036e673204 6d6e6574271a8080 2110010000108106"
        "0000000083060000 0000000d00000a00 006440080055f501 0019d01000434006"
        "0055f5011022",
        "000d40680000"
        "0500000005c08000 0107000800030001 00001a003b3a2710 3fdafa0a0209d011"
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
        60,
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

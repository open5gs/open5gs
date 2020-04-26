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

#include "ogs-gtp.h"
#include "ogs-sctp.h"
#include "ogs-app.h"

#include "pgw/pgw-config.h"

#if HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif

#if HAVE_NETINET_IP6_H
#include <netinet/ip6.h>
#endif

#if HAVE_NETINET_IP_ICMP_H
#include <netinet/ip_icmp.h>
#endif

#if HAVE_NETINET_ICMP6_H
#include <netinet/icmp6.h>
#endif

#include "mme/s1ap-build.h"
#include "mme/s1ap-path.h"

#include "mme/sgsap-path.h"

ogs_socknode_t *testsctp_server(const char *ipstr)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, ipstr, OGS_SGSAP_SCTP_PORT, 0);
    ogs_assert(rv == OGS_OK);

    node = ogs_socknode_new(addr);
    ogs_assert(node);
    ogs_socknode_nodelay(node, true);

    ogs_sctp_server(SOCK_SEQPACKET, node);
    ogs_assert(node->sock);

    return node;
}

ogs_socknode_t *testsctp_client(const char *ipstr)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, ipstr, OGS_S1AP_SCTP_PORT, 0);
    ogs_assert(rv == OGS_OK);

    node = ogs_socknode_new(addr);
    ogs_assert(node);
    ogs_socknode_nodelay(node, true);

    ogs_sctp_client(SOCK_STREAM, node);
    ogs_assert(node->sock);

    return node;
}

static ogs_sockaddr_t sctp_last_addr;

ogs_pkbuf_t *testsctp_read(ogs_socknode_t *node, int type)
{
    int size;
    ogs_pkbuf_t *recvbuf = NULL;

    ogs_assert(node);
    ogs_assert(node->sock);

    recvbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put(recvbuf, OGS_MAX_SDU_LEN);

    size = ogs_sctp_recvdata(node->sock, recvbuf->data, OGS_MAX_SDU_LEN,
            type == 1 ? &sctp_last_addr : NULL, NULL);
    if (size <= 0) {
        ogs_error("sgsap_recv() failed");
        return NULL;
    }

    ogs_pkbuf_trim(recvbuf, size);
    return recvbuf;;
}

int testenb_s1ap_send(ogs_socknode_t *node, ogs_pkbuf_t *sendbuf)
{
    return s1ap_send(node->sock, sendbuf, NULL, 0);
}

int testvlr_sgsap_send(ogs_socknode_t *node, ogs_pkbuf_t *sendbuf)
{
    return sgsap_send(node->sock, sendbuf, &sctp_last_addr, 0);
}

ogs_socknode_t *testenb_gtpu_server(const char *ipstr)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, ipstr, OGS_GTPV1_U_UDP_PORT, 0);
    ogs_assert(rv == OGS_OK);

    node = ogs_socknode_new(addr);
    ogs_assert(node);

    sock = ogs_udp_server(node);
    ogs_assert(sock);

    return node;
}

ogs_pkbuf_t *testenb_gtpu_read(ogs_socknode_t *node)
{
    int rc = 0;
    ogs_pkbuf_t *recvbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put(recvbuf, OGS_MAX_SDU_LEN);

    ogs_assert(node);
    ogs_assert(node->sock);

    while (1) {
        rc = ogs_recv(node->sock->fd, recvbuf->data, recvbuf->len, 0);
        if (rc <= 0) {
            if (errno == EAGAIN) {
                continue;
            }
            break;
        } else {
            break;
        }
    }
    recvbuf->len = rc;

    return recvbuf;
}

bool test_no_mme_self = 0;

int testenb_gtpu_send(ogs_socknode_t *node, ogs_pkbuf_t *sendbuf)
{
    int rv;
    ogs_hash_index_t *hi = NULL;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    ogs_sockaddr_t sgw;
    ssize_t sent;

    ogs_assert(node);
    ogs_assert(node->sock);

    memset(&sgw, 0, sizeof(ogs_sockaddr_t));
    sgw.ogs_sin_port = htons(OGS_GTPV1_U_UDP_PORT);
    if (test_no_mme_self) {
        sgw.ogs_sa_family = AF_INET;
        sgw.sin.sin_addr.s_addr = inet_addr("127.0.0.2");
    } else {
        mme_ue = ogs_list_first(&mme_self()->mme_ue_list);
        ogs_assert(mme_ue);
        sess = mme_sess_first(mme_ue);
        ogs_assert(sess);
        bearer = mme_bearer_first(sess);
        ogs_assert(bearer);

        if (bearer->sgw_s1u_ip.ipv6) {
            sgw.ogs_sa_family = AF_INET6;
            if (bearer->sgw_s1u_ip.ipv4)
                memcpy(sgw.sin6.sin6_addr.s6_addr,
                        bearer->sgw_s1u_ip.both.addr6, OGS_IPV6_LEN);
            else
                memcpy(sgw.sin6.sin6_addr.s6_addr,
                        bearer->sgw_s1u_ip.addr6, OGS_IPV6_LEN);
            rv = ogs_socknode_fill_scope_id_in_local(&sgw);
            ogs_assert(rv == OGS_OK);
        } else {
            sgw.ogs_sa_family = AF_INET;
            sgw.sin.sin_addr.s_addr = bearer->sgw_s1u_ip.addr;
        }
    }

    sent = ogs_sendto(node->sock->fd, sendbuf->data, sendbuf->len, 0, &sgw);
    ogs_pkbuf_free(sendbuf);
    if (sent < 0 || sent != sendbuf->len)
        return OGS_ERROR;

    return OGS_OK;
}

void testenb_gtpu_close(ogs_socknode_t *node)
{
    ogs_socknode_free(node);
}

int tests1ap_build_setup_req(
        ogs_pkbuf_t **pkbuf, S1AP_ENB_ID_PR present, uint32_t enb_id,
        int tac, uint16_t mcc, uint16_t mnc, uint16_t mnc_len)
{
    int rv;

    ogs_plmn_id_t plmn_id;

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
        CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_S1Setup;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_S1SetupRequest;

    S1SetupRequest = &initiatingMessage->value.choice.S1SetupRequest;

    ie = CALLOC(1, sizeof(S1AP_S1SetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&S1SetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Global_ENB_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_Global_ENB_ID;

    Global_ENB_ID = &ie->value.choice.Global_ENB_ID;

    ie = CALLOC(1, sizeof(S1AP_S1SetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&S1SetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SupportedTAs;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_SupportedTAs;

    SupportedTAs = &ie->value.choice.SupportedTAs;

    ie = CALLOC(1, sizeof(S1AP_S1SetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&S1SetupRequest->protocolIEs, ie);
    
    ie->id = S1AP_ProtocolIE_ID_id_DefaultPagingDRX;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_PagingDRX;

    PagingDRX = &ie->value.choice.PagingDRX;

    ogs_plmn_id_build(&plmn_id, mcc, mnc, mnc_len);

    ogs_s1ap_uint32_to_ENB_ID(present, enb_id, &Global_ENB_ID->eNB_ID);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &plmn_id, OGS_PLMN_ID_LEN, &Global_ENB_ID->pLMNidentity);

    SupportedTAs_Item = (S1AP_SupportedTAs_Item_t *)
        CALLOC(1, sizeof(S1AP_SupportedTAs_Item_t));
    ogs_s1ap_uint16_to_OCTET_STRING(tac, &SupportedTAs_Item->tAC);
    PLMNidentity = (S1AP_PLMNidentity_t *)
        CALLOC(1, sizeof(S1AP_PLMNidentity_t));
    ogs_s1ap_buffer_to_OCTET_STRING(
            &plmn_id, OGS_PLMN_ID_LEN, PLMNidentity);
    ASN_SEQUENCE_ADD(&SupportedTAs_Item->broadcastPLMNs.list, PLMNidentity);

    ASN_SEQUENCE_ADD(&SupportedTAs->list, SupportedTAs_Item);

    *PagingDRX = S1AP_PagingDRX_v64;

    *pkbuf = ogs_s1ap_encode(&pdu);
    if (*pkbuf == NULL) {
        ogs_error("ogs_s1ap_encode() failed");
        return OGS_ERROR;
    }
    return OGS_OK;
}

#define TESTS1AP_MAX_MESSAGE 64

int tests1ap_build_invalid_packet(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "0011002100000300 3b40080062f22400 0001700040000700 00004062f224002c"
        "00030a0100",
        "",
    };

    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        37,
        0,
    };

    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_initial_ue_msg(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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

        
        /* 3 */
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


        /* 6 */
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


        /* 9 */
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

        /* 12 */
        "000c"
        "4080810000050008 000340072d001a00 5857074172080910 10103254967305e0"
        "60c0401100270209 d031d12720808021 1001000010810600 0000008306000000"
        "00000d0000030000 0a000010005c0a00 3103e5e02e901103 5758a64008040260"
        "0000021f005d0103 c1004300060000f1 1030390064400800 00f1100787b80000"
        "86400130",
        "",
        "",

        /* 15 */
        "000c"
        "404c000005000800 020018001a002423 0741710839011094 7856044005e0e000"
        "0000000e0201d011 d1270780000a0000 0d00c10043000600 1300410033006440"
        "08001300418d01b7 800086400130",
        "",
        "",

        /* 18 */
        "000c408096000005 000800020001001a 006e6d177d3278f5 090741020bf609f1"
        "07000201ca001310 05f0f0c040090021 0234d011d1271a80 8021100101001081"
        "0600000000830600 000000000d000010 005209f10700075c 08033103e5e07e90"
        "1103571882200a60 140462918100127e 00400800021f0004 0240045d0100e0c1"
        "004300060009f107 0007006440080009 f1070019b0100086 400130",
        "000c4080b1000005 000800048003e993 001a008086808417 6e2da9da06074102"
        "0bf627f412000201 f700571805f070c0 4011002d026ed031 d127268080211001"
        "0000108106000000 0083060000000000 0d00000300000a00 0005000010000011"
        "005227f41230395c 0a003103e5e03e13 27f412c958110357 58a6200c601404ef"
        "65233b8878d2f280 4008040260040002 1f025d0103e0c110 0203800043000600"
        "27f4123039006440 080027f412000640 200086400130",
        "",

        /* 21 */
        "000c407400000700 08000200d0001a00 3c3b17c000c55201 0741010bf600f110"
        "000201c5001d9005 f0f0000000001702 01d011d027108000 0a00000d00001000"
        "ff00031301845264 f0130002e0004300 060000f110303900 6440080000f11000"
        "0640100086400130 004b00070000f110 02000100aa400100",
        "000c407400000700 08000200d1001a00 3c3b17f1f96e4a02 0741010bf600f110"
        "000201c5001d9005 f0f0000000001702 01d011d027108000 0a00000d00001000"
        "ff00031301845264 f0130002e0004300 060000f110303900 6440080000f11000"
        "0640100086400130 004b00070000f110 02000100aa400100",
        "",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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

        80,
        0,
        0,

        155,
        182,
        0,

        /* 21 */
        120,
        120,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_identity_response(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "",
        "",
        "000d"
        "403e000005000000 05c00100009f0008 00020003001a00 121117d7e3248b57"
        "0756080910101032 5486510064400800 00f11054f6401000 4340060000f1105b"
        "a0",

        /* 3 */
        "",
        "",
        "",

        "",
        "",
        "",
        
        /* 9 */
        "",
        "000d"
        "403e000005000000 05c0020000c40008 0002000c001a0012 1117c07eb5f50607"
        "5608091010103254 8651006440080000 f110001f20a00043 40060000f1103039",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

        /* 18 */
        "000d403b00000500 0000020001000800 020001001a001211 177b973f4c0a0756"
        "0829262400001118 93006440080009f1 070019b010004340 060009f1070007",
        "000d403d00000500 00000200f8000800 048003e993001a00 121117c651b5f907"
        "0756087942120000 0000300064400800 27f4120006402000 4340060027f41230"
        "39",
        "",

        /* 21 */
        "000d403b00000500 00000200d0000800 0200d0001a001211 17d2074692020756"
        "0839118900000070 52006440080064f0 1300064010004340 060064f0130002",
        "000d403b00000500 00000200d1000800 0200d1001a001211 170baa6c40030756"
        "0839118900000070 52006440080064f0 1300064010004340 060064f0130002",
        "",
    };

    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
        0,

        0,
        0,
        0,

        0,
        0,
        0,

        63,
        65,
        0,

        /* 21 */
        63,
        63,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_authentication_response(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        /* 0 */
        "000d403e00000500 000005c00100009d 000800020001001a 001211177c0bca9d"
        "030753086a91970e 838fd07900644008 0000f1101079baf0 004340060000f110"
        "5ba0",
        "",
        "",

        /* 3 */
        "000d403500000500 0000020001000800 02001f001a000c0b 07530831c964f076"
        "1378760064400800 00f110002343d000 4340060000f11030 39",
        "",
        "",

        /* 6 */
        "000d"
        "4038000005000000 05c0020000c80008 00020002001a000c 0b0753087dc78e7c"
        "421f9eb900644008 0000f11054f64010 004340060000f110 2b67",
        "",
        "000d"
        "4035000005000000 0200010008000200 01001a000c0b0753 08d8b388ddacaa1f"
        "36006440080000f1 100019b010004340 060000f1100001",

        /* 9 */
        "000d"
        "4038000005000000 05c0010001da0008 00020001001a000c 0b0753084ce11ef1"
        "24b1854500644008 0000f11004615380 004340060000f110 5ba0",
        "000d"
        "403e000005000000 05c0020000c40008 0002000c001a0012 1117deff957c0707"
        "5308e3b925330735 7093006440080000 f110001f20a00043 40060000f1103039",
        "",

        /* 12 */
        "000d"
        "4036000005000000 0200020008000340 072d001a000c0b07 5308e2b7921bc6fe"
        "b4c9006440080000 f1100787b8000043 40060000f1103039",
        "",
        "",

        /* 15 */
        "000d"
        "4038000005000000 05c001a001020008 00020018001a000c 0b0753082a080419"
        "d7becfdb00644008 001300418d01b780 0043400600130041 0033",
        "",
        "",

        /* 18 */
        "000d403b00000500 0000020001000800 020001001a001211 17fda45fe50b0753"
        "086f7ff4fc4be85f d1006440080009f1 070019b010004340 060009f1070007",
        "000d403d00000500 00000200f8000800 048003e993001a00 121117e6e4dd6508"
        "0753086f7ff4fc4b e85fd10064400800 27f4120006402000 4340060027f41230"
        "39",
        "",

        /* 21 */
        "000d403b00000500 00000200d0000800 0200d0001a001211 17c3cf6653030753"
        "0831c964f0761378 76006440080064f0 1300064010004340 060064f0130002",
        "000d403b00000500 00000200d1000800 0200d1001a001211 17e41759f6040753"
        "0831c964f0761378 76006440080064f0 1300064010004340 060064f0130002",
        "",
    };

    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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

        60,
        0,
        0,

        63,
        65,
        0,

        /* 21 */
        63,
        63,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}


int tests1ap_build_authentication_failure(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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

    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_security_mode_complete(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000d403500000500 000005c00100009d 000800020001001a 000908476b8f5f64"
        "00075e0064400800 00f1101079baf000 4340060000f1105b a0",
        "",
        "",

        /* 3 */
        "000d403200000500 0000020001000800 02001f001a000908 473c0c819e00075e"
        "006440080000f110 002343d000434006 0000f1103039",
        "",
        "",

        /* 6 */
        "000d"
        "4035000005000000 05c0020000c80008 00020002001a0009 0847c0eb1eb80007"
        "5e006440080000f1 1054f64010004340 060000f1102b67",
        "",
        "000d"
        "4032000005000000 0200010008000200 01001a00090847c8 38f8d700075e0064"
        "40080000f1100019 b010004340060000 f1100001",

        /* 9 */
        "000d"
        "4035000005000000 05c0010001da0008 00020001001a0009 0847d3b0ef030007"
        "5e006440080000f1 1004615380004340 060000f1105ba0",

        "000d"
        "4035000005000000 05c0020000c40008 0002000c001a0009 084790282df90007"
        "5e006440080000f1 10001f20a0004340 060000f1103039",
        "",

        /* 12 */
        "000d"
        "4033000005000000 0200020008000340 072d001a00090847 aaaf94cf00075e00"
        "6440080000f11007 87b8000043400600 00f1103039",
        "",
        "",

        /* 15 */
        "000d"
        "4035000005000000 05c001a001020008 00020018001a0009 084785cfd7fa0007"
        "5e00644008001300 418d01b780004340 06001300410033",
        "",
        "",

        /* 18 */
        "000d403d00000500 0000020001000800 020001001a001413 471647b75200075e"
        "2309337522002954 3118f40064400800 09f1070019b01000 4340060009f10700"
        "07",

        "000d403400000500 00000200f8000800 048003e993001a00 090847592cf09600"
        "075e006440080027 f412000640200043 40060027f4123039",
        "",

        /* 21 */
        "000d403200000500 00000200d0000800 0200d0001a000908 473c0c819e00075e"
        "006440080064f013 0006401000434006 0064f0130002",
        "000d403200000500 00000200d1000800 0200d1001a000908 47e68241c900075e"
        "006440080064f013 0006401000434006 0064f0130002",
        "",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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

        57,
        0,
        0,

        65,
        56,
        0,

        /* 21 */
        54,
        54,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_esm_information_response(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] =  {
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

        /* 3 */
        "000d403e00000500 0000020001000800 02001f001a001514 27505a0b5301023c"
        "da280908696e7465 726e657400644008 0000f110002343d0 004340060000f110"
        "3039",
        "000d403e00000500 0000020002000800 020011001a001514 27b531c64f01023c"
        "da280908696e7465 726e657400644008 0000f110002343d0 004340060000f110"
        "3039",
        "",

        /* 6 */
        "000d"
        "4041000005000000 05c0020000c80008 00020002001a0015 142793b2bedc0102"
        "01da280908696e74 65726e6574006440 080000f11054f640 10004340060000f1"
        "102b67",
        "",
        "",

        /* 9 */
        "000d"
        "4041000005000000 05c0010001da0008 00020001001a0015 14279a2476c80102"
        "07da280908696e74 65726e6574006440 080000f110046153 80004340060000f1"
        "105ba0",
        "000d"
        "4041000005000000 05c0020000c40008 0002000c001a0015 14274f280bff0102"
        "04da280908696e74 65726e6574006440 080000f110001f20 a0004340060000f1"
        "103039",
        "",

        /* 12 */
        "000d"
        "403f000005000000 0200020008000340 072d001a00151427 c4a524d8010209da"
        "280908696e746572 6e65740064400800 00f1100787b80000 4340060000f11030"
        "39",
        "",
        "",

        /* 15 */
        "000d"
        "4044000005000000 05c001a001020008 00020018001a0018 1727675df9c50102"
        "01da280c07737461 72656e7403636f6d 0064400800130041 8d01b78000434006"
        "001300410033",
        "",
        "",

        /* 18 */
        "000d405a00000500 0000020001000800 020001001a003130 27ebeeb5d4010234"
        "da280908696e7465 726e6574271a8080 2110010100108106 0000000083060000"
        "0000000d00001000 006440080009f107 0019b01000434006 0009f1070007",
        "000d404000000500 00000200f8000800 048003e993001a00 151427f9e0e95c01"
        "026eda280908696e 7465726e65740064 40080027f4120006 4020004340060027"
        "f4123039",
        "",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        146,
        146,
        0,

        66,
        66,
        0,

        69,
        0,
        0,

        /* 9 */
        69,
        69,
        0,

        67,
        0,
        0,

        72,
        0,
        0,

        /* 18 */
        94,
        68,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_ue_capability_info_indication(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "0016"
        "40809c0000030000 0005c00100009d00 0800020001004a40 8085808304040003"
        "04ec598007000820 81839b4e1e3ff8ff f1ffc7ff8ffe3ffc 7ff1ffe3ff8fff1f"
        "fc7ff8ffe3ffc7ff 1ffdfd3ffa7a2060 090e194e9525c8c2 fd80000000e03fe7"
        "ff5f60000000384f e9ffd3d800000002 1033035758a66014 042f6513b8800d2f"
        "0831c1a53432b259 ef989007000cdd9c 6331200e0019a332 c662401c003200",
        "",
        "",

        /* 3 */
        "0016402d00000300 0000020001000800 02001f004a401a19 00b801014c598080"
        "9c000bf06ec4d001 40302c0000000000 000000",
        "",
        "",

        /* 6 */
        "0016"
        "403f000003000000 05c0020000c80008 00020002004a4029 28013001023cd980"
        "00bc000ff06ec4d0 0141b82c00000000 07d404000ef08020 00012a0300008800"
        "00",
        "0016"
        "403f000003000000 05c0020000c70008 00020004004a4029 28013001023cd980"
        "00bc000ff06ec4d0 0141b82c00000000 07d404000ef08020 00012a0300008800"
        "00",
        "",

        /* 9 */
        "0016"
        "4030000003000000 05c0010001da0008 00020001004a401a 1900b801014c5980"
        "809c000bf06ec4d0 0140302c00000000 0000",
        "0016"
        "4030000003000000 05c0020000c40008 0002000c004a401a 1900b801014c5980"
        "0018000bf06ec4d0 0100302c00000000 0000",
        "",

        /* 12 */
        "0016"
        "4080800000030000 0002000200080003 40072d004a406c6b 034803042c998001"
        "08201838bfc1c5fd bd3ffa8a20400902 3a54930382c60000 00001c4083840008"
        "0881001c40004042 080060e2ff0717f0 0040000203fa0000 0007e8000000020f"
        "33035758a6601404 e09c100125600031 218d3432b2386200 40008d17960c4008"
        "001100",
        "",
        "",

        /* 15 */
        "0016"
        "403f000003000000 05c001a001020008 00020018004a4029 28013001023cd980"
        "00bc000ff06ec4d0 0141b82c00000000 07d404000ef08020 00016a0300008800"
        "00",
        "",
        "",

        "",
        "00164081f7000003 0000000200f80008 00048003e993004a 4081e181df040ee2"
        "03081a4cd9800818 1004e103a54ca044 8ff91ff91ff23ff2 3fe47fe47fc8ffc8"
        "ff91ff91ff23ff23 fe47fe47fc8ffc8f f91ff7fcffebe882 002429c329d2a4b8"
        "fc37f00000000010 254b7000000003ea 83234a801c600040 4608006300024018"
        "c000880e10002021 0400708001010020 028410300001000c 200048039c001200"
        "e50004807a000101 982002d010398001 000e60004807a000 101a02001d000090"
        "0738082401ca0209 00730082401d0020 900e100024200400 50824600002001ce"
        "020980738002603d 002090cc1001e800 048660800b4048e6 0004007a00010198"
        "2402d01039804120 168081cc000900f4 008243404007a000 121a02002d0123a0"
        "001000e80104c03a 000131048ff91ff9 1ff23ff23fe47fe4 7fc8ffc8ff91ff91"
        "ff23ff23fe47fe47 fc8ffc8ff91ff91f f23ff23fe47fe47f c8ffc8ff91ff91ff"
        "23ff23fe47fe47fc 8ffc8ff91ff91ff2 3ff23fe47fe47fc8 ffc8ff91ff91ff23"
        "ff23fe47fe47fc8f fc8ff91ff91ff23f f23fe47fe47fc8ff c8ff91ff91ff23ff"
        "23fe47fe47fc8ffc 8ff91ff402c00004 d043c5c10e10f1fc 391f8000648803c6"
        "5a600923ffffcd02 1133035758a66014 04ef65233b8878d2 f2803201bb3432b2"
        "59ef9891e7000d80 1bbe8c662479c003 60068f8b19891e70 00d80100",
        "",

        /* 21 */
        "00164080b6000003 0000000200d00008 000200d0004a4080 a280a00404e80108"
        "099cd98002a552f0 40b82e0bff06ec4d 00141b8748000000 02a5647400c00c00"
        "80007d404380e900 04103a8001040ef0 004107a400101a82 003d200080de1001"
        "ea00040690800f50 00203784007bc001 01a42003de00080d 41000e90004807a4"
        "00101a42001d4000 900f500020350400 3bc001201ef00040 6f080381705c1705"
        "c1705c1705c1705c 1705c1705c172a32 0000887003fc0000 000000",
        "00164080b6000003 0000000200d10008 000200d1004a4080 a280a00404e80108"
        "099cd98002a552f0 40b82e0bff06ec4d 00141b8748000000 02a5647400c00c00"
        "80007d404380e900 04103a8001040ef0 004107a400101a82 003d200080de1001"
        "ea00040690800f50 00203784007bc001 01a42003de00080d 41000e90004807a4"
        "00101a42001d4000 900f500020350400 3bc001201ef00040 6f080381705c1705"
        "c1705c1705c1705c 1705c1705c172a32 0000887003fc0000 000000",
        "",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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

        67,
        0,
        0,

        0,
        508,
        0,

        /* 21 */
        187,
        187,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_initial_context_setup_response(
        ogs_pkbuf_t **pkbuf, 
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        uint8_t ebi, uint32_t teid, const char *ipstr)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_InitialContextSetupResponse_t *InitialContextSetupResponse = NULL;
    ogs_sockaddr_t *addr = NULL;

    S1AP_InitialContextSetupResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABSetupListCtxtSURes_t *E_RABSetupListCtxtSURes = NULL;

    S1AP_E_RABSetupItemCtxtSUResIEs_t *item = NULL;
    S1AP_E_RABSetupItemCtxtSURes_t *e_rab = NULL;

    ogs_gtp_f_teid_t f_teid;
    ogs_ip_t ip;
    int len;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = 
        CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_InitialContextSetupResponse;

    InitialContextSetupResponse = 
        &successfulOutcome->value.choice.InitialContextSetupResponse;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABSetupListCtxtSURes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_E_RABSetupListCtxtSURes;

    E_RABSetupListCtxtSURes = &ie->value.choice.E_RABSetupListCtxtSURes;

    *MME_UE_S1AP_ID = mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;

    item = CALLOC(1, sizeof(S1AP_E_RABSetupItemCtxtSUResIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupListCtxtSURes->list, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABSetupItemCtxtSURes;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present =
        S1AP_E_RABSetupItemCtxtSUResIEs__value_PR_E_RABSetupItemCtxtSURes;

    e_rab = &item->value.choice.E_RABSetupItemCtxtSURes;

    e_rab->e_RAB_ID = ebi;

    rv = ogs_getaddrinfo(&addr, AF_INET, ipstr, OGS_GTPV1_U_UDP_PORT, 0);
    ogs_assert(rv == OGS_OK);
    rv = ogs_gtp_sockaddr_to_f_teid(addr, NULL, &f_teid, &len);
    ogs_assert(rv == OGS_OK);
    ogs_freeaddrinfo(addr);

    rv = ogs_gtp_f_teid_to_ip(&f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    rv = ogs_s1ap_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
    ogs_assert(rv == OGS_OK);
    ogs_s1ap_uint32_to_OCTET_STRING(teid, &e_rab->gTP_TEID);

    *pkbuf = ogs_s1ap_encode(&pdu);
    if (*pkbuf == NULL) {
        ogs_error("ogs_s1ap_encode() failed");
        return OGS_ERROR;
    }
    return OGS_OK;
}

int tests1ap_build_ue_context_modification_response(
        ogs_pkbuf_t **pkbuf, 
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_UEContextModificationResponse_t *UEContextModificationResponse = NULL;
    ogs_sockaddr_t *addr = NULL;

    S1AP_UEContextModificationResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = 
        CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_UEContextModification;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_UEContextModificationResponse;

    UEContextModificationResponse = 
        &successfulOutcome->value.choice.UEContextModificationResponse;

    ie = CALLOC(1, sizeof(S1AP_UEContextModificationResponseIEs_t));
    ASN_SEQUENCE_ADD(&UEContextModificationResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UEContextModificationResponseIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_UEContextModificationResponseIEs_t));
    ASN_SEQUENCE_ADD(&UEContextModificationResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UEContextModificationResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *MME_UE_S1AP_ID = mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;

    *pkbuf = ogs_s1ap_encode(&pdu);
    if (*pkbuf == NULL) {
        ogs_error("ogs_s1ap_encode() failed");
        return OGS_ERROR;
    }
    return OGS_OK;
}

int tests1ap_build_initial_context_setup_failure(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "4009 0015000003000040 0200020008400200 01000240020000",
        "",
        "",

        /* 3 */
        "",
        "",
        "",

        /* 6 */
        "",
        "",
        "4009001500000300 0040020001000840 0200010002400202 a0",

        /* 9 */
        "",
        "",
        "",

        /* 12 */
        "",
        "",
        "",

        /* 15 */
        "",
        "",
        "",

        /* 18 */
        "",
        "",
        "",

        /* 21 */
        "4009001500000300 00400200d0000840 0200d00002400203 40",
        "4009001500000300 00400200d2000840 0200d20002400203 40",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        25,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        25,

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
        0,

        /* 21 */
        25,
        25,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_attach_complete(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000d"
        "403a000005000000 05c00100009d0008 00020001001a000e 0d27c183eb950207"
        "4300035200c20064 40080000f1101079 baf0004340060000 f1105ba0",
        "000d"
        "403a000005000000 05c00100009e0008 00020002001a000e 0d27e8a2a3f10207"
        "4300035200c20064 40080000f1101079 baf0004340060000 f1105ba0",
        "",

        /* 3 */
        "000d"
        "403a000005000000 05c0000000010008 0002001f001a000e 0d27127c47860207"
        "4300035200c20064 40080000f1101079 baf0004340060000 f1105ba0",
        "000d"
        "403a000005000000 05c0010000020008 00020011001a000e 0d27225d92bb0207"
        "4300035200c20064 40080000f1101079 baf0004340060000 f1105ba0",
        "",

        /* 6 */
        "000d"
        "403a000005000000 05c0020000c80008 00020002001a000e 0d27f190fc2b0207"
        "4300035200c20064 40080000f11054f6 4010004340060000 f1102b67",
        "",
        "",

        /* 9 */
        "000d"
        "403a000005000000 05c0010001da0008 00020001001a000e 0d272e3456f70207"
        "4300035200c20064 40080000f1100461 5380004340060000 f1105ba0",
        "000d"
        "403a000005000000 05c0020000c40008 0002000c001a000e 0d276a543e560207"
        "4300035200c20064 40080000f110001f 20a0004340060000 f1103039",
        "",

        /* 12 */
        "000d"
        "4038000005000000 0200020008000340 072d001a000e0d27 600bbd3802074300"
        "035200c200644008 0000f1100787b800 004340060000f110 3039",
        "",
        "",

        /* 15 */
        "000d"
        "403a000005000000 05c001a001020008 00020018001a000e 0d27be5dbbc90207"
        "4300035200c20064 4008001300418d01 b780004340060013 00410033",
        "",
        "",

        /* 18 */
        "000d403700000500 0000020001000800 020001001a000e0d 27ffe827a8020743"
        "00035200c2006440 080009f1070019b0 10004340060009f1 070007",
        "000d403900000500 00000200f8000800 048003e993001a00 0e0d27b1f1539802"
        "074300035200c200 6440080027f41200 0640200043400600 27f4123039",
        "",

        /* 21 */
        "",
        "000d403700000500 00000200d1000800 0200d1001a000e0d 27f6962d51010743"
        "00035200c2006440 080064f013000640 10004340060064f0 130002",
        "",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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

        62,
        0,
        0,

        59,
        61,
        0,

        /* 21 */
        0,
        59,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_emm_status(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_detach_request(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000c404800000600 0800020002001a 001615172ba435db 040745090bf600f1"
        "10000201020003e6 004300060000f110 2b67006440080000 f1109d67aa500086"
        "4001300060000600 40020003e6",
        "000c404800000600 0800020001001a 00161517ba258500 040745090bf600f1"
        "10000201020003e6 004300060000f110 2b67006440080000 f1109d67aa500086"
        "4001300060000600 40020003e6",
        "",

        /* 3 */
        "",
        "",
        "",

        /* 6 */
        "",
        "",
        "",

        /* 9 */
        "",
        "",
        "",

        /* 12 */
        "",
        "",
        "",

        /* 15 */
        "",
        "",
        "",

        /* 18 */
        "000d403f00000500 0000020002000800 020002001a001615 2754d17d69040745"
        "0b0bf609f1070002 01f500075e006440 080009f1070019b0 10004340060009f1"
        "070007",
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        76,
        76,
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
        0,
        0,

        0,
        0,
        0,

        67,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_ue_context_release_request(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "0012"
        "4018000003000000 05c00100009d0008 0002000100024002 0280",
        "",
        "",

        /* 3 */
        "",
        "",
        "",

        /* 6 */
        "0012"
        "4018000003000000 05c0020000c80008 0002000200024002 0280",
        "",
        "0012401500000300 0000020001000800 0200010002400202 a0",

        /* 9 */
        "",
        "",
        "",

        /* 12 */
        "",
        "",
        "",

        /* 15 */
        "",
        "",
        "",

        /* 18 */
        "0012401500000300 0000020001000800 0200010002400202 80",
        "0012401500000300 0000020002000800 0200020002400202 e0",
        "0012401700000300 00000200f8000800 048003e993000240 0202e0",

        /* 21 */
        "0012401500000300 00000200d1000800 0200d10002400202 80",
        "0012401500000300 00000200d2000800 0200d20002400202 80",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        28,
        0,
        0,

        0,
        0,
        0,

        28,
        0,
        25,

        0,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        0,

        25,
        25,
        27,

        25,
        25,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_ue_context_release_complete(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "2017001300000200 004005c00100009e 00084003400002",
        "2017001300000200 004005c000000001 00084003400001",
        "2017001200000200 004005c00100009f 000840020003",

        /* 3 */
        "2017001300000200 004005c00100009d 00084003400001",
        "",
        "2017001300000200 004005c000000003 00084003400021",

        /* 6 */
        "2017"
        "0012000002000040 05c0020000c80008 40020002",
        "2017"
        "0012000002000040 05c0020000cb0008 40020026",
        "2017001200000200 004005c000000001 00084002001f",

        /* 9 */
        "2017"
        "0012000002000040 05c0020000c40008 4002000c",
        "2017"
        "0012000002000040 05c0020000c50008 40020008",
        "2017"
        "0012000002000040 05c0020000c70008 40020008",

        /* 12 */
        "2017"
        "0010000002000040 0200010008400340 072c",
        "2017"
        "0010000002000040 0200030008400340 072e",
        "",

        /* 15 */
        "",
        "",
        "",

        /* 18 */
        "2017000f00000200 0040020001000840 020001",
        "2017000f00000200 0040020002000840 020002",
        "2017001100000200 00400200f8000840 048003e993",

        /* 21 */
        "2017000f00000200 00400200d0000840 0200d0",
        "2017000f00000200 00400200d1000840 0200d1",
        "2017000f00000200 00400200d3000840 0200d3",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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

        0,
        0,
        0,

        19,
        19,
        21,
        
        /* 21 */
        19,
        19,
        19,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_service_request(ogs_pkbuf_t **pkbuf,
        uint32_t enb_ue_s1ap_id, uint8_t seq,
        uint16_t mac, uint32_t m_tmsi)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000c"
        "4037000006000800 020004001a0005 04c7049551004300 060000f110303900"
        "6440080000f11007 87b8000086400140 0060000600400000 0001",

        "000c"
        "4038000006000800 03400700001a0005 04c7049551004300 060000f110303900"
        "6440080000f11007 87b8000086400140 0060000600400000 0001",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        59,
        60,
    };
    char hexbuf[OGS_MAX_SDU_LEN];
    int i = 0;

    if (enb_ue_s1ap_id & 0x400000) i = 1;

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    enb_ue_s1ap_id = htonl(enb_ue_s1ap_id << 8);
    memcpy((*pkbuf)->data + 11, &enb_ue_s1ap_id, 2+i);
    mac = htons(mac);
    memcpy((*pkbuf)->data + 19+i, &seq, 1);
    memcpy((*pkbuf)->data + 20+i, &mac, 2);
    m_tmsi = htonl(m_tmsi);
    memcpy((*pkbuf)->data + 55+i, &m_tmsi, 4);

    return OGS_OK;
}

int tests1ap_build_tau_request(ogs_pkbuf_t **pkbuf, int i,
    uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id, uint8_t active_flag,
    uint32_t m_tmsi, uint8_t seq, uint32_t mac, uint8_t *knas_int)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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

        /* Crash : 3 */

        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        113,
        103,
        0,

        77,
        68,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    if (i == 0) {
        enb_ue_s1ap_id = htonl(enb_ue_s1ap_id << 8);
        memcpy((*pkbuf)->data + 11, &enb_ue_s1ap_id, 3);
        if (active_flag) {
            unsigned char *active_buf = (*pkbuf)->data + 26;
            *active_buf |= 0x08;
        }
        mac = htonl(mac);
        memcpy((*pkbuf)->data + 19, &mac, 4);
        memcpy((*pkbuf)->data + 23, &seq, 1);
        m_tmsi = htonl(m_tmsi);
        memcpy((*pkbuf)->data + 109, &m_tmsi, 4);
    } else if (i == 1) {
        ogs_assert(knas_int);

        mme_ue_s1ap_id = htonl(mme_ue_s1ap_id << 8);
        memcpy((*pkbuf)->data + 11, &mme_ue_s1ap_id, 3);
        enb_ue_s1ap_id = htonl(enb_ue_s1ap_id << 8);
        memcpy((*pkbuf)->data + 17, &enb_ue_s1ap_id, 3);

        memcpy((*pkbuf)->data + 29, &seq, 1);

        m_tmsi = htonl(m_tmsi);
        memcpy((*pkbuf)->data + 41, &m_tmsi, 4);

        snow_3g_f9(knas_int, seq, (0 << 27), 0,
                (*pkbuf)->data + 29, (52 << 3),
                (*pkbuf)->data + 25);
    } else
        ogs_assert_if_reached();

    return OGS_OK;
}

int tests1ap_build_extended_service_request(ogs_pkbuf_t **pkbuf, int i,
        uint8_t service_type, uint32_t m_tmsi, uint8_t seq, uint8_t *knas_int)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "",
        "",
        "",

        /* 3 */
        "",
        "",
        "",

        /* 6 */
        "",
        "",
        "",

        /* 9 */
        "",
        "",
        "",

        /* 12 */
        "",
        "",
        "",

        /* 15 */
        "",
        "",
        "",

        /* 18 */
        "000c"
        "4038000005000800 020002001a00100f 17b51a57a504074c 000504e900a25200"
        "4300060009f10700 07006440080009f1 0707080140008640 0130",

        "000d403900000500 0000020001000800 020001001a00100f 17b51a57a504074c"
        "000504e900a25200 6440080009f10700 19b0100043400600 09f1070007",

        "",

        /* 21 */
        "000c404900000600 0800048004141300 1a001514172cf294 2e04074c0105f4e6"
        "004551b157022000 004300060027f412 3039006440080027 f412000640300086"
        "4001200060000600 40e6004551",
        "000d404000000500 00000200f8000800 048003e993001a00 151427b426655c03"
        "074c0105f4e60045 51b1570220000064 40080027f4120006 4020004340060027"
        "f4123039",
        "",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
        0,

        0,
        0,
        0,

        0,
        0,
        0,

        60,
        61,
        0,
        
        /* 21 */
        77,
        68,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);
    if (i == 18) {
        memcpy((*pkbuf)->data + 26, &service_type, sizeof service_type);
        m_tmsi = htonl(m_tmsi);
        memcpy((*pkbuf)->data + 29, &m_tmsi, sizeof m_tmsi);

        snow_3g_f9(knas_int, seq, (0 << 27), 0,
                (*pkbuf)->data + 23, (10 << 3),
                (*pkbuf)->data + 19);
    } else if (i == 19) {
        memcpy((*pkbuf)->data + 32, &service_type, sizeof service_type);
        m_tmsi = htonl(m_tmsi);
        memcpy((*pkbuf)->data + 35, &m_tmsi, sizeof m_tmsi);
        snow_3g_f9(knas_int, seq, (0 << 27), 0,
                (*pkbuf)->data + 29, (10 << 3),
                (*pkbuf)->data + 25);
    } else if (i == 21) {
        memcpy((*pkbuf)->data + 28, &service_type, sizeof service_type);
        m_tmsi = htonl(m_tmsi);
        memcpy((*pkbuf)->data + 31, &m_tmsi, sizeof m_tmsi);
        snow_3g_f9(knas_int, seq, (0 << 27), 0,
                (*pkbuf)->data + 25, (15 << 3),
                (*pkbuf)->data + 21);
    } else if (i == 22) {
        memcpy((*pkbuf)->data + 34, &service_type, sizeof service_type);
        m_tmsi = htonl(m_tmsi);
        memcpy((*pkbuf)->data + 37, &m_tmsi, sizeof m_tmsi);
        snow_3g_f9(knas_int, seq, (0 << 27), 0,
                (*pkbuf)->data + 31, (15 << 3),
                (*pkbuf)->data + 27);
    }

    return OGS_OK;
}

int tests1ap_build_pdn_connectivity_request(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_pdn_disconnectivity_request(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_e_rab_setup_response(
        ogs_pkbuf_t **pkbuf, 
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        uint8_t ebi, uint32_t teid, const char *ipstr)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_E_RABSetupResponse_t *E_RABSetupResponse = NULL;

    S1AP_E_RABSetupResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABSetupListBearerSURes_t *E_RABSetupListBearerSURes = NULL;

    S1AP_E_RABSetupItemBearerSUResIEs_t *item = NULL;
    S1AP_E_RABSetupItemBearerSURes_t *e_rab = NULL;

    ogs_gtp_f_teid_t f_teid;
    ogs_ip_t ip;
    int len;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = 
        CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_E_RABSetup;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_E_RABSetupResponse;

    E_RABSetupResponse = &successfulOutcome->value.choice.E_RABSetupResponse;

    ie = CALLOC(1, sizeof(S1AP_E_RABSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_E_RABSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_E_RABSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABSetupListBearerSURes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABSetupResponseIEs__value_PR_E_RABSetupListBearerSURes;

    E_RABSetupListBearerSURes = &ie->value.choice.E_RABSetupListBearerSURes;

    *MME_UE_S1AP_ID = mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;

    item = CALLOC(1, sizeof(S1AP_E_RABSetupItemBearerSUResIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupListBearerSURes->list, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABSetupItemBearerSURes;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present =
        S1AP_E_RABSetupItemBearerSUResIEs__value_PR_E_RABSetupItemBearerSURes;

    e_rab = &item->value.choice.E_RABSetupItemBearerSURes;
    e_rab->e_RAB_ID = ebi;

    rv = ogs_getaddrinfo(&addr, AF_INET, ipstr, OGS_GTPV1_U_UDP_PORT, 0);
    ogs_assert(rv == OGS_OK);
    rv = ogs_gtp_sockaddr_to_f_teid(addr, NULL, &f_teid, &len);
    ogs_assert(rv == OGS_OK);
    ogs_freeaddrinfo(addr);
    rv = ogs_gtp_f_teid_to_ip(&f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    rv = ogs_s1ap_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
    ogs_assert(rv == OGS_OK);
    ogs_s1ap_uint32_to_OCTET_STRING(teid, &e_rab->gTP_TEID);

    *pkbuf = ogs_s1ap_encode(&pdu);
    if (*pkbuf == NULL) {
        ogs_error("ogs_s1ap_encode() failed");
        return OGS_ERROR;
    }
    return OGS_OK;
}

int tests1ap_build_e_rab_modify_response(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "2006"
        "001b000003000040 0480000001000840 020001001f400600 002500010e",
        "2006"
        "001b000003000040 0480000001000840 020001001f400600 002500010e",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        31,
        31,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_e_rab_release_response(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_activate_default_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_activate_dedicated_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_modify_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000d"
        "4035000005000000 0480000001000800 020001001a000a09 27a5c0d564067200"
        "ca006440080064f0 430020a000004340 060064f043020a",
        "000d"
        "4035000005000000 0480000001000800 020001001a000a09 27a6746cea097204"
        "ca006440080064f0 430020a000004340 060064f043020a",
        "000d"
        "4035000005000000 0480000001000800 020001001a000a09 2726b727fe0a7205"
        "ca006440080064f0 430020a000004340 060064f043020a",

        "000d"
        "4035000005000000 0480000001000800 020001001a000a09 271070e13b0d7203"
        "ca006440080064f0 430020a000004340 060064f043020a",
        "",
        "",

        "",
        "",
        "",

        "",
        "",
        "",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        57,
        57,
        57,

        57,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_deactivate_bearer_accept(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_bearer_resource_allocation_request(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000d"
        "403f000005000000 0200010008000200 01001a00161517fe b2b150070203d406"
        "04212001000501ff ffffff0064400800 2143650003039000 4340060021436509"
        "29",
        "",
        "",

        "",
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        67,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

static void build_bearer_resource_modification_request(ogs_pkbuf_t **pkbuf,
    uint8_t pti, uint32_t mac, uint8_t seq, uint8_t ebi,
    uint8_t tft_code, uint8_t qci,
    uint8_t ul_mbr, uint8_t dl_mbr, uint8_t ul_gbr, uint8_t dl_gbr)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_nas_message_t message;
    ogs_nas_bearer_resource_modification_request_t
        *req = &message.esm.bearer_resource_modification_request;
    ogs_nas_traffic_flow_aggregate_description_t *tad =
        &req->traffic_flow_aggregate;
    ogs_nas_eps_quality_of_service_t *qos = &req->required_traffic_flow_qos;

    ogs_gtp_tft_t tft;
    ogs_tlv_octet_t octet;
    int len;
    char tft_buf[OGS_GTP_MAX_TRAFFIC_FLOW_TEMPLATE];
    ogs_ipsubnet_t ipsubnet;

    memset(&message, 0, sizeof(message));
    message.esm.h.eps_bearer_identity = 0;
    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = pti;
    message.esm.h.message_type = OGS_NAS_BEARER_RESOURCE_MODIFICATION_REQUEST;

    req->eps_bearer_identity_for_packet_filter.eps_bearer_identity = ebi;

    memset(&tft, 0, sizeof tft);
    tft.code = tft_code;
    if (tft.code == OGS_GTP_TFT_CODE_REPLACE_PACKET_FILTERS_IN_EXISTING) {
        tft.num_of_packet_filter = 1;
        tft.pf[0].direction = 1;
        tft.pf[0].identifier = 0;
        tft.pf[0].precedence = 0x0f;
        tft.pf[0].length = 9;
        tft.pf[0].component[0].type =
            GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "201.20.2.5", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[0].component[0].ipv4.addr = ipsubnet.sub[0];
        tft.pf[0].component[0].ipv4.mask = ipsubnet.mask[0];
        tft.pf[0].num_of_component = 1;
    } else if (tft.code ==
            OGS_GTP_TFT_CODE_ADD_PACKET_FILTERS_TO_EXISTING_TFT) {
        tft.num_of_packet_filter = 1;
        tft.pf[0].direction = 1;
        tft.pf[0].identifier = 4;
        tft.pf[0].precedence = 0x0f;

        rv = ogs_ipsubnet(&ipsubnet, "cafe::9", "120");
        ogs_assert(rv == OGS_OK);
#if 1
        tft.pf[0].length = 18;
        tft.pf[0].component[0].type =
            GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE;
        memcpy(tft.pf[0].component[0].ipv6.addr, ipsubnet.sub,
                sizeof(tft.pf[0].component[0].ipv6.addr));
        tft.pf[0].component[0].ipv6.prefixlen = 120;
#else
        tft.pf[0].length = 33;
        tft.pf[0].component[0].type =
            GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE;
        memcpy(tft.pf[0].component[0].ipv6_mask.addr, ipsubnet.sub,
                sizeof(tft.pf[0].component[0].ipv6_mask.addr));
        memcpy(tft.pf[0].component[0].ipv6_mask.mask, ipsubnet.mask,
                sizeof(tft.pf[0].component[0].ipv6_mask.mask));
#endif
        tft.pf[0].num_of_component = 1;

    } else if (tft.code == OGS_GTP_TFT_CODE_CREATE_NEW_TFT) {
        tft.num_of_packet_filter = 4;

        tft.pf[0].direction = 1;
        tft.pf[0].identifier = 0;
        tft.pf[0].precedence = 0x01;
        tft.pf[0].length = 0x17;
        tft.pf[0].component[0].type =
            GTP_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
        tft.pf[0].component[0].proto = 0x11; /* UDP */
        tft.pf[0].component[1].type =
            GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.20.166.84", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[0].component[1].ipv4.addr = ipsubnet.sub[0];
        tft.pf[0].component[1].ipv4.mask = ipsubnet.mask[0];
        tft.pf[0].component[2].type =
            GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.18.128.20", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[0].component[2].ipv4.addr = ipsubnet.sub[0];
        tft.pf[0].component[2].ipv4.mask = ipsubnet.mask[0];
        tft.pf[0].component[3].type =
            GTP_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
        tft.pf[0].component[3].port.low = 20001;
        tft.pf[0].num_of_component = 4;

        tft.pf[1].direction = 2;
        tft.pf[1].identifier = 1;
        tft.pf[1].precedence = 0x02;
        tft.pf[1].length = 0x17;
        tft.pf[1].component[0].type =
            GTP_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
        tft.pf[1].component[0].proto = 0x11; /* UDP */
        tft.pf[1].component[1].type =
            GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.20.166.84", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[1].component[1].ipv4.addr = ipsubnet.sub[0];
        tft.pf[1].component[1].ipv4.mask = ipsubnet.mask[0];
        tft.pf[1].component[2].type =
            GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.18.128.20", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[1].component[2].ipv4.addr = ipsubnet.sub[0];
        tft.pf[1].component[2].ipv4.mask = ipsubnet.mask[0];
        tft.pf[1].component[3].type =
            GTP_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
        tft.pf[1].component[3].port.low = 20360;
        tft.pf[1].num_of_component = 4;

        tft.pf[2].direction = 1;
        tft.pf[2].identifier = 2;
        tft.pf[2].precedence = 0x03;
        tft.pf[2].length = 0x17;
        tft.pf[2].component[0].type =
            GTP_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
        tft.pf[2].component[0].proto = 0x11; /* UDP */
        tft.pf[2].component[1].type =
            GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.20.166.84", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[2].component[1].ipv4.addr = ipsubnet.sub[0];
        tft.pf[2].component[1].ipv4.mask = ipsubnet.mask[0];
        tft.pf[2].component[2].type =
            GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.18.128.20", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[2].component[2].ipv4.addr = ipsubnet.sub[0];
        tft.pf[2].component[2].ipv4.mask = ipsubnet.mask[0];
        tft.pf[2].component[3].type =
            GTP_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
        tft.pf[2].component[3].port.low = 20002;
        tft.pf[2].num_of_component = 4;

        tft.pf[3].direction = 2;
        tft.pf[3].identifier = 3;
        tft.pf[3].precedence = 0x04;
        tft.pf[3].length = 0x17;
        tft.pf[3].component[0].type =
            GTP_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
        tft.pf[3].component[0].proto = 0x11; /* UDP */
        tft.pf[3].component[1].type =
            GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.20.166.84", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[3].component[1].ipv4.addr = ipsubnet.sub[0];
        tft.pf[3].component[1].ipv4.mask = ipsubnet.mask[0];
        tft.pf[3].component[2].type =
            GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.18.128.20", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[3].component[2].ipv4.addr = ipsubnet.sub[0];
        tft.pf[3].component[2].ipv4.mask = ipsubnet.mask[0];
        tft.pf[3].component[3].type =
            GTP_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
        tft.pf[3].component[3].port.low = 20361;
        tft.pf[3].num_of_component = 4;
    }
    tad->length = ogs_gtp_build_tft(&octet,
            &tft, tad->buffer, OGS_GTP_MAX_TRAFFIC_FLOW_TEMPLATE);

    if (ul_mbr || dl_mbr || ul_gbr || dl_gbr) {
        req->presencemask |= OGS_NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_PRESENT;
        qos->length = 5;
        qos->qci = qci;
        qos->ul_mbr = ul_mbr;
        qos->dl_mbr = dl_mbr;
        qos->ul_gbr = ul_gbr;
        qos->dl_gbr = dl_gbr;
    }

    emmbuf = ogs_nas_plain_encode(&message);

    message.h.security_header_type =
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.h.message_authentication_code = htobe32(mac);
    message.h.sequence_number = seq;

    ogs_assert(ogs_pkbuf_push(emmbuf, sizeof(ogs_nas_security_header_t)));
    memcpy(emmbuf->data, &message.h, sizeof(ogs_nas_security_header_t));

    *pkbuf = emmbuf;
}

int tests1ap_build_bearer_resource_modification_request(
        ogs_pkbuf_t **pkbuf,
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        uint8_t pti, uint32_t mac, uint8_t seq, uint8_t ebi,
        uint8_t tft_code, uint8_t qci,
        uint8_t ul_mbr, uint8_t dl_mbr, uint8_t ul_gbr, uint8_t dl_gbr)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UplinkNASTransport_t *UplinkNASTransport = NULL;

    S1AP_UplinkNASTransport_IEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_TAI_t *TAI = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_uplinkNASTransport;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_UplinkNASTransport;

    UplinkNASTransport = &initiatingMessage->value.choice.UplinkNASTransport;

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UplinkNASTransport_IEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UplinkNASTransport_IEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_NAS_PDU;

    NAS_PDU = &ie->value.choice.NAS_PDU;

    build_bearer_resource_modification_request(
            &emmbuf, pti, mac, seq, ebi,
            tft_code, qci, ul_mbr, dl_mbr, ul_gbr, dl_gbr);
    ogs_assert(emmbuf);
    NAS_PDU->size = emmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    memcpy(NAS_PDU->buf, emmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(emmbuf);

    *MME_UE_S1AP_ID = mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;

    *pkbuf = ogs_s1ap_encode(&pdu);
    if (*pkbuf == NULL) {
        ogs_error("ogs_s1ap_encode() failed");
        return OGS_ERROR;
    }
    return OGS_OK;
}

int tests1ap_build_bearer_resource_modification_request2(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000d403c00000500 0000020001000800 020001001a001312 279d165c7b0c0203"
        "d60705a400010203 5824006440080000 f110000019c00043 40060000f1100001",
        "",
        "",

        "",
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        64,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_path_switch_request(
        ogs_pkbuf_t **pkbuf, int target,
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        int num_of_bearer, uint8_t ebi, uint32_t teid,
        const char *ipstr1, const char *ipstr2)
{
    int rv;
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
        CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_PathSwitchRequest;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_PathSwitchRequest;

    PathSwitchRequest = &initiatingMessage->value.choice.PathSwitchRequest;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedDLList;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_PathSwitchRequestIEs__value_PR_E_RABToBeSwitchedDLList;

    E_RABToBeSwitchedDLList = &ie->value.choice.E_RABToBeSwitchedDLList;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SourceMME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_EUTRAN_CGI;

    EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_TAI;

    TAI = &ie->value.choice.TAI;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_PathSwitchRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;
    *MME_UE_S1AP_ID = mme_ue_s1ap_id;

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(mme_ue_s1ap_id);
    ogs_assert(enb_ue);
    mme_ue = enb_ue->mme_ue;
    ogs_assert(mme_ue);

    for (i = 0; i < num_of_bearer; i++) {
        S1AP_E_RABToBeSwitchedDLItemIEs_t *item = NULL;
        S1AP_E_RABToBeSwitchedDLItem_t *e_rab = NULL;

        ogs_sockaddr_t *addr = NULL;
        ogs_gtp_f_teid_t f_teid;
        ogs_ip_t ip;
        int len;

        item = CALLOC(1, sizeof(S1AP_E_RABToBeSwitchedDLItemIEs_t));
        ASN_SEQUENCE_ADD(&E_RABToBeSwitchedDLList->list, item);

        item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedDLItem;
        item->criticality = S1AP_Criticality_reject;
        item->value.present = 
            S1AP_E_RABToBeSwitchedDLItemIEs__value_PR_E_RABToBeSwitchedDLItem;

        e_rab = &item->value.choice.E_RABToBeSwitchedDLItem;

        e_rab->e_RAB_ID = ebi+i;

        if (target == 0) {
            rv = ogs_getaddrinfo(&addr, AF_INET, ipstr1,
                    OGS_GTPV1_U_UDP_PORT, 0);
            rv = ogs_gtp_sockaddr_to_f_teid(
                    addr, NULL, &f_teid, &len);
            ogs_freeaddrinfo(addr);
        } else {
            rv = ogs_getaddrinfo(&addr, AF_INET, ipstr2,
                    OGS_GTPV1_U_UDP_PORT, 0);
            rv = ogs_gtp_sockaddr_to_f_teid(
                    addr, NULL, &f_teid, &len);
            ogs_freeaddrinfo(addr);
        }
        ogs_assert(rv == OGS_OK);
        rv = ogs_gtp_f_teid_to_ip(&f_teid, &ip);
        ogs_assert(rv == OGS_OK);

        rv = ogs_s1ap_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
        ogs_assert(rv == OGS_OK);
        ogs_s1ap_uint32_to_OCTET_STRING(teid+i, &e_rab->gTP_TEID);
    }

    ogs_s1ap_buffer_to_OCTET_STRING(
            &mme_ue->e_cgi.plmn_id, OGS_PLMN_ID_LEN, &EUTRAN_CGI->pLMNidentity);
    EUTRAN_CGI->cell_ID.size = 4;
    EUTRAN_CGI->cell_ID.buf =  CALLOC(
         EUTRAN_CGI->cell_ID.size, sizeof(uint8_t));
    ogs_assert(EUTRAN_CGI->cell_ID.buf);
    EUTRAN_CGI->cell_ID.buf[0] = (mme_ue->e_cgi.cell_id >> 24);
    EUTRAN_CGI->cell_ID.buf[1] = (mme_ue->e_cgi.cell_id >> 16);
    EUTRAN_CGI->cell_ID.buf[2] = (mme_ue->e_cgi.cell_id >> 8);
    EUTRAN_CGI->cell_ID.buf[3] = (mme_ue->e_cgi.cell_id);
    EUTRAN_CGI->cell_ID.bits_unused = 4;

    ogs_s1ap_uint16_to_OCTET_STRING(
            mme_ue->tai.tac, &TAI->tAC);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &mme_ue->tai.plmn_id, OGS_PLMN_ID_LEN, &TAI->pLMNidentity);

    UESecurityCapabilities->encryptionAlgorithms.size = 2;
    UESecurityCapabilities->encryptionAlgorithms.buf = 
        CALLOC(UESecurityCapabilities->encryptionAlgorithms.size, 
                    sizeof(uint8_t));
    UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->encryptionAlgorithms.buf[0] = 
        (mme_ue->ue_network_capability.eea << 1);

    UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] =
        (mme_ue->ue_network_capability.eia << 1);

    *pkbuf = ogs_s1ap_encode(&pdu);
    if (*pkbuf == NULL) {
        ogs_error("ogs_s1ap_encode() failed");
        return OGS_ERROR;
    }
    return OGS_OK;
}

int tests1ap_build_handover_required(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_handover_failure(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "4001 0012000002000040 05c0020000c5 000240020000",

        "",
        "",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        22,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_handover_request_ack(
        ogs_pkbuf_t **pkbuf, int target,
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id,
        int num_of_bearer, uint8_t ebi, uint32_t teid,
        const char *ipstr1, const char *ipstr2)
{
    int rv;
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

    char hexbuf[OGS_MAX_SDU_LEN];
    const char *payload =
        "00 80810bf900d8af40 00a0339057801f47 88009e81de2c20a4"
        "81de2c404a00ef16 2000010044013f21 2249008093efd243 3914cd2aa0a0142f"
        "f2214d6dfb82c194 0b10080000020040 bbfd55aeab41ad80 8fd50398381c08fd"
        "503983804d037868 baa016423342bc3a 18b58fa084ca833f a17970acfc10e84e"
        "0004f14550d00096 c88900";

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = 
        CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_HandoverResourceAllocation;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_HandoverRequestAcknowledge;

    HandoverRequestAcknowledge =
        &successfulOutcome->value.choice.HandoverRequestAcknowledge;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABAdmittedList;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_E_RABAdmittedList;

    E_RABAdmittedList = &ie->value.choice.E_RABAdmittedList;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Target_ToSource_TransparentContainer;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverRequestAcknowledgeIEs__value_PR_Target_ToSource_TransparentContainer;

    Target_ToSource_TransparentContainer =
        &ie->value.choice.Target_ToSource_TransparentContainer;

    *MME_UE_S1AP_ID = mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;

    for (i = 0; i < num_of_bearer; i++) {
        S1AP_E_RABAdmittedItemIEs_t *item = NULL;
        S1AP_E_RABAdmittedItem_t *e_rab = NULL;

        ogs_sockaddr_t *addr = NULL;
        ogs_gtp_f_teid_t f_teid;
        ogs_ip_t ip;
        int len;

        item = CALLOC(1, sizeof(S1AP_E_RABAdmittedItemIEs_t));
        ASN_SEQUENCE_ADD(&E_RABAdmittedList->list, item);

        item->id = S1AP_ProtocolIE_ID_id_E_RABAdmittedItem;
        item->criticality = S1AP_Criticality_ignore;
        item->value.present =
            S1AP_E_RABAdmittedItemIEs__value_PR_E_RABAdmittedItem;

        e_rab = &item->value.choice.E_RABAdmittedItem;

        e_rab->e_RAB_ID = ebi+i;

        if (target == 0) {
            rv = ogs_getaddrinfo(&addr, AF_INET, ipstr1,
                    OGS_GTPV1_U_UDP_PORT, 0);
            rv = ogs_gtp_sockaddr_to_f_teid(
                    addr, NULL, &f_teid, &len);
            ogs_freeaddrinfo(addr);
        } else {
            rv = ogs_getaddrinfo(&addr, AF_INET, ipstr2,
                    OGS_GTPV1_U_UDP_PORT, 0);
            rv = ogs_gtp_sockaddr_to_f_teid(
                    addr, NULL, &f_teid, &len);
            ogs_freeaddrinfo(addr);
        }
        ogs_assert(rv == OGS_OK);
        rv = ogs_gtp_f_teid_to_ip(&f_teid, &ip);
        ogs_assert(rv == OGS_OK);

        rv = ogs_s1ap_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
        ogs_assert(rv == OGS_OK);
        ogs_s1ap_uint32_to_OCTET_STRING(teid+i, &e_rab->gTP_TEID);

        e_rab->dL_transportLayerAddress =
            (S1AP_TransportLayerAddress_t *)
            CALLOC(1, sizeof(S1AP_TransportLayerAddress_t));
        rv = ogs_s1ap_ip_to_BIT_STRING(&ip, e_rab->dL_transportLayerAddress);
        ogs_assert(rv == OGS_OK);
        e_rab->dL_gTP_TEID = (S1AP_GTP_TEID_t *)
            CALLOC(1, sizeof(S1AP_GTP_TEID_t));
        ogs_s1ap_uint32_to_OCTET_STRING(teid+i+10, e_rab->dL_gTP_TEID);

        e_rab->uL_TransportLayerAddress =
            (S1AP_TransportLayerAddress_t *)
            CALLOC(1, sizeof(S1AP_TransportLayerAddress_t));
        rv = ogs_s1ap_ip_to_BIT_STRING(&ip, e_rab->uL_TransportLayerAddress);
        ogs_assert(rv == OGS_OK);
        e_rab->uL_GTP_TEID = (S1AP_GTP_TEID_t *)
            CALLOC(1, sizeof(S1AP_GTP_TEID_t));
        ogs_s1ap_uint32_to_OCTET_STRING(teid+i+20, e_rab->uL_GTP_TEID);
    }

    ogs_s1ap_buffer_to_OCTET_STRING(
            OGS_HEX(payload, strlen(payload), hexbuf), 132,
            Target_ToSource_TransparentContainer);

    *pkbuf = ogs_s1ap_encode(&pdu);
    if (*pkbuf == NULL) {
        ogs_error("ogs_s1ap_encode() failed");
        return OGS_ERROR;
    }
    return OGS_OK;
}

int tests1ap_build_handover_request_ack_static(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_enb_status_transfer(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_enb_configuration_transfer(
        ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_handover_notify(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_handover_cancel(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_s1_reset(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
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
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
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
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int tests1ap_build_uplink_nas_transport(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "000d40809d000005 0000000200010008 00020001001a0074 7327f908d4bd0307"
        "636a390167000300 0581005155f55d11 030c914477680205 490000055ad2e2b1"
        "252d467ff6de6c47 efd568375b303613 166fb51c6d160cc2 8ab462b006a3d98a"
        "31da90060b0673c5 9c512684158bb119 2c88b3058b37e1ad 081bca84c1582d07"
        "93ede4bddc6d2693 e566371b00644008 0009f1070019b010 004340060009f107"
        "0007",
        "",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        162,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

uint16_t in_cksum(uint16_t *addr, int len); /* from pgw_gtp_path.c */

int testgtpu_build_ping(ogs_pkbuf_t **sendbuf,
        const uint32_t teid, const char *src_ip, const char *dst_ip)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_header_t *gtp_h = NULL;
    ogs_ipsubnet_t src_ipsub, dst_ipsub;

    ogs_assert(src_ip);
    ogs_assert(dst_ip);
    rv = ogs_ipsubnet(&src_ipsub, src_ip, NULL);
    ogs_assert(rv == OGS_OK);
    rv = ogs_ipsubnet(&dst_ipsub, dst_ip, NULL);
    ogs_assert(rv == OGS_OK);

    pkbuf = ogs_pkbuf_alloc(NULL,
            200 /* enough for ICMP; use smaller buffer */);
    ogs_pkbuf_put(pkbuf, 200);
    memset(pkbuf->data, 0, pkbuf->len);
    
    gtp_h = (ogs_gtp_header_t *)pkbuf->data;
    gtp_h->flags = 0x30;
    gtp_h->type = OGS_GTPU_MSGTYPE_GPDU;
    gtp_h->teid = htonl(teid);

    if (dst_ipsub.family == AF_INET) {
        struct ip *ip_h = NULL;
        struct icmp *icmp_h = NULL;

        gtp_h->length = htons(sizeof *ip_h + ICMP_MINLEN);

        ip_h = (struct ip *)(pkbuf->data + OGS_GTPV1U_HEADER_LEN);
        icmp_h = (struct icmp *)((uint8_t *)ip_h + sizeof *ip_h);

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
        ip_h->ip_sum = in_cksum((uint16_t *)ip_h, sizeof *ip_h);
        
        icmp_h->icmp_type = 8;
        icmp_h->icmp_seq = rand();
        icmp_h->icmp_id = rand();
        icmp_h->icmp_cksum = in_cksum((uint16_t *)icmp_h, ICMP_MINLEN);
    } else if (dst_ipsub.family == AF_INET6) {
        struct ip6_hdr *ip6_h = NULL;
        struct icmp6_hdr *icmp6_h = NULL;
        uint16_t plen = 0;
        uint8_t nxt = 0;
        uint8_t *p = NULL;

        gtp_h->length = htons(sizeof *ip6_h + sizeof *icmp6_h);
        plen =  htons(sizeof *icmp6_h);
        nxt = IPPROTO_ICMPV6;

        p = (uint8_t *)pkbuf->data + OGS_GTPV1U_HEADER_LEN;
        ip6_h = (struct ip6_hdr *)p;
        icmp6_h = (struct icmp6_hdr *)((uint8_t *)ip6_h + sizeof *ip6_h);

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
                (uint16_t *)ip6_h, sizeof *ip6_h + sizeof *icmp6_h);

        ip6_h->ip6_flow = htonl(0x60000001);
        ip6_h->ip6_plen = plen;
        ip6_h->ip6_nxt = nxt;;
        ip6_h->ip6_hlim = 0xff;
        memcpy(ip6_h->ip6_src.s6_addr, src_ipsub.sub, sizeof src_ipsub.sub);
        memcpy(ip6_h->ip6_dst.s6_addr, dst_ipsub.sub, sizeof dst_ipsub.sub);
    } else
        ogs_assert_if_reached();

    *sendbuf = pkbuf;

    return OGS_OK;
}

int testgtpu_build_slacc_rs(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "32ff003400000001 00000000 6000000000083aff fe80000000000000 0000000000000002"
        "ff02000000000000 0000000000000002 85007d3500000000",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        60,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int testsgsap_location_update_accept(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "0a01082926240000 111893040509f107 09260e05f49ee88e 64",
        "0a01087942120000 000030040527f412 c9580e05f437ab9c c5",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        25,
        25,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int testsgsap_location_update_reject(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "0b01082926240000 1118930f01030405 09f1070926",
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        21,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int testsgsap_imsi_detach_ack(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "1401082926240000 111893"
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        11,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int testsgsap_paging_request(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "0101082926240000 111893021003766c 72076578616d706c 65036e6574200101"
        "040509f1070926",
        "0101082926240000 111892021003766c 72076578616d706c 65036e6574200101"
        "040509f1070926",  /* Paging-Reject */
        "0101082926240000 111893021003766c 72076578616d706c 65036e6574200102"
        "040509f1070926",  /* Paging-Request with SMS */

        /* Paging-Request for crash-test */
        "0101087942120000 000030021003766c 72076578616d706c 65036e6574200101"
        "040527f412c958",
        "",
        "",
    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        39,
        39,
        39,

        39,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int testsgsap_reset_indication(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "15021003766c7207 6578616d706c6503 6e6574",
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        19,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int testsgsap_release_request(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "1b01082926240000 111893",
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        11,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int testsgsap_downlink_unitdata(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "0701082926240000 1118931626090123 0100079144775810 0650001700048032"
        "2400009160404044 150009c8329bfd06 4d9b53",
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        51,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

int testsgsap_mm_information_request(ogs_pkbuf_t **pkbuf, int i)
{
    const char *payload[TESTS1AP_MAX_MESSAGE] = {
        "1a01087942120000 0000301714430483 d46413450483d464 1347917071028401"
        "29",
        "",
        "",

    };
    uint16_t len[TESTS1AP_MAX_MESSAGE] = {
        33,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    *pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put_data(*pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return OGS_OK;
}

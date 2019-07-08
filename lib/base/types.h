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

#ifndef __BASE_TYPES__
#define __BASE_TYPES__

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S1AP_SCTP_PORT              36412
#define GTPV2_C_UDP_PORT            2123
#define GTPV1_U_UDP_PORT            2152
#define SGSAP_SCTP_PORT             29118

#define SCTP_S1AP_PPID              18
#define SCTP_X2AP_PPID              27
#define SCTP_SGSAP_PPID             0

#define MAX_NUM_OF_SESS             4   /* Num of APN(Session) per UE */
#define MAX_NUM_OF_BEARER           4   /* Num of Bearer per APN(Session) */
#define MAX_NUM_OF_TUNNEL           3   /* Num of Tunnel per Bearer */
#define MAX_NUM_OF_PF               16  /* Num of Packet Filter per Bearer */

#define MAX_NUM_OF_HOSTNAME         16
#define MAX_NUM_OF_PCC_RULE         8 
#define MAX_NUM_OF_FLOW             8   /* Num of Flow per PCC Rule */
#define MAX_NUM_OF_PACKET_FILTER    16  /* Num of Packet Filter per Bearer */

#define MAX_SDU_LEN                 8192
#define PLMN_ID_LEN                 3

#define BCD_TO_BUFFER_LEN(x)        (((x)+1)/2)
#define MAX_IMSI_BCD_LEN            15
#define MAX_IMSI_LEN                BCD_TO_BUFFER_LEN(MAX_IMSI_BCD_LEN)

#define RAND_LEN                    16
#define AUTN_LEN                    16
#define AUTS_LEN                    14
#define MAX_RES_LEN                 16

#define MAX_APN_LEN                 100
#define MAX_PCO_LEN                 251
#define MAX_FILEPATH_LEN            256
#define MAX_FQDN_LEN                256

#define NEXT_ID(__id, __min, __max) \
    ((__id) = ((__id) == (__max) ? (__min) : ((__id) + 1)))
#define COMPARE_ID(__id1, __id2, __max) \
    ((__id2) > (__id1) ? ((__id2) - (__id1) < ((__max)-1) ? -1 : 1) : \
     (__id1) > (__id2) ? ((__id1) - (__id2) < ((__max)-1) ? 1 : -1) : 0)

#define TIME_TO_BCD(x) \
    (((((x) % 10) << 4) & 0xf0) | (((x) / 10) & 0x0f))

/**********************************
 * PLMN_ID Structure             */
typedef struct plmn_id_s {
ED2(uint8_t mcc2:4;,
    uint8_t mcc1:4;)
ED2(uint8_t mnc1:4;,
    uint8_t mcc3:4;)
ED2(uint8_t mnc3:4;,
    uint8_t mnc2:4;)
} __attribute__ ((packed)) plmn_id_t;

uint32_t plmn_id_hexdump(void *plmn_id);

uint16_t plmn_id_mcc(plmn_id_t *plmn_id);
uint16_t plmn_id_mnc(plmn_id_t *plmn_id);
uint16_t plmn_id_mnc_len(plmn_id_t *plmn_id);

void *plmn_id_build(plmn_id_t *plmn_id, 
        uint16_t mcc, uint16_t mnc, uint16_t mnc_len);

#define MAX_NUM_OF_TAI              16

typedef struct tai_s {
    plmn_id_t plmn_id;
    uint16_t tac;
} __attribute__ ((packed)) tai_t;

typedef struct e_cgi_s {
    plmn_id_t plmn_id;
    uint32_t cell_id; /* 28 bit */
} __attribute__ ((packed)) e_cgi_t;

/**************************************************
 * Common Structure
 * S1AP : 9.2.2.1 Transport Layer Address, See 36.414
 * GTP : 8.22 Fully Qualified TEID (F-TEID) */
#define IPV4_LEN                4
#define IPV6_LEN                16
#define IPV4V6_LEN              20
typedef struct ip_s {
    union {
        uint32_t addr;
        uint8_t addr6[IPV6_LEN];
        struct {
            uint32_t addr;
            uint8_t addr6[IPV6_LEN];
        } both;
    };
    uint32_t      len;
ED3(uint8_t       ipv4:1;,
    uint8_t       ipv6:1;,
    uint8_t       reserved:6;)
} ip_t;

/**************************************************
 * 8.14 PDN Address Allocation (PAA) */
#define PAA_IPV4_LEN                                    5
#define PAA_IPV6_LEN                                    18
#define PAA_IPV4V6_LEN                                  22
typedef struct paa_s {
/* 8.34 PDN Type  */
#define GTP_PDN_TYPE_IPV4                               1
#define GTP_PDN_TYPE_IPV6                               2
#define GTP_PDN_TYPE_IPV4V6                             3
#define GTP_PDN_TYPE_NON_IP                             4
ED2(uint8_t spare:5;,
    uint8_t pdn_type:3;)
    union {
        /* GTP_PDN_TYPE_IPV4 */
        uint32_t addr;      

        /* GTP_PDN_TYPE_IPV6 */
        struct {
            /* the IPv6 Prefix Length */
            uint8_t len;
            /* IPv6 Prefix and Interface Identifier */
            uint8_t addr6[IPV6_LEN];
        };

        /* GTP_PDN_TYPE_BOTH */
        struct {
            struct {
                /* the IPv6 Prefix Length */
                uint8_t len;
                /* IPv6 Prefix and Interface Identifier */
                uint8_t addr6[IPV6_LEN];
            };
            uint32_t addr;      
        } __attribute__ ((packed)) both;
    };
} __attribute__ ((packed)) paa_t;

#define MAX_BIT_RATE 10000000000UL

typedef struct bitrate_s {
    uint64_t downlink;        /* bits per seconds */
    uint64_t uplink;          /* bits per seconds */
} bitrate_t;

/**********************************
 * QoS Structure                 */
typedef struct qos_s {
#define PDN_QCI_1                                       1
#define PDN_QCI_2                                       2
#define PDN_QCI_3                                       3
#define PDN_QCI_4                                       4
#define PDN_QCI_5                                       5
#define PDN_QCI_6                                       6
#define PDN_QCI_7                                       7
#define PDN_QCI_8                                       8
#define PDN_QCI_9                                       9
#define PDN_QCI_65                                      65
#define PDN_QCI_66                                      66
#define PDN_QCI_69                                      69
#define PDN_QCI_70                                      70
    uint8_t         qci;

    struct {
    /* Values 1 to 8 should only be assigned for services that are
     * authorized to receive prioritized treatment within an operator domain. 
     * Values 9 to 15 may be assigned to resources that are authorized 
     * by the home network and thus applicable when a UE is roaming. */
        uint8_t     priority_level;

#define PDN_PRE_EMPTION_CAPABILITY_ENABLED              0
#define PDN_PRE_EMPTION_CAPABILITY_DISABLED             1
        uint8_t     pre_emption_capability;
#define PDN_PRE_EMPTION_VULNERABILITY_ENABLED           0
#define PDN_PRE_EMPTION_VULNERABILITY_DISABLED          1
        uint8_t     pre_emption_vulnerability;
    } arp;

    bitrate_t       mbr;  /* Maxmimum Bit Rate (MBR) */
    bitrate_t       gbr;  /* Guaranteed Bit Rate (GBR) */
} qos_t;

/**********************************
 * Flow  Structure               */
#define FLOW_DOWNLINK_ONLY    1
#define FLOW_UPLINK_ONLY      2
typedef struct flow_s {
    uint8_t direction;
    char *description;
} flow_t;

#define FLOW_FREE(__fLOW) \
    do { \
        if ((__fLOW)->description) \
        { \
            ogs_free((__fLOW)->description); \
        } \
        else \
            ogs_assert_if_reached(); \
    } while(0)

/**********************************
 * PCC Rule Structure            */
typedef struct pcc_rule_s {
#define PCC_RULE_TYPE_INSTALL               1
#define PCC_RULE_TYPE_REMOVE                2
    uint8_t type;

#define MAX_PCC_RULE_NAME_LEN               256
    char *name;

    flow_t flow[MAX_NUM_OF_FLOW];
    int num_of_flow;

#define FLOW_STATUS_ENABLED_UPLINK          0
#define FLOW_STATUS_ENABLED_DOWNLINK        1
#define FLOW_STATUS_ENABLED                 2
#define FLOW_STATUS_DISABLED                3
#define FLOW_STATUS_REMOVE                  4
    int flow_status;
    uint32_t precedence;
        
    qos_t  qos;
} pcc_rule_t;

#define PCC_RULE_FREE(__pCCrULE) \
    do { \
        int __pCCrULE_iNDEX; \
        ogs_assert((__pCCrULE)); \
        if ((__pCCrULE)->name) \
        { \
            ogs_free((__pCCrULE)->name); \
        } \
        else \
            ogs_assert_if_reached(); \
        for (__pCCrULE_iNDEX = 0; \
            __pCCrULE_iNDEX < (__pCCrULE)->num_of_flow; __pCCrULE_iNDEX++) \
        { \
            FLOW_FREE(&((__pCCrULE)->flow[__pCCrULE_iNDEX])); \
        } \
        (__pCCrULE)->num_of_flow = 0; \
    } while(0)

/**********************************
 * PDN Structure                 */
typedef struct pdn_s {
    uint32_t        context_identifier;
    char            apn[MAX_APN_LEN+1];
#define HSS_PDN_TYPE_IPV4                       0
#define HSS_PDN_TYPE_IPV6                       1
#define HSS_PDN_TYPE_IPV4V6                     2
#define HSS_PDN_TYPE_IPV4_OR_IPV6               3
    int             pdn_type;

    qos_t           qos;
    bitrate_t       ambr; /* APN-AMBR */

    paa_t           paa;
    ip_t            pgw_ip;
} pdn_t;

int fqdn_build(char *dst, char *src, int len);
int fqdn_parse(char *dst, char *src, int len);

/**************************************************
 * Protocol Configuration Options Structure
 * 8.13 Protocol Configuration Options (PCO) 
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 
 * RFC 3232 [103]
 * RFC 1661 [102] */
#define PCO_PPP_FOR_USE_WITH_IP_PDP_TYPE_OR_IP_PDN_TYPE 0

#define PCO_ID_INTERNET_PROTOCOL_CONTROL_PROTOCOL           0x8021
#define PCO_ID_CHALLENGE_HANDSHAKE_AUTHENTICATION_PROTOCOL  0xc223
#define PCO_ID_P_CSCF_IPV6_ADDRESS_REQUEST                  0x0001
#define PCO_ID_DNS_SERVER_IPV6_ADDRESS_REQUEST              0x0003
#define PCO_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING     0x000a
#define PCO_ID_P_CSCF_IPV4_ADDRESS_REQUEST                  0x000c
#define PCO_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST              0x000d
#define PCO_ID_IPV4_LINK_MTU_REQUEST                        0x0010
typedef struct pco_ipcp_options_s {
    uint8_t type;
    uint8_t len;
    uint32_t addr;
} __attribute__ ((packed)) pco_ipcp_options_t;

#define PCO_MAX_NUM_OF_IPCO_OPTIONS 4
typedef struct pco_ipcp_s {
    uint8_t code;
    uint8_t identifier;
    uint16_t len;
    pco_ipcp_options_t options[PCO_MAX_NUM_OF_IPCO_OPTIONS];
} __attribute__ ((packed)) pco_ipcp_t;

typedef struct pco_id_s {
    uint16_t id;
    uint8_t len;
    void *data;
} pco_id_t;

#define MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID    8
typedef struct pco_s {
ED3(uint8_t ext:1;,
    uint8_t spare:4;,
    uint8_t configuration_protocol:3;)
    uint8_t num_of_id;
    pco_id_t ids[MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID];
} pco_t;

int pco_parse(pco_t *pco, void *data, int data_len);
int pco_build(void *data, int data_len, pco_t *pco);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BASE_TYPES_H__ */

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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_3GPP_TYPES_H
#define OGS_3GPP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OGS_MAX_FILEPATH_LEN            256

#define OGS_MAX_NUM_OF_SESS             4   /* Num of APN(Session) per UE */
#define OGS_MAX_NUM_OF_RULE             4   /* Num of Rule per Session */

#define OGS_MAX_SDU_LEN                 8192
#define OGS_PLMN_ID_LEN                 3

#define OGS_BCD_TO_BUFFER_LEN(x)        (((x)+1)/2)
#define OGS_MAX_IMSI_BCD_LEN            15
#define OGS_MAX_IMSI_LEN                \
    OGS_BCD_TO_BUFFER_LEN(OGS_MAX_IMSI_BCD_LEN)

#define OGS_MAX_IMEISV_BCD_LEN          16
#define OGS_MAX_IMEISV_LEN              \
    OGS_BCD_TO_BUFFER_LEN(OGS_MAX_IMEISV_BCD_LEN)

#define OGS_MAX_NUM_OF_HOSTNAME         16
#define OGS_MAX_APN_LEN                 100
#define OGS_MAX_PCO_LEN                 251
#define OGS_MAX_FQDN_LEN                256
#define OGS_MAX_IFNAME_LEN              32

#define OGS_NEXT_ID(__id, __min, __max) \
    ((__id) = ((__id) == (__max) ? (__min) : ((__id) + 1)))
#define OGS_COMPARE_ID(__id1, __id2, __max) \
    ((__id2) > (__id1) ? ((__id2) - (__id1) < ((__max)-1) ? -1 : 1) : \
     (__id1) > (__id2) ? ((__id1) - (__id2) < ((__max)-1) ? 1 : -1) : 0)

#define OGS_TIME_TO_BCD(x) \
    (((((x) % 10) << 4) & 0xf0) | (((x) / 10) & 0x0f))

#define OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED 0

/**********************************
 * PLMN_ID Structure             */
typedef struct ogs_plmn_id_s {
ED2(uint8_t mcc2:4;,
    uint8_t mcc1:4;)
ED2(uint8_t mnc1:4;,
    uint8_t mcc3:4;)
ED2(uint8_t mnc3:4;,
    uint8_t mnc2:4;)
} __attribute__ ((packed)) ogs_plmn_id_t;

uint32_t ogs_plmn_id_hexdump(void *plmn_id);

uint16_t ogs_plmn_id_mcc(ogs_plmn_id_t *plmn_id);
uint16_t ogs_plmn_id_mnc(ogs_plmn_id_t *plmn_id);
uint16_t ogs_plmn_id_mnc_len(ogs_plmn_id_t *plmn_id);

void *ogs_plmn_id_build(ogs_plmn_id_t *plmn_id, 
        uint16_t mcc, uint16_t mnc, uint16_t mnc_len);

#define OGS_MAX_NUM_OF_TAI              16

typedef struct ogs_tai_s {
    ogs_plmn_id_t plmn_id;
    uint16_t tac;
} __attribute__ ((packed)) ogs_tai_t;

typedef struct ogs_e_cgi_s {
    ogs_plmn_id_t plmn_id;
    uint32_t cell_id; /* 28 bit */
} __attribute__ ((packed)) ogs_e_cgi_t;

/**************************************************
 * Common Structure
 * S1AP : 9.2.2.1 Transport Layer Address, See 36.414
 * GTP : 8.22 Fully Qualified TEID (F-TEID) */
#define OGS_IPV4_LEN                4
#define OGS_IPV6_LEN                16
#define OGS_IPV4V6_LEN              20
typedef struct ogs_ip_s {
    union {
        uint32_t addr;
        uint8_t addr6[OGS_IPV6_LEN];
        struct {
            uint32_t addr;
            uint8_t addr6[OGS_IPV6_LEN];
        } both;
    };
    uint32_t      len;
ED3(uint8_t       ipv4:1;,
    uint8_t       ipv6:1;,
    uint8_t       reserved:6;)
} ogs_ip_t;

int ogs_ip_to_sockaddr(ogs_ip_t *ip, uint16_t port, ogs_sockaddr_t **list);

/**************************************************
 * 8.14 PDN Address Allocation (PAA) */
#define OGS_PAA_IPV4_LEN                                5
#define OGS_PAA_IPV6_LEN                                18
#define OGS_PAA_IPV4V6_LEN                              22
typedef struct ogs_paa_s {
ED2(uint8_t spare:5;,
/* 8.34 PDN Type  */
#define OGS_GTP_PDN_TYPE_IPV4                           1
#define OGS_GTP_PDN_TYPE_IPV6                           2
#define OGS_GTP_PDN_TYPE_IPV4V6                         3
#define OGS_GTP_PDN_TYPE_NON_IP                         4
#define OGS_PFCP_PDN_TYPE_IPV4                          OGS_GTP_PDN_TYPE_IPV4
#define OGS_PFCP_PDN_TYPE_IPV6                          OGS_GTP_PDN_TYPE_IPV6
#define OGS_PFCP_PDN_TYPE_IPV4V6                        OGS_GTP_PDN_TYPE_IPV4V6
#define OGS_PFCP_PDN_TYPE_NONIP                         OGS_GTP_PDN_TYPE_NONIP
    uint8_t pdn_type:3;)
    union {
        /* GTP_PDN_TYPE_IPV4 */
        uint32_t addr;      

        /* GTP_PDN_TYPE_IPV6 */
        struct {
            /* the IPv6 Prefix Length */
            uint8_t len;
            /* IPv6 Prefix and Interface Identifier */
            uint8_t addr6[OGS_IPV6_LEN];
        };

        /* GTP_PDN_TYPE_BOTH */
        struct {
            struct {
                /* the IPv6 Prefix Length */
                uint8_t len;
                /* IPv6 Prefix and Interface Identifier */
                uint8_t addr6[OGS_IPV6_LEN];
            };
            uint32_t addr;      
        } __attribute__ ((packed)) both;
    };
} __attribute__ ((packed)) ogs_paa_t;

#define MAX_BIT_RATE 10000000000UL

typedef struct ogs_bitrate_s {
    uint64_t downlink;        /* bits per seconds */
    uint64_t uplink;          /* bits per seconds */
} ogs_bitrate_t;

/**********************************
 * QoS Structure                 */
typedef struct ogs_qos_s {
#define OGS_PDN_QCI_1                                       1
#define OGS_PDN_QCI_2                                       2
#define OGS_PDN_QCI_3                                       3
#define OGS_PDN_QCI_4                                       4
#define OGS_PDN_QCI_5                                       5
#define OGS_PDN_QCI_6                                       6
#define OGS_PDN_QCI_7                                       7
#define OGS_PDN_QCI_8                                       8
#define OGS_PDN_QCI_9                                       9
#define OGS_PDN_QCI_65                                      65
#define OGS_PDN_QCI_66                                      66
#define OGS_PDN_QCI_69                                      69
#define OGS_PDN_QCI_70                                      70
    uint8_t         qci;

    struct {
    /* Values 1 to 8 should only be assigned for services that are
     * authorized to receive prioritized treatment within an operator domain. 
     * Values 9 to 15 may be assigned to resources that are authorized 
     * by the home network and thus applicable when a UE is roaming. */
        uint8_t     priority_level;

#define OGS_PDN_PRE_EMPTION_CAPABILITY_ENABLED              0
#define OGS_PDN_PRE_EMPTION_CAPABILITY_DISABLED             1
        uint8_t     pre_emption_capability;
#define OGS_PDN_PRE_EMPTION_VULNERABILITY_ENABLED           0
#define OGS_PDN_PRE_EMPTION_VULNERABILITY_DISABLED          1
        uint8_t     pre_emption_vulnerability;
    } arp;

    ogs_bitrate_t   mbr;  /* Maxmimum Bit Rate (MBR) */
    ogs_bitrate_t   gbr;  /* Guaranteed Bit Rate (GBR) */
} ogs_qos_t;

/**********************************
 * Flow  Structure               */
#define OGS_FLOW_DOWNLINK_ONLY    1
#define OGS_FLOW_UPLINK_ONLY      2
typedef struct ogs_flow_s {
    uint8_t direction;
    char *description;
} ogs_flow_t;

#define OGS_FLOW_FREE(__fLOW) \
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
typedef struct ogs_pcc_rule_s {
#define OGS_PCC_RULE_TYPE_INSTALL               1
#define OGS_PCC_RULE_TYPE_REMOVE                2
    uint8_t type;

#define OGS_MAX_PCC_RULE_NAME_LEN               256
    char *name;

/* Num of Flow per PCC Rule */
#define OGS_MAX_NUM_OF_FLOW                     8
    ogs_flow_t flow[OGS_MAX_NUM_OF_FLOW];
    int num_of_flow;

#define OGS_FLOW_STATUS_ENABLED_UPLINK          0
#define OGS_FLOW_STATUS_ENABLED_DOWNLINK        1
#define OGS_FLOW_STATUS_ENABLED                 2
#define OGS_FLOW_STATUS_DISABLED                3
#define OGS_FLOW_STATUS_REMOVE                  4
    int flow_status;
    uint32_t precedence;
        
    ogs_qos_t  qos;
} ogs_pcc_rule_t;

#define OGS_STORE_PCC_RULE(__dST, __sRC) \
    do { \
        int __iNDEX; \
        ogs_assert((__sRC)); \
        ogs_assert((__dST)); \
        OGS_PCC_RULE_FREE(__dST); \
        (__dST)->type = (__sRC)->type; \
        if ((__sRC)->name) { \
            (__dST)->name = ogs_strdup((__sRC)->name); \
            ogs_assert((__dST)->name); \
        } else \
            ogs_assert_if_reached(); \
        for (__iNDEX = 0; __iNDEX < (__sRC)->num_of_flow; __iNDEX++) { \
            (__dST)->flow[__iNDEX].direction = (__sRC)->flow[__iNDEX].direction; \
            (__dST)->flow[__iNDEX].description = \
                ogs_strdup((__sRC)->flow[__iNDEX].description);  \
            ogs_assert((__dST)->flow[__iNDEX].description); \
        } \
        (__dST)->num_of_flow = (__sRC)->num_of_flow; \
        (__dST)->flow_status = (__sRC)->flow_status; \
        (__dST)->precedence = (__sRC)->precedence; \
        memcpy(&(__dST)->qos, &(__sRC)->qos, sizeof(ogs_qos_t)); \
    } while(0)

#define OGS_PCC_RULE_FREE(__pCCrULE) \
    do { \
        int __pCCrULE_iNDEX; \
        ogs_assert((__pCCrULE)); \
        if ((__pCCrULE)->name) \
            ogs_free((__pCCrULE)->name); \
        for (__pCCrULE_iNDEX = 0; \
            __pCCrULE_iNDEX < (__pCCrULE)->num_of_flow; __pCCrULE_iNDEX++) { \
            OGS_FLOW_FREE(&((__pCCrULE)->flow[__pCCrULE_iNDEX])); \
        } \
        (__pCCrULE)->num_of_flow = 0; \
    } while(0)

/**********************************
 * PDN Structure                 */
typedef struct ogs_pdn_s {
    uint32_t        context_identifier;
    char            apn[OGS_MAX_APN_LEN+1];
#define OGS_DIAM_PDN_TYPE_IPV4                      0
#define OGS_DIAM_PDN_TYPE_IPV6                      1
#define OGS_DIAM_PDN_TYPE_IPV4V6                    2
#define OGS_DIAM_PDN_TYPE_IPV4_OR_IPV6              3
    uint8_t         pdn_type;

    ogs_qos_t       qos;
    ogs_bitrate_t   ambr; /* APN-AMBR */

    ogs_paa_t       paa;
    ogs_ip_t        pgw_ip;
} ogs_pdn_t;

int ogs_fqdn_build(char *dst, char *src, int len);
int ogs_fqdn_parse(char *dst, char *src, int len);

/**************************************************
 * Protocol Configuration Options Structure
 * 8.13 Protocol Configuration Options (PCO) 
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 
 * RFC 3232 [103]
 * RFC 1661 [102] */
#define OGS_PCO_PPP_FOR_USE_WITH_IP_PDP_TYPE_OR_IP_PDN_TYPE 0

#define OGS_PCO_ID_INTERNET_PROTOCOL_CONTROL_PROTOCOL           0x8021
#define OGS_PCO_ID_CHALLENGE_HANDSHAKE_AUTHENTICATION_PROTOCOL  0xc223
#define OGS_PCO_ID_P_CSCF_IPV6_ADDRESS_REQUEST                  0x0001
#define OGS_PCO_ID_DNS_SERVER_IPV6_ADDRESS_REQUEST              0x0003
#define OGS_PCO_ID_MS_SUPPORTS_BCM                              0x0005
#define OGS_PCO_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING     0x000a
#define OGS_PCO_ID_P_CSCF_IPV4_ADDRESS_REQUEST                  0x000c
#define OGS_PCO_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST              0x000d
#define OGS_PCO_ID_IPV4_LINK_MTU_REQUEST                        0x0010
#define OGS_PCO_ID_MS_SUPPORT_LOCAL_ADDR_TFT_INDICATOR          0x0011
typedef struct ogs_pco_ipcp_options_s {
    uint8_t type;
    uint8_t len;
    uint32_t addr;
} __attribute__ ((packed)) ogs_pco_ipcp_options_t;

#define OGS_PCO_MAX_NUM_OF_IPCO_OPTIONS 4
typedef struct ogs_pco_ipcp_s {
    uint8_t code;
    uint8_t identifier;
    uint16_t len;
    ogs_pco_ipcp_options_t options[OGS_PCO_MAX_NUM_OF_IPCO_OPTIONS];
} __attribute__ ((packed)) ogs_pco_ipcp_t;

typedef struct ogs_pco_id_s {
    uint16_t id;
    uint8_t len;
    void *data;
} ogs_pco_id_t;

#define OGS_MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID    16
typedef struct ogs_pco_s {
ED3(uint8_t ext:1;,
    uint8_t spare:4;,
    uint8_t configuration_protocol:3;)
    uint8_t num_of_id;
    ogs_pco_id_t ids[OGS_MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID];
} ogs_pco_t;

int ogs_pco_parse(ogs_pco_t *pco, unsigned char *data, int data_len);
int ogs_pco_build(unsigned char *data, int data_len, ogs_pco_t *pco);

#ifdef __cplusplus
}
#endif

#endif /* OGS_3GPP_TYPES_H */

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

#define OGS_MAX_NUM_OF_SESS             4   /* Num of APN(Session) per UE */
#define OGS_MAX_NUM_OF_BEARER           4   /* Num of Bearer per Session */
#define OGS_MAX_NUM_OF_PACKET_BUFFER    64  /* Num of PacketBuffer per UE */

/*
 * The array of TLV messages is limited to 8.
 * So, Flow(PDI.SDF_Filter) in PDR is limited to 8.
 *
 * However, the number of flow in bearer context seems to need more than 16.
 *
 * Therefore, the maximum number of flows of messages is defined as 8,
 * and the maximum number of flows stored by the context is 16.
 */
#define OGS_MAX_NUM_OF_FLOW_IN_PDR      8
#define OGS_MAX_NUM_OF_FLOW_IN_GTP      OGS_MAX_NUM_OF_FLOW_IN_PDR
#define OGS_MAX_NUM_OF_FLOW_IN_NAS      OGS_MAX_NUM_OF_FLOW_IN_PDR
#define OGS_MAX_NUM_OF_FLOW_IN_PCC_RULE OGS_MAX_NUM_OF_FLOW_IN_PDR
#define OGS_MAX_NUM_OF_FLOW_IN_MEDIA_SUB_COMPONENT OGS_MAX_NUM_OF_FLOW_IN_PDR
#define OGS_MAX_NUM_OF_FLOW_IN_BEARER   16

#define OGS_MAX_NUM_OF_GTPU_RESOURCE    4

#define OGS_MAX_SDU_LEN                 8192
#define OGS_MAX_PKT_LEN                 2048
#define OGS_PLMN_ID_LEN                 3
#define OGS_MAX_PLMN_ID_BCD_LEN         6

#define OGS_BCD_TO_BUFFER_LEN(x)        (((x)+1)/2)
#define OGS_MAX_IMSI_BCD_LEN            15
#define OGS_MAX_IMSI_LEN                \
    OGS_BCD_TO_BUFFER_LEN(OGS_MAX_IMSI_BCD_LEN)

#define OGS_MAX_IMEISV_BCD_LEN          16
#define OGS_MAX_IMEISV_LEN              \
    OGS_BCD_TO_BUFFER_LEN(OGS_MAX_IMEISV_BCD_LEN)

#define OGS_MAX_MSISDN_BCD_LEN          15
#define OGS_MAX_MSISDN_LEN              \
    OGS_BCD_TO_BUFFER_LEN(OGS_MAX_MSISDN_BCD_LEN)

#define OGS_MAX_NUM_OF_CELL_ID          16
#define OGS_MAX_NUM_OF_ENB_ID           16
#define OGS_MAX_NUM_OF_DNN              16
#define OGS_MAX_NUM_OF_APN              OGS_MAX_NUM_OF_DNN
#define OGS_MAX_NUM_OF_HOSTNAME         16
#define OGS_MAX_DNN_LEN                 100
#define OGS_MAX_APN_LEN                 OGS_MAX_DNN_LEN
#define OGS_MAX_PCO_LEN                 251
#define OGS_MAX_FQDN_LEN                256

#define OGS_MAX_NUM_OF_SERVED_TAI       16
#define OGS_MAX_NUM_OF_ALGORITHM        8

#define OGS_MAX_NUM_OF_BPLMN            6

#define OGS_NEXT_ID(__id, __min, __max) \
    ((__id) = ((__id) == (__max) ? (__min) : ((__id) + 1)))
#define OGS_COMPARE_ID(__id1, __id2, __max) \
    ((__id2) > (__id1) ? ((__id2) - (__id1) < ((__max)-1) ? -1 : 1) : \
     (__id1) > (__id2) ? ((__id1) - (__id2) < ((__max)-1) ? 1 : -1) : 0)

#define OGS_TIME_TO_BCD(x) \
    (((((x) % 10) << 4) & 0xf0) | (((x) / 10) & 0x0f))

#define OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED 0
#define OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED 0

#define OGS_ACCESS_TYPE_3GPP 1
#define OGS_ACCESS_TYPE_NON_3GPP 2
#define OGS_ACCESS_TYPE_BOTH_3GPP_AND_NON_3GPP 3

#define OGS_MAX_QOS_FLOW_ID             63

#define OGS_DEFAULT_SCTP_MAX_NUM_OF_OSTREAMS 30

/************************************
 * PLMN_ID Structure                */
#define OGS_MAX_NUM_OF_PLMN         6
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

char *ogs_serving_network_name_from_plmn_id(ogs_plmn_id_t *plmn_id);
char *ogs_plmn_id_mcc_string(ogs_plmn_id_t *plmn_id);
char *ogs_plmn_id_mnc_string(ogs_plmn_id_t *plmn_id);

#define OGS_PLMNIDSTRLEN    (sizeof(ogs_plmn_id_t)*2+1)
char *ogs_plmn_id_to_string(ogs_plmn_id_t *plmn_id, char *buf);

/*************************
 * NAS PLMN_ID Structure */
typedef struct ogs_nas_plmn_id_s {
ED2(uint8_t mcc2:4;,
    uint8_t mcc1:4;)
ED2(uint8_t mnc3:4;,
    uint8_t mcc3:4;)
ED2(uint8_t mnc2:4;,
    uint8_t mnc1:4;)
} __attribute__ ((packed)) ogs_nas_plmn_id_t;

void *ogs_nas_from_plmn_id(
        ogs_nas_plmn_id_t *ogs_nas_plmn_id, ogs_plmn_id_t *plmn_id);
void *ogs_nas_to_plmn_id(
        ogs_plmn_id_t *plmn_id, ogs_nas_plmn_id_t *ogs_nas_plmn_id);

/************************************
 * AMF_ID Structure                 */
typedef struct ogs_amf_id_s {
    uint8_t region;
    uint8_t set1;
ED2(uint8_t set2:2;,
    uint8_t pointer:6;)
} __attribute__ ((packed)) ogs_amf_id_t;

typedef struct ogs_guami_s {
    ogs_plmn_id_t plmn_id;
    ogs_amf_id_t amf_id;
} ogs_guami_t;

uint32_t ogs_amf_id_hexdump(ogs_amf_id_t *amf_id);

ogs_amf_id_t *ogs_amf_id_from_string(ogs_amf_id_t *amf_id, const char *hex);
char *ogs_amf_id_to_string(ogs_amf_id_t *amf_id);

uint8_t ogs_amf_region_id(ogs_amf_id_t *amf_id);
uint16_t ogs_amf_set_id(ogs_amf_id_t *amf_id);
uint8_t ogs_amf_pointer(ogs_amf_id_t *amf_id);

ogs_amf_id_t *ogs_amf_id_build(ogs_amf_id_t *amf_id,
        uint8_t region, uint16_t set, uint8_t pointer);

/************************************
 * SUPI/SUCI                       */
char *ogs_supi_from_suci(char *suci);

/************************************
 * SUPI/GPSI                       */
#define OGS_ID_SUPI_TYPE_IMSI "imsi"
#define OGS_ID_GPSI_TYPE_MSISDN "msisdn"
char *ogs_id_get_type(char *str);
char *ogs_id_get_value(char *str);

/************************************
 * TAI Structure                    */
#define OGS_MAX_NUM_OF_TAI              16
typedef struct ogs_eps_tai_s {
    ogs_plmn_id_t plmn_id;
    uint16_t tac;
} __attribute__ ((packed)) ogs_eps_tai_t;

typedef struct ogs_5gs_tai_s {
    ogs_plmn_id_t plmn_id;
    ogs_uint24_t tac;
} __attribute__ ((packed)) ogs_5gs_tai_t;

typedef struct ogs_e_cgi_s {
    ogs_plmn_id_t plmn_id;
    uint32_t cell_id; /* 28 bit */
} __attribute__ ((packed)) ogs_e_cgi_t;

typedef struct ogs_nr_cgi_s {
    ogs_plmn_id_t plmn_id;
    uint64_t cell_id; /* 36 bit */
} __attribute__ ((packed)) ogs_nr_cgi_t;

/************************************
 * S-NSSAI Structure                */
#define OGS_MAX_NUM_OF_SLICE        8
#define OGS_S_NSSAI_NO_SD_VALUE     0xffffff
typedef struct ogs_s_nssai_s {
    uint8_t sst;
    ogs_uint24_t sd;
} __attribute__ ((packed)) ogs_s_nssai_t;

char *ogs_s_nssai_sd_to_string(ogs_uint24_t sd);
ogs_uint24_t ogs_s_nssai_sd_from_string(const char *hex);

/**************************************************
 * Common Structure
 * S1AP : 9.2.2.1 Transport Layer Address, See 36.414
 * GTP : 8.22 Fully Qualified TEID (F-TEID) */
#define OGS_IPV4_LEN                        4
#define OGS_IPV6_LEN                        16
#define OGS_IPV6_DEFAULT_PREFIX_LEN         64
#define OGS_IPV6_128_PREFIX_LEN             128
#define OGS_IPV4V6_LEN                      20
typedef struct ogs_ip_s {
    uint32_t addr;
    uint8_t addr6[OGS_IPV6_LEN];
    uint32_t len;
ED3(uint8_t ipv4:1;,
    uint8_t ipv6:1;,
    uint8_t reserved:6;)
} ogs_ip_t;

int ogs_ip_to_sockaddr(ogs_ip_t *ip, uint16_t port, ogs_sockaddr_t **list);
void ogs_sockaddr_to_ip(
        ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6, ogs_ip_t *ip);

char *ogs_ipv4_to_string(uint32_t addr);
char *ogs_ipv6addr_to_string(uint8_t *addr6);
char *ogs_ipv6prefix_to_string(uint8_t *addr6, uint8_t prefixlen);
int ogs_ipv4_from_string(uint32_t *addr, char *string);
int ogs_ipv6addr_from_string(uint8_t *addr6, char *string);
int ogs_ipv6prefix_from_string(
        uint8_t *addr6, uint8_t *prefixlen, char *string);

/**************************************************
 * GTPv1-C: TS 29.060 7.7.27 End User Address (EUA) */
#define OGS_PDP_EUA_ORG_ETSI 0
#define OGS_PDP_EUA_ORG_IETF 1
#define OGS_PDP_EUA_ETSI_PPP 1
#define OGS_PDP_EUA_IETF_IPV4 0x21
#define OGS_PDP_EUA_IETF_IPV6 0x57
#define OGS_PDP_EUA_IETF_IPV4V6 0x8D
typedef struct ogs_eua_s {
ED2(uint8_t spare:4;,
    uint8_t organization:4;)
    uint8_t type;
    union {
        /* PDU_SESSION_TYPE_IPV4 */
        uint32_t addr;

        /* PDU_SESSION_TYPE_IPV6 */
        uint8_t addr6[OGS_IPV6_LEN];

        /* PDU_SESSION_TYPE_IPV4V6 */
        struct {
            uint32_t addr;
            uint8_t addr6[OGS_IPV6_LEN];
        } __attribute__ ((packed)) both;
    };
} __attribute__ ((packed)) ogs_eua_t;

/**************************************************
 * GTPv2-C: TS 29.274 8.14 PDN Address Allocation (PAA) */
#define OGS_PAA_IPV4_LEN                                5
#define OGS_PAA_IPV6_LEN                                18
#define OGS_PAA_IPV4V6_LEN                              22
typedef struct ogs_paa_s {
ED2(uint8_t spare:5;,
/* 8.34 PDN Type  */
#define OGS_PDU_SESSION_TYPE_IS_VALID(x) \
        ((x) == OGS_PDU_SESSION_TYPE_IPV4 || \
         (x) == OGS_PDU_SESSION_TYPE_IPV6 || \
         (x) == OGS_PDU_SESSION_TYPE_IPV4V6) \

    uint8_t session_type:3;)
    union {
        /* PDU_SESSION_TYPE_IPV4 */
        uint32_t addr;

        /* PDU_SESSION_TYPE_IPV6 */
        struct {
            /* the IPv6 Prefix Length */
            uint8_t len;
            /* IPv6 Prefix and Interface Identifier */
            uint8_t addr6[OGS_IPV6_LEN];
        };

        /* PDU_SESSION_TYPE_IPV4V6 */
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
#define OGS_QOS_INDEX_1                                       1
#define OGS_QOS_INDEX_2                                       2
#define OGS_QOS_INDEX_5                                       5
    uint8_t         index;

    struct {
    /* Values 1 to 8 should only be assigned for services that are
     * authorized to receive prioritized treatment within an operator domain.
     * Values 9 to 15 may be assigned to resources that are authorized
     * by the home network and thus applicable when a UE is roaming. */
        uint8_t     priority_level;
/*
 * Ch 7.3.40 Allocation-Retenion-Proirty in TS 29.272 V15.9.0
 *
 * If the Pre-emption-Capability AVP is not present in the
 * Allocation-Retention-Priority AVP, the default value shall be
 * PRE-EMPTION_CAPABILITY_DISABLED (1).
 *
 * If the Pre-emption-Vulnerability AVP is not present in the
 * Allocation-Retention-Priority AVP, the default value shall be
 * PRE-EMPTION_VULNERABILITY_ENABLED (0).
 *
 * However, to easily set up VoLTE service,
 * enable Pre-emption Capability/Vulnerablility
 * in Default Bearer
 */
#define OGS_EPC_PRE_EMPTION_DISABLED                        1
#define OGS_EPC_PRE_EMPTION_ENABLED                         0

#define OGS_5GC_PRE_EMPTION_DISABLED                        1
#define OGS_5GC_PRE_EMPTION_ENABLED                         2
        uint8_t     pre_emption_capability;
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
        if ((__fLOW)->description) { \
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

    char *id;   /* 5GC */
    char *name; /* EPC */

    ogs_flow_t flow[OGS_MAX_NUM_OF_FLOW_IN_PCC_RULE];
    int num_of_flow;

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
        } \
        if ((__sRC)->id) { \
            (__dST)->id = ogs_strdup((__sRC)->id); \
            ogs_assert((__dST)->id); \
        } \
        for (__iNDEX = 0; __iNDEX < (__sRC)->num_of_flow; __iNDEX++) { \
            (__dST)->flow[__iNDEX].direction = \
                (__sRC)->flow[__iNDEX].direction; \
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
        if ((__pCCrULE)->id) \
            ogs_free((__pCCrULE)->id); \
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
typedef struct ogs_session_s {
    char *name;

    uint32_t context_identifier; /* EPC */
    bool default_dnn_indicator; /* 5GC */

#define OGS_PDU_SESSION_TYPE_IPV4                   1
#define OGS_PDU_SESSION_TYPE_IPV6                   2
#define OGS_PDU_SESSION_TYPE_IPV4V6                 3
#define OGS_PDU_SESSION_TYPE_UNSTRUCTURED           4
#define OGS_PDU_SESSION_TYPE_ETHERNET               5

#define OGS_PDU_SESSION_TYPE_TO_DIAMETER(x)         ((x)-1)
#define OGS_PDU_SESSION_TYPE_FROM_DIAMETER(x)       ((x)+1)
    uint8_t session_type;

#define OGS_SSC_MODE_1                              1
#define OGS_SSC_MODE_2                              2
#define OGS_SSC_MODE_3                              3
    uint8_t ssc_mode;

    ogs_qos_t qos;
    ogs_bitrate_t ambr; /* APN-AMBR */

    ogs_paa_t paa;
    ogs_ip_t ue_ip;
    ogs_ip_t smf_ip;
} ogs_session_t;

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
#define OGS_PCO_ID_PASSWORD_AUTHENTICATION_PROTOCOL             0xc023
#define OGS_PCO_ID_CHALLENGE_HANDSHAKE_AUTHENTICATION_PROTOCOL  0xc223
#define OGS_PCO_ID_P_CSCF_IPV6_ADDRESS_REQUEST                  0x0001
#define OGS_PCO_ID_DNS_SERVER_IPV6_ADDRESS_REQUEST              0x0003
#define OGS_PCO_ID_MS_SUPPORTS_BCM                              0x0005
#define OGS_PCO_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING     0x000a
#define OGS_PCO_ID_P_CSCF_IPV4_ADDRESS_REQUEST                  0x000c
#define OGS_PCO_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST              0x000d
#define OGS_PCO_ID_IPV4_LINK_MTU_REQUEST                        0x0010
#define OGS_PCO_ID_MS_SUPPORT_LOCAL_ADDR_TFT_INDICATOR          0x0011
#define OGS_PCO_ID_P_CSCF_RE_SELECTION_SUPPORT                  0x0012

enum ogs_pco_ipcp_options {
    OGS_IPCP_OPT_IPADDR = 3,
    OGS_IPCP_OPT_PRIMARY_DNS = 129,
    OGS_IPCP_OPT_SECONDARY_DNS = 131,
};

typedef struct ogs_pco_ipcp_options_s {
    uint8_t type;
    uint8_t len;
    uint32_t addr;
} __attribute__ ((packed)) ogs_pco_ipcp_options_t;

#define OGS_PCO_MAX_NUM_OF_IPCP_OPTIONS 4
typedef struct ogs_pco_ipcp_s {
    uint8_t code;
    uint8_t identifier;
    uint16_t len;
    ogs_pco_ipcp_options_t options[OGS_PCO_MAX_NUM_OF_IPCP_OPTIONS];
} __attribute__ ((packed)) ogs_pco_ipcp_t;

typedef struct ogs_pco_pap_s {
    uint8_t code;
    uint8_t identifier;
    uint16_t len;
    uint8_t welcome_len;
    char welcome[255];
} __attribute__ ((packed)) ogs_pco_pap_t;

typedef struct ogs_pco_chap_s {
    uint8_t code;
    uint8_t identifier;
    uint16_t len;
} __attribute__ ((packed)) ogs_pco_chap_t;

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

/*
 * PFCP Specification
 *
 * TS29.244, Ch 8.2.82 User Plane IP Resource Information
 *
 * The following flags are coded within Octet 5:
 * - Bit 1 – V4: If this bit is set to "1", then the IPv4 address field
 *   shall be present, otherwise the IPv4 address field shall not be present.
 * - Bit 2 – V6: If this bit is set to "1", then the IPv6 address field
 *   shall be present, otherwise the IPv6 address field shall not be present.
 * - Bit 3-5 – TEID Range Indication (TEIDRI): the value of this field
 *   indicates the number of bits in the most significant octet of a TEID
 *   that are used to partition the TEID range,
 *   e.g. if this field is set to "4", then the first 4 bits in the TEID
 *   are used to partition the TEID range.
 * - Bit 6 – Associated Network Instance (ASSONI): if this bit is set to "1",
 *   then the Network Instance field shall be present, otherwise the Network
 *   Instance field shall not be present.
 * - Bit 7 – Associated Source Interface (ASSOSI): if this bit is set to "1",
 *   then the Source Interface field shall be present,
 *   otherwise the Source Interface field shall not be present.
 * - Bit 8: Spare, for future use and set to 0.
 *
 * At least one of the V4 and V6 flags shall be set to "1",
 * and both may be set to "1".
 *
 * If both the ASSONI and ASSOSI flags are set to "0", this shall indicate
 * that the User Plane IP Resource Information provided can be used
 * by CP function for any Network Instance and any Source Interface
 * of GTP-U user plane in the UP function.  Octet 6 (TEID Range) shall be
 * present if the TEID Range Indication is not set to zero and
 * shall contain a value of the bits which are used to partition the TEID range.
 * E.g. if the TEID Range Indication is set to "4", then Octet 6 shall be
 * one of values between 0 and 15. When TEID Range Indication is set to zero,
 * the Octet 6 shall not be present, the TEID is not partitioned,
 * i.e. all TEID values are available for use by the CP function.
 *
 * Octets "m to (m+3)" and/or "p to (p+15)" (IPv4 address / IPv6 address fields)
 * , if present, shall contain the respective IP address values.
 *
 * Octets "k to l", if present, shall contain a Network Instance value
 * as encoded in octet "5 to n+4" of the Figure 8.2.4-1 in clause 8.2.4,
 * identifying a Network Instance with which the IP address or TEID Range
 * is associated.
 *
 * Octet r, if present, shall contain a Source Interface value as encoded
 * in octet 5 of the Figure 8.2.2-1 in clause 8.2.2,
 * identifying the Source Interface with which the IP address or TEID Range
 * is associated.
 */

/* Flags(1) + TEID Range(1) + IPV4(4) + IPV6(16) + Source Interface(1) = 23 */
#define OGS_MAX_USER_PLANE_IP_RESOURCE_INFO_LEN \
    (23 + (OGS_MAX_APN_LEN+1))
typedef struct ogs_user_plane_ip_resource_info_s {
    union {
        struct {
ED6(uint8_t     spare:1;,
    uint8_t     assosi:1;,
    uint8_t     assoni:1;,
    uint8_t     teidri:3;,
    uint8_t     v6:1;,
    uint8_t     v4:1;)
        };
        uint8_t flags;
    };

    /*
     * OGS_PFCP-GTPU-TEID   = INDEX              | TEID_RANGE
     * INDEX                = OGS_PFCP-GTPU-TEID & ~TEID_RANGE
     */
#define OGS_PFCP_GTPU_TEID_TO_INDEX(__tEID, __iND, __rANGE) \
    (__tEID & ~(__rANGE << (32 - __iND)))
#define OGS_PFCP_GTPU_INDEX_TO_TEID(__iNDEX, __iND, __rANGE) \
    (__iNDEX | (__rANGE << (32 - __iND)))
    uint8_t     teid_range;
    uint32_t    addr;
    uint8_t     addr6[OGS_IPV6_LEN];
    char        network_instance[OGS_MAX_APN_LEN+1];
    uint8_t     source_interface;
} __attribute__ ((packed)) ogs_user_plane_ip_resource_info_t;

int ogs_sockaddr_to_user_plane_ip_resource_info(
    ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6,
    ogs_user_plane_ip_resource_info_t *info);
int ogs_user_plane_ip_resource_info_to_sockaddr(
    ogs_user_plane_ip_resource_info_t *info,
    ogs_sockaddr_t **addr, ogs_sockaddr_t **addr6);

typedef struct ogs_slice_data_s {
    ogs_s_nssai_t s_nssai;
    bool default_indicator;

    uint32_t context_identifier; /* EPC for checking default APN */

    int num_of_session;
    ogs_session_t session[OGS_MAX_NUM_OF_SESS];
} ogs_slice_data_t;

ogs_slice_data_t *ogs_slice_find_by_s_nssai(
        ogs_slice_data_t *slice_data, int num_of_slice_data,
        ogs_s_nssai_t *s_nssai);

typedef struct ogs_subscription_data_s {
#define OGS_ACCESS_RESTRICTION_UTRAN_NOT_ALLOWED                (1)
#define OGS_ACCESS_RESTRICTION_GERAN_NOT_ALLOWED                (1<<1)
#define OGS_ACCESS_RESTRICTION_GAN_NOT_ALLOWED                  (1<<2)
#define OGS_ACCESS_RESTRICTION_I_HSPA_EVOLUTION_NOT_ALLOWED     (1<<3)
#define OGS_ACCESS_RESTRICTION_WB_E_UTRAN_NOT_ALLOWED           (1<<4)
#define OGS_ACCESS_RESTRICTION_HO_TO_NON_3GPP_ACCESS_NOT_ALLOWED (1<<5)
#define OGS_ACCESS_RESTRICTION_NB_IOT_NOT_ALLOWED               (1<<6)
    uint32_t                access_restriction_data;
#define OGS_SUBSCRIBER_STATUS_SERVICE_GRANTED                   0
#define OGS_SUBSCRIBER_STATUS_OPERATOR_DETERMINED_BARRING       1
    uint32_t                subscriber_status;
#define OGS_NETWORK_ACCESS_MODE_PACKET_AND_CIRCUIT              0
#define OGS_NETWORK_ACCESS_MODE_RESERVED                        1
#define OGS_NETWORK_ACCESS_MODE_ONLY_PACKET                     2
    uint32_t                network_access_mode;

    ogs_bitrate_t           ambr;                           /* UE-AMBR */

#define OGS_RAU_TAU_DEFAULT_TIME                (12*60)     /* 12 min */
    uint32_t                subscribed_rau_tau_timer;       /* unit : seconds */

    int num_of_slice;
    ogs_slice_data_t slice[OGS_MAX_NUM_OF_SLICE];

#define OGS_MAX_NUM_OF_MSISDN                                   2
    int num_of_msisdn;
    struct {
        uint8_t buf[OGS_MAX_MSISDN_LEN];
        int len;
        char bcd[OGS_MAX_MSISDN_BCD_LEN+1];
    } msisdn[OGS_MAX_NUM_OF_MSISDN];
} ogs_subscription_data_t;

void ogs_subscription_data_free(ogs_subscription_data_t *subscription_data);

typedef struct ogs_session_data_s {
    ogs_session_t session;
#define OGS_MAX_NUM_OF_PCC_RULE         8   /* Num of PCC Rule */
    ogs_pcc_rule_t pcc_rule[OGS_MAX_NUM_OF_PCC_RULE];
    int num_of_pcc_rule;
} ogs_session_data_t;

void ogs_session_data_free(ogs_session_data_t *session_data);

typedef struct ogs_media_sub_component_s {
    uint32_t            flow_number;
/*
 * TS29.214
 * 5.3.12 Flow-Usage AVP
 *   NO_INFORMATION(0)
 *   RTCP(1)
 *   AF_SIGNALLING(2)
 *
 * TS29.514
 * 5.6.3.14 Enumeration: FlowUsage
 *   NO_INFO : 1
 *   RTCP : 2
 *   AF_SIGNALLING : 3
 *
 * EPC and 5GC have different values for FlowUsage
 * At this point, we will use the 5GC value.
 */
#define OGS_FLOW_USAGE_NO_INFO          1
#define OGS_FLOW_USAGE_RTCP             2
#define OGS_FLOW_USAGE_AF_SIGNALLING    3
    uint32_t            flow_usage;
    ogs_flow_t          flow[OGS_MAX_NUM_OF_FLOW_IN_MEDIA_SUB_COMPONENT];
    int                 num_of_flow;
} ogs_media_sub_component_t;

typedef struct ogs_media_component_s {
    uint32_t            media_component_number;
    uint32_t            media_type;

    uint64_t            max_requested_bandwidth_dl;
    uint64_t            max_requested_bandwidth_ul;
    uint64_t            min_requested_bandwidth_dl;
    uint64_t            min_requested_bandwidth_ul;
    uint64_t            rr_bandwidth;
    uint64_t            rs_bandwidth;

    int                 flow_status;

#define OGS_MAX_NUM_OF_MEDIA_SUB_COMPONENT     8
    ogs_media_sub_component_t sub[OGS_MAX_NUM_OF_MEDIA_SUB_COMPONENT];
    int                 num_of_sub;
} ogs_media_component_t;

typedef struct ogs_ims_data_s {
    int num_of_msisdn;
    struct {
        uint8_t buf[OGS_MAX_MSISDN_LEN];
        int len;
        char bcd[OGS_MAX_MSISDN_BCD_LEN+1];
    } msisdn[OGS_MAX_NUM_OF_MSISDN];

#define OGS_MAX_NUM_OF_MEDIA_COMPONENT 16
    ogs_media_component_t media_component[OGS_MAX_NUM_OF_MEDIA_COMPONENT];
    int num_of_media_component;
} ogs_ims_data_t;

void ogs_ims_data_free(ogs_ims_data_t *ims_data);

int ogs_pcc_rule_num_of_flow_equal_to_media(
        ogs_pcc_rule_t *pcc_rule, ogs_media_component_t *media_component);
int ogs_pcc_rule_install_flow_from_media(
        ogs_pcc_rule_t *pcc_rule, ogs_media_component_t *media_component);
int ogs_pcc_rule_update_qos_from_media(
        ogs_pcc_rule_t *pcc_rule, ogs_media_component_t *media_component);

#ifdef __cplusplus
}
#endif

#endif /* OGS_3GPP_TYPES_H */

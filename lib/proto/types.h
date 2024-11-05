/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_PROTO_INSIDE) && !defined(OGS_PROTO_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PROTO_TYPES_H
#define OGS_PROTO_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OGS_MAX_NUM_OF_SESS             4   /* Num of APN(Session) per UE */
#define OGS_MAX_NUM_OF_BEARER           4   /* Num of Bearer per Session */
#define OGS_BEARER_PER_UE               8   /* Num of Bearer per UE */
#define OGS_MAX_NUM_OF_PACKET_BUFFER    64  /* Num of PacketBuffer per UE */

/*
 * TS24.008
 * 10.5.6.12 Traffic Flow Template
 * Table 10.5.162: Traffic flow template information element
 *
 * Number of packet filters (octet 3)
 * The number of packet filters contains the binary coding
 * for the number of packet filters in the packet filter list.
 * The number of packet filters field is encoded in bits 4
 * through 1 of octet 3 where bit 4 is the most significant
 * and bit 1 is the least significant bit.
 *
 * For the "delete existing TFT" operation and
 * for the "no TFT operation", the number of packet filters shall be
 * coded as 0. For all other operations, the number of packet filters
 * shall be greater than 0 and less than or equal to 15.
 *
 * TS24.501
 * 9.11.4.13 QoS rules
 * Table 9.11.4.13.1: QoS rules information element
 *
 * For the "delete existing QoS rule" operation and for the "modify existing
 * QoS rule without modifying packet filters" operation, the number of packet
 * filters shall be coded as 0. For the "create new QoS rule" operation
 * and the "modify existing QoS rule and replace all packet filters" operation,
 * the number of packet filters shall be greater than or equal to 0
 * and less than or equal to 15. For all other operations, the number of packet
 * filters shall be greater than 0 and less than or equal to 15.
 *
 * The array of TLV messages is limited to 15.
 * So, Flow(PDI.SDF_Filter) in PDR is limited to 15.
 *
 * Therefore, we defined the maximum number of flows as 15.
 */
#define OGS_MAX_NUM_OF_FLOW_IN_PDR      15
#define OGS_MAX_NUM_OF_FLOW_IN_GTP      OGS_MAX_NUM_OF_FLOW_IN_PDR
#define OGS_MAX_NUM_OF_FLOW_IN_NAS      OGS_MAX_NUM_OF_FLOW_IN_PDR
#define OGS_MAX_NUM_OF_FLOW_IN_PCC_RULE OGS_MAX_NUM_OF_FLOW_IN_PDR
#define OGS_MAX_NUM_OF_FLOW_IN_MEDIA_SUB_COMPONENT OGS_MAX_NUM_OF_FLOW_IN_PDR
#define OGS_MAX_NUM_OF_FLOW_IN_BEARER   15

#define OGS_MAX_NUM_OF_GTPU_RESOURCE    4
#define OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI 8

#define OGS_PLMN_ID_LEN                 3
#define OGS_MAX_PLMN_ID_BCD_LEN         6

#define OGS_CHRGCHARS_LEN               2

#define OGS_MSIN_LEN                    5

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
#define OGS_MAX_EPCO_LEN                65535
#define OGS_MAX_FQDN_LEN                256

#define OGS_MAX_NUM_OF_ALGORITHM        8

#define OGS_MAX_5G_GUTI_LEN             28

#define OGS_MAX_NUM_OF_SERVED_GUMMEI    8   /* maxnoofRATs: 8 */
#define OGS_MAX_NUM_OF_SERVED_GUAMI     256 /* maxnoofServedGUAMIs: 256 */
#define OGS_MAX_NUM_OF_SUPPORTED_TA     256 /* maxnoofTACs: 256 */

/*
 * <December 3, 2023>
 * If I set it to 1024, the AMF crashes in the 'meson test -v registration'.
 * So for now, I will use 512. Once I figure out the cause of this problem,
 * I will try 1024.
 *
 * <December 4, 2023>
 * After increasing the delay in test/app/5gc-init.c from 300ms to 500ms,
 * the problem has been resolved. It seems that as the context memory increases,
 * it takes time for the AMF execution to be completed."
 */
#define OGS_MAX_NUM_OF_SLICE_SUPPORT    1024 /* maxnoofSliceItems: 1024 */

#define OGS_MAX_NUM_OF_PLMN_PER_MME     32  /* maxnoofPLMNs(MME): 32 */
#define OGS_MAX_NUM_OF_PLMN             12  /* maxnoofPLMNs(AMF): 12 */
#define OGS_MAX_NUM_OF_BPLMN            OGS_MAX_NUM_OF_PLMN

#define OGS_MAX_NUM_OF_TAI              16
#define OGS_MAX_NUM_OF_SLICE            8

#define OGS_NEXT_ID(__id, __min, __max) \
    ((__id) = ((__id) == (__max) ? (__min) : ((__id) + 1)))
#define OGS_COMPARE_ID(__id1, __id2, __max) \
    ((__id2) > (__id1) ? ((__id2) - (__id1) < ((__max)-1) ? -1 : 1) : \
     (__id1) > (__id2) ? ((__id1) - (__id2) < ((__max)-1) ? 1 : -1) : 0)

#define OGS_TIME_TO_BCD(x) \
    (((((x) % 10) << 4) & 0xf0) | (((x) / 10) & 0x0f))

/* 3GPP TS 24.007 Table 11.6: */
#define OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED 0
/* 3GPP TS 24.007 Table 11.2.3.1c.1: */
#define OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED 0

#define OGS_ACCESS_TYPE_3GPP 1
#define OGS_ACCESS_TYPE_NON_3GPP 2
#define OGS_ACCESS_TYPE_BOTH_3GPP_AND_NON_3GPP 3

#define OGS_MAX_QOS_FLOW_ID             63

#define OGS_IMSI_STRING "imsi"
#define OGS_MSISDN_STRING "msisdn"
#define OGS_IMEISV_STRING "imeisv"

#define OGS_ACCESS_RESTRICTION_DATA_STRING "access_restriction_data"
#define OGS_SUBSCRIBER_STATUS_STRING "subscriber_status"
#define OGS_OPERATOR_DETERMINED_BARRING_STRING "operator_determined_barring"
#define OGS_NETWORK_ACCESS_MODE_STRING "network_access_mode"
#define OGS_SUBSCRIBED_RAU_TAU_TIMER_STRING "subscribed_rau_tau_timer"

#define OGS_SECURITY_STRING "security"
#define OGS_K_STRING "k"
#define OGS_OPC_STRING "opc"
#define OGS_OP_STRING "op"
#define OGS_AMF_STRING "amf"
#define OGS_RAND_STRING "rand"
#define OGS_SQN_STRING "sqn"

#define OGS_MME_HOST_STRING "mme_host"
#define OGS_MME_REALM_STRING "mme_realm"
#define OGS_MME_TIMESTAMP_STRING "mme_timestamp"
#define OGS_PURGE_FLAG_STRING "purge_flag"

#define OGS_AMBR_STRING "ambr"
#define OGS_DOWNLINK_STRING "downlink"
#define OGS_UPLINK_STRING "uplink"
#define OGS_VALUE_STRING "value"
#define OGS_UNIT_STRING "unit"

#define OGS_POLICY_STRING "policy"
#define OGS_SLICE_STRING "slice"
#define OGS_SST_STRING "sst"
#define OGS_SD_STRING "sd"
#define OGS_DEFAULT_INDICATOR_STRING "default_indicator"
#define OGS_SESSION_STRING "session"
#define OGS_NAME_STRING "name"
#define OGS_TYPE_STRING "type"
#define OGS_QOS_STRING "qos"
#define OGS_INDEX_STRING "index"
#define OGS_ARP_STRING "arp"
#define OGS_PRIORITY_LEVEL_STRING "priority_level"
#define OGS_PRE_EMPTION_CAPABILITY_STRING "pre_emption_capability"
#define OGS_PRE_EMPTION_VULNERABILITY_STRING "pre_emption_vulnerability"

#define OGS_PCC_RULE_STRING "pcc_rule"
#define OGS_MBR_STRING "mbr"
#define OGS_GBR_STRING "gbr"
#define OGS_FLOW_STRING "flow"
#define OGS_DIRECTION_STRING "direction"
#define OGS_DESCRIPTION_STRING "description"

#define OGS_SMF_STRING "smf"
#define OGS_IPV4_STRING "ipv4"
#define OGS_IPV6_STRING "ipv6"
#define OGS_UE_STRING "ue"
#define OGS_IPV4_FRAMED_ROUTES_STRING "ipv4_framed_routes"
#define OGS_IPV6_FRAMED_ROUTES_STRING "ipv6_framed_routes"

/************************************
 * PLMN_ID Structure                */
typedef struct ogs_plmn_id_s {
ED2(uint8_t mcc2:4;,
    uint8_t mcc1:4;)
ED2(uint8_t mnc1:4;,
    uint8_t mcc3:4;)
ED2(uint8_t mnc3:4;,
    uint8_t mnc2:4;)
} __attribute__ ((packed)) ogs_plmn_id_t;

uint32_t ogs_plmn_id_hexdump(const void *plmn_id);

uint16_t ogs_plmn_id_mcc(const ogs_plmn_id_t *plmn_id);
uint16_t ogs_plmn_id_mnc(const ogs_plmn_id_t *plmn_id);
uint16_t ogs_plmn_id_mnc_len(const ogs_plmn_id_t *plmn_id);

void *ogs_plmn_id_build(ogs_plmn_id_t *plmn_id,
        uint16_t mcc, uint16_t mnc, uint16_t mnc_len);

char *ogs_plmn_id_mcc_string(const ogs_plmn_id_t *plmn_id);
char *ogs_plmn_id_mnc_string(const ogs_plmn_id_t *plmn_id);

#define OGS_PLMNIDSTRLEN    (sizeof(ogs_plmn_id_t)*2+1)
char *ogs_plmn_id_to_string(const ogs_plmn_id_t *plmn_id, char *buf);

char *ogs_serving_network_name_from_plmn_id(const ogs_plmn_id_t *plmn_id);
char *ogs_home_network_domain_from_plmn_id(const ogs_plmn_id_t *plmn_id);
char *ogs_epc_domain_from_plmn_id(const ogs_plmn_id_t *plmn_id);
char *ogs_nrf_fqdn_from_plmn_id(const ogs_plmn_id_t *plmn_id);
char *ogs_nssf_fqdn_from_plmn_id(const ogs_plmn_id_t *plmn_id);
char *ogs_home_network_domain_from_fqdn(char *fqdn);
uint16_t ogs_plmn_id_mnc_from_fqdn(char *fqdn);
uint16_t ogs_plmn_id_mcc_from_fqdn(char *fqdn);

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
        ogs_nas_plmn_id_t *ogs_nas_plmn_id, const ogs_plmn_id_t *plmn_id);
void *ogs_nas_to_plmn_id(
        ogs_plmn_id_t *plmn_id, const ogs_nas_plmn_id_t *ogs_nas_plmn_id);

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

uint32_t ogs_amf_id_hexdump(const ogs_amf_id_t *amf_id);

ogs_amf_id_t *ogs_amf_id_from_string(ogs_amf_id_t *amf_id, const char *hex);
char *ogs_amf_id_to_string(const ogs_amf_id_t *amf_id);

uint8_t ogs_amf_region_id(const ogs_amf_id_t *amf_id);
uint16_t ogs_amf_set_id(const ogs_amf_id_t *amf_id);
uint8_t ogs_amf_pointer(const ogs_amf_id_t *amf_id);

ogs_amf_id_t *ogs_amf_id_build(ogs_amf_id_t *amf_id,
        uint8_t region, uint16_t set, uint8_t pointer);

/************************************
 * 9.11.3.4 5GS mobile identity
 * - Protection Scheme */
#define OGS_PROTECTION_SCHEME_NULL 0
#define OGS_PROTECTION_SCHEME_PROFILE_A 1
#define OGS_PROTECTION_SCHEME_PROFILE_B 2

/************************************
 * SUPI/GPSI/GUTI                   */
#define OGS_ID_SUPI_TYPE_IMSI "imsi"
#define OGS_ID_GPSI_TYPE_MSISDN "msisdn"
#define OGS_ID_SUPI_TYPE_IMEISV "imeisv"
#define OGS_ID_5G_GUTI_TYPE "5g-guti"
char *ogs_id_get_type(const char *str);
char *ogs_id_get_value(const char *str);

/************************************
 * TAI Structure                    */
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
#define OGS_S_NSSAI_NO_SD_VALUE     0xffffff
typedef struct ogs_s_nssai_s {
    uint8_t sst;
    ogs_uint24_t sd;
} __attribute__ ((packed)) ogs_s_nssai_t;

char *ogs_s_nssai_sd_to_string(const ogs_uint24_t sd);
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
int ogs_sockaddr_to_ip(
        ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6, ogs_ip_t *ip);

char *ogs_ipv4_to_string(uint32_t addr);
char *ogs_ipv6addr_to_string(const uint8_t *addr6);
char *ogs_ipv6prefix_to_string(const uint8_t *addr6, uint8_t prefixlen);
int ogs_ipv4_from_string(uint32_t *addr, const char *string);
int ogs_ipv6addr_from_string(uint8_t *addr6, const char *string);
int ogs_ipv6prefix_from_string(
        uint8_t *addr6, uint8_t *prefixlen, const char *string);

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

int ogs_check_br_conf(ogs_bitrate_t *br);

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

int ogs_check_qos_conf(ogs_qos_t *qos);

/**********************************
 * TS29.212
 * Ch 5.3.65 Flow-Direction AVP
 *
 * The Flow-Direction AVP (AVP code 1080) is of type Enumerated.
 * It indicates the direction/directions that a filter is applicable,
 * downlink only, uplink only or both down- and uplink (bidirectional).
 *
 *  UNSPECIFIED (0)
 *    The corresponding filter applies for traffic to the UE (downlink),
 *    but has no specific direction declared. The service data flow detection
 *    shall apply the filter for uplink traffic as if the filter was
 *    bidirectional. The PCRF shall not use the value UNSPECIFIED
 *    in filters created by the network in NW-initiated procedures.
 *    The PCRF shall only include the value UNSPECIFIED in filters
 *    in UE-initiated procedures if the same value is received from
 *    in the CCR request from the PCEF.
 *
 *  DOWNLINK (1)
 *    The corresponding filter applies for traffic to the UE.
 *
 *  UPLINK (2)
 *    The corresponding filter applies for traffic from the UE.
 *
 *  BIDIRECTIONAL (3)
 *    The corresponding filter applies for traffic both to and from the UE.
 *
 *  NOTE: The corresponding filter data is unidirectional. The filter
 *        for the opposite direction has the same parameters, but having
 *        the source and destination address/port parameters swapped.
 */
#define OGS_FLOW_UNSPECIFIED      0
#define OGS_FLOW_DOWNLINK_ONLY    1
#define OGS_FLOW_UPLINK_ONLY      2
#define OGS_FLOW_BIDIRECTIONAL    3
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
 * TS29.212
 * Ch 5.3.2 Charging-Rule-Install AVP
 *
 * PCC Rule Structure
 */
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
    uint32_t rating_group;

    ogs_qos_t  qos;
} ogs_pcc_rule_t;

#define OGS_STORE_PCC_RULE(__dST, __sRC) \
    do { \
        int __iNDEX; \
        ogs_assert((__sRC) != NULL); \
        ogs_assert((__dST) != NULL); \
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
        ogs_assert((__pCCrULE) != NULL); \
        if ((__pCCrULE)->id) \
            ogs_free((__pCCrULE)->id); \
        if ((__pCCrULE)->name) \
            ogs_free((__pCCrULE)->name); \
        for (__pCCrULE_iNDEX = 0; \
            __pCCrULE_iNDEX < (__pCCrULE)->num_of_flow; __pCCrULE_iNDEX++) { \
            OGS_FLOW_FREE(&((__pCCrULE)->flow[__pCCrULE_iNDEX])); \
        } \
        memset((__pCCrULE), 0, sizeof(ogs_pcc_rule_t)); \
    } while(0)

/**********************************
 * PDN Structure                 */
typedef struct ogs_session_s {
    char *name;

    uint32_t context_identifier; /* EPC */
    bool default_dnn_indicator; /* 5GC */

    uint8_t charging_characteristics[OGS_CHRGCHARS_LEN];
    bool charging_characteristics_presence;

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

    ogs_ip_t ue_ip;
    char **ipv4_framed_routes;
    char **ipv6_framed_routes;
    ogs_ip_t smf_ip;
} ogs_session_t;

int ogs_fqdn_build(char *dst, const char *src, int len);
int ogs_fqdn_parse(char *dst, const char *src, int len);

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

#define OGS_MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID    32
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

#define OGS_ALL_APN_CONFIGURATIONS_INCLUDED 0
#define OGS_MODIFIED_ADDED_APN_CONFIGURATIONS_INCLUDED 1
    uint32_t all_apn_config_inc;

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
#define OGS_OP_DET_BARRING_ALL_PS_BARRED                                            (1<<0)
#define OGS_OP_DET_BARRING_ROAM_ACC_HPLMN_AP_BARRED                                 (1<<1)
#define OGS_OP_DET_BARRING_ROAM_ACC_VPLMN_AP_BARRED                                 (1<<2)
#define OGS_OP_DET_BARRING_ALL_OUT_CALLS                                            (1<<3)
#define OGS_OP_DET_BARRING_ALL_OUT_INT_CALLS                                        (1<<4)
#define OGS_OP_DET_BARRING_ALL_OUT_INT_CALLS_EXCL_HPLMN_COUNTRY                     (1<<5)
#define OGS_OP_DET_BARRING_ALL_OUT_INTERZONE_CALLS                                  (1<<6)
#define OGS_OP_DET_BARRING_ALL_OUT_INTERZONE_CALLS_EXCL_HPLMN_COUNTRY               (1<<7)
#define OGS_OPD_ETEBARRING_OUT_INT_CALLS_EXCL_EXCL_HPLMN_COUNTRY_AND_INTERZONE_CALLS (1<<8)
    uint32_t operator_determined_barring; /* 3GPP TS 29.272 7.3.30 */
#define OGS_NETWORK_ACCESS_MODE_PACKET_AND_CIRCUIT              0
#define OGS_NETWORK_ACCESS_MODE_RESERVED                        1
#define OGS_NETWORK_ACCESS_MODE_ONLY_PACKET                     2
    uint32_t                network_access_mode;

    ogs_bitrate_t           ambr;                           /* UE-AMBR */

#define OGS_RAU_TAU_DEFAULT_TIME                (12*60)     /* 12 min */
    uint32_t                subscribed_rau_tau_timer;       /* unit : seconds */

    int num_of_slice;
    ogs_slice_data_t slice[OGS_MAX_NUM_OF_SLICE];

    char *imsi;

#define OGS_MAX_NUM_OF_MSISDN                                   2
    int num_of_msisdn;
    struct {
        uint8_t buf[OGS_MAX_MSISDN_LEN];
        int len;
        char bcd[OGS_MAX_MSISDN_BCD_LEN+1];
    } msisdn[OGS_MAX_NUM_OF_MSISDN];

    char *mme_host;
    char *mme_realm;
    bool purge_flag;
} ogs_subscription_data_t;

void ogs_subscription_data_free(ogs_subscription_data_t *subscription_data);

typedef struct ogs_session_data_s {
    ogs_session_t session;
#define OGS_MAX_NUM_OF_PCC_RULE         8   /* Num of PCC Rule */
    ogs_pcc_rule_t pcc_rule[OGS_MAX_NUM_OF_PCC_RULE];
    int num_of_pcc_rule;
} ogs_session_data_t;

#define OGS_STORE_SESSION_DATA(__dST, __sRC) \
    do { \
        int rv, j; \
        ogs_assert((__dST) != NULL); \
        ogs_assert((__sRC) != NULL); \
        OGS_SESSION_DATA_FREE(__dST); \
        if ((__sRC)->session.name) { \
            (__dST)->session.name = ogs_strdup((__sRC)->session.name); \
            ogs_assert((__dST)->session.name); \
        } \
        (__dST)->session.session_type = (__sRC)->session.session_type; \
        memcpy(&(__dST)->session.ambr, &(__sRC)->session.ambr, \
                sizeof((__dST)->session.ambr)); \
        memcpy(&(__dST)->session.qos, &(__sRC)->session.qos, \
                sizeof((__dST)->session.qos)); \
        (__dST)->num_of_pcc_rule = (__sRC)->num_of_pcc_rule; \
        for (j = 0; j < (__dST)->num_of_pcc_rule; j++) { \
            rv = ogs_check_qos_conf(&(__sRC)->pcc_rule[j].qos); \
            ogs_assert(rv == OGS_OK); \
            OGS_STORE_PCC_RULE(&(__dST)->pcc_rule[j], &(__sRC)->pcc_rule[j]); \
        } \
    } while(0)

#define OGS_SESSION_DATA_FREE(__sESSdATA) \
    do { \
        int i; \
        ogs_assert((__sESSdATA) != NULL); \
        if ((__sESSdATA)->session.name) \
            ogs_free((__sESSdATA)->session.name); \
        for (i = 0; i < (__sESSdATA)->num_of_pcc_rule; i++) \
            OGS_PCC_RULE_FREE(&(__sESSdATA)->pcc_rule[i]); \
        memset((__sESSdATA), 0, sizeof(ogs_session_data_t)); \
    } while(0)

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

typedef struct ogs_datum_s {
    unsigned char *data;
    unsigned int size;
} ogs_datum_t;

typedef struct ogs_port_s {
    bool presence;
    uint16_t port;
} ogs_port_t;

#ifdef __cplusplus
}
#endif

#endif /* OGS_PROTO_TYPES_H */

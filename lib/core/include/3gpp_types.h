#ifndef __TYPES_H__
#define __TYPES_H__

#include "core_list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S1AP_SCTP_PORT              36412
#define GTPV2_C_UDP_PORT            2123
#define GTPV1_U_UDP_PORT            2152

#define SCTP_S1AP_PPID              18
#define SCTP_X2AP_PPID              27

#define MAX_NUM_OF_ENB              128
#define MAX_NUM_OF_UE               128
#define MAX_NUM_OF_SESS             4
#define MAX_NUM_OF_BEARER           4
#define MAX_NUM_OF_TUNNEL           3   /* Num of Tunnel per Bearer */
#define MAX_NUM_OF_PF               16  /* Num of Packet Filter per Bearer */

#define MAX_POOL_OF_UE              (MAX_NUM_OF_ENB * MAX_NUM_OF_UE)
#define MAX_POOL_OF_SESS            (MAX_POOL_OF_UE * MAX_NUM_OF_SESS)
#define MAX_POOL_OF_BEARER          (MAX_POOL_OF_SESS * MAX_NUM_OF_BEARER)
#define MAX_POOL_OF_TUNNEL          (MAX_POOL_OF_BEARER * MAX_NUM_OF_TUNNEL)
#define MAX_POOL_OF_PF              (MAX_POOL_OF_BEARER * MAX_NUM_OF_PF)
#define MAX_POOL_OF_DIAMETER_SESS   (MAX_POOL_OF_UE * MAX_NUM_OF_SESS)

#define MAX_NUM_OF_HOSTNAME         16
#define MAX_NUM_OF_PCC_RULE         8 
#define MAX_NUM_OF_FLOW             8   /* Num of Flow per PCC Rule */
#define MAX_NUM_OF_PACKET_FILTER    16  /* Num of Packet Filter per Bearer */

#define MAX_SDU_LEN             8192
#define PLMN_ID_LEN             3

#define BCD_TO_BUFFER_LEN(x)    (((x)+1)/2)
#define MAX_IMSI_BCD_LEN        15
#define MAX_IMSI_LEN            BCD_TO_BUFFER_LEN(MAX_IMSI_BCD_LEN)

#define RAND_LEN                16
#define AUTN_LEN                16
#define AUTS_LEN                14
#define MAX_RES_LEN             16

#define MAX_APN_LEN             100
#define MAX_PCO_LEN             251

#define NEXT_ID(__id, __min, __max) \
    ((__id) = ((__id) == (__max) ? (__min) : ((__id) + 1)))
#define COMPARE_ID(__id1, __id2, __max) \
    ((__id2) > (__id1) ? ((__id2) - (__id1) < ((__max)-1) ? -1 : 1) : \
     (__id1) > (__id2) ? ((__id1) - (__id2) < ((__max)-1) ? 1 : -1) : 0)

#define TIME_TO_BCD(x) \
    (((((x) % 10) << 4) & 0xf0) | (((x) / 10) & 0x0f))

/**********************************
 * PLMN_ID Structure             */
typedef struct _plmn_id_t {
ED2(c_uint8_t mcc2:4;,
    c_uint8_t mcc1:4;)
ED2(c_uint8_t mnc1:4;,
    c_uint8_t mcc3:4;)
ED2(c_uint8_t mnc3:4;,
    c_uint8_t mnc2:4;)
} __attribute__ ((packed)) plmn_id_t;

CORE_DECLARE(c_uint16_t) plmn_id_mcc(plmn_id_t *plmn_id);
CORE_DECLARE(c_uint16_t) plmn_id_mnc(plmn_id_t *plmn_id);
CORE_DECLARE(c_uint16_t) plmn_id_mnc_len(plmn_id_t *plmn_id);

CORE_DECLARE(void *) plmn_id_build(plmn_id_t *plmn_id, 
        c_uint16_t mcc, c_uint16_t mnc, c_uint16_t mnc_len);

#define MAX_NUM_OF_TAI              16

typedef struct _tai_t {
    plmn_id_t plmn_id;
    c_uint16_t tac;
} __attribute__ ((packed)) tai_t;

typedef struct _e_cgi_t {
    plmn_id_t plmn_id;
    c_uint32_t cell_id; /* 28 bit */
} __attribute__ ((packed)) e_cgi_t;

typedef struct _guti_t {
    plmn_id_t plmn_id;
    c_uint16_t mme_gid;
    c_uint8_t mme_code;
    c_uint32_t m_tmsi;
} __attribute__ ((packed)) guti_t;

/**************************************************
 * Common Structure
 * S1AP : 9.2.2.1 Transport Layer Address, See 36.414
 * GTP : 8.22 Fully Qualified TEID (F-TEID) */
#define IPV4_LEN                4
#define IPV6_LEN                16
#define IPV4V6_LEN              20
typedef struct _ip_t {
    union {
        c_uint32_t addr;
        c_uint8_t addr6[IPV6_LEN];
        struct {
            c_uint32_t addr;
            c_uint8_t addr6[IPV6_LEN];
        } both;
    };
    c_uint32_t      len;
ED3(c_uint8_t       ipv4:1;,
    c_uint8_t       ipv6:1;,
    c_uint8_t       reserved:6;)
} ip_t;

/**************************************************
 * 8.14 PDN Address Allocation (PAA) */
#define PAA_IPV4_LEN                                    5
#define PAA_IPV6_LEN                                    18
#define PAA_IPV4V6_LEN                                  22
typedef struct _paa_t {
/* 8.34 PDN Type  */
#define GTP_PDN_TYPE_IPV4                               1
#define GTP_PDN_TYPE_IPV6                               2
#define GTP_PDN_TYPE_IPV4V6                             3
#define GTP_PDN_TYPE_NON_IP                             4
ED2(c_uint8_t spare:5;,
    c_uint8_t pdn_type:3;)
    union {
        /* GTP_PDN_TYPE_IPV4 */
        c_uint32_t addr;      

        /* GTP_PDN_TYPE_IPV6 */
        struct {
            /* the IPv6 Prefix Length */
            c_uint8_t len;
            /* IPv6 Prefix and Interface Identifier */
            c_uint8_t addr6[IPV6_LEN];
        };

        /* GTP_PDN_TYPE_BOTH */
        struct {
            struct {
                /* the IPv6 Prefix Length */
                c_uint8_t len;
                /* IPv6 Prefix and Interface Identifier */
                c_uint8_t addr6[IPV6_LEN];
            };
            c_uint32_t addr;      
        } __attribute__ ((packed)) both;
    };
} __attribute__ ((packed)) paa_t;

#define MAX_BIT_RATE C_UINT64_C(10000000000)

typedef struct _bitrate_t {
    c_uint64_t downlink;        /* bits per seconds */
    c_uint64_t uplink;          /* bits per seconds */
} bitrate_t;

/**********************************
 * QoS Structure                 */
typedef struct _qos_t {
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
    c_uint8_t       qci;

    struct {
    /* Values 1 to 8 should only be assigned for services that are
     * authorized to receive prioritized treatment within an operator domain. 
     * Values 9 to 15 may be assigned to resources that are authorized 
     * by the home network and thus applicable when a UE is roaming. */
        c_uint8_t       priority_level;

#define PDN_PRE_EMPTION_CAPABILITY_ENABLED              0
#define PDN_PRE_EMPTION_CAPABILITY_DISABLED             1
        c_uint8_t       pre_emption_capability;
#define PDN_PRE_EMPTION_VULNERABILITY_ENABLED           0
#define PDN_PRE_EMPTION_VULNERABILITY_DISABLED          1
        c_uint8_t       pre_emption_vulnerability;
    } arp;

    bitrate_t       mbr;  /* Maxmimum Bit Rate (MBR) */
    bitrate_t       gbr;  /* Guaranteed Bit Rate (GBR) */
} qos_t;

/**********************************
 * Flow  Structure               */
#define FLOW_DOWNLINK_ONLY    1
#define FLOW_UPLINK_ONLY      2
typedef struct _flow_t {
    c_uint8_t direction;
    c_int8_t *description;
} flow_t;

#define FLOW_FREE(__fLOW) \
    do { \
        if ((__fLOW)->description) \
        { \
            CORE_FREE((__fLOW)->description); \
        } \
        else \
            d_assert(0,, "Null param"); \
    } while(0)

/**********************************
 * PCC Rule Structure            */
typedef struct _pcc_rule_t {
#define PCC_RULE_TYPE_INSTALL               1
#define PCC_RULE_TYPE_REMOVE                2
    c_uint8_t type;

#define MAX_PCC_RULE_NAME_LEN               256
    c_int8_t *name;

    flow_t flow[MAX_NUM_OF_FLOW];
    int num_of_flow;

#define FLOW_STATUS_ENABLED_UPLINK          0
#define FLOW_STATUS_ENABLED_DOWNLINK        1
#define FLOW_STATUS_ENABLED                 2
#define FLOW_STATUS_DISABLED                3
#define FLOW_STATUS_REMOVE                  4
    c_int8_t flow_status;
    c_uint32_t precedence;
        
    qos_t  qos;
} pcc_rule_t;

#define PCC_RULE_FREE(__pCCrULE) \
    do { \
        int __pCCrULE_iNDEX; \
        d_assert((__pCCrULE), break,); \
        if ((__pCCrULE)->name) \
        { \
            CORE_FREE((__pCCrULE)->name); \
        } \
        else \
            d_assert(0,, "Null param"); \
        for (__pCCrULE_iNDEX = 0; \
            __pCCrULE_iNDEX < (__pCCrULE)->num_of_flow; __pCCrULE_iNDEX++) \
        { \
            FLOW_FREE(&((__pCCrULE)->flow[__pCCrULE_iNDEX])); \
        } \
        (__pCCrULE)->num_of_flow = 0; \
    } while(0)

/**********************************
 * PDN Structure                 */
typedef struct _pdn_t {
    c_uint32_t      context_identifier;
    c_int8_t        apn[MAX_APN_LEN+1];
#define HSS_PDN_TYPE_IPV4                       0
#define HSS_PDN_TYPE_IPV6                       1
#define HSS_PDN_TYPE_IPV4V6                     2
#define HSS_PDN_TYPE_IPV4_OR_IPV6               3
    c_int8_t        pdn_type;

    qos_t           qos;
    bitrate_t       ambr; /* APN-AMBR */

    paa_t           paa;
    ip_t            pgw_ip;
} pdn_t;

CORE_DECLARE(c_int16_t) apn_build(c_int8_t *dst, c_int8_t *src, c_int16_t len);
CORE_DECLARE(c_int16_t) apn_parse(c_int8_t *dst, c_int8_t *src, c_int16_t len);

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
typedef struct _pco_ipcp_options_t {
    c_uint8_t type;
    c_uint8_t len;
    c_uint32_t addr;
} __attribute__ ((packed)) pco_ipcp_options_t;

#define PCO_MAX_NUM_OF_IPCO_OPTIONS 4
typedef struct _pco_ipcp_t {
    c_uint8_t code;
    c_uint8_t identifier;
    c_uint16_t len;
    pco_ipcp_options_t options[PCO_MAX_NUM_OF_IPCO_OPTIONS];
} __attribute__ ((packed)) pco_ipcp_t;

typedef struct _pco_id_t {
    c_uint16_t id;
    c_uint8_t len;
    void *data;
} pco_id_t;

#define MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID    8
typedef struct _pco_t {
ED3(c_uint8_t ext:1;,
    c_uint8_t spare:4;,
    c_uint8_t configuration_protocol:3;)
    c_uint8_t num_of_id;
    pco_id_t ids[MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID];
} pco_t;

CORE_DECLARE(c_int16_t) pco_parse(pco_t *pco, void *data, int data_len);
CORE_DECLARE(c_int16_t) pco_build(void *data, int data_len, pco_t *pco);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __3GPP_COMMON_H__ */

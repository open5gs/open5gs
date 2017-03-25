#ifndef __GTP_TYPES_H__
#define __GTP_TYPES_H__

#include "core_tlv_msg.h"
#include "3gpp_defs.h"
#include "3gpp_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 8.7 Aggregate Maximum Bit Rate (AMBR) */
typedef struct _gtp_ambr_t {
    c_uint32_t uplink;
    c_uint32_t downlink;
} __attribute__ ((packed)) gtp_ambr_t;

/* 8.13 Protocol Configuration Options (PCO) 
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 
 * RFC 3232 [103]
 * RFC 1661 [102] */
#define GTP_PCO_PPP_FOR_USE_WITH_IP_PDP_TYPE_OR_IP_PDN_TYPE 0

#define GTP_PROTOCOL_OR_CONTAINER_ID_INTERNET_PROTOCOL_CONTROL_PROTOCOL 0x8021
#define GTP_PROTOCOL_OR_CONTAINER_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST 0x000d
#define GTP_PROTOCOL_OR_CONTAINER_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING 0x000a
typedef struct _gtp_protocol_or_container_id_t {
    c_uint16_t id;
    c_uint8_t length;
    void *contents;
} gtp_protocol_or_container_id_t;

#define GTP_MAX_PROTOCOL_OR_CONTAINER_ID    4
#define GTP_MAX_PCO_LEN 253
typedef struct _gtp_pco_t {
ED3(c_uint8_t ext:1;,
    c_uint8_t spare:4;,
    c_uint8_t configuration_protocol:3;)
    c_uint8_t num_of_id;
    gtp_protocol_or_container_id_t ids[GTP_MAX_PROTOCOL_OR_CONTAINER_ID];
} gtp_pco_t;

CORE_DECLARE(c_int16_t) gtp_parse_pco(gtp_pco_t *pco, tlv_octet_t *octet);
CORE_DECLARE(c_int16_t) gtp_build_pco(
        tlv_octet_t *octet, gtp_pco_t *pco, void *data, int data_len);

/* 8.15 Bearer Quality of Service (Bearer QoS) */
#define GTP_BEARER_QOS_LEN 22
typedef struct _gtp_bearer_qos_t {
ED5(c_uint8_t spare1:1;,
    /* See 3GPP TS 29.212[29], clause 5.3.46 Pre-emption-Capability AVP. */
    c_uint8_t pci:1;, 
    /* See 3GPP TS 29.212[29], clause 5.3.45 Priority-Level AVP. 
     * PL encodes each priority level defined for the Priority-Level AVP 
     * as the binary value of the priority level.  */
    c_uint8_t pl:4;,
    c_uint8_t spare2:1;,
    /* See 3GPP TS 29.212[29], clause 5.3.47 Pre-emption-Vulnerability AVP. */
    c_uint8_t pvi:1;)
    c_uint8_t qci; /* specified in 3GPP TS 23.203 [48]. */

    /* specified in 3GPP TS 36.413 [10]. */
    c_uint64_t ul_mbr;
    c_uint64_t dl_mbr;
    c_uint64_t ul_gbr;
    c_uint64_t dl_gbr;

    /* NOTE : The encoding in 3GPP TS 24.301 [23] and 3GPP TS 36.413 [10] 
     * is different from the encoding within this specification.  */
} __attribute__ ((packed)) gtp_bearer_qos_t;

CORE_DECLARE(c_int16_t) gtp_parse_bearer_qos(
    gtp_bearer_qos_t *bearer_qos, tlv_octet_t *octet);
CORE_DECLARE(c_int16_t) gtp_build_bearer_qos(
    tlv_octet_t *octet, gtp_bearer_qos_t *bearer_qos, void *data, int data_len);

/* 8.17 RAT Type */
#define GTP_RAT_TYPE_UTRAN                                  1
#define GTP_RAT_TYPE_GERAN                                  2
#define GTP_RAT_TYPE_WLAN                                   3
#define GTP_RAT_TYPE_GAN                                    4
#define GTP_RAT_TYPE_HSPA_EVOLUTION                         5
#define GTP_RAT_TYPE_EUTRAN                                 6
#define GTP_RAT_TYPE_VIRTUAL                                7
#define GTP_RAT_TYPE_EUTRAN_NB_IOT                          8

/* 8.21 User Location Information (ULI) */
#define GTP_MAX_ULI_LEN sizeof(gtp_uli_t)
typedef struct _gtp_uli_cgi_t {
    plmn_id_t plmn_id;
    c_uint16_t lac;
    c_uint16_t ci;
} __attribute__ ((packed)) gtp_uli_cgi_t;

typedef struct _gtp_uli_sai_t {
    plmn_id_t plmn_id;
    c_uint16_t lac;
    c_uint16_t sac;
} __attribute__ ((packed)) gtp_uli_sai_t;

typedef struct _gtp_uli_rai_t {
    plmn_id_t plmn_id;
    c_uint16_t lac;
    c_uint16_t rac;
} __attribute__ ((packed)) gtp_uli_rai_t;

typedef struct _gtp_uli_tai_t {
    plmn_id_t plmn_id;
    c_uint16_t tac;
} __attribute__ ((packed)) gtp_uli_tai_t;

typedef struct _gtp_uli_ecgi_t {
    plmn_id_t plmn_id;
    c_uint32_t eci; /* 28 bit */
} __attribute__ ((packed)) gtp_uli_ecgi_t;

typedef struct _gtp_uli_lai_t {
    plmn_id_t plmn_id;
    c_uint16_t lac;
} __attribute__ ((packed)) gtp_uli_lai_t;

typedef struct _gtp_uli_t {
    struct {
    ED7(c_uint8_t spare:2;,
        c_uint8_t lai:1;,
        c_uint8_t ecgi:1;,
        c_uint8_t tai:1;,
        c_uint8_t rai:1;,
        c_uint8_t sai:1;,
        c_uint8_t cgi:1;)
    } flags;
    gtp_uli_cgi_t cgi;
    gtp_uli_sai_t sai;
    gtp_uli_rai_t rai;
    gtp_uli_tai_t tai;
    gtp_uli_ecgi_t ecgi;
    gtp_uli_lai_t lai;
} gtp_uli_t;

CORE_DECLARE(c_int16_t) gtp_parse_uli(gtp_uli_t *uli, tlv_octet_t *octet);
CORE_DECLARE(c_int16_t) gtp_build_uli(
        tlv_octet_t *octet, gtp_uli_t *uli, void *data, int data_len);

/* 8.14 PDN Address Allocation (PAA) */
#define GTP_PAA_IPV4_LEN                                    5
#define GTP_PAA_IPV6_LEN                                    18
typedef struct _gtp_paa_t {
ED2(c_uint8_t spare:6;,
    c_uint8_t pdn_type:2;)
    union {
        c_uint32_t ipv4_prefix;;
        struct {
            c_uint8_t ipv6_prefix_lengh;
            c_uint8_t ipv6_prefix[16];
        };
    };
} gtp_paa_t;

/* 8.22 Fully Qualified TEID (F-TEID) */
#define GTP_F_TEID_S1_U_ENODEB_GTP_U                        0
#define GTP_F_TEID_S1_U_SGW_GTP_U                           1
#define GTP_F_TEID_S12_RNC_GTP_U                            2
#define GTP_F_TEID_S12_SGW_GTP_U                            3
#define GTP_F_TEID_S5_S8_SGW_GTP_U                          4
#define GTP_F_TEID_S5_S8_PGW_GTP_U                          5
#define GTP_F_TEID_S5_S8_SGW_GTP_C                          6
#define GTP_F_TEID_S5_S8_PGW_GTP_C                          7
#define GTP_F_TEID_S5_S8_SGW_PMIPV6                         8
#define GTP_F_TEID_S5_S8_PGW_PMIPV6                         9
#define GTP_F_TEID_S11_MME_GTP_C                            10
#define GTP_F_TEID_S11_S4_SGW_GTP_C                         11
#define GTP_F_TEID_S10_MME_GTP_C                            12
#define GTP_F_TEID_S3_MME_GTP_C                             13
#define GTP_F_TEID_S3_SGSN_GTP_C                            14
#define GTP_F_TEID_S4_SGSN_GTP_U                            15
#define GTP_F_TEID_S4_SGW_GTP_U                             16
#define GTP_F_TEID_S4_SGSN_GTP_C                            17
#define GTP_F_TEID_S16_SGSN_GTP_C                           18
#define GTP_F_TEID_ENODEB_GTP_U_FOR_DL_DATA_FORWARDING      19
#define GTP_F_TEID_ENODEB_GTP_U_FOR_UL_DATA_FORWARDING      20
#define GTP_F_TEID_RNC_GTP_U_FOR_DATA_FORWARDING            21
#define GTP_F_TEID_SGSN_GTP_U_FOR_DATA_FORWARDING           22
#define GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING         23
#define GTP_F_TEID_SM_MBMS_GW_GTP_C                         24
#define GTP_F_TEID_SN_MBMS_GW_GTP_C                         25
#define GTP_F_TEID_SM_MME_GTP_C                             26
#define GTP_F_TEID_SN_SGSN_GTP_C                            27
#define GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING         28
#define GTP_F_TEID_SN_SGSN_GTP_U                            29
#define GTP_F_TEID_S2B_EPDG_GTP_C                           30
#define GTP_F_TEID_S2B_U_EPDG_GTP_U                         31
#define GTP_F_TEID_S2B_PGW_GTP_C                            32
#define GTP_F_TEID_S2B_U_PGW_GTP_U                          33
#define GTP_F_TEID_S2A_TWAN_GTP_U                           34
#define GTP_F_TEID_S2A_TWAN_GTP_C                           35
#define GTP_F_TEID_S2A_PGW_GTP_C                            36
#define GTP_F_TEID_S2A_PGW_GTP_U                            37
#define GTP_F_TEID_S11_MME_GTP_U                            38
#define GTP_F_TEID_S11_SGW_GTP_U                            39

#define GTP_F_TEID_IPV4_LEN                                 9
#define GTP_F_TEID_IPV6_LEN                                 21
typedef struct _gtp_f_teid_t {
ED3(c_uint8_t ipv4:1;,
    c_uint8_t ipv6:1;,
    c_uint8_t interface_type:6;)
    c_uint32_t teid;
    union {
        c_uint32_t ipv4_addr;
        c_uint8_t ipv6_addr[IPV6_LEN];
    };
} __attribute__ ((packed)) gtp_f_teid_t;

/* 8.34 PDN Type  */
#define GTP_PDN_TYPE_IPV4                                   1
#define GTP_PDN_TYPE_IPV6                                   2
#define GTP_PDN_TYPE_BOTH                                   3
#define GTP_PDN_TYPE_NON_IP                                 4

/* 8.44 UE Time Zone */
#define GTP_UE_TIME_ZONE_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME 0
#define GTP_UE_TIME_ZONE_1_HOUR_FOR_DAYLIGHT_SAVING_TIME        1
#define GTP_UE_TIME_ZONE_2_HOUR_FOR_DAYLIGHT_SAVING_TIME        2
typedef struct _gtp_ue_timezone_t {
    /* Time Zone" IE in 3GPP TS 24.008 [5].
     * This field uses the same format as the Timezone field used in the 
     * TP-Service-Centre-Time-Stamp, which is defined in 3GPP TS 23.040 [90], 
     * and its value shall be set as defined in 3GPP TS 22.042 */
    c_uint8_t timezone;
ED2(c_uint8_t spare:6;,
    c_uint8_t daylight_saving_time:2;)
} __attribute__ ((packed)) gtp_ue_timezone_t;

/* 8.57 APN Restriction */
#define GTP_APN_NO_RESTRICTION                              0
#define GTP_APN_RESTRICTION_PUBLIC_1                        1
#define GTP_APN_RESTRICTION_PUBLIC_2                        2
#define GTP_APN_RESTRICTION_PRIVATE_1                       3
#define GTP_APN_RESTRICTION_PRIVATE_2                       4

/* 8.58 Selection Mode */
#define GTP_SELECTION_MODE_MS_OR_NETWORK_PROVIDED_APN       0
#define GTP_SELECTION_MODE_MS_PROVIDED_APN                  1
#define GTP_SELECTION_MODE_NETWORK_PROVIDED_APN             2

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_TYPES_H__ */


#ifndef __GTP_TYPES_H__
#define __GTP_TYPES_H__

#include "core_tlv_msg.h"
#include "3gpp_types.h"

typedef struct c_sockaddr_t c_sockaddr_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 8.4 Cause */
#define GTP_CAUSE_LOCAL_DETACH  2
#define GTP_CAUSE_COMPLETE_DETACH_3
#define GTP_CAUSE_RAT_CHANGED_FROM_3GPP_TO_NON_3GPP 4
#define GTP_CAUSE_ISR_DEACTIVATION  5
#define GTP_CAUSE_ERROR_INDICATION_RECEIVED_FROM_RNC_ENODEB_S4_SGSN_MME 6
#define GTP_CAUSE_IMSI_DETACH_ONLY  7
#define GTP_CAUSE_REACTIVATION_REQUESTED 8
#define GTP_CAUSE_PDN_RECONNECTION_TO_THIS_APN_DISALLOWED 9
#define GTP_CAUSE_ACCESS_CHANGED_FROM_NON_3GPP_TO_3GPP  10
#define GTP_CAUSE_PDN_CONNECTION_INACTIVITY_TIMER_EXPIRES 11
#define GTP_CAUSE_PGW_NOT_RESPONDING 12
#define GTP_CAUSE_NETWORK_FAILURE 13
#define GTP_CAUSE_QOS_PARAMETER_MISMATCH 14
#define GTP_CAUSE_REQUEST_ACCEPTED  16
#define GTP_CAUSE_REQUEST_ACCEPTED_PARTIALLY 17
#define GTP_CAUSE_NEW_PDN_TYPE_DUE_TO_NETWORK_PREFERENCE 18
#define GTP_CAUSE_NEW_PDN_TYPE_DUE_TO_SINGLE_ADDRESS_BEARER_ONLY 19
#define GTP_CAUSE_CONTEXT_NOT_FOUND 64
#define GTP_CAUSE_INVALID_MESSAGE_FORMAT 65
#define GTP_CAUSE_VERSION_NOT_SUPPORTED_BY_NEXT_PEER 66
#define GTP_CAUSE_INVALID_LENGTH 67
#define GTP_CAUSE_SERVICE_NOT_SUPPORTED 68
#define GTP_CAUSE_MANDATORY_IE_INCORRECT 69
#define GTP_CAUSE_MANDATORY_IE_MISSING  70
#define GTP_CAUSE_SYSTEM_FAILURE 72
#define GTP_CAUSE_NO_RESOURCES_AVAILABLE 73
#define GTP_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION 74
#define GTP_CAUSE_SYNTACTIC_ERROR_IN_THE_TFT_OPERATION  75
#define GTP_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTER 76
#define GTP_CAUSE_SYNTACTIC_ERRORS_IN_PACKET_FILTER  77
#define GTP_CAUSE_MISSING_OR_UNKNOWN_APN 78
#define GTP_CAUSE_GRE_KEY_NOT_FOUND 80
#define GTP_CAUSE_RELOCATION_FAILURE 81
#define GTP_CAUSE_DENIED_IN_RAT 82
#define GTP_CAUSE_PREFERRED_PDN_TYPE_NOT_SUPPORTED  83
#define GTP_CAUSE_ALL_DYNAMIC_ADDRESSES_ARE_OCCUPIED 84
#define GTP_CAUSE_UE_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED 85
#define GTP_CAUSE_PROTOCOL_TYPE_NOT_SUPPORTED 86
#define GTP_CAUSE_UE_NOT_RESPONDING 87
#define GTP_CAUSE_UE_REFUSES 88
#define GTP_CAUSE_SERVICE_DENIED 89
#define GTP_CAUSE_UNABLE_TO_PAGE_UE 90
#define GTP_CAUSE_NO_MEMORY_AVAILABLE 91
#define GTP_CAUSE_USER_AUTHENTICATION_FAILED 92
#define GTP_CAUSE_APN_ACCESS_DENIED_NO_SUBSCRIPTION 93
#define GTP_CAUSE_REQUEST_REJECTED_REASON_NOT_SPECIFIED 94
#define GTP_CAUSE_P_TMSI_SIGNATURE_MISMATCH 95
#define GTP_CAUSE_IMSI_IMEI_NOT_KNOWN 96
#define GTP_CAUSE_SEMANTIC_ERROR_IN_THE_TAD_OPERATION 97
#define GTP_CAUSE_SYNTACTIC_ERROR_IN_THE_TAD_OPERATION 98
#define GTP_CAUSE_REMOTE_PEER_NOT_RESPONDING 100
#define GTP_CAUSE_COLLISION_WITH_NETWORK_INITIATED_REQUEST  101
#define GTP_CAUSE_UNABLE_TO_PAGE_UE_DUE_TO_SUSPENSION 102
#define GTP_CAUSE_CONDITIONAL_IE_MISSING 103
#define GTP_CAUSE_APN_RESTRICTION_TYPE_INCOMPATIBLE 104
#define GTP_CAUSE_INVALID_OVERALL_LENGTH 105
#define GTP_CAUSE_DATA_FORWARDING_NOT_SUPPORTED 106
#define GTP_CAUSE_INVALID_REPLY_FROM_REMOTE_PEER 107
#define GTP_CAUSE_FALLBACK_TO_GTPV1 108
#define GTP_CAUSE_INVALID_PEER  109
#define GTP_CAUSE_TEMPORARILY_REJECTED_DUE_TO_HANDOVER_IN_PROGRESS 110
#define GTP_CAUSE_MODIFICATIONS_NOT_LIMITED_TO_S1_U_BEARERS 111
#define GTP_CAUSE_REQUEST_REJECTED_FOR_A_PMIPV6_REASON 112
#define GTP_CAUSE_APN_CONGESTION 113
#define GTP_CAUSE_BEARER_HANDLING_NOT_SUPPORTED 114
#define GTP_CAUSE_UE_ALREADY_RE_ATTACHED 115
#define GTP_CAUSE_MULTIPLE_PDN_CONNECTIONS_FOR_A_GIVEN_APN_NOT_ALLOWED  116
#define GTP_CAUSE_TARGET_ACCESS_RESTRICTED_FOR_THE_SUBSCRIBER 117
#define GTP_CAUSE_MME_SGSN_REFUSES_DUE_TO_VPLMN_POLICY  119
#define GTP_CAUSE_GTP_C_ENTITY_CONGESTION 120
#define GTP_CAUSE_LATE_OVERLAPPING_REQUEST  121
#define GTP_CAUSE_TIMED_OUT_REQUEST 122
#define GTP_CAUSE_UE_IS_TEMPORARILY_NOT_REACHABLE_DUE_TO_POWER_SAVING 123
#define GTP_CAUSE_RELOCATION_FAILURE_DUE_TO_NAS_MESSAGE_REDIRECTION 124
#define GTP_CAUSE_UE_NOT_AUTHORISED_BY_OCS_OR_EXTERNAL_AAA_SERVER 125
#define GTP_CAUSE_MULTIPLE_ACCESSES_TO_A_PDN_CONNECTION_NOT_ALLOWED 126
#define GTP_CAUSE_REQUEST_REJECTED_DUE_TO_UE_CAPABILITY 127

typedef struct _gtp_cause_t {
    c_uint8_t value;
ED4(c_uint8_t spare:5;,
    c_uint8_t pce:1;,
    c_uint8_t bce:1;,
    c_uint8_t cs:1;)
} __attribute__ ((packed)) gtp_cause_t;

/* 8.7 Aggregate Maximum Bit Rate (AMBR) */
typedef struct _gtp_ambr_t {
    c_uint32_t uplink;
    c_uint32_t downlink;
} __attribute__ ((packed)) gtp_ambr_t;

/* 8.12 Indication */
typedef struct _gtp_indication_t {
ED8(c_uint8_t daf:1;,
    c_uint8_t dtf:1;,
    c_uint8_t hi:1;,
    c_uint8_t dfi:1;,
    c_uint8_t oi:1;,
    c_uint8_t isrsi:1;,
    c_uint8_t israi:1;,
    c_uint8_t sgwci:1;)

ED8(c_uint8_t sqci:1;,
    c_uint8_t uimsi:1;,
    c_uint8_t cfsi:1;,
    c_uint8_t crsi:1;,
    c_uint8_t p:1;,
    c_uint8_t pt:1;,
    c_uint8_t si:1;,
    c_uint8_t msv:1;)

ED8(c_uint8_t retloc:1;,
    c_uint8_t pbic:1;,
    c_uint8_t srni:1;,
    c_uint8_t s6af:1;,
    c_uint8_t s4af:1;,
    c_uint8_t mbmdt:1;,
    c_uint8_t israu:1;,
    c_uint8_t ccrsi:1;)

ED8(c_uint8_t spare1:1;,
    c_uint8_t spare2:1;,
    c_uint8_t spare3:1;,
    c_uint8_t spare4:1;,
    c_uint8_t spare5:1;,
    c_uint8_t csfbi:1;,
    c_uint8_t clii:1;,
    c_uint8_t cpsr:1;)
} __attribute__ ((packed)) gtp_indication_t;

/* 8.13 Protocol Configuration Options (PCO) 
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 
 * RFC 3232 [103]
 * RFC 1661 [102] */

/* 8.15 Bearer Quality of Service (Bearer QoS) */
#define GTP_BEARER_QOS_LEN 22
typedef struct _gtp_bearer_qos_t {
ED5(c_uint8_t spare1:1;,
    /* See 3GPP TS 29.212[29], clause 5.3.46 Pre-emption-Capability AVP. */
    c_uint8_t pre_emption_capability:1;, 
    /* See 3GPP TS 29.212[29], clause 5.3.45 Priority-Level AVP. 
     * PL encodes each priority level defined for the Priority-Level AVP 
     * as the binary value of the priority level.  */
    c_uint8_t priority_level:4;,
    c_uint8_t spare2:1;,
    /* See 3GPP TS 29.212[29], clause 5.3.47 Pre-emption-Vulnerability AVP. */
    c_uint8_t pre_emption_vulnerability:1;)
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

/* 8.19 EPS Bearer Level Traffic Flow Template (Bearer TFT) 
 * See subclause 10.5.6.12 in 3GPP TS 24.008 [13]. */
#define GTP_MAX_TRAFFIC_FLOW_TEMPLATE 255

#define GTP_MAX_NUM_OF_PACKET_FILTER_COMPONENT 16
typedef struct _gtp_tft_t {
    union {
        struct {
#define GTP_TFT_CODE_IGNORE_THIS_IE                         0
#define GTP_TFT_CODE_CREATE_NEW_TFT                         1
#define GTP_TFT_CODE_DELETE_EXISTING_TFT                    2
#define GTP_TFT_CODE_ADD_PACKET_FILTERS_TO_EXISTING_TFT     3
#define GTP_TFT_CODE_REPLACE_PACKET_FILTERS_IN_EXISTING     4
#define GTP_TFT_CODE_DELETE_PACKET_FILTERS_FROM_EXISTING    5
#define GTP_TFT_CODE_NO_TFT_OPERATION                       6
ED3(c_uint8_t code:3;,
    c_uint8_t e_bit:1;,
    c_uint8_t num_of_packet_filter:4;)
        };
        c_uint8_t flags;
    };
    struct {
        union {
            struct {
            ED3(c_uint8_t spare:2;,
                c_uint8_t direction:2;,
                c_uint8_t identifier:4;)
            };
            c_uint8_t flags;
        };
        c_uint8_t precedence;
        c_uint8_t length;
#define GTP_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE 48
#define GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE 16
#define GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE 17
#define GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE 32
#define GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE 33
#define GTP_PACKET_FILTER_IPV6_LOCAL_ADDRESS_TYPE 34
#define GTP_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE 35
#define GTP_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE 64
#define GTP_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE 65
#define GTP_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE 80
#define GTP_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE 81
#define GTP_PACKET_FILTER_SECURITY_PARAMETER_INDEX_TYPE 96
#define GTP_PACKET_FILTER_TOS_TRAFFIC_CLASS_TYPE 112
#define GTP_PACKET_FILTER_FLOW_LABEL_TYPE 128
        struct {
            c_uint8_t type;
            union {
                c_uint8_t proto;
                struct {
                    c_uint32_t addr;
                    c_uint32_t mask;
                } ipv4;
                struct {
                    c_uint32_t addr[4];
                    c_uint8_t prefixlen;
                } ipv6;
                struct {
                    c_uint32_t addr[4];
                    c_uint32_t mask[4];
                } ipv6_mask;
                struct {
                    c_uint16_t low;
                    c_uint16_t high;
                } port;
            };
        } component[GTP_MAX_NUM_OF_PACKET_FILTER_COMPONENT];
        c_uint8_t num_of_component;
    } pf[MAX_NUM_OF_PACKET_FILTER];
} gtp_tft_t;

CORE_DECLARE(c_int16_t) gtp_build_tft(
    tlv_octet_t *octet, gtp_tft_t *tft, void *data, int data_len);

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

typedef struct _gtp_uli_lai_t {
    plmn_id_t plmn_id;
    c_uint16_t lac;
} __attribute__ ((packed)) gtp_uli_lai_t;

typedef struct _gtp_uli_t {
    struct {
    ED7(c_uint8_t spare:2;,
        c_uint8_t lai:1;,
        c_uint8_t e_cgi:1;,
        c_uint8_t tai:1;,
        c_uint8_t rai:1;,
        c_uint8_t sai:1;,
        c_uint8_t cgi:1;)
    } flags;
    gtp_uli_cgi_t cgi;
    gtp_uli_sai_t sai;
    gtp_uli_rai_t rai;
    tai_t tai;
    e_cgi_t e_cgi;
    gtp_uli_lai_t lai;
} gtp_uli_t;

CORE_DECLARE(c_int16_t) gtp_parse_uli(gtp_uli_t *uli, tlv_octet_t *octet);
CORE_DECLARE(c_int16_t) gtp_build_uli(
        tlv_octet_t *octet, gtp_uli_t *uli, void *data, int data_len);

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

#define GTP_F_TEID_HDR_LEN                      5
#define GTP_F_TEID_IPV4_LEN                     IPV4_LEN+GTP_F_TEID_HDR_LEN
#define GTP_F_TEID_IPV6_LEN                     IPV6_LEN+GTP_F_TEID_HDR_LEN
#define GTP_F_TEID_IPV4V6_LEN                   IPV4V6_LEN+GTP_F_TEID_HDR_LEN
typedef struct _gtp_f_teid_t {
ED3(c_uint8_t       ipv4:1;,
    c_uint8_t       ipv6:1;,
    c_uint8_t       interface_type:6;)
    c_uint32_t      teid;
    union {
        /* GTP_F_TEID_IPV4 */
        c_uint32_t addr;

        /* GTP_F_TEID_IPV6 */
        c_uint8_t addr6[IPV6_LEN];

        /* GTP_F_TEID_BOTH */
        struct {
            c_uint32_t addr;
            c_uint8_t addr6[IPV6_LEN];
        } both;
    };
} __attribute__ ((packed)) gtp_f_teid_t;

/* 8.44 UE Time Zone */
#define GTP_UE_TIME_ZONE_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME 0
#define GTP_UE_TIME_ZONE_1_HOUR_FOR_DAYLIGHT_SAVING_TIME        1
#define GTP_UE_TIME_ZONE_2_HOUR_FOR_DAYLIGHT_SAVING_TIME        2
/* Time Zone" IE in 3GPP TS 24.008 [5].
 * This field uses the same format as the Timezone field used in the 
 * TP-Service-Centre-Time-Stamp, which is defined in 3GPP TS 23.040 [90], 
 * and its value shall be set as defined in 3GPP TS 22.042 */
typedef struct _gtp_ue_timezone_t {
#define GTP_TIME_TO_BCD(x) TIME_TO_BCD(x)
    /* The Time Zone indicates the difference, expressed in quarters of an hour,
     * between the local time and GMT. In the first of the two semi-octets, 
     * the first bit (bit 3 of the seventh octet of 
     * the TP-Service-Centre-Time-Stamp field) represents 
     * the algebraic sign of this difference (0: positive, 1: negative). */
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

/* 8.65 Node Type */
#define GTP_NODE_TYPE_MME                                   0
#define GTP_NODE_TYPE_SGSN                                  1

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_TYPES_H__ */


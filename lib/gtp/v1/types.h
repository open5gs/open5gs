/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#if !defined(OGS_GTP_INSIDE) && !defined(OGS_GTP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_GTP1_TYPES_H
#define OGS_GTP1_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_GTP1_MAX_INDIRECT_TUNNEL 8

#define OGS_GTPV1U_5GC_HEADER_LEN 16
/*
 * 5GC GTP Header (16byte)
 *  o Flags(1byte) : 0x34
 *  o Message Type(1byte) : T-PDU (0xff)
 *  o Length(2byte) : 36
 *  o TEID(4byte) : 0x00000001
 *  o Next extension header type(4byte)
 *    - Sequence Number(2byte) : 0x0000
 *    - N PDU Number(1byte) : 0x00
 *    - PDU Session container(1byte) : (0x85)
 *  o Extension header(4byte)
 *    - Extension HEader Length(1byte) : 1
 *    - PDU Session Container(2byte)
 *      ; PDU Type : UL PDU SESSION INFORMATION (1)
 *      ; QoS Flow Identifier (QFI) : 1
 *    - Next extension header type : No more extension headers (0x00)
 */

#define OGS_GTPV1U_EXTENSION_HEADER_LEN 4
typedef struct ogs_gtp1_extension_header_s {
#define OGS_GTP1_EXTENSION_HEADER_TYPE_UDP_PORT 0x40
#define OGS_GTP1_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER 0x85
#define OGS_GTP1_EXTENSION_HEADER_TYPE_NO_MORE_EXTENSION_HEADERS 0x0
    uint16_t sequence_number;
    uint8_t n_pdu_number;
    uint8_t type;
    uint8_t len;
#define OGS_GTP1_EXTENSION_HEADER_PDU_TYPE_DL_PDU_SESSION_INFORMATION 0
#define OGS_GTP1_EXTENSION_HEADER_PDU_TYPE_UL_PDU_SESSION_INFORMATION 1
    ED2(uint8_t pdu_type:4;,
        uint8_t spare1:4;);
    ED3(uint8_t paging_policy_presence:1;,
        uint8_t reflective_qos_indicator:1;,
        uint8_t qos_flow_identifier:6;);
    uint8_t next_type;
} __attribute__ ((packed)) ogs_gtp1_extension_header_t;

/* TS 29.060 16.0.0 Table 38: Cause Values */
#define OGS_GTP1_CAUSE_REQUEST_IMSI 0
#define OGS_GTP1_CAUSE_REQUEST_IMEI 1
#define OGS_GTP1_CAUSE_REQUEST_IMSI_IMEI 2
#define OGS_GTP1_CAUSE_NO_ID_NEEDED 3
#define OGS_GTP1_CAUSE_MS_REFUSES 4
#define OGS_GTP1_CAUSE_MS_NOT_GPRS_RESPONDING 5
#define OGS_GTP1_CAUSE_REACTIACTION_REQUESTED 6
#define OGS_GTP1_CAUSE_PDP_ADDRESS_INACTIVITY_TIMER_EXP 7
#define OGS_GTP1_CAUSE_NETWORK_FAILURE 8
#define OGS_GTP1_CAUSE_QOS_PARAMETER_MISMATCH 9
/* 10-48 for future use */
/* 49-63 reserved for GTP' */
/* 64-127 for future use */
#define OGS_GTP1_CAUSE_REQUEST_ACCEPTED 128
#define OGS_GTP1_CAUSE_NEW_PDP_TYPE_DUE_TO_NETWORK_PREFERENCE 129
#define OGS_GTP1_CAUSE_NEW_PDP_TYPE_DUE_TO_SINGLE_ADDRESS_BEARER_ONLY 130
/* 131-176 for future use */
/* 177-191 reserved for GTP' */
#define OGS_GTP1_CAUSE_NON_EXISTENT 192
#define OGS_GTP1_CAUSE_INVALID_MESSAGE_FORMAT 193
#define OGS_GTP1_CAUSE_IMSI_IMEI_NOT_KNOWN 194
#define OGS_GTP1_CAUSE_MS_GPRS_DETACHED 195
#define OGS_GTP1_CAUSE_REJ_MS_NOT_GPRS_RESPONDING 196
#define OGS_GTP1_CAUSE_REJ_MS_REFUSES 197
#define OGS_GTP1_CAUSE_VERSION_NOT_SUPPORTED 198
#define OGS_GTP1_CAUSE_NO_RESOURCES_AVAILABLE 199
#define OGS_GTP1_CAUSE_SERVICE_NOT_SUPPORTED 200
#define OGS_GTP1_CAUSE_MANDATORY_IE_INCORRECT 201
#define OGS_GTP1_CAUSE_MANDATORY_IE_MISSING 202
#define OGS_GTP1_CAUSE_OPTIONAL_IE_INCORRECT 203
#define OGS_GTP1_CAUSE_SYSTEM_FAILURE 204
#define OGS_GTP1_CAUSE_ROAMING_RESTRICTION 205
#define OGS_GTP1_CAUSE_P_TMSI_SIGNATURE_MISMATCH 206
#define OGS_GTP1_CAUSE_GPRS_CONN_SUSPENDED 207
#define OGS_GTP1_CAUSE_AUTHENTICATION_FAILURE 208
#define OGS_GTP1_CAUSE_USER_AUTHENTICATION_FAILED 209
#define OGS_GTP1_CAUSE_CONTEXT_NOT_FOUND 210
#define OGS_GTP1_CAUSE_ALL_DYNAMIC_PDP_ADDRS_OCCUPIED 211
#define OGS_GTP1_CAUSE_NO_MEMORY_AVAILABLE 212
#define OGS_GTP1_CAUSE_RELOCATION_FAILURE 213
#define OGS_GTP1_CAUSE_UNKNOWN_MANDATORY_EXTENSION_HEADER 214
#define OGS_GTP1_CAUSE_SEMANTIC_ERR_TFT_OPERATION 215
#define OGS_GTP1_CAUSE_SYNTATIC_ERR_TFT_OPERATION 216
#define OGS_GTP1_CAUSE_SEMANTIC_ERR_PKT_FILTER 217
#define OGS_GTP1_CAUSE_SYNTACTIC_ERR_PKT_FILTER 218
#define OGS_GTP1_CAUSE_MISSING_OR_UNKNOWN_APN 219
#define OGS_GTP1_CAUSE_UNKNWON_PDP_ADDR_OR_TYPE 220
#define OGS_GTP1_CAUSE_PDP_CONTEXT_WITHOUT_TFT_ACTIVATED 221
#define OGS_GTP1_CAUSE_APN_ACCESS_DENIED 222
#define OGS_GTP1_CAUSE_APN_INCOMPATIBLE_WITH_ACTIVE_PDP_CONTEXTS 223
#define OGS_GTP1_CAUSE_MS_MBMS_CAP_INSUFFICIENT 224
#define OGS_GTP1_CAUSE_INVALID_CORRELATION_ID 225
#define OGS_GTP1_CAUSE_MBMS_BEARER_CONTEXT_SUPERSEDED 226
#define OGS_GTP1_CAUSE_BEARER_CONTROL_MODE_VIOLATION 227
#define OGS_GTP1_CAUSE_COLLISION_WITH_NET_INITIATED_REQ 228
#define OGS_GTP1_CAUSE_APN_CONGESTION 229
#define OGS_GTP1_CAUSE_BEARER_HANDLING_NOT_SUPPORED 230
#define OGS_GTP1_CAUSE_TGT_ACC_RESTRICTED_SUBSCRIBER 231
#define OGS_GTP1_CAUSE_UE_TMP_NOT_REACHABLE_POWER_SAVING 232
#define OGS_GTP1_CAUSE_RELOCATION_FAILURE_NAS_MSG_REDIR 233
/* 234-240 for future use */
/* 241-255 reserved for GTP' */

/* TS 29.060 16.0.0 Table 39: Use of the Cause Values */
#define OGS_GTP1_CAUSE_ACCEPT 0x80
#define OGS_GTP1_CAUSE_REJECT 0xc0

typedef struct ogs_gtp1_cause_s {
    uint8_t value;
ED4(uint8_t spare:5;,
    uint8_t pce:1;,
    uint8_t bce:1;,
    uint8_t cs:1;)
} __attribute__ ((packed)) ogs_gtp1_cause_t;


/* TS 29.060 16.0.0 Table 7.7.50.1 RAT Type values */
#define OGS_GTP1_RAT_TYPE_RESERVED                               0
#define OGS_GTP1_RAT_TYPE_UTRAN                                  1
#define OGS_GTP1_RAT_TYPE_GERAN                                  2
#define OGS_GTP1_RAT_TYPE_WLAN                                   3
#define OGS_GTP1_RAT_TYPE_GAN                                    4
#define OGS_GTP1_RAT_TYPE_HSPA_EVOLUTION                         5
#define OGS_GTP1_RAT_TYPE_EUTRAN                                 6

/* 8.21 User Location Information (ULI) */
#define OGS_GTP1_MAX_ULI_LEN sizeof(ogs_gtp1_uli_t)
#define OGS_GTP1_GEO_LOC_TYPE_CGI 0
#define OGS_GTP1_GEO_LOC_TYPE_SAI 1
#define OGS_GTP1_GEO_LOC_TYPE_RAI 2
typedef struct ogs_gtp1_uli_cgi_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t lac;
    uint16_t ci;
} __attribute__ ((packed)) ogs_gtp1_uli_cgi_t;
typedef struct ogs_gtp1_uli_sai_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t lac;
    uint16_t sac;
} __attribute__ ((packed)) ogs_gtp1_uli_sai_t;
typedef struct ogs_gtp1_uli_rai_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t lac;
    uint16_t rac;
} __attribute__ ((packed)) ogs_gtp1_uli_rai_t;
typedef struct ogs_gtp1_uli_s {
    uint8_t geo_loc_type;
    union {
        ogs_gtp1_uli_cgi_t cgi;
        ogs_gtp1_uli_sai_t sai;
        ogs_gtp1_uli_rai_t rai;
    };
} ogs_gtp1_uli_t;

int16_t ogs_gtp1_parse_uli(ogs_gtp1_uli_t *uli, ogs_tlv_octet_t *octet);
int16_t ogs_gtp1_build_uli(ogs_tlv_octet_t *octet,
        ogs_gtp1_uli_t *uli, void *data, int data_len);

#define OGS_GTP_GSN_ADDRESS_IPV4_LEN    OGS_IPV4_LEN
#define OGS_GTP_GSN_ADDRESS_IPV6_LEN    OGS_IPV6_LEN
typedef struct ogs_gtp1_gsn_addr_s {
    union {
        /* OGS_GTP_F_TEID_IPV4 */
        uint32_t addr;
        /* OGS_GTP_F_TEID_IPV6 */
        uint8_t addr6[OGS_IPV6_LEN];
    };
} __attribute__ ((packed)) ogs_gtp1_gsn_addr_t;

#ifdef __cplusplus
}
#endif

/* TS 29.060 7.7.49 APN Restriction
 * TS 23.060 15.4 APN Restriction */
#define OGS_GTP1_APN_NO_RESTRICTION         0
#define OGS_GTP1_APN_RESTRICTION_PUBLIC_1   1
#define OGS_GTP1_APN_RESTRICTION_PUBLIC_2   2
#define OGS_GTP1_APN_RESTRICTION_PRIVATE_1  3
#define OGS_GTP1_APN_RESTRICTION_PRIVATE_2  4

/* TS 29.060 7.7.34 Quality of Service (QoS) Profile */
#define OGS_GTP1_QOS_PROFILE_MAX_LEN 255
/* TS 24.008 10.5.6.5 Quality of service */
typedef struct ogs_gtp1_qos_profile_data_extended_bit_rate_s {
    uint8_t max_bit_rate_uplink;
    uint8_t guaranteed_bit_rate_uplink;
    uint8_t max_bit_rate_downlink;
    uint8_t guaranteed_bit_rate_downlink;
} __attribute__ ((packed)) ogs_gtp1_qos_profile_data_extended_bit_rate_t;
typedef struct ogs_gtp1_qos_profile_data_s {
ED3(uint8_t spare1:2;,
    uint8_t delay_class:3;,
    uint8_t reliability_class:3;)
ED3(uint8_t peak_throughput:4;,
    uint8_t spare2:1;,
    uint8_t precedence_class:3;)
ED2(uint8_t spare3:3;,
    uint8_t mean_throughput:5;)
ED3(uint8_t traffic_class:3;,
    uint8_t delivery_order:2;,
    uint8_t delivery_erroneous_sdu:3;)
    uint8_t max_sdu_size;
    uint8_t max_bit_rate_uplink;
    uint8_t max_bit_rate_downlink;
ED2(uint8_t residual_ber:4;,
    uint8_t sdu_error_ratio:4;)
ED2(uint8_t transfer_delay:6;,
    uint8_t traffic_handling_priority:2;)
    uint8_t guaranteed_bit_rate_uplink;
    uint8_t guaranteed_bit_rate_downlink;
ED3(uint8_t spare4:3;,
    uint8_t signalling_indication:1;,
    uint8_t source_statistics_descriptor:4;)
    ogs_gtp1_qos_profile_data_extended_bit_rate_t extended;
    ogs_gtp1_qos_profile_data_extended_bit_rate_t extended2;
} __attribute__ ((packed)) ogs_gtp1_qos_profile_data_t;

typedef struct ogs_gtp1_qos_profile_s {
    uint8_t arp;
    ogs_gtp1_qos_profile_data_t data;
} __attribute__ ((packed)) ogs_gtp1_qos_profile_t;
typedef struct ogs_gtp1_qos_profile_decoded_s {
    ogs_gtp1_qos_profile_t qos_profile;
    /* Filled in by the decoder function: */
    bool data_octet6_to_13_present; /* from traffic_class to guaranteed_bit_rate_downlink */
    bool data_octet14_present; /* byte containing signalling_indication */
    bool bit_rate_downlink_extended_present;
    bool bit_rate_uplink_extended_present;
    bool bit_rate_downlink_extended2_present;
    bool bit_rate_uplink_extended2_present;
    uint16_t dec_transfer_delay;
    uint32_t dec_mbr_kbps_dl; /* decoded MBR in kbps */
    uint32_t dec_mbr_kbps_ul; /* decoded MBR in kbps */
} __attribute__ ((packed)) ogs_gtp1_qos_profile_decoded_t;

#define OGS_GTP1_QOS_TRAFFIC_CLASS_SUBSCRIBED 0
#define OGS_GTP1_QOS_TRAFFIC_CLASS_CONVERSATIONAL 1
#define OGS_GTP1_QOS_TRAFFIC_CLASS_STREAMING 2
#define OGS_GTP1_QOS_TRAFFIC_CLASS_INTERACTIVE 3
#define OGS_GTP1_QOS_TRAFFIC_CLASS_BACKGROUND 4

#define OGS_GTP1_QOS_SRC_STATS_DESC_UNKNOWN 0
#define OGS_GTP1_QOS_SRC_STATS_DESC_SPEECH 1

/* 7.7.98 APN Aggregate Maximum Bit Rate (APN-AMBR) */
typedef struct ogs_gtp1_apn_ambr_s {
    uint32_t uplink;
    uint32_t downlink;
} __attribute__ ((packed)) ogs_gtp1_apn_ambr_t;

int16_t ogs_gtp1_parse_qos_profile(
    ogs_gtp1_qos_profile_decoded_t *decoded, const ogs_tlv_octet_t *octet);
int16_t ogs_gtp1_build_qos_profile(ogs_tlv_octet_t *octet,
    const ogs_gtp1_qos_profile_decoded_t *decoded, void *data, int data_len);

#endif /* OGS_GTP1_TYPES_H */

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

#if !defined(OGS_GTP_INSIDE) && !defined(OGS_GTP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_GTP_TYPES_H
#define OGS_GTP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_GTP_MAX_INDIRECT_TUNNEL 8

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
typedef struct ogs_gtp_extension_header_s {
#define OGS_GTP_EXTENSION_HEADER_TYPE_UDP_PORT 0x40
#define OGS_GTP_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER 0x85
#define OGS_GTP_EXTENSION_HEADER_TYPE_NO_MORE_EXTENSION_HEADERS 0x0
    uint16_t sequence_number;
    uint8_t n_pdu_number;
    uint8_t type;
    uint8_t len;
#define OGS_GTP_EXTENSION_HEADER_PDU_TYPE_DL_PDU_SESSION_INFORMATION 0
#define OGS_GTP_EXTENSION_HEADER_PDU_TYPE_UL_PDU_SESSION_INFORMATION 1
    ED2(uint8_t pdu_type:4;,
        uint8_t spare1:4;);
    ED3(uint8_t paging_policy_presence:1;,
        uint8_t reflective_qos_indicator:1;,
        uint8_t qos_flow_identifier:6;);
    uint8_t next_type;
} __attribute__ ((packed)) ogs_gtp_extension_header_t;

/* 8.4 Cause */
#define OGS_GTP_CAUSE_UNDEFINED_VALUE 0
#define OGS_GTP_CAUSE_LOCAL_DETACH 2
#define OGS_GTP_CAUSE_COMPLETE_DETACH_3
#define OGS_GTP_CAUSE_RAT_CHANGED_FROM_3GPP_TO_NON_3GPP 4
#define OGS_GTP_CAUSE_ISR_DEACTIVATION 5
#define OGS_GTP_CAUSE_ERROR_INDICATION_RECEIVED 6
#define OGS_GTP_CAUSE_IMSI_DETACH_ONLY 7
#define OGS_GTP_CAUSE_REACTIVATION_REQUESTED 8
#define OGS_GTP_CAUSE_PDN_RECONNECTION_TO_THIS_APN_DISALLOWED 9
#define OGS_GTP_CAUSE_ACCESS_CHANGED_FROM_NON_3GPP_TO_3GPP  10
#define OGS_GTP_CAUSE_PDN_CONNECTION_INACTIVITY_TIMER_EXPIRES 11
#define OGS_GTP_CAUSE_PGW_NOT_RESPONDING 12
#define OGS_GTP_CAUSE_NETWORK_FAILURE 13
#define OGS_GTP_CAUSE_QOS_PARAMETER_MISMATCH 14
#define OGS_GTP_CAUSE_REQUEST_ACCEPTED 16
#define OGS_GTP_CAUSE_REQUEST_ACCEPTED_PARTIALLY 17
#define OGS_GTP_CAUSE_NEW_PDN_TYPE_DUE_TO_NETWORK_PREFERENCE 18
#define OGS_GTP_CAUSE_NEW_PDN_TYPE_DUE_TO_SINGLE_ADDRESS_BEARER_ONLY 19
#define OGS_GTP_CAUSE_CONTEXT_NOT_FOUND 64
#define OGS_GTP_CAUSE_INVALID_MESSAGE_FORMAT 65
#define OGS_GTP_CAUSE_VERSION_NOT_SUPPORTED_BY_NEXT_PEER 66
#define OGS_GTP_CAUSE_INVALID_LENGTH 67
#define OGS_GTP_CAUSE_SERVICE_NOT_SUPPORTED 68
#define OGS_GTP_CAUSE_MANDATORY_IE_INCORRECT 69
#define OGS_GTP_CAUSE_MANDATORY_IE_MISSING  70
#define OGS_GTP_CAUSE_SYSTEM_FAILURE 72
#define OGS_GTP_CAUSE_NO_RESOURCES_AVAILABLE 73
#define OGS_GTP_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION 74
#define OGS_GTP_CAUSE_SYNTACTIC_ERROR_IN_THE_TFT_OPERATION  75
#define OGS_GTP_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTER 76
#define OGS_GTP_CAUSE_SYNTACTIC_ERRORS_IN_PACKET_FILTER  77
#define OGS_GTP_CAUSE_MISSING_OR_UNKNOWN_APN 78
#define OGS_GTP_CAUSE_GRE_KEY_NOT_FOUND 80
#define OGS_GTP_CAUSE_RELOCATION_FAILURE 81
#define OGS_GTP_CAUSE_DENIED_IN_RAT 82
#define OGS_GTP_CAUSE_PREFERRED_PDN_TYPE_NOT_SUPPORTED  83
#define OGS_GTP_CAUSE_ALL_DYNAMIC_ADDRESSES_ARE_OCCUPIED 84
#define OGS_GTP_CAUSE_UE_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED 85
#define OGS_GTP_CAUSE_PROTOCOL_TYPE_NOT_SUPPORTED 86
#define OGS_GTP_CAUSE_UE_NOT_RESPONDING 87
#define OGS_GTP_CAUSE_UE_REFUSES 88
#define OGS_GTP_CAUSE_SERVICE_DENIED 89
#define OGS_GTP_CAUSE_UNABLE_TO_PAGE_UE 90
#define OGS_GTP_CAUSE_NO_MEMORY_AVAILABLE 91
#define OGS_GTP_CAUSE_USER_AUTHENTICATION_FAILED 92
#define OGS_GTP_CAUSE_APN_ACCESS_DENIED_NO_SUBSCRIPTION 93
#define OGS_GTP_CAUSE_REQUEST_REJECTED_REASON_NOT_SPECIFIED 94
#define OGS_GTP_CAUSE_P_TMSI_SIGNATURE_MISMATCH 95
#define OGS_GTP_CAUSE_IMSI_IMEI_NOT_KNOWN 96
#define OGS_GTP_CAUSE_SEMANTIC_ERROR_IN_THE_TAD_OPERATION 97
#define OGS_GTP_CAUSE_SYNTACTIC_ERROR_IN_THE_TAD_OPERATION 98
#define OGS_GTP_CAUSE_REMOTE_PEER_NOT_RESPONDING 100
#define OGS_GTP_CAUSE_COLLISION_WITH_NETWORK_INITIATED_REQUEST  101
#define OGS_GTP_CAUSE_UNABLE_TO_PAGE_UE_DUE_TO_SUSPENSION 102
#define OGS_GTP_CAUSE_CONDITIONAL_IE_MISSING 103
#define OGS_GTP_CAUSE_APN_RESTRICTION_TYPE_INCOMPATIBLE 104
#define OGS_GTP_CAUSE_INVALID_OVERALL_LENGTH 105
#define OGS_GTP_CAUSE_DATA_FORWARDING_NOT_SUPPORTED 106
#define OGS_GTP_CAUSE_INVALID_REPLY_FROM_REMOTE_PEER 107
#define OGS_GTP_CAUSE_FALLBACK_TO_GTPV1 108
#define OGS_GTP_CAUSE_INVALID_PEER 109
#define OGS_GTP_CAUSE_TEMPORARILY_REJECTED_DUE_TO_HANDOVER_IN_PROGRESS 110
#define OGS_GTP_CAUSE_MODIFICATIONS_NOT_LIMITED_TO_S1_U_BEARERS 111
#define OGS_GTP_CAUSE_REQUEST_REJECTED_FOR_A_PMIPV6_REASON 112
#define OGS_GTP_CAUSE_APN_CONGESTION 113
#define OGS_GTP_CAUSE_BEARER_HANDLING_NOT_SUPPORTED 114
#define OGS_GTP_CAUSE_UE_ALREADY_RE_ATTACHED 115
#define OGS_GTP_CAUSE_MULTIPLE_PDN_CONNECTIONS_FOR_A_GIVEN_APN_NOT_ALLOWED  116
#define OGS_GTP_CAUSE_TARGET_ACCESS_RESTRICTED_FOR_THE_SUBSCRIBER 117
#define OGS_GTP_CAUSE_MME_SGSN_REFUSES_DUE_TO_VPLMN_POLICY  119
#define OGS_GTP_CAUSE_GTP_C_ENTITY_CONGESTION 120
#define OGS_GTP_CAUSE_LATE_OVERLAPPING_REQUEST  121
#define OGS_GTP_CAUSE_TIMED_OUT_REQUEST 122
#define OGS_GTP_CAUSE_UE_IS_TEMPORARILY_NOT_REACHABLE_DUE_TO_POWER_SAVING 123
#define OGS_GTP_CAUSE_RELOCATION_FAILURE_DUE_TO_NAS_MESSAGE_REDIRECTION 124
#define OGS_GTP_CAUSE_UE_NOT_AUTHORISED_BY_OCS_OR_EXTERNAL_AAA_SERVER 125
#define OGS_GTP_CAUSE_MULTIPLE_ACCESSES_TO_A_PDN_CONNECTION_NOT_ALLOWED 126
#define OGS_GTP_CAUSE_REQUEST_REJECTED_DUE_TO_UE_CAPABILITY 127

typedef struct ogs_gtp_cause_s {
    uint8_t value;
ED4(uint8_t spare:5;,
    uint8_t pce:1;,
    uint8_t bce:1;,
    uint8_t cs:1;)
} __attribute__ ((packed)) ogs_gtp_cause_t;

/* 8.7 Aggregate Maximum Bit Rate (AMBR) */
typedef struct ogs_gtp_ambr_s {
    uint32_t uplink;
    uint32_t downlink;
} __attribute__ ((packed)) ogs_gtp_ambr_t;

/* 8.12 Indication */
typedef struct ogs_gtp_indication_s {
ED8(uint8_t dual_address_bearer_flag:1;,
    uint8_t direct_tunnel_flag:1;,
    uint8_t handover_indication:1;,
    uint8_t direct_forwarding_indication:1;,
    uint8_t operation_indication:1;,
    uint8_t idle_mode_signalling_reduction_supported_indication:1;,
    uint8_t idle_mode_signalling_reduction_activation_indication:1;,
    uint8_t sgw_change_indication:1;)

ED8(uint8_t subscribed_qos_change_indication:1;,
    uint8_t unauthenticated_imsi:1;,
    uint8_t change_f_teid_support_indication:1;,
    uint8_t change_reporting_support_indication:1;,
    uint8_t piggybacking_supported:1;,
    uint8_t s5_s8_protocol_type:1;,
    uint8_t scope_indication:1;,
    uint8_t ms_validated:1;)

ED8(uint8_t retrieve_location_indication_flag:1;,
    uint8_t propagate_bbai_information_change:1;,
    uint8_t sgw_restoration_needed_indication:1;,
    uint8_t static_ipv6_address_flag:1;,
    uint8_t static_ipv4_address_flag:1;,
    uint8_t management_based_mdt_allowed_flag:1;,
    uint8_t isr_is_activated_for_the_ue:1;,
    uint8_t csg_change_reporting_support_indication:1;)

ED8(uint8_t change_of_presence_reporting_area_information_indication:1;,
    uint8_t abnormal_release_of_radio_link:1;,
    uint8_t pdn_pause_off_indication:1;,
    uint8_t pdn_pause_on_indication:1;,
    uint8_t pdn_pause_support_indication:1;,
    uint8_t csfb_indication:1;,
    uint8_t change_of_location_information_indication:1;,
    uint8_t cs_to_ps_srvcc_indication:1;)

ED8(uint8_t nbifom_support_indication:1;,
    uint8_t ue_available_for_signalling_indication:1;,
    uint8_t delay_tolerant_connection_indication:1;,
    uint8_t buffered_dl_data_waiting_indication:1;,
    uint8_t pending_subscription_change_indication:1;,
    uint8_t p_cscf_restoration_indication:1;,
    uint8_t associate_oci_with_sgw_node_s_identity:1;,
    uint8_t associate_oci_with_pgw_node_s_identity:1;)

ED8(uint8_t release_over_any_access_indication:1;,
    uint8_t extended_pco_support_indication:1;,
    uint8_t control_plane_only:1;,
    uint8_t pending_mt_short_message_indication:1;,
    uint8_t s11_u_tunnel_flag:1;,
    uint8_t pending_network_initiated_pdn_connection_signalling_indication:1;,
    uint8_t ue_not_authorised_cause_code_support_indication:1;,
    uint8_t wlcp_pdn_connection_modification_support_indication:1;)

ED8(uint8_t _5gs_interworking_without_n26_indication:1;,
    uint8_t return_preferred_indication:1;,
    uint8_t _5gs_interworking_indication:1;,
    uint8_t extended_ebi_value_range_support_indication:1;,
    uint8_t lte_m_ue_indication:1;,
    uint8_t lte_m_rat_type_reporting_to_pgw_indication:1;,
    uint8_t enb_change_reporting_support_indication:1;,
    uint8_t triggering_sgsn_initiated_pdp_context_creation_modification_indication:1;)

ED8(uint8_t create_session_request_message_forwarded_indication:1;,
    uint8_t mt_edt_not_applicable:1;,
    uint8_t mt_edt_applicable:1;,
    uint8_t no_5gs_n26_mobility_indication:1;,
    uint8_t _5gc_not_restricted_support:1;,
    uint8_t _5gc_not_restricted_indication:1;,
    uint8_t _5g_srvcc_ho_indication:1;,
    uint8_t ethernet_pdn_support_indication:1;)

ED8(uint8_t spare1:1;,
    uint8_t spare2:1;,
    uint8_t spare3:1;,
    uint8_t spare4:1;,
    uint8_t same_iwk_scef_selected_for_monitoring_event_indication:1;,
    uint8_t notify_source_enodeb_indication:1;,
    uint8_t indirect_data_forwarding_with_upf_indication:1;,
    uint8_t emergency_pdu_session_indication:1;)
} __attribute__ ((packed)) ogs_gtp_indication_t;

/* 8.13 Protocol Configuration Options (PCO)
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008
 * RFC 3232 [103]
 * RFC 1661 [102] */

/* 8.15 Bearer Quality of Service (Bearer QoS) */
#define GTP_BEARER_QOS_LEN 22
typedef struct ogs_gtp_bearer_qos_s {
ED5(uint8_t spare1:1;,
    /* See 3GPP TS 29.212[29], clause 5.3.46 Pre-emption-Capability AVP. */
    uint8_t pre_emption_capability:1;,
    /* See 3GPP TS 29.212[29], clause 5.3.45 Priority-Level AVP.
     * PL encodes each priority level defined for the Priority-Level AVP
     * as the binary value of the priority level.  */
    uint8_t priority_level:4;,
    uint8_t spare2:1;,
    /* See 3GPP TS 29.212[29], clause 5.3.47 Pre-emption-Vulnerability AVP. */
    uint8_t pre_emption_vulnerability:1;)
    uint8_t qci; /* specified in 3GPP TS 23.203 [48]. */

    /* specified in 3GPP TS 36.413 [10]. */
    uint64_t ul_mbr;
    uint64_t dl_mbr;
    uint64_t ul_gbr;
    uint64_t dl_gbr;

    /* NOTE : The encoding in 3GPP TS 24.301 [23] and 3GPP TS 36.413 [10]
     * is different from the encoding within this specification.  */
} __attribute__ ((packed)) ogs_gtp_bearer_qos_t;

int16_t ogs_gtp_parse_bearer_qos(
    ogs_gtp_bearer_qos_t *bearer_qos, ogs_tlv_octet_t *octet);
int16_t ogs_gtp_build_bearer_qos(ogs_tlv_octet_t *octet,
    ogs_gtp_bearer_qos_t *bearer_qos, void *data, int data_len);

/* 8.16 Flow Quality of Service (Flow QoS) */
#define GTP_FLOW_QOS_LEN 21
typedef struct ogs_gtp_flow_qos_s {
    uint8_t qci; /* specified in 3GPP TS 23.203 [48]. */

    /* specified in 3GPP TS 36.413 [10]. */
    uint64_t ul_mbr;
    uint64_t dl_mbr;
    uint64_t ul_gbr;
    uint64_t dl_gbr;

    /* NOTE : The encoding in 3GPP TS 24.301 [23] and 3GPP TS 36.413 [10]
     * is different from the encoding within this specification.  */
} __attribute__ ((packed)) ogs_gtp_flow_qos_t;

#define ogs_gtp_qos_to_bps(br, extended, extended2) \
    ogs_gtp_qos_to_kbps(br, extended, extended2) * 1024;

uint64_t ogs_gtp_qos_to_kbps(uint8_t br, uint8_t extended, uint8_t extended2);

int16_t ogs_gtp_parse_flow_qos(
    ogs_gtp_flow_qos_t *flow_qos, ogs_tlv_octet_t *octet);
int16_t ogs_gtp_build_flow_qos(ogs_tlv_octet_t *octet,
    ogs_gtp_flow_qos_t *flow_qos, void *data, int data_len);

/* 8.17 RAT Type */
#define OGS_GTP_RAT_TYPE_UTRAN                                  1
#define OGS_GTP_RAT_TYPE_GERAN                                  2
#define OGS_GTP_RAT_TYPE_WLAN                                   3
#define OGS_GTP_RAT_TYPE_GAN                                    4
#define OGS_GTP_RAT_TYPE_HSPA_EVOLUTION                         5
#define OGS_GTP_RAT_TYPE_EUTRAN                                 6
#define OGS_GTP_RAT_TYPE_VIRTUAL                                7
#define OGS_GTP_RAT_TYPE_EUTRAN_NB_IOT                          8

/* 8.19 EPS Bearer Level Traffic Flow Template (Bearer TFT)
 * See subclause 10.5.6.12 in 3GPP TS 24.008 [13]. */
#define OGS_GTP_MAX_TRAFFIC_FLOW_TEMPLATE 255

typedef struct ogs_gtp_tft_s {
    union {
        struct {
#define OGS_GTP_TFT_CODE_IGNORE_THIS_IE                         0
#define OGS_GTP_TFT_CODE_CREATE_NEW_TFT                         1
#define OGS_GTP_TFT_CODE_DELETE_EXISTING_TFT                    2
#define OGS_GTP_TFT_CODE_ADD_PACKET_FILTERS_TO_EXISTING_TFT     3
#define OGS_GTP_TFT_CODE_REPLACE_PACKET_FILTERS_IN_EXISTING     4
#define OGS_GTP_TFT_CODE_DELETE_PACKET_FILTERS_FROM_EXISTING    5
#define OGS_GTP_TFT_CODE_NO_TFT_OPERATION                       6
ED3(uint8_t code:3;,
    uint8_t e_bit:1;,
    uint8_t num_of_packet_filter:4;)
        };
        uint8_t flags;
    };
    struct {
        union {
            struct {
            ED3(uint8_t spare:2;,
                uint8_t direction:2;,
                uint8_t identifier:4;)
            };
            uint8_t flags;
        };
        uint8_t precedence;
        ogs_pf_content_t content;
    } pf[OGS_MAX_NUM_OF_FLOW_IN_GTP];
} ogs_gtp_tft_t;

int16_t ogs_gtp_parse_tft(ogs_gtp_tft_t *tft, ogs_tlv_octet_t *octet);
int16_t ogs_gtp_build_tft(
    ogs_tlv_octet_t *octet, ogs_gtp_tft_t *tft, void *data, int data_len);

/* 8.21 User Location Information (ULI) */
#define OGS_GTP_MAX_ULI_LEN sizeof(ogs_gtp_uli_t)
typedef struct ogs_gtp_uli_cgi_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t lac;
    uint16_t ci;
} __attribute__ ((packed)) ogs_gtp_uli_cgi_t;

typedef struct ogs_gtp_uli_sai_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t lac;
    uint16_t sac;
} __attribute__ ((packed)) ogs_gtp_uli_sai_t;

typedef struct ogs_gtp_uli_rai_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t lac;
    uint16_t rac;
} __attribute__ ((packed)) ogs_gtp_uli_rai_t;

typedef struct ogs_gtp_uli_lai_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t lac;
} __attribute__ ((packed)) ogs_gtp_uli_lai_t;

typedef struct ogs_gtp_uli_tai_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t tac;
} __attribute__ ((packed)) ogs_gtp_uli_tai_t;

typedef struct ogs_gtp_uli_e_cgi_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint32_t cell_id;
} __attribute__ ((packed)) ogs_gtp_uli_e_cgi_t;

typedef struct ogs_gtp_uli_s {
    struct {
    ED7(uint8_t spare:2;,
        uint8_t lai:1;,
        uint8_t e_cgi:1;,
        uint8_t tai:1;,
        uint8_t rai:1;,
        uint8_t sai:1;,
        uint8_t cgi:1;)
    } flags;
    ogs_gtp_uli_cgi_t cgi;
    ogs_gtp_uli_sai_t sai;
    ogs_gtp_uli_rai_t rai;
    ogs_gtp_uli_tai_t tai;
    ogs_gtp_uli_e_cgi_t e_cgi;
    ogs_gtp_uli_lai_t lai;
} ogs_gtp_uli_t;

int16_t ogs_gtp_parse_uli(ogs_gtp_uli_t *uli, ogs_tlv_octet_t *octet);
int16_t ogs_gtp_build_uli(ogs_tlv_octet_t *octet,
        ogs_gtp_uli_t *uli, void *data, int data_len);

/* 8.22 Fully Qualified TEID (F-TEID) */
#define OGS_GTP_F_TEID_S1_U_ENODEB_GTP_U                        0
#define OGS_GTP_F_TEID_S1_U_SGW_GTP_U                           1
#define OGS_GTP_F_TEID_S12_RNC_GTP_U                            2
#define OGS_GTP_F_TEID_S12_SGW_GTP_U                            3
#define OGS_GTP_F_TEID_S5_S8_SGW_GTP_U                          4
#define OGS_GTP_F_TEID_S5_S8_PGW_GTP_U                          5
#define OGS_GTP_F_TEID_S5_S8_SGW_GTP_C                          6
#define OGS_GTP_F_TEID_S5_S8_PGW_GTP_C                          7
#define OGS_GTP_F_TEID_S5_S8_SGW_PMIPV6                         8
#define OGS_GTP_F_TEID_S5_S8_PGW_PMIPV6                         9
#define OGS_GTP_F_TEID_S11_MME_GTP_C                            10
#define OGS_GTP_F_TEID_S11_S4_SGW_GTP_C                         11
#define OGS_GTP_F_TEID_S10_MME_GTP_C                            12
#define OGS_GTP_F_TEID_S3_MME_GTP_C                             13
#define OGS_GTP_F_TEID_S3_SGSN_GTP_C                            14
#define OGS_GTP_F_TEID_S4_SGSN_GTP_U                            15
#define OGS_GTP_F_TEID_S4_SGW_GTP_U                             16
#define OGS_GTP_F_TEID_S4_SGSN_GTP_C                            17
#define OGS_GTP_F_TEID_S16_SGSN_GTP_C                           18
#define OGS_GTP_F_TEID_ENODEB_GTP_U_FOR_DL_DATA_FORWARDING      19
#define OGS_GTP_F_TEID_ENODEB_GTP_U_FOR_UL_DATA_FORWARDING      20
#define OGS_GTP_F_TEID_RNC_GTP_U_FOR_DATA_FORWARDING            21
#define OGS_GTP_F_TEID_SGSN_GTP_U_FOR_DATA_FORWARDING           22
#define OGS_GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING         23
#define OGS_GTP_F_TEID_SM_MBMS_GW_GTP_C                         24
#define OGS_GTP_F_TEID_SN_MBMS_GW_GTP_C                         25
#define OGS_GTP_F_TEID_SM_MME_GTP_C                             26
#define OGS_GTP_F_TEID_SN_SGSN_GTP_C                            27
#define OGS_GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING         28
#define OGS_GTP_F_TEID_SN_SGSN_GTP_U                            29
#define OGS_GTP_F_TEID_S2B_EPDG_GTP_C                           30
#define OGS_GTP_F_TEID_S2B_U_EPDG_GTP_U                         31
#define OGS_GTP_F_TEID_S2B_PGW_GTP_C                            32
#define OGS_GTP_F_TEID_S2B_U_PGW_GTP_U                          33
#define OGS_GTP_F_TEID_S2A_TWAN_GTP_U                           34
#define OGS_GTP_F_TEID_S2A_TWAN_GTP_C                           35
#define OGS_GTP_F_TEID_S2A_PGW_GTP_C                            36
#define OGS_GTP_F_TEID_S2A_PGW_GTP_U                            37
#define OGS_GTP_F_TEID_S11_MME_GTP_U                            38
#define OGS_GTP_F_TEID_S11_SGW_GTP_U                            39

#define OGS_GTP_F_TEID_HDR_LEN          5
#define OGS_GTP_F_TEID_IPV4_LEN         OGS_IPV4_LEN+OGS_GTP_F_TEID_HDR_LEN
#define OGS_GTP_F_TEID_IPV6_LEN         OGS_IPV6_LEN+OGS_GTP_F_TEID_HDR_LEN
#define OGS_GTP_F_TEID_IPV4V6_LEN       OGS_IPV4V6_LEN+OGS_GTP_F_TEID_HDR_LEN
typedef struct ogs_gtp_f_teid_s {
ED3(uint8_t       ipv4:1;,
    uint8_t       ipv6:1;,
    uint8_t       interface_type:6;)
    uint32_t      teid;
    union {
        /* OGS_GTP_F_TEID_IPV4 */
        uint32_t addr;

        /* OGS_GTP_F_TEID_IPV6 */
        uint8_t addr6[OGS_IPV6_LEN];

        /* OGS_GTP_F_TEID_BOTH */
        struct {
            uint32_t addr;
            uint8_t addr6[OGS_IPV6_LEN];
        } both;
    };
} __attribute__ ((packed)) ogs_gtp_f_teid_t;

/* 8.44 UE Time Zone */
#define OGS_GTP_UE_TIME_ZONE_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME 0
#define OGS_GTP_UE_TIME_ZONE_1_HOUR_FOR_DAYLIGHT_SAVING_TIME        1
#define OGS_GTP_UE_TIME_ZONE_2_HOUR_FOR_DAYLIGHT_SAVING_TIME        2
/* Time Zone" IE in 3GPP TS 24.008 [5].
 * This field uses the same format as the Timezone field used in the
 * TP-Service-Centre-Time-Stamp, which is defined in 3GPP TS 23.040 [90],
 * and its value shall be set as defined in 3GPP TS 22.042 */
typedef struct ogs_gtp_ue_timezone_s {
#define OGS_GTP_TIME_TO_BCD(x) OGS_TIME_TO_BCD(x)
    /* The Time Zone indicates the difference, expressed in quarters of an hour,
     * between the local time and GMT. In the first of the two semi-octets,
     * the first bit (bit 3 of the seventh octet of
     * the TP-Service-Centre-Time-Stamp field) represents
     * the algebraic sign of this difference (0: positive, 1: negative). */
    uint8_t timezone;
ED2(uint8_t spare:6;,
    uint8_t daylight_saving_time:2;)
} __attribute__ ((packed)) ogs_gtp_ue_timezone_t;

/* 8.57 APN Restriction */
#define OGS_GTP_APN_NO_RESTRICTION                              0
#define OGS_GTP_APN_RESTRICTION_PUBLIC_1                        1
#define OGS_GTP_APN_RESTRICTION_PUBLIC_2                        2
#define OGS_GTP_APN_RESTRICTION_PRIVATE_1                       3
#define OGS_GTP_APN_RESTRICTION_PRIVATE_2                       4

/* 8.58 Selection Mode */
#define OGS_GTP_SELECTION_MODE_MS_OR_NETWORK_PROVIDED_APN       0
#define OGS_GTP_SELECTION_MODE_MS_PROVIDED_APN                  1
#define OGS_GTP_SELECTION_MODE_NETWORK_PROVIDED_APN             2

/* 8.65 Node Type */
#define OGS_GTP_NODE_TYPE_MME                                   0
#define OGS_GTP_NODE_TYPE_SGSN                                  1

/* 8.86 Allocation/Retention Priority (ARP) */
typedef struct ogs_gtp_arp_s {
#define OGS_GTP_TIME_TO_BCD(x) OGS_TIME_TO_BCD(x)
ED5(uint8_t spare1:1;,
    uint8_t pre_emption_vulnerability:1;,
    uint8_t priority_level:4;,
    uint8_t spare2:1;,
    uint8_t pre_emption_capability:1;)
} __attribute__ ((packed)) ogs_gtp_arp_t;

#ifdef __cplusplus
}
#endif

#endif /* OGS_GTP_TYPES_H */

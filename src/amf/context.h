/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef AMF_CONTEXT_H
#define AMF_CONTEXT_H

#include "ogs-app.h"
#include "ogs-sbi.h"
#include "ogs-sctp.h"
#include "ogs-ngap.h"
#include "ogs-nas-5gs.h"

#include "amf-sm.h"
#include "timer.h"
#include "metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUM_OF_SERVED_GUAMI     8

extern int __amf_log_domain;
extern int __gmm_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __amf_log_domain

typedef struct ran_ue_s ran_ue_t;
typedef struct amf_ue_s amf_ue_t;

typedef uint32_t amf_m_tmsi_t;

typedef struct amf_context_s {
    /* Served GUAMI */
    uint8_t num_of_served_guami;
    ogs_guami_t served_guami[MAX_NUM_OF_SERVED_GUAMI];

    /* Served TAI */
    uint8_t num_of_served_tai;
    struct {
        ogs_5gs_tai0_list_t list0;
        ogs_5gs_tai2_list_t list2;
    } served_tai[OGS_MAX_NUM_OF_SERVED_TAI];

    /* PLMN Support */
    uint8_t num_of_plmn_support;
    struct {
        ogs_plmn_id_t plmn_id;
        int num_of_s_nssai;
        ogs_s_nssai_t s_nssai[OGS_MAX_NUM_OF_SLICE];
    } plmn_support[OGS_MAX_NUM_OF_PLMN];

    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EEA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EEA2    2
     * #define NAS_SECURITY_ALGORITHMS_128_EEA3    3 */
    uint8_t         num_of_ciphering_order;
    uint8_t         ciphering_order[OGS_MAX_NUM_OF_ALGORITHM];
    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    2
     * #define NAS_SECURITY_ALGORITHMS_128_EIA3    3 */
    uint8_t         num_of_integrity_order;
    uint8_t         integrity_order[OGS_MAX_NUM_OF_ALGORITHM];

    /* Network Name */    
    ogs_nas_network_name_t short_name;  /* Network short name */
    ogs_nas_network_name_t full_name;   /* Network Full Name */

    /* AMF Name */
    const char *amf_name;

    /* NGSetupResponse */
    uint8_t         relative_capacity;

    /* Generator for unique identification */
    uint64_t        amf_ue_ngap_id; /* amf_ue_ngap_id generator */

    ogs_list_t      gnb_list;       /* GNB NGAP Client List */
    ogs_list_t      amf_ue_list;

    ogs_hash_t      *gnb_addr_hash; /* hash table for GNB Address */
    ogs_hash_t      *gnb_id_hash;   /* hash table for GNB-ID */
    ogs_hash_t      *guti_ue_hash;          /* hash table (GUTI : AMF_UE) */
    ogs_hash_t      *suci_hash;     /* hash table (SUCI) */
    ogs_hash_t      *supi_hash;     /* hash table (SUPI) */

    OGS_POOL(m_tmsi, amf_m_tmsi_t); /* M-TMSI Pool */

    uint16_t        ngap_port;      /* Default NGAP Port */

    ogs_list_t      ngap_list;      /* AMF NGAP IPv4 Server List */
    ogs_list_t      ngap_list6;     /* AMF NGAP IPv6 Server List */

} amf_context_t;

typedef struct amf_gnb_s {
    ogs_lnode_t     lnode;

    ogs_fsm_t       sm;         /* A state machine */

    uint32_t        gnb_id;     /* gNB_ID received from gNB */
    ogs_sctp_sock_t sctp;       /* SCTP socket */

    struct {
        bool ng_setup_success;  /* gNB NGAP Setup complete successfuly */
    } state;

    uint16_t        max_num_of_ostreams;/* SCTP Max num of outbound streams */
    uint16_t        ostream_id;         /* gnb_ostream_id generator */

    uint8_t         num_of_supported_ta_list;
    struct {
        ogs_uint24_t tac;
        uint8_t num_of_bplmn_list;
        struct {
            ogs_plmn_id_t plmn_id;
            uint8_t num_of_s_nssai;
            ogs_s_nssai_t s_nssai[OGS_MAX_NUM_OF_SLICE];
        } bplmn_list[OGS_MAX_NUM_OF_BPLMN];
    } supported_ta_list[OGS_MAX_NUM_OF_TAI];

    OpenAPI_rat_type_e rat_type;

    ogs_pkbuf_t     *ng_reset_ack; /* Reset message */

    ogs_list_t      ran_ue_list;

} amf_gnb_t;

struct ran_ue_s {
    ogs_lnode_t     lnode;
    uint32_t        index;

    /* UE identity */
#define INVALID_UE_NGAP_ID      0xffffffff /* Initial value of ran_ue_ngap_id */
    uint32_t        ran_ue_ngap_id; /* eNB-UE-NGAP-ID received from eNB */
    uint64_t        amf_ue_ngap_id; /* AMF-UE-NGAP-ID received from AMF */

    uint16_t        gnb_ostream_id; /* SCTP output stream id for eNB */

    /* UE context */
    bool            ue_context_requested;
    bool            initial_context_setup_request_sent;
    bool            ue_ambr_sent;

    /* Handover Info */
    ran_ue_t        *source_ue;
    ran_ue_t        *target_ue;

    /* Use amf_ue->nr_tai, amf_ue->nr_cgi.
     * Do not access ran_ue->saved.tai ran_ue->saved.nr_cgi.
     * 
     * Save TAI and CGI. And then, this will copy 'amf_ue_t' context later */
    struct {
        ogs_5gs_tai_t   nr_tai;
        ogs_nr_cgi_t    nr_cgi;
    } saved;

    /* NG Holding timer for removing this context */
    ogs_timer_t     *t_ng_holding;

    /* Store by UE Context Release Command
     * Retrieve by UE Context Release Complete */
#define NGAP_UE_CTX_REL_INVALID_ACTION                      0
#define NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE                   1
#define NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK                2
#define NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE                   3
#define NGAP_UE_CTX_REL_NG_HANDOVER_COMPLETE                4
#define NGAP_UE_CTX_REL_NG_HANDOVER_CANCEL                  5
#define NGAP_UE_CTX_REL_NG_HANDOVER_FAILURE                 6
    uint8_t         ue_ctx_rel_action;

    bool            part_of_ng_reset_requested;

    /* Related Context */
    amf_gnb_t       *gnb;
    amf_ue_t        *amf_ue;
}; 

#define AMF_NF_INSTANCE_CLEAR(_cAUSE, _nFInstance) \
    do { \
        ogs_assert(_nFInstance); \
        if ((_nFInstance)->reference_count == 1) { \
            ogs_info("[%s] (%s) NF removed", (_nFInstance)->id, (_cAUSE)); \
            amf_nf_fsm_fini((_nFInstance)); \
        } else { \
            /* There is an assocation with other context */ \
            ogs_info("[%s:%d] (%s) NF suspended", \
                    _nFInstance->id, _nFInstance->reference_count, (_cAUSE)); \
            OGS_FSM_TRAN(&_nFInstance->sm, amf_nf_state_de_registered); \
            ogs_fsm_dispatch(&_nFInstance->sm, NULL); \
        } \
        ogs_sbi_nf_instance_remove(_nFInstance); \
    } while(0)

struct amf_ue_s {
    ogs_sbi_object_t sbi;
    ogs_fsm_t sm;

    struct {
        uint8_t message_type; /* Type of last specific NAS message received */
        int access_type; /* 3GPP or Non-3GPP */

        struct {
        ED3(uint8_t tsc:1;,
            uint8_t ksi:3;,
            uint8_t spare:4;)
        } amf, ue;

        ogs_nas_5gs_registration_type_t registration;
        ogs_nas_de_registration_type_t de_registration;

        struct {
        ED4(uint8_t uplink_data_status:1;,
            uint8_t pdu_session_status:1;,
            uint8_t allowed_pdu_session_status:1;,
            uint8_t reserved:5;)
        } present;

    } __attribute__ ((packed)) nas;

    /* UE identity */
#define AMF_UE_HAVE_SUCI(__aMF) \
    ((__aMF) && ((__aMF)->suci))
    char            *suci; /* TS33.501 : SUCI */
    char            *supi; /* TS33.501 : SUPI */
    ogs_nas_5gs_mobile_identity_suci_t nas_mobile_identity_suci;

    char            *pei;
    uint8_t         masked_imeisv[OGS_MAX_IMEISV_LEN];
    int             masked_imeisv_len;
    char            imeisv_bcd[OGS_MAX_IMEISV_BCD_LEN+1];
    ogs_nas_mobile_identity_imeisv_t nas_mobile_identity_imeisv;

    int             num_of_msisdn;
    char            *msisdn[OGS_MAX_NUM_OF_MSISDN];

    struct {
        amf_m_tmsi_t *m_tmsi;
        ogs_nas_5gs_guti_t guti;
    } current, next;

    /* UE Info */
    ogs_guami_t     *guami;
    uint16_t        gnb_ostream_id;
    ogs_5gs_tai_t   nr_tai;
    ogs_nr_cgi_t    nr_cgi;
    ogs_time_t      ue_location_timestamp;
    ogs_plmn_id_t   last_visited_plmn_id;
    ogs_nas_ue_usage_setting_t ue_usage_setting;

    struct {
        int num_of_s_nssai;
        ogs_nas_s_nssai_ie_t s_nssai[OGS_MAX_NUM_OF_SLICE];
    } requested_nssai, allowed_nssai;

    struct {
        int num_of_s_nssai;
        ogs_nas_rejected_s_nssai_t s_nssai[OGS_MAX_NUM_OF_SLICE];
    } rejected_nssai;

    /* PCF sends the RESPONSE
     * of [POST] /npcf-am-polocy-control/v1/policies */
#define PCF_AM_POLICY_ASSOCIATED(__aMF) \
    ((__aMF) && ((__aMF)->policy_association_id))

#define PCF_AM_POLICY_CLEAR(__aMF) \
    OGS_MEM_CLEAR((__aMF)->policy_association_id);
#define PCF_AM_POLICY_STORE(__aMF, __iD) \
    OGS_STRING_DUP((__aMF)->policy_association_id, __iD);
    char *policy_association_id;

    /* 5GMM Capability */
    struct {
        bool lte_positioning_protocol_capability;
        bool ho_attach;
        bool s1_mode;
    } gmm_capability;

#define SECURITY_CONTEXT_IS_VALID(__aMF) \
    ((__aMF) && \
    ((__aMF)->security_context_available == 1) && \
     ((__aMF)->mac_failed == 0) && \
     ((__aMF)->nas.ue.ksi != OGS_NAS_KSI_NO_KEY_IS_AVAILABLE))
    int             security_context_available;
    int             mac_failed;

    /* Security Context */
    ogs_nas_ue_security_capability_t ue_security_capability;
    ogs_nas_ue_network_capability_t ue_network_capability;
    char            *confirmation_url_for_5g_aka;
    uint8_t         rand[OGS_RAND_LEN];
    uint8_t         autn[OGS_AUTN_LEN];
    uint8_t         xres_star[OGS_MAX_RES_LEN];

    uint8_t         abba[OGS_NAS_MAX_ABBA_LEN];
    uint8_t         abba_len;

    uint8_t         hxres_star[OGS_MAX_RES_LEN];
    uint8_t         kamf[OGS_SHA256_DIGEST_SIZE];
    OpenAPI_auth_result_e auth_result;

    uint8_t         knas_int[OGS_SHA256_DIGEST_SIZE/2];
    uint8_t         knas_enc[OGS_SHA256_DIGEST_SIZE/2];
    uint32_t        dl_count;
    union {
        struct {
        ED3(uint8_t spare;,
            uint16_t overflow;,
            uint8_t sqn;)
        } __attribute__ ((packed));
        uint32_t i32;
    } ul_count;
    uint8_t         kgnb[OGS_SHA256_DIGEST_SIZE];

    struct {
    ED2(uint8_t nhcc_spare:5;,
        uint8_t nhcc:3;) /* Next Hop Channing Counter */
    };
    uint8_t         nh[OGS_SHA256_DIGEST_SIZE]; /* NH Security Key */

    /* defined in 'lib/nas/common/types.h'
     * #define OGS_NAS_SECURITY_ALGORITHMS_NEA0        0
     * #define OGS_NAS_SECURITY_ALGORITHMS_128_NEA1    1
     * #define OGS_NAS_SECURITY_ALGORITHMS_128_NEA2    2
     * #define OGS_NAS_SECURITY_ALGORITHMS_128_NEA3    3 */
    uint8_t         selected_enc_algorithm;
    /* defined in 'lib/nas/common/types.h'
     * #define OGS_NAS_SECURITY_ALGORITHMS_NIA0        0
     * #define OGS_NAS_SECURITY_ALGORITHMS_128_NIA1    1
     * #define OGS_NAS_SECURITY_ALGORITHMS_128_NIA1    2
     * #define OGS_NAS_SECURITY_ALGORITHMS_128_NIA3    3 */
    uint8_t         selected_int_algorithm;

    /* SubscribedInfo */
    ogs_bitrate_t   ue_ambr;
    int num_of_slice;
    ogs_slice_data_t slice[OGS_MAX_NUM_OF_SLICE];

    uint64_t        am_policy_control_features; /* SBI Features */

#define CM_CONNECTED(__aMF) \
    ((__aMF) && ((__aMF)->ran_ue != NULL) && ran_ue_cycle((__aMF)->ran_ue))
#define CM_IDLE(__aMF) \
    ((__aMF) && \
     (((__aMF)->ran_ue == NULL) || (ran_ue_cycle((__aMF)->ran_ue) == NULL)))
    /* NG UE context */
    ran_ue_t        *ran_ue;

#define CLEAR_AMF_UE_ALL_TIMERS(__aMF) \
    do { \
        CLEAR_AMF_UE_TIMER((__aMF)->t3513); \
        CLEAR_AMF_UE_TIMER((__aMF)->t3522); \
        CLEAR_AMF_UE_TIMER((__aMF)->t3550); \
        CLEAR_AMF_UE_TIMER((__aMF)->t3555); \
        CLEAR_AMF_UE_TIMER((__aMF)->t3560); \
        CLEAR_AMF_UE_TIMER((__aMF)->t3570); \
    } while(0);
#define CLEAR_AMF_UE_TIMER(__aMF_UE_TIMER) \
    do { \
        ogs_timer_stop((__aMF_UE_TIMER).timer); \
        if ((__aMF_UE_TIMER).pkbuf) { \
            ogs_pkbuf_free((__aMF_UE_TIMER).pkbuf); \
            (__aMF_UE_TIMER).pkbuf = NULL; \
        } \
        (__aMF_UE_TIMER).retry_count = 0; \
    } while(0);
    struct {
        ogs_pkbuf_t     *pkbuf;
        ogs_timer_t     *timer;
        uint32_t        retry_count;;
    } t3513, t3522, t3550, t3555, t3560, t3570;

    /* UE Radio Capability */
    OCTET_STRING_t  ueRadioCapability;

    /* UEContextReleaseRequest or InitialContextSetupFailure */
    struct {
        NGAP_Cause_PR group;
        long cause;
    } deactivation;

    /* Handover Info */
    struct {
        NGAP_HandoverType_t type;
        OCTET_STRING_t container;
        NGAP_Cause_PR group;
        long cause;
    } handover;

    /* Network Initiated De-Registration */
    bool network_initiated_de_reg;

    ogs_list_t      sess_list;
};

typedef struct amf_sess_s {
    ogs_sbi_object_t sbi;

    uint8_t psi;            /* PDU Session Identity */
    uint8_t pti;            /* Procedure Trasaction Identity */

#define SESSION_CONTEXT_IN_SMF(__sESS)  \
    ((__sESS) && (__sESS)->sm_context_ref)
#define CLEAR_SM_CONTEXT_REF(__sESS) \
    do { \
        ogs_assert(__sESS); \
        ogs_assert((__sESS)->sm_context_ref); \
        ogs_free((__sESS)->sm_context_ref); \
        (__sESS)->sm_context_ref = NULL; \
    } while(0);

    /* SMF sends the RESPONSE
     * of [POST] /nsmf-pdusession/v1/sm-contexts */
    char *sm_context_ref;

    /* SMF sends the REQUEST
     * of [POST] /namf-comm/v1/ue-contexts/{supi}/n1-n2-messages */
    ogs_pkbuf_t *pdu_session_establishment_accept;

    /*
     * [AMF]
     *   1. PDUSessionResourceReleaseResponse
     *      REQUEST [POST] /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
     *   2. PDU Session release complete
     *      REQUEST [POST] /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
     *
     * [SMF]
     *   1. RESPONSE /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
     *      sess->n2_released = true;
     *
     *   2. RESPONSE /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
     *      sess->n1_released = true;
     *
     *   3. NOTIFY /namf-callback/v1/{supi}/sm-context-status/{psi})
     *      sess->resource_status = OpenAPI_resource_status_RELEASED;
     *
     * if (sess->n1_released == true &&
     *     sess->n2_released == true &&
     *     sess->resource_status == OpenAPI_resource_status_RELEASED) {
     *
     *     REMOVE_SESSION_CONTEXT()
     * }
     */
    OpenAPI_resource_status_e resource_status;
    bool n1_released;
    bool n2_released;

    struct {
        ogs_pkbuf_t *pdu_session_resource_setup_request;
        ogs_pkbuf_t *pdu_session_resource_modification_command;
        ogs_pkbuf_t *path_switch_request_ack;
        ogs_pkbuf_t *handover_request;
        ogs_pkbuf_t *handover_command;
    } transfer;
#define AMF_SESS_STORE_N2_TRANSFER(__sESS, __n2Type, __n2Buf) \
    do { \
        ogs_assert(__sESS); \
        ogs_assert((__sESS)->amf_ue); \
        if ((__sESS)->transfer.__n2Type) { \
            ogs_warn("[%s:%d] N2 transfer message duplicated. Overwritten", \
                    ((__sESS)->amf_ue)->supi, (__sESS)->psi); \
            ogs_pkbuf_free((__sESS)->transfer.__n2Type); \
        } \
        (__sESS)->transfer.__n2Type = __n2Buf; \
        ogs_assert((__sESS)->transfer.__n2Type); \
    } while(0);

#define AMF_SESS_CLEAR_N2_TRANSFER(__sESS, __n2Type) \
    do { \
        if ((__sESS)->transfer.__n2Type) \
            ogs_pkbuf_free((__sESS)->transfer.__n2Type); \
        (__sESS)->transfer.__n2Type = NULL; \
    } while(0);

#define AMF_UE_CLEAR_N2_TRANSFER(__aMF, __n2Type) \
    do { \
        amf_sess_t *sess = NULL; \
        ogs_list_for_each(&((__aMF)->sess_list), sess) { \
            AMF_SESS_CLEAR_N2_TRANSFER(sess, __n2Type) \
        } \
    } while(0);

    struct {
        /* Paging Ongoing */
        bool ongoing;
        /* Location in N1N2MessageTransferRspData */
        char *location;
        /* last Received n1-n2-trasfer-failure-notification-uri from SMF */
        char *n1n2_failure_txf_notif_uri;
        /* notification client */
        ogs_sbi_client_t *client;
    } paging;
#define AMF_SESS_STORE_PAGING_INFO(__sESS, __lOCATION, __uRI) \
    do { \
        ogs_assert(__sESS); \
        ogs_assert(__lOCATION); \
        AMF_SESS_CLEAR_PAGING_INFO(__sESS) \
        (__sESS)->paging.ongoing = true; \
        ((__sESS)->paging.location) = ogs_strdup(__lOCATION); \
        ogs_assert((__sESS)->paging.location); \
        if (__uRI) { \
            ((__sESS)->paging.n1n2_failure_txf_notif_uri) = ogs_strdup(__uRI); \
            ogs_assert((__sESS)->paging.n1n2_failure_txf_notif_uri); \
        } \
    } while(0);
#define AMF_SESS_CLEAR_PAGING_INFO(__sESS) \
    do { \
        if ((__sESS)->paging.ongoing == true) { \
            ogs_assert((__sESS)->paging.location); \
            ogs_free((__sESS)->paging.location); \
            ((__sESS)->paging.location) = NULL; \
            if ((__sESS)->paging.n1n2_failure_txf_notif_uri) { \
                ogs_free((__sESS)->paging.n1n2_failure_txf_notif_uri); \
                ((__sESS)->paging.n1n2_failure_txf_notif_uri) = NULL; \
            } \
            ((__sESS)->paging.ongoing) = false; \
        } \
    } while(0);
#define AMF_UE_CLEAR_PAGING_INFO(__aMF) \
    do { \
        amf_sess_t *sess = NULL; \
        ogs_list_for_each(&((__aMF)->sess_list), sess) { \
            AMF_SESS_CLEAR_PAGING_INFO(sess); \
        } \
    } while(0);

    struct {
        uint8_t type;
        ogs_pkbuf_t *n1buf;
        ogs_pkbuf_t *n2buf;
    } gsm_message;

#define AMF_SESS_STORE_5GSM_MESSAGE(__sESS, __tYPE, __n1Buf, __n2Buf) \
    do { \
        ogs_assert(__sESS); \
        ogs_assert((__sESS)->amf_ue); \
        if ((__sESS)->gsm_message.n1buf) { \
            ogs_warn("[%s:%d] N1 message duplicated. Overwritten", \
                    ((__sESS)->amf_ue)->supi, (__sESS)->psi); \
            ogs_pkbuf_free((__sESS)->gsm_message.n1buf); \
        } \
        (__sESS)->gsm_message.n1buf = __n1Buf; \
        ogs_assert((__sESS)->gsm_message.n1buf); \
        if ((__sESS)->gsm_message.n2buf) { \
            ogs_warn("[%s:%d] N2 message duplicated. Overwritten", \
                    ((__sESS)->amf_ue)->supi, (__sESS)->psi); \
            ogs_pkbuf_free((__sESS)->gsm_message.n2buf); \
        } \
        (__sESS)->gsm_message.n2buf = __n2Buf; \
        ogs_assert((__sESS)->gsm_message.n2buf); \
        (__sESS)->gsm_message.type = __tYPE; \
    } while(0);

#define AMF_SESS_CLEAR_5GSM_MESSAGE(__sESS) \
    do { \
        if ((__sESS)->gsm_message.n1buf) \
            ogs_pkbuf_free((__sESS)->gsm_message.n1buf); \
        (__sESS)->gsm_message.n1buf = NULL; \
        if ((__sESS)->gsm_message.n2buf) \
            ogs_pkbuf_free((__sESS)->gsm_message.n2buf); \
        (__sESS)->gsm_message.n2buf = NULL; \
        (__sESS)->gsm_message.type = 0; \
    } while(0);

#define AMF_UE_CLEAR_5GSM_MESSAGE(__aMF) \
    do { \
        amf_sess_t *sess = NULL; \
        ogs_list_for_each(&((__aMF)->sess_list), sess) { \
            AMF_SESS_CLEAR_5GSM_MESSAGE(sess) \
        } \
    } while(0);

    struct {
        char *nsi_id;
        struct {
            char *id;
            ogs_sbi_client_t *client;
        } nrf;
    } nssf;

    /* last payload for sending back to the UE */
    uint8_t         payload_container_type;
    ogs_pkbuf_t     *payload_container;

    /* amf_bearer_first(sess) : Default Bearer Context */
    ogs_list_t      bearer_list;

    /* Related Context */
    amf_ue_t        *amf_ue;

    ogs_s_nssai_t s_nssai;
    ogs_s_nssai_t mapped_hplmn;
    char *dnn;

    /* Save Protocol Configuration Options from UE */
    struct {
        uint8_t length;
        uint8_t *buffer;
    } ue_pco; 

    /* Save Protocol Configuration Options from PGW */
    ogs_tlv_octet_t pgw_pco;

} amf_sess_t;

void amf_context_init(void);
void amf_context_final(void);
amf_context_t *amf_self(void);

int amf_context_parse_config(void);

amf_gnb_t *amf_gnb_add(ogs_sock_t *sock, ogs_sockaddr_t *addr);
void amf_gnb_remove(amf_gnb_t *gnb);
void amf_gnb_remove_all(void);
amf_gnb_t *amf_gnb_find_by_addr(ogs_sockaddr_t *addr);
amf_gnb_t *amf_gnb_find_by_gnb_id(uint32_t gnb_id);
int amf_gnb_set_gnb_id(amf_gnb_t *gnb, uint32_t gnb_id);
int amf_gnb_sock_type(ogs_sock_t *sock);

ran_ue_t *ran_ue_add(amf_gnb_t *gnb, uint32_t ran_ue_ngap_id);
void ran_ue_remove(ran_ue_t *ran_ue);
void ran_ue_switch_to_gnb(ran_ue_t *ran_ue, amf_gnb_t *new_gnb);
ran_ue_t *ran_ue_find_by_ran_ue_ngap_id(
        amf_gnb_t *gnb, uint32_t ran_ue_ngap_id);
ran_ue_t *ran_ue_find(uint32_t index);
ran_ue_t *ran_ue_find_by_amf_ue_ngap_id(uint64_t amf_ue_ngap_id);
ran_ue_t *ran_ue_cycle(ran_ue_t *ran_ue);

void amf_ue_new_guti(amf_ue_t *amf_ue);
void amf_ue_confirm_guti(amf_ue_t *amf_ue);

amf_ue_t *amf_ue_add(ran_ue_t *ran_ue);
void amf_ue_remove(amf_ue_t *amf_ue);
void amf_ue_remove_all(void);

void amf_ue_fsm_init(amf_ue_t *amf_ue);
void amf_ue_fsm_fini(amf_ue_t *amf_ue);

amf_ue_t *amf_ue_find_by_guti(ogs_nas_5gs_guti_t *nas_guti);
amf_ue_t *amf_ue_find_by_teid(uint32_t teid);
amf_ue_t *amf_ue_find_by_suci(char *suci);
amf_ue_t *amf_ue_find_by_supi(char *supi);

amf_ue_t *amf_ue_find_by_message(ogs_nas_5gs_message_t *message);
void amf_ue_set_suci(amf_ue_t *amf_ue,
        ogs_nas_5gs_mobile_identity_t *mobile_identity);
void amf_ue_set_supi(amf_ue_t *amf_ue, char *supi);

OpenAPI_rat_type_e amf_ue_rat_type(amf_ue_t *amf_ue);

/* 
 * o RECV Initial UE-Message : S-TMSI
 * o RECV Attach Request : IMSI, GUTI
 * o RECV TAU Request : GUTI
 * ### AMF_UE_ASSOCIATE_GNB_UE() ###
 * ### AMF_UE_ECM_CONNECTED() ###
 *
 * o RECV Initial Context Setup Failure in EMM Registered State
 * ### AMF_UE_DEASSOCIATE_GNB_UE() ###
 * ### GNB_UE_REMOVE() ###
 * ### AMF_UE_DEASSOCIATE() ###
 *
 * o SEND UE Context Release Command with NO_ACTION
 *   - RECV UE Context Release Complete
 * ### GNB_UE_REMOVE() ###
 * ### AMF_UE_DEASSOCIATE() ###
 *
 * o SEND UE Context Release Command with REMOVE_AMF_UE_CONTEXT
 *   - RECV UE Context Release Complete
 * ### GNB_UE_REMOVE() ###
 * ### AMF_UE_REMOVE() ###
 *
 *
 * o RECV Handover Required
 * ### SOURCE_UE_ASSOCIATE_TARGET_UE() ####
 *   - SEND Handover Request
 *
 * o RECV Handover Notify
 * ### AMF_UE_ASSOCIATE_GNB_UE(TARGET) ###
 * ### AMF_UE_ECM_CONNECTED(TARGET) ###
 *   - Modify Bearer Request/Response
 *   - UE Context Release Command/Complete
 * ### SOURCE_UE_DEASSOCIATE_TARGET_UE() ####
 * ### GNB_UE_REMOVE() ####
 *   - Delete Indirect Data Forwarding Tunnel Request/Response
 *
 * o RECV Handover Cancel
 *   - UE Context Release Command/Complete
 * ### SOURCE_UE_DEASSOCIATE_TARGET_UE() ####
 * ### GNB_UE_REMOVE() ####
 *   - Delete Indirect Data Forwarding Tunnel Request/Response
 *
 * o RECV Handover Failure
 *   - UE Context Release Command/Complete
 * ### SOURCE_UE_DEASSOCIATE_TARGET_UE() ####
 * ### GNB_UE_REMOVE() ####
 *   - Delete Indirect Data Forwarding Tunnel Request/Response
 */
void amf_ue_associate_ran_ue(amf_ue_t *amf_ue, ran_ue_t *ran_ue);
void ran_ue_deassociate(ran_ue_t *ran_ue);
void amf_ue_deassociate(amf_ue_t *amf_ue);
void source_ue_associate_target_ue(ran_ue_t *source_ue, ran_ue_t *target_ue);
void source_ue_deassociate_target_ue(ran_ue_t *ran_ue);

amf_sess_t *amf_sess_add(amf_ue_t *amf_ue, uint8_t psi);

/*
 * If there is SBI transaction,
 * we will not remove session context at this point.
 */
#define AMF_SESS_CLEAR(__sESS) \
    do { \
        ogs_sbi_object_t *sbi_object = NULL; \
        ogs_assert(__sESS); \
        sbi_object = &sess->sbi; \
        ogs_assert(sbi_object); \
        \
        if (ogs_list_count(&sbi_object->xact_list)) { \
            ogs_error("SBI running [%d]", \
                    ogs_list_count(&sbi_object->xact_list)); \
        } else { \
            amf_sess_remove(__sESS); \
        } \
    } while(0)
void amf_sess_remove(amf_sess_t *sess);
void amf_sess_remove_all(amf_ue_t *amf_ue);
amf_sess_t *amf_sess_find_by_psi(amf_ue_t *amf_ue, uint8_t psi);
amf_sess_t *amf_sess_find_by_dnn(amf_ue_t *amf_ue, char *dnn);

amf_ue_t *amf_ue_cycle(amf_ue_t *amf_ue);
amf_sess_t *amf_sess_cycle(amf_sess_t *sess);

void amf_ue_select_nf(amf_ue_t *amf_ue, OpenAPI_nf_type_e nf_type);
void amf_sess_select_nf(amf_sess_t *sess, OpenAPI_nf_type_e nf_type);

void amf_sess_select_smf(amf_sess_t *sess);

#define AMF_SESSION_SYNC_DONE(__aMF, __sTATE) \
    (amf_sess_xact_state_count(__aMF, __sTATE) == 0)
int amf_sess_xact_count(amf_ue_t *amf_ue);
int amf_sess_xact_state_count(amf_ue_t *amf_ue, int state);

#define PDU_RES_SETUP_REQ_TRANSFER_NEEDED(__aMF) \
    (amf_pdu_res_setup_req_transfer_needed(__aMF) == true)
bool amf_pdu_res_setup_req_transfer_needed(amf_ue_t *amf_ue);
#define HANDOVER_REQUEST_TRANSFER_NEEDED(__aMF) \
    (amf_handover_request_transfer_needed(__aMF) == true)
bool amf_handover_request_transfer_needed(amf_ue_t *amf_ue);

#define PAGING_ONGOING(__aMF) \
    (amf_paging_ongoing(__aMF) == true)
bool amf_paging_ongoing(amf_ue_t *amf_ue);
#define DOWNLINK_SIGNALLING_PENDING(__aMF) \
    (amf_downlink_signalling_pending(__aMF) == true)
bool amf_downlink_signalling_pending(amf_ue_t *amf_ue);

int amf_find_served_tai(ogs_5gs_tai_t *nr_tai);
ogs_s_nssai_t *amf_find_s_nssai(
        ogs_plmn_id_t *served_plmn_id, ogs_s_nssai_t *s_nssai);

int amf_m_tmsi_pool_generate(void);
amf_m_tmsi_t *amf_m_tmsi_alloc(void);
int amf_m_tmsi_free(amf_m_tmsi_t *tmsi);

uint8_t amf_selected_int_algorithm(amf_ue_t *amf_ue);
uint8_t amf_selected_enc_algorithm(amf_ue_t *amf_ue);

void amf_clear_subscribed_info(amf_ue_t *amf_ue);

bool amf_update_allowed_nssai(amf_ue_t *amf_ue);

#ifdef __cplusplus
}
#endif

#endif /* AMF_CONTEXT_H */

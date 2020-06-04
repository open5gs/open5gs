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

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUM_OF_SERVED_GUAMI     8

extern int __amf_log_domain;
extern int __gmm_log_domain;
extern int __gsm_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __amf_log_domain

typedef struct ran_ue_s ran_ue_t;
typedef struct amf_ue_s amf_ue_t;

typedef uint32_t amf_m_tmsi_t;
typedef uint32_t amf_p_tmsi_t;

typedef struct amf_context_s {
    ogs_queue_t     *queue;         /* Queue for processing UPF control */
    ogs_timer_mgr_t *timer_mgr;     /* Timer Manager */
    ogs_pollset_t   *pollset;       /* Poll Set for I/O Multiplexing */

    OpenAPI_nf_type_e   nf_type;

    /* Served GUAMI */
    uint8_t num_of_served_guami;
    struct {
        ogs_plmn_id_t plmn_id;
        ogs_amf_id_t amf_id;
    } served_guami[MAX_NUM_OF_SERVED_GUAMI];

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
        ogs_s_nssai_t s_nssai[OGS_MAX_NUM_OF_S_NSSAI];
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
    ogs_hash_t      *amf_ue_ngap_id_hash;   /* hash table for AMF-UE-NGAP-ID */
    ogs_hash_t      *imsi_ue_hash;          /* hash table (IMSI : AMF_UE) */
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
    int             sock_type;  /* SOCK_STREAM or SOCK_SEQPACKET */
    ogs_sock_t      *sock;      /* gNB NGAP Socket */
    ogs_sockaddr_t  *addr;      /* gNB NGAP Address */
    ogs_poll_t      *poll;      /* gNB NGAP Poll */

    struct {
        bool ng_setup_success;  /* gNB NGAP Setup complete successfuly */
    } state;

    uint16_t        max_num_of_ostreams;/* SCTP Max num of outbound streams */
    uint16_t        ostream_id;         /* gnb_ostream_id generator */


    uint8_t         num_of_supported_ta_list;
    ogs_5gs_tai_t   supported_ta_list[OGS_MAX_NUM_OF_TAI*OGS_MAX_NUM_OF_BPLMN];

    ogs_list_t      ran_ue_list;

} amf_gnb_t;

struct ran_ue_s {
    ogs_lnode_t     lnode;

    /* UE identity */
#define INVALID_UE_NGAP_ID      0xffffffff /* Initial value of ran_ue_ngap_id */
    uint32_t        ran_ue_ngap_id; /* eNB-UE-NGAP-ID received from eNB */
    uint64_t        amf_ue_ngap_id; /* AMF-UE-NGAP-ID received from AMF */

    uint16_t        gnb_ostream_id; /* SCTP output stream id for eNB */

    /* Handover Info */
    NGAP_HandoverType_t handover_type;
    ran_ue_t        *source_ue;
    ran_ue_t        *target_ue;

    /* Use amf_ue->tai, amf_ue->e_cgi.
     * Do not access ran_ue->saved.tai ran_ue->saved.e_cgi.
     * 
     * Save TAI and ECGI. And then, this will copy 'amf_ue_t' context later */
    struct {
        ogs_5gs_tai_t   tai;
        ogs_nr_cgi_t    cgi;
    } saved;

    /* Store by UE Context Release Command
     * Retrieve by UE Context Release Complete */
#define NGAP_UE_CTX_REL_INVALID_ACTION                      0
#define NGAP_UE_CTX_REL_S1_CONTEXT_REMOVE                   1
#define NGAP_UE_CTX_REL_S1_REMOVE_AND_UNLINK                2
#define NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE                   3
#define NGAP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL              4
    uint8_t         ue_ctx_rel_action;

    /* Related Context */
    amf_gnb_t       *gnb;
    amf_ue_t        *amf_ue;
}; 

struct amf_ue_s {
    ogs_lnode_t     lnode;
    ogs_fsm_t       sm;     /* A state machine */

    struct {
#define OGS_NAS_SECURITY_BEARER_3GPP 1
#define OGS_NAS_SECURITY_BEARER_NON_3GPP 2
        uint8_t     connection_identifier;
        uint8_t     type;
        uint8_t     ksi;
        union {
            ogs_nas_5gs_registration_type_t registration;
#if 0
            ogs_5gs_update_type_t update;
            ogs_5gs_service_type_t service;
            ogs_5gs_detach_type_t detach;
#endif
            uint8_t data;
        };
    } nas;

    /* UE identity */
    char            *suci; /* TS33.501 : SUCI */
    char            *supi; /* TS33.501 : SUPI */

#define AMF_UE_HAVE_SUCI(__aMF) \
    ((__aMF) && ((__aMF)->suci))
    uint8_t         imsi[OGS_MAX_IMSI_LEN];
    int             imsi_len;
    char            imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];
    ogs_nas_5gs_mobile_identity_imsi_t nas_mobile_identity_imsi;

    bool            imeisv_presence;
    uint8_t         imeisv[OGS_MAX_IMEISV_LEN];
    int             imeisv_len;
    char            imeisv_bcd[OGS_MAX_IMEISV_BCD_LEN+1];
    ogs_nas_mobile_identity_imeisv_t nas_mobile_identity_imeisv;

    amf_m_tmsi_t    *m_tmsi;
    amf_p_tmsi_t    p_tmsi;
    ogs_nas_5gs_guti_t guti;
    int             guti_present;

    uint16_t        vlr_ostream_id; /* SCTP output stream id for VLR */

    /* UE Info */
    ogs_5gs_tai_t   tai;
    ogs_nr_cgi_t    cgi;
    ogs_plmn_id_t   last_visited_plmn_id;

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
     ((__aMF)->nas.ksi != OGS_NAS_KSI_NO_KEY_IS_AVAILABLE))
#define CLEAR_SECURITY_CONTEXT(__aMF) \
    do { \
        ogs_assert((__aMF)); \
        (__aMF)->security_context_available = 0; \
        (__aMF)->mac_failed = 0; \
        (__aMF)->nas.ksi = 0; \
    } while(0)
    int             security_context_available;
    int             mac_failed;

    /* Security Context */
    ogs_nas_ue_security_capability_t ue_security_capability;
#if 0
    ogs_nas_ue_network_capability_t ue_network_capability;
    ogs_nas_ms_network_capability_t ms_network_capability;
#endif
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

    /* ESM Info */
#define MIN_5GS_BEARER_ID           5
#define MAX_5GS_BEARER_ID           15

#define CLEAR_5GS_BEARER_ID(__aMF) \
    do { \
        ogs_assert((__aMF)); \
        (__aMF)->ebi = MIN_5GS_BEARER_ID - 1; \
    } while(0)
    uint8_t         ebi; /* 5GS Bearer ID generator */
    ogs_list_t      sess_list;

#define ECM_CONNECTED(__aMF) \
    ((__aMF) && ((__aMF)->ran_ue != NULL))
#define ECM_IDLE(__aMF) (!ECM_CONNECTED(__aMF))
    /* S1 UE context */
    ran_ue_t        *ran_ue;

#define CLEAR_AMF_UE_ALL_TIMERS(__aMF) \
    do { \
        CLEAR_AMF_UE_TIMER((__aMF)->sbi_client_wait); \
        CLEAR_AMF_UE_TIMER((__aMF)->t3513); \
        CLEAR_AMF_UE_TIMER((__aMF)->t3522); \
        CLEAR_AMF_UE_TIMER((__aMF)->t3550); \
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
    } sbi_client_wait, t3513, t3522, t3550, t3560, t3570;

    /* UE Radio Capability */
    OCTET_STRING_t  ueRadioCapability;

    /* NGAP Transparent Container */
    OCTET_STRING_t  container;

    /*
     * If the AMF sends Delete-Session-Request to the SMF for all sessions,
     *    session_context_will_deleted = 1
     * When the AMF receives a Delete-Session-Response for the last session,
     *    session_context_will_deleted = 0
     */
    int             session_context_will_deleted;

#if 0
    amf_csmap_t     *csmap;
#endif

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

    ogs_sbi_nf_types_t nf_types;
};

#define AMF_HAVE_SMF_S1U_PATH(__sESS) \
    ((__sESS) && (amf_bearer_first(__sESS)) && \
     ((amf_default_bearer_in_sess(__sESS)->smf_s1u_teid)))
#define CLEAR_SMF_S1U_PATH(__sESS) \
    do { \
        amf_bearer_t *__bEARER = NULL; \
        ogs_assert((__sESS)); \
        __bEARER = amf_default_bearer_in_sess(__sESS); \
        __bEARER->smf_s1u_teid = 0; \
    } while(0)

#define SESSION_CONTEXT_IS_AVAILABLE(__aMF) \
     ((__aMF) && ((__aMF)->smf_s11_teid))

#define SESSION_CONTEXT_WILL_DELETED(__aMF) \
     ((__aMF) && ((__aMF)->session_context_will_deleted))

#define CLEAR_SESSION_CONTEXT(__aMF) \
    do { \
        ogs_assert((__aMF)); \
        (__aMF)->smf_s11_teid = 0; \
        (__aMF)->session_context_will_deleted = 0; \
    } while(0)
typedef struct amf_sess_s {
    ogs_lnode_t     lnode;

    uint8_t         pti;        /* Procedure Trasaction Identity */

#if 0
    /* PDN Connectivity Request */
    ogs_nas_request_type_t request_type; 
#endif

    /* amf_bearer_first(sess) : Default Bearer Context */
    ogs_list_t      bearer_list;

    /* Related Context */
    amf_ue_t        *amf_ue;

#define AMF_UE_HAVE_APN(__aMF) \
    ((__aMF) && (amf_sess_first(__aMF)) && \
    ((amf_sess_first(__aMF))->pdn))
    ogs_pdn_t       *pdn;

    /* Save Protocol Configuration Options from UE */
    struct {
        uint8_t length;
        uint8_t *buffer;
    } ue_pco; 

    /* Save Protocol Configuration Options from PGW */
    ogs_tlv_octet_t pgw_pco;
} amf_sess_t;

#define BEARER_CONTEXT_IS_ACTIVE(__aMF)  \
    (amf_bearer_is_inactive(__aMF) == 0)
#define CLEAR_BEARER_CONTEXT(__aMF)   \
    amf_bearer_set_inactive(__aMF)

#define AMF_HAVE_GNB_S1U_PATH(__bEARER) \
    ((__bEARER) && ((__bEARER)->gnb_s1u_teid))
#define CLEAR_GNB_S1U_PATH(__bEARER) \
    do { \
        ogs_assert((__bEARER)); \
        (__bEARER)->gnb_s1u_teid = 0; \
    } while(0)

#define AMF_HAVE_GNB_DL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->gnb_dl_teid))
#define AMF_HAVE_GNB_UL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->gnb_ul_teid))
#define AMF_HAVE_SMF_DL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->smf_dl_teid))
#define AMF_HAVE_SMF_UL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->smf_ul_teid))
#define CLEAR_INDIRECT_TUNNEL(__bEARER) \
    do { \
        ogs_assert((__bEARER)); \
        (__bEARER)->gnb_dl_teid = 0; \
        (__bEARER)->gnb_ul_teid = 0; \
        (__bEARER)->smf_dl_teid = 0; \
        (__bEARER)->smf_ul_teid = 0; \
    } while(0)
typedef struct amf_bearer_s {
    ogs_lnode_t     lnode;
    ogs_fsm_t       sm;             /* State Machine */

    uint8_t         ebi;            /* 5GS Bearer ID */

    uint32_t        gnb_s1u_teid;
    ogs_ip_t        gnb_s1u_ip;
    uint32_t        smf_s1u_teid;
    ogs_ip_t        smf_s1u_ip;

    uint32_t        target_s1u_teid;    /* Target S1U TEID from HO-Req-Ack */
    ogs_ip_t        target_s1u_ip;      /* Target S1U ADDR from HO-Req-Ack */

    uint32_t        gnb_dl_teid;
    ogs_ip_t        gnb_dl_ip;
    uint32_t        gnb_ul_teid;
    ogs_ip_t        gnb_ul_ip;

    uint32_t        smf_dl_teid;
    ogs_ip_t        smf_dl_ip;
    uint32_t        smf_ul_teid;
    ogs_ip_t        smf_ul_ip;

    ogs_qos_t       qos;
    ogs_tlv_octet_t tft;   /* Saved TFT */

#define CLEAR_BEARER_ALL_TIMERS(__bEARER) \
    do { \
        CLEAR_BEARER_TIMER((__bEARER)->t3589); \
    } while(0);
#define CLEAR_BEARER_TIMER(__bEARER_TIMER) \
    do { \
        ogs_timer_stop((__bEARER_TIMER).timer); \
        if ((__bEARER_TIMER).pkbuf) \
        { \
            ogs_pkbuf_free((__bEARER_TIMER).pkbuf); \
            (__bEARER_TIMER).pkbuf = NULL; \
        } \
        (__bEARER_TIMER).retry_count = 0; \
    } while(0);
    struct {
        ogs_pkbuf_t     *pkbuf;
        ogs_timer_t     *timer;
        uint32_t        retry_count;;
    } t3589;

    /* Related Context */
    amf_ue_t        *amf_ue;
    amf_sess_t      *sess;
#if 0
    ogs_gtp_xact_t  *xact;
#endif
} amf_bearer_t;

void amf_context_init(void);
void amf_context_final(void);
amf_context_t *amf_self(void);

int amf_context_parse_config(void);

amf_gnb_t *amf_gnb_add(ogs_sock_t *sock, ogs_sockaddr_t *addr);
int amf_gnb_remove(amf_gnb_t *gnb);
int amf_gnb_remove_all(void);
amf_gnb_t *amf_gnb_find_by_addr(ogs_sockaddr_t *addr);
amf_gnb_t *amf_gnb_find_by_gnb_id(uint32_t gnb_id);
int amf_gnb_set_gnb_id(amf_gnb_t *gnb, uint32_t gnb_id);
int amf_gnb_sock_type(ogs_sock_t *sock);

ran_ue_t *ran_ue_add(amf_gnb_t *gnb, uint32_t ran_ue_ngap_id);
unsigned int ran_ue_count(void);
void ran_ue_remove(ran_ue_t *ran_ue);
void ran_ue_remove_in_gnb(amf_gnb_t *gnb);
void ran_ue_switch_to_gnb(ran_ue_t *ran_ue, amf_gnb_t *new_gnb);
ran_ue_t *ran_ue_find_by_ran_ue_ngap_id(
        amf_gnb_t *gnb, uint32_t ran_ue_ngap_id);
ran_ue_t *ran_ue_find_by_amf_ue_ngap_id(uint64_t amf_ue_ngap_id);
ran_ue_t *ran_ue_first_in_gnb(amf_gnb_t *gnb);
ran_ue_t *ran_ue_next_in_gnb(ran_ue_t *ran_ue);

amf_ue_t *amf_ue_add(ran_ue_t *ran_ue);
void amf_ue_remove(amf_ue_t *amf_ue);
void amf_ue_remove_all(void);

amf_ue_t *amf_ue_find_by_imsi(uint8_t *imsi, int imsi_len);
amf_ue_t *amf_ue_find_by_imsi_bcd(char *imsi_bcd);
amf_ue_t *amf_ue_find_by_guti(ogs_nas_5gs_guti_t *nas_guti);
amf_ue_t *amf_ue_find_by_teid(uint32_t teid);
amf_ue_t *amf_ue_find_by_suci(char *suci);
amf_ue_t *amf_ue_find_by_supi(char *supi);

amf_ue_t *amf_ue_find_by_message(ogs_nas_5gs_message_t *message);
void amf_ue_set_suci(amf_ue_t *amf_ue,
        ogs_nas_5gs_mobile_identity_t *mobile_identity);
void amf_ue_set_supi(amf_ue_t *amf_ue, char *supi);

int amf_ue_have_indirect_tunnel(amf_ue_t *amf_ue);
int amf_ue_clear_indirect_tunnel(amf_ue_t *amf_ue);

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

amf_sess_t *amf_sess_add(amf_ue_t *amf_ue, uint8_t pti);
void amf_sess_remove(amf_sess_t *sess);
void amf_sess_remove_all(amf_ue_t *amf_ue);
amf_sess_t *amf_sess_find_by_pti(amf_ue_t *amf_ue, uint8_t pti);
amf_sess_t *amf_sess_find_by_ebi(amf_ue_t *amf_ue, uint8_t ebi);
amf_sess_t *amf_sess_find_by_dnn(amf_ue_t *amf_ue, char *dnn);
amf_sess_t *amf_sess_first(amf_ue_t *amf_ue);
amf_sess_t *amf_sess_next(amf_sess_t *sess);
unsigned int amf_sess_count(amf_ue_t *amf_ue);

amf_bearer_t *amf_bearer_add(amf_sess_t *sess);
void amf_bearer_remove(amf_bearer_t *bearer);
void amf_bearer_remove_all(amf_sess_t *sess);
amf_bearer_t *amf_bearer_find_by_sess_ebi(amf_sess_t *sess, uint8_t ebi);
amf_bearer_t *amf_bearer_find_by_ue_ebi(amf_ue_t *amf_ue, uint8_t ebi);
amf_bearer_t *amf_bearer_find_or_add_by_message(
                    amf_ue_t *amf_ue, ogs_nas_5gs_message_t *message);
amf_bearer_t *amf_default_bearer_in_sess(amf_sess_t *sess);
amf_bearer_t *amf_linked_bearer(amf_bearer_t *bearer);
amf_bearer_t *amf_bearer_first(amf_sess_t *sess);
amf_bearer_t *amf_bearer_next(amf_bearer_t *bearer);

int amf_bearer_is_inactive(amf_ue_t *amf_ue);
int amf_bearer_set_inactive(amf_ue_t *amf_ue);

void amf_pdn_remove_all(amf_ue_t *amf_ue);
ogs_pdn_t *amf_pdn_find_by_dnn(amf_ue_t *amf_ue, char *dnn);
ogs_pdn_t *amf_default_pdn(amf_ue_t *amf_ue);

int amf_find_served_tai(ogs_5gs_tai_t *tai);

int amf_m_tmsi_pool_generate(void);
amf_m_tmsi_t *amf_m_tmsi_alloc(void);
int amf_m_tmsi_free(amf_m_tmsi_t *tmsi);

uint8_t amf_selected_int_algorithm(amf_ue_t *amf_ue);
uint8_t amf_selected_enc_algorithm(amf_ue_t *amf_ue);

#ifdef __cplusplus
}
#endif

#endif /* AMF_CONTEXT_H */

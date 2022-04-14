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

#ifndef MME_CONTEXT_H
#define MME_CONTEXT_H

#include "ogs-crypt.h"

#include "ogs-s1ap.h"
#include "ogs-diameter-s6a.h"
#include "ogs-gtp.h"
#include "ogs-nas-eps.h"
#include "ogs-app.h"
#include "ogs-sctp.h"

/* S1AP */
#include "S1AP_Cause.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GRP_PER_MME                 256    /* According to spec it is 65535 */
#define CODE_PER_MME                256    /* According to spec it is 256 */

#define MAX_NUM_OF_SERVED_GUMMEI    8

extern int __mme_log_domain;
extern int __emm_log_domain;
extern int __esm_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __mme_log_domain

typedef struct mme_sgw_s mme_sgw_t;
typedef struct mme_pgw_s mme_pgw_t;
typedef struct mme_vlr_s mme_vlr_t;
typedef struct mme_csmap_s mme_csmap_t;

typedef struct enb_ue_s enb_ue_t;
typedef struct mme_ue_s mme_ue_t;
typedef struct mme_sess_s mme_sess_t;
typedef struct mme_bearer_s mme_bearer_t;

typedef struct ogs_diam_config_s ogs_diam_config_t;

typedef uint32_t mme_m_tmsi_t;
typedef uint32_t mme_p_tmsi_t;

typedef struct served_gummei_s {
    uint32_t        num_of_plmn_id;
    ogs_plmn_id_t   plmn_id[OGS_MAX_NUM_OF_PLMN];

    uint32_t        num_of_mme_gid;
    uint16_t        mme_gid[GRP_PER_MME];
    uint32_t        num_of_mme_code;
    uint8_t         mme_code[CODE_PER_MME];
} served_gummei_t;

typedef struct mme_context_s {
    const char          *diam_conf_path;  /* MME Diameter conf path */
    ogs_diam_config_t   *diam_config;     /* MME Diameter config */

    uint16_t        s1ap_port;      /* Default S1AP Port */
    uint16_t        sgsap_port;     /* Default SGsAP Port */

    ogs_list_t      s1ap_list;      /* MME S1AP IPv4 Server List */
    ogs_list_t      s1ap_list6;     /* MME S1AP IPv6 Server List */

    ogs_list_t      sgw_list;       /* SGW GTPC Client List */
    mme_sgw_t       *sgw;           /* Iterator for SGW round-robin */

    ogs_list_t      pgw_list;       /* PGW GTPC Client List */
    ogs_sockaddr_t  *pgw_addr;      /* First IPv4 Address Selected */
    ogs_sockaddr_t  *pgw_addr6;     /* First IPv6 Address Selected */

    ogs_list_t      enb_list;       /* ENB S1AP Client List */

    ogs_list_t      vlr_list;       /* VLR SGsAP Client List */
    ogs_list_t      csmap_list;     /* TAI-LAI Map List */

    /* Served GUMME */
    uint8_t         max_num_of_served_gummei;
    served_gummei_t served_gummei[MAX_NUM_OF_SERVED_GUMMEI];

    /* Served TAI */
    uint8_t         num_of_served_tai;
    struct {
        ogs_eps_tai0_list_t list0;
        ogs_eps_tai2_list_t list2;
    } served_tai[OGS_MAX_NUM_OF_SERVED_TAI];

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
    ogs_nas_network_name_t short_name; /* Network short name */
    ogs_nas_network_name_t full_name; /* Network Full Name */

    /* MME Name */
    const char *mme_name;

    /* S1SetupResponse */
    uint8_t         relative_capacity;

    /* Generator for unique identification */
    uint32_t        mme_ue_s1ap_id;         /* mme_ue_s1ap_id generator */

    /* M-TMSI Pool */
    OGS_POOL(m_tmsi, mme_m_tmsi_t);

    ogs_list_t      mme_ue_list;

    ogs_hash_t      *enb_addr_hash;         /* hash table for ENB Address */
    ogs_hash_t      *enb_id_hash;           /* hash table for ENB-ID */
    ogs_hash_t      *imsi_ue_hash;          /* hash table (IMSI : MME_UE) */
    ogs_hash_t      *guti_ue_hash;          /* hash table (GUTI : MME_UE) */

} mme_context_t;

typedef struct mme_sgw_s {
    ogs_gtp_node_t  gnode;

    uint16_t        tac[OGS_MAX_NUM_OF_TAI];
    uint8_t         num_of_tac;
    uint32_t        e_cell_id[OGS_MAX_NUM_OF_CELL_ID];
    uint8_t         num_of_e_cell_id;
} mme_sgw_t;

typedef struct mme_pgw_s {
    ogs_lnode_t     lnode;

    ogs_sockaddr_t  *sa_list;
    const char      *apn;
} mme_pgw_t;

#define MME_SGSAP_IS_CONNECTED(__mME) \
    ((__mME) && ((__mME)->csmap) && ((__mME)->csmap->vlr) && \
     (OGS_FSM_CHECK(&(__mME)->csmap->vlr->sm, sgsap_state_connected)))
#define MME_P_TMSI_IS_AVAILABLE(__mME) \
    (MME_SGSAP_IS_CONNECTED(__mME) && (__mME)->p_tmsi)

typedef struct mme_vlr_s {
    ogs_lnode_t     lnode;

    ogs_fsm_t       sm;          /* A state machine */

    ogs_timer_t     *t_conn;     /* client timer to connect to server */

    uint16_t        max_num_of_ostreams;/* SCTP Max num of outbound streams */
    uint16_t        ostream_id;     /* vlr_ostream_id generator */

    ogs_sockaddr_t  *sa_list;   /* VLR SGsAP Socket Address List */

    ogs_sock_t      *sock;      /* VLR SGsAP Socket */
    ogs_sockaddr_t  *addr;      /* VLR SGsAP Connected Socket Address */
    ogs_sockopt_t   *option;    /* VLR SGsAP Socket Option */
    ogs_poll_t      *poll;      /* VLR SGsAP Poll */
} mme_vlr_t;

typedef struct mme_csmap_s {
    ogs_lnode_t     lnode;

    ogs_nas_eps_tai_t tai;
    ogs_nas_lai_t   lai;

    mme_vlr_t       *vlr;
} mme_csmap_t;

typedef struct mme_enb_s {
    ogs_lnode_t     lnode;

    ogs_fsm_t       sm;         /* A state machine */

    uint32_t        enb_id;     /* eNB_ID received from eNB */
    ogs_sctp_sock_t sctp;       /* SCTP socket */

    struct {
        bool s1_setup_success;  /* eNB S1AP Setup complete successfuly */
    } state;

    uint16_t        max_num_of_ostreams;/* SCTP Max num of outbound streams */
    uint16_t        ostream_id;         /* enb_ostream_id generator */

    uint8_t         num_of_supported_ta_list;
    ogs_eps_tai_t   supported_ta_list[OGS_MAX_NUM_OF_TAI*OGS_MAX_NUM_OF_BPLMN];

    ogs_pkbuf_t     *s1_reset_ack; /* Reset message */

    ogs_list_t      enb_ue_list;

} mme_enb_t;

struct enb_ue_s {
    ogs_lnode_t     lnode;
    uint32_t        index;

    /* UE identity */
#define INVALID_UE_S1AP_ID      0xffffffff /* Initial value of enb_ue_s1ap_id */
    uint32_t        enb_ue_s1ap_id; /* eNB-UE-S1AP-ID received from eNB */
    uint32_t        mme_ue_s1ap_id; /* MME-UE-S1AP-ID received from MME */

    uint16_t        enb_ostream_id; /* SCTP output stream id for eNB */

    /* Handover Info */
    S1AP_HandoverType_t handover_type;
    enb_ue_t        *source_ue;
    enb_ue_t        *target_ue;

    /* Use mme_ue->tai, mme_ue->e_cgi.
     * Do not access enb_ue->saved.tai enb_ue->saved.e_cgi.
     * 
     * Save TAI and ECGI. And then, this will copy 'mme_ue_t' context later */
    struct {
        ogs_eps_tai_t   tai;
        ogs_e_cgi_t     e_cgi;
    } saved;

    /* S1 Holding timer for removing this context */
    ogs_timer_t     *t_s1_holding;

    /* Store by UE Context Release Command
     * Retrieve by UE Context Release Complete */
#define S1AP_UE_CTX_REL_INVALID_ACTION                      0
#define S1AP_UE_CTX_REL_S1_CONTEXT_REMOVE                   1
#define S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK                2
#define S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE                   3
#define S1AP_UE_CTX_REL_S1_HANDOVER_COMPLETE                4
#define S1AP_UE_CTX_REL_S1_HANDOVER_CANCEL                  5
#define S1AP_UE_CTX_REL_S1_HANDOVER_FAILURE                 6
#define S1AP_UE_CTX_REL_S1_PAGING                           7
    uint8_t         ue_ctx_rel_action;

    bool            part_of_s1_reset_requested;

    /* Related Context */
    mme_enb_t       *enb;
    mme_ue_t        *mme_ue;
}; 

struct mme_ue_s {
    ogs_lnode_t     lnode;
    ogs_fsm_t       sm;     /* A state machine */

    struct {
#define MME_EPS_TYPE_ATTACH_REQUEST                 1
#define MME_EPS_TYPE_TAU_REQUEST                    2
#define MME_EPS_TYPE_SERVICE_REQUEST                3
#define MME_EPS_TYPE_EXTENDED_SERVICE_REQUEST       4
#define MME_EPS_TYPE_DETACH_REQUEST_FROM_UE         5 
#define MME_EPS_TYPE_DETACH_REQUEST_TO_UE           6 
        uint8_t     type;
        uint8_t     ksi;
        union {
            ogs_nas_eps_attach_type_t attach;
            ogs_nas_eps_update_type_t update;
            ogs_nas_service_type_t service;
            ogs_nas_detach_type_t detach;
            uint8_t data;
        };
    } nas_eps;

    /* UE identity */
#define MME_UE_HAVE_IMSI(__mME) \
    ((__mME) && ((__mME)->imsi_len))
    uint8_t         imsi[OGS_MAX_IMSI_LEN];
    int             imsi_len;
    char            imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];
    ogs_nas_mobile_identity_imsi_t nas_mobile_identity_imsi;

    uint8_t         imeisv[OGS_MAX_IMEISV_LEN];
    int             imeisv_len;
    uint8_t         masked_imeisv[OGS_MAX_IMEISV_LEN];
    int             masked_imeisv_len;
    char            imeisv_bcd[OGS_MAX_IMEISV_BCD_LEN+1];
    ogs_nas_mobile_identity_imeisv_t nas_mobile_identity_imeisv;

    uint8_t         msisdn[OGS_MAX_MSISDN_LEN];
    int             msisdn_len;
    char            msisdn_bcd[OGS_MAX_MSISDN_BCD_LEN+1];

    uint8_t         a_msisdn[OGS_MAX_MSISDN_LEN];
    int             a_msisdn_len;
    char            a_msisdn_bcd[OGS_MAX_MSISDN_BCD_LEN+1];

    mme_p_tmsi_t    p_tmsi;

    struct {
        mme_m_tmsi_t *m_tmsi;
        ogs_nas_eps_guti_t guti;
    } current, next;

    uint32_t        mme_s11_teid;   /* MME-S11-TEID is derived from INDEX */
    uint32_t        sgw_s11_teid;   /* SGW-S11-TEID is received from SGW */

    uint16_t        vlr_ostream_id; /* SCTP output stream id for VLR */

    /* UE Info */
    uint16_t        enb_ostream_id;
    ogs_eps_tai_t   tai;
    ogs_e_cgi_t     e_cgi;
    ogs_plmn_id_t   last_visited_plmn_id;

#define SECURITY_CONTEXT_IS_VALID(__mME) \
    ((__mME) && \
    ((__mME)->security_context_available == 1) && \
     ((__mME)->mac_failed == 0) && \
     ((__mME)->nas_eps.ksi != OGS_NAS_KSI_NO_KEY_IS_AVAILABLE))
#define CLEAR_SECURITY_CONTEXT(__mME) \
    do { \
        ogs_assert((__mME)); \
        (__mME)->security_context_available = 0; \
        (__mME)->mac_failed = 0; \
        (__mME)->nas_eps.ksi = 0; \
    } while(0)
    int             security_context_available;
    int             mac_failed;

    /* Security Context */
    ogs_nas_ue_network_capability_t ue_network_capability;
    ogs_nas_ms_network_capability_t ms_network_capability;
    ogs_nas_ue_additional_security_capability_t
        ue_additional_security_capability;
    uint8_t         xres[OGS_MAX_RES_LEN];
    uint8_t         xres_len;
    uint8_t         kasme[OGS_SHA256_DIGEST_SIZE];
    uint8_t         rand[OGS_RAND_LEN];
    uint8_t         autn[OGS_AUTN_LEN];
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
    uint8_t         kenb[OGS_SHA256_DIGEST_SIZE];
    uint8_t         hash_mme[OGS_HASH_MME_LEN];

    struct {
    ED2(uint8_t nhcc_spare:5;,
        uint8_t nhcc:3;) /* Next Hop Channing Counter */
    };
    uint8_t         nh[OGS_SHA256_DIGEST_SIZE]; /* NH Security Key */

    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EEA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EEA2    2
     * #define NAS_SECURITY_ALGORITHMS_128_EEA3    3 */
    uint8_t         selected_enc_algorithm;
    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    2
     * #define NAS_SECURITY_ALGORITHMS_128_EIA3    3 */
    uint8_t         selected_int_algorithm;

    /* HSS Info */
    ogs_bitrate_t   ambr; /* UE-AMBR */
    uint32_t        network_access_mode; /* Permitted EPS Attach Type */

    uint32_t        context_identifier; /* default APN */

    int num_of_session;
    ogs_session_t session[OGS_MAX_NUM_OF_SESS];

    /* ESM Info */
    ogs_list_t      sess_list;

#define MIN_EPS_BEARER_ID           5
#define MAX_EPS_BEARER_ID           15

#define CLEAR_EPS_BEARER_ID(__mME) \
    do { \
        ogs_assert((__mME)); \
        mme_ebi_pool_clear(__mME); \
    } while(0)
    OGS_POOL(ebi_pool, uint8_t);

    /* Paging Info */
#define ECM_CONNECTED(__mME) \
    ((__mME) && ((__mME)->enb_ue != NULL) && enb_ue_cycle((__mME)->enb_ue))
#define ECM_IDLE(__mME) \
    ((__mME) && \
     (((__mME)->enb_ue == NULL) || (enb_ue_cycle((__mME)->enb_ue) == NULL)))
    enb_ue_t        *enb_ue;    /* S1 UE context */

    /* Save PDN Connectivity Request */
    ogs_nas_esm_message_container_t pdn_connectivity_request;

#define CLEAR_MME_UE_ALL_TIMERS(__mME) \
    do { \
        mme_sess_t *sess = NULL; \
        mme_bearer_t *bearer = NULL; \
        \
        CLEAR_MME_UE_TIMER((__mME)->t3413); \
        CLEAR_MME_UE_TIMER((__mME)->t3422); \
        CLEAR_MME_UE_TIMER((__mME)->t3450); \
        CLEAR_MME_UE_TIMER((__mME)->t3460); \
        CLEAR_MME_UE_TIMER((__mME)->t3470); \
        \
        ogs_list_for_each(&mme_ue->sess_list, sess) { \
            ogs_list_for_each(&sess->bearer_list, bearer) { \
                CLEAR_BEARER_ALL_TIMERS(bearer); \
            } \
        } \
    } while(0);
#define CLEAR_MME_UE_TIMER(__mME_UE_TIMER) \
    do { \
        ogs_timer_stop((__mME_UE_TIMER).timer); \
        if ((__mME_UE_TIMER).pkbuf) { \
            ogs_pkbuf_free((__mME_UE_TIMER).pkbuf); \
            (__mME_UE_TIMER).pkbuf = NULL; \
        } \
        (__mME_UE_TIMER).retry_count = 0; \
    } while(0);
    struct {
        ogs_pkbuf_t     *pkbuf;
        ogs_timer_t     *timer;
        uint32_t        retry_count;;
    } t3413, t3422, t3450, t3460, t3470;

#define CLEAR_SERVICE_INDICATOR(__mME) \
    do { \
        ogs_assert((__mME)); \
        (__mME)->service_indicator = 0; \
    } while(0);

#define CS_CALL_SERVICE_INDICATOR(__mME) \
    (MME_P_TMSI_IS_AVAILABLE(__mME) && \
     ((__mME)->service_indicator) == SGSAP_CS_CALL_SERVICE_INDICATOR)
#define SMS_SERVICE_INDICATOR(__mME) \
    (MME_P_TMSI_IS_AVAILABLE(__mME) && \
     ((__mME)->service_indicator) == SGSAP_SMS_SERVICE_INDICATOR)
    uint8_t         service_indicator;

    /* UE Radio Capability */
    OCTET_STRING_t  ueRadioCapability;

    /* S1AP Transparent Container */
    OCTET_STRING_t  container;

    /* GTP Request/Response Counter */
#define GTP_COUNTER_CLEAR(__mME, __tYPE) \
        do { \
            ogs_assert((__mME)); \
            ((__mME)->gtp_counter[__tYPE].request) = 0; \
            ((__mME)->gtp_counter[__tYPE].response) = 0; \
        } while(0);
#define GTP_COUNTER_INCREMENT(__mME, __tYPE) \
        do { \
            ogs_assert((__mME)); \
            ((__mME)->gtp_counter[__tYPE].request)++; \
        } while(0);

#define GTP_COUNTER_CHECK(__mME, __tYPE, __eXPR) \
        do { \
            ogs_assert((__mME)); \
            if ((__mME)->gtp_counter[__tYPE].request == 0) break; \
            ((__mME)->gtp_counter[__tYPE].response)++; \
            if (((__mME)->gtp_counter[__tYPE].request) == \
                ((__mME)->gtp_counter[__tYPE].response)) \
            { \
                GTP_COUNTER_CLEAR(__mME, __tYPE) \
                __eXPR \
            } \
        } while(0);

#define MAX_NUM_OF_GTP_COUNTER                                  16

#define GTP_COUNTER_MODIFY_BEARER_BY_PATH_SWITCH                1
#define GTP_COUNTER_MODIFY_BEARER_BY_E_RAB_MODIFICATION         2
    struct {
        uint8_t request;
        uint8_t response;
    } gtp_counter[MAX_NUM_OF_GTP_COUNTER];

    /*
     * If the MME sends Delete-Session-Request to the SGW for all sessions,
     *    session_context_will_deleted = 1
     * When the MME receives a Delete-Session-Response for the last session,
     *    session_context_will_deleted = 0
     */
    int             session_context_will_deleted;

    union {
        mme_sgw_t       *sgw;
        ogs_gtp_node_t  *gnode;
    };
    mme_csmap_t     *csmap;
};

#define SESSION_CONTEXT_IS_AVAILABLE(__mME) \
     ((__mME) && ((__mME)->sgw_s11_teid))

#define SESSION_CONTEXT_WILL_DELETED(__mME) \
     ((__mME) && ((__mME)->session_context_will_deleted))

#define CLEAR_SESSION_CONTEXT(__mME) \
    do { \
        ogs_assert((__mME)); \
        (__mME)->sgw_s11_teid = 0; \
        (__mME)->session_context_will_deleted = 0; \
    } while(0)

#define ACTIVE_EPS_BEARERS_IS_AVAIABLE(__mME) \
    (mme_ue_have_active_eps_bearers(__mME))
typedef struct mme_sess_s {
    ogs_lnode_t     lnode;

    uint8_t         pti;        /* Procedure Trasaction Identity */

    /* PDN Connectivity Request */
    ogs_nas_request_type_t request_type; 

    /* mme_bearer_first(sess) : Default Bearer Context */
    ogs_list_t      bearer_list;

    /* Related Context */
    mme_ue_t *mme_ue;

    ogs_session_t *session;

    /* Save Protocol Configuration Options from UE */
    struct {
        uint8_t length;
        uint8_t *buffer;
    } ue_pco; 

    /* Save Protocol Configuration Options from PGW */
    ogs_tlv_octet_t pgw_pco;
} mme_sess_t;

#define MME_HAVE_ENB_S1U_PATH(__bEARER) \
    ((__bEARER) && ((__bEARER)->enb_s1u_teid))
#define CLEAR_ENB_S1U_PATH(__bEARER) \
    do { \
        ogs_assert((__bEARER)); \
        (__bEARER)->enb_s1u_teid = 0; \
    } while(0)

#define MME_HAVE_SGW_S1U_PATH(__sESS) \
    ((__sESS) && (mme_bearer_first(__sESS)) && \
     ((mme_default_bearer_in_sess(__sESS)->sgw_s1u_teid)))
#define CLEAR_SGW_S1U_PATH(__sESS) \
    do { \
        mme_bearer_t *__bEARER = NULL; \
        ogs_assert((__sESS)); \
        __bEARER = mme_default_bearer_in_sess(__sESS); \
        __bEARER->sgw_s1u_teid = 0; \
    } while(0)

#define MME_HAVE_ENB_DL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->enb_dl_teid))
#define MME_HAVE_ENB_UL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->enb_ul_teid))
#define MME_HAVE_SGW_DL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->sgw_dl_teid))
#define MME_HAVE_SGW_UL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->sgw_ul_teid))
#define CLEAR_INDIRECT_TUNNEL(__bEARER) \
    do { \
        ogs_assert((__bEARER)); \
        (__bEARER)->enb_dl_teid = 0; \
        (__bEARER)->enb_ul_teid = 0; \
        (__bEARER)->sgw_dl_teid = 0; \
        (__bEARER)->sgw_ul_teid = 0; \
    } while(0)
typedef struct mme_bearer_s {
    ogs_lnode_t     lnode;
    uint32_t        index;
    ogs_fsm_t       sm;             /* State Machine */

    uint8_t         *ebi_node;      /* Pool-Node for EPS Bearer ID */
    uint8_t         ebi;            /* EPS Bearer ID */

    uint32_t        enb_s1u_teid;
    ogs_ip_t        enb_s1u_ip;
    uint32_t        sgw_s1u_teid;
    ogs_ip_t        sgw_s1u_ip;

    uint32_t        target_s1u_teid;    /* Target S1U TEID from HO-Req-Ack */
    ogs_ip_t        target_s1u_ip;      /* Target S1U ADDR from HO-Req-Ack */

    uint32_t        enb_dl_teid;
    ogs_ip_t        enb_dl_ip;
    uint32_t        enb_ul_teid;
    ogs_ip_t        enb_ul_ip;

    uint32_t        sgw_dl_teid;
    ogs_ip_t        sgw_dl_ip;
    uint32_t        sgw_ul_teid;
    ogs_ip_t        sgw_ul_ip;

    ogs_qos_t       qos;
    ogs_tlv_octet_t tft;   /* Saved TFT */

#define CLEAR_BEARER_ALL_TIMERS(__bEARER) \
    do { \
        CLEAR_BEARER_TIMER((__bEARER)->t3489); \
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
    } t3489;

    /* Related Context */
    mme_ue_t        *mme_ue;
    mme_sess_t      *sess;
    struct {
        ogs_gtp_xact_t  *xact;
    } create, update, delete, notify, current;
} mme_bearer_t;

void mme_context_init(void);
void mme_context_final(void);
mme_context_t *mme_self(void);

int mme_context_parse_config(void);

mme_sgw_t *mme_sgw_add(ogs_sockaddr_t *addr);
void mme_sgw_remove(mme_sgw_t *sgw);
void mme_sgw_remove_all(void);
mme_sgw_t *mme_sgw_find_by_addr(ogs_sockaddr_t *addr);

mme_pgw_t *mme_pgw_add(ogs_sockaddr_t *addr);
void mme_pgw_remove(mme_pgw_t *pgw);
void mme_pgw_remove_all(void);
ogs_sockaddr_t *mme_pgw_addr_find_by_apn(
        ogs_list_t *list, int family, char *apn);

mme_vlr_t *mme_vlr_add(ogs_sockaddr_t *addr, ogs_sockopt_t *option);
void mme_vlr_remove(mme_vlr_t *vlr);
void mme_vlr_remove_all(void);
void mme_vlr_close(mme_vlr_t *vlr);
mme_vlr_t *mme_vlr_find_by_addr(ogs_sockaddr_t *addr);

mme_csmap_t *mme_csmap_add(mme_vlr_t *vlr);
void mme_csmap_remove(mme_csmap_t *csmap);
void mme_csmap_remove_all(void);

mme_csmap_t *mme_csmap_find_by_tai(ogs_eps_tai_t *tai);
mme_csmap_t *mme_csmap_find_by_nas_lai(ogs_nas_lai_t *lai);

mme_enb_t *mme_enb_add(ogs_sock_t *sock, ogs_sockaddr_t *addr);
int mme_enb_remove(mme_enb_t *enb);
int mme_enb_remove_all(void);
mme_enb_t *mme_enb_find_by_addr(ogs_sockaddr_t *addr);
mme_enb_t *mme_enb_find_by_enb_id(uint32_t enb_id);
int mme_enb_set_enb_id(mme_enb_t *enb, uint32_t enb_id);
int mme_enb_sock_type(ogs_sock_t *sock);

enb_ue_t *enb_ue_add(mme_enb_t *enb, uint32_t enb_ue_s1ap_id);
void enb_ue_remove(enb_ue_t *enb_ue);
void enb_ue_switch_to_enb(enb_ue_t *enb_ue, mme_enb_t *new_enb);
enb_ue_t *enb_ue_find_by_enb_ue_s1ap_id(
        mme_enb_t *enb, uint32_t enb_ue_s1ap_id);
enb_ue_t *enb_ue_find(uint32_t index);
enb_ue_t *enb_ue_find_by_mme_ue_s1ap_id(uint32_t mme_ue_s1ap_id);
enb_ue_t *enb_ue_cycle(enb_ue_t *enb_ue);

void mme_ue_new_guti(mme_ue_t *mme_ue);
void mme_ue_confirm_guti(mme_ue_t *mme_ue);

mme_ue_t *mme_ue_add(enb_ue_t *enb_ue);
void mme_ue_hash_remove(mme_ue_t *mme_ue);
void mme_ue_remove(mme_ue_t *mme_ue);
void mme_ue_remove_all(void);

void mme_ue_fsm_init(mme_ue_t *mme_ue);
void mme_ue_fsm_fini(mme_ue_t *mme_ue);

mme_ue_t *mme_ue_find_by_imsi(uint8_t *imsi, int imsi_len);
mme_ue_t *mme_ue_find_by_imsi_bcd(char *imsi_bcd);
mme_ue_t *mme_ue_find_by_guti(ogs_nas_eps_guti_t *nas_guti);
mme_ue_t *mme_ue_find_by_teid(uint32_t teid);

mme_ue_t *mme_ue_find_by_message(ogs_nas_eps_message_t *message);
int mme_ue_set_imsi(mme_ue_t *mme_ue, char *imsi_bcd);

bool mme_ue_have_indirect_tunnel(mme_ue_t *mme_ue);
void mme_ue_clear_indirect_tunnel(mme_ue_t *mme_ue);

bool mme_ue_have_active_eps_bearers(mme_ue_t *mme_ue);

/* 
 * o RECV Initial UE-Message : S-TMSI
 * o RECV Attach Request : IMSI, GUTI
 * o RECV TAU Request : GUTI
 * ### MME_UE_ASSOCIATE_ENB_UE() ###
 * ### MME_UE_ECM_CONNECTED() ###
 *
 * o RECV Initial Context Setup Failure in EMM Registered State
 * ### MME_UE_DEASSOCIATE_ENB_UE() ###
 * ### ENB_UE_REMOVE() ###
 * ### MME_UE_DEASSOCIATE() ###
 *
 * o SEND UE Context Release Command with NO_ACTION
 *   - RECV UE Context Release Complete
 * ### ENB_UE_REMOVE() ###
 * ### MME_UE_DEASSOCIATE() ###
 *
 * o SEND UE Context Release Command with REMOVE_MME_UE_CONTEXT
 *   - RECV UE Context Release Complete
 * ### ENB_UE_REMOVE() ###
 * ### MME_UE_REMOVE() ###
 *
 *
 * o RECV Handover Required
 * ### SOURCE_UE_ASSOCIATE_TARGET_UE() ####
 *   - SEND Handover Request
 *
 * o RECV Handover Notify
 * ### MME_UE_ASSOCIATE_ENB_UE(TARGET) ###
 * ### MME_UE_ECM_CONNECTED(TARGET) ###
 *   - Modify Bearer Request/Response
 *   - UE Context Release Command/Complete
 * ### SOURCE_UE_DEASSOCIATE_TARGET_UE() ####
 * ### ENB_UE_REMOVE() ####
 *   - Delete Indirect Data Forwarding Tunnel Request/Response
 *
 * o RECV Handover Cancel
 *   - UE Context Release Command/Complete
 * ### SOURCE_UE_DEASSOCIATE_TARGET_UE() ####
 * ### ENB_UE_REMOVE() ####
 *   - Delete Indirect Data Forwarding Tunnel Request/Response
 *
 * o RECV Handover Failure
 *   - UE Context Release Command/Complete
 * ### SOURCE_UE_DEASSOCIATE_TARGET_UE() ####
 * ### ENB_UE_REMOVE() ####
 *   - Delete Indirect Data Forwarding Tunnel Request/Response
 */
void mme_ue_associate_enb_ue(mme_ue_t *mme_ue, enb_ue_t *enb_ue);
void enb_ue_deassociate(enb_ue_t *enb_ue);
void mme_ue_deassociate(mme_ue_t *mme_ue);
void source_ue_associate_target_ue(enb_ue_t *source_ue, enb_ue_t *target_ue);
void source_ue_deassociate_target_ue(enb_ue_t *enb_ue);

mme_sess_t *mme_sess_add(mme_ue_t *mme_ue, uint8_t pti);
void mme_sess_remove(mme_sess_t *sess);
void mme_sess_remove_all(mme_ue_t *mme_ue);
mme_sess_t *mme_sess_find_by_pti(mme_ue_t *mme_ue, uint8_t pti);
mme_sess_t *mme_sess_find_by_ebi(mme_ue_t *mme_ue, uint8_t ebi);
mme_sess_t *mme_sess_find_by_apn(mme_ue_t *mme_ue, char *apn);

mme_sess_t *mme_sess_first(mme_ue_t *mme_ue);
mme_sess_t *mme_sess_next(mme_sess_t *sess);
unsigned int mme_sess_count(mme_ue_t *mme_ue);

mme_bearer_t *mme_bearer_add(mme_sess_t *sess);
void mme_bearer_remove(mme_bearer_t *bearer);
void mme_bearer_remove_all(mme_sess_t *sess);
mme_bearer_t *mme_bearer_find_by_sess_ebi(mme_sess_t *sess, uint8_t ebi);
mme_bearer_t *mme_bearer_find_by_ue_ebi(mme_ue_t *mme_ue, uint8_t ebi);
mme_bearer_t *mme_bearer_find_or_add_by_message(
        mme_ue_t *mme_ue, ogs_nas_eps_message_t *message, int create_action);
mme_bearer_t *mme_default_bearer_in_sess(mme_sess_t *sess);
mme_bearer_t *mme_linked_bearer(mme_bearer_t *bearer);
mme_bearer_t *mme_bearer_first(mme_sess_t *sess);
mme_bearer_t *mme_bearer_next(mme_bearer_t *bearer);
mme_bearer_t *mme_bearer_cycle(mme_bearer_t *bearer);

void mme_session_remove_all(mme_ue_t *mme_ue);
ogs_session_t *mme_session_find_by_apn(mme_ue_t *mme_ue, char *apn);
ogs_session_t *mme_default_session(mme_ue_t *mme_ue);

int mme_find_served_tai(ogs_eps_tai_t *tai);

int mme_m_tmsi_pool_generate(void);
mme_m_tmsi_t *mme_m_tmsi_alloc(void);
int mme_m_tmsi_free(mme_m_tmsi_t *tmsi);

void mme_ebi_pool_init(mme_ue_t *mme_ue);
void mme_ebi_pool_final(mme_ue_t *mme_ue);
void mme_ebi_pool_clear(mme_ue_t *mme_ue);

uint8_t mme_selected_int_algorithm(mme_ue_t *mme_ue);
uint8_t mme_selected_enc_algorithm(mme_ue_t *mme_ue);

mme_sgw_t *mme_changed_sgw_node(mme_sgw_t *current, enb_ue_t *enb_ue);

#ifdef __cplusplus
}
#endif

#endif /* MME_CONTEXT_H */

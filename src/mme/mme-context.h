/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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
#include "metrics.h"

/* S1AP */
#include "S1AP_Cause.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GRP_PER_MME                 256    /* According to spec it is 65535 */
#define CODE_PER_MME                256    /* According to spec it is 256 */

extern int __mme_log_domain;
extern int __emm_log_domain;
extern int __esm_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __mme_log_domain

typedef struct mme_sgsn_s mme_sgsn_t;
typedef struct mme_sgw_s mme_sgw_t;
typedef struct mme_pgw_s mme_pgw_t;
typedef struct mme_vlr_s mme_vlr_t;
typedef struct mme_csmap_s mme_csmap_t;
typedef struct mme_hssmap_s mme_hssmap_t;

typedef struct enb_ue_s enb_ue_t;
typedef struct sgw_ue_s sgw_ue_t;
typedef struct mme_ue_s mme_ue_t;
typedef struct mme_sess_s mme_sess_t;
typedef struct mme_bearer_s mme_bearer_t;

typedef struct ogs_diam_config_s ogs_diam_config_t;

typedef uint32_t mme_m_tmsi_t;
typedef uint32_t mme_p_tmsi_t;

typedef struct served_gummei_s {
    int             num_of_plmn_id;
    ogs_plmn_id_t   plmn_id[OGS_MAX_NUM_OF_PLMN_PER_MME];

    int             num_of_mme_gid;
    uint16_t        mme_gid[GRP_PER_MME];
    int             num_of_mme_code;
    uint8_t         mme_code[CODE_PER_MME];
} served_gummei_t;

typedef struct mme_context_s {
    const char          *diam_conf_path;  /* MME Diameter conf path */
    ogs_diam_config_t   *diam_config;     /* MME Diameter config */

    uint16_t        s1ap_port;      /* Default S1AP Port */
    uint16_t        sgsap_port;     /* Default SGsAP Port */

    ogs_list_t      s1ap_list;      /* MME S1AP IPv4 Server List */
    ogs_list_t      s1ap_list6;     /* MME S1AP IPv6 Server List */

    ogs_list_t      sgw_list;       /* SGW GTPv2C Client List */
    mme_sgw_t       *sgw;           /* Iterator for SGW round-robin */

    ogs_list_t      sgsn_list;       /* SGW GTPv1C Client List */

    ogs_list_t      pgw_list;       /* PGW GTPC Client List */
    ogs_sockaddr_t  *pgw_addr;      /* First IPv4 Address Selected */
    ogs_sockaddr_t  *pgw_addr6;     /* First IPv6 Address Selected */

    ogs_list_t      enb_list;       /* ENB S1AP Client List */

    ogs_list_t      vlr_list;       /* VLR SGsAP Client List */
    ogs_list_t      csmap_list;     /* TAI-LAI Map List */
    ogs_list_t      hssmap_list;    /* PLMN HSS Map List */

    /* Served GUMME */
    int             num_of_served_gummei;
    served_gummei_t served_gummei[OGS_MAX_NUM_OF_SERVED_GUMMEI];

    /* Served TAI */
    int             num_of_served_tai;
    struct {
        ogs_eps_tai0_list_t list0;
        ogs_eps_tai1_list_t list1;
        ogs_eps_tai2_list_t list2;
    } served_tai[OGS_MAX_NUM_OF_SUPPORTED_TA];

    /* Access Control */
    int             default_reject_cause;
    int             num_of_access_control;
    struct {
        int reject_cause;
        ogs_plmn_id_t plmn_id;
    } access_control[OGS_MAX_NUM_OF_PLMN_PER_MME];

    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EEA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EEA2    2
     * #define NAS_SECURITY_ALGORITHMS_128_EEA3    3 */
    int             num_of_ciphering_order;
    uint8_t         ciphering_order[OGS_MAX_NUM_OF_ALGORITHM];
    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    2
     * #define NAS_SECURITY_ALGORITHMS_128_EIA3    3 */
    int             num_of_integrity_order;
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

    ogs_list_t      mme_ue_list;

    ogs_hash_t *enb_addr_hash;  /* hash table for ENB Address */
    ogs_hash_t *enb_id_hash;    /* hash table for ENB-ID */
    ogs_hash_t *imsi_ue_hash;   /* hash table (IMSI : MME_UE) */
    ogs_hash_t *guti_ue_hash;   /* hash table (GUTI : MME_UE) */

    ogs_hash_t *mme_s11_teid_hash;  /* hash table (MME-S11-TEID : MME_UE) */
    ogs_hash_t *mme_gn_teid_hash;  /* hash table (MME-GN-TEID : MME_UE) */

    struct {
        struct {
            ogs_time_t value;       /* Timer Value(Seconds) */
        } t3402, t3412, t3423;
    } time;
} mme_context_t;

typedef struct mme_sgsn_route_s {
    ogs_list_t    list;       /* listed in mme_sgsn_t->route_list */
    ogs_nas_rai_t rai;
    uint16_t cell_id;
} mme_sgsn_route_t;

typedef struct mme_sgsn_s {
    ogs_gtp_node_t  gnode;
    ogs_list_t      route_list; /* list of mme_sgsn_route_t */
    bool            default_route; /* use this SGSN as default route */
} mme_sgsn_t;

typedef struct mme_sgw_s {
    ogs_gtp_node_t  gnode;

    uint16_t        tac[OGS_MAX_NUM_OF_TAI];
    int             num_of_tac;
    uint32_t        e_cell_id[OGS_MAX_NUM_OF_CELL_ID];
    int             num_of_e_cell_id;

    ogs_list_t      sgw_ue_list;
} mme_sgw_t;

typedef struct mme_pgw_s {
    ogs_lnode_t     lnode;

    ogs_sockaddr_t  *sa_list;

    const char      *apn[OGS_MAX_NUM_OF_APN];
    uint8_t         num_of_apn;
    uint16_t        tac[OGS_MAX_NUM_OF_TAI];
    uint8_t         num_of_tac;
    uint32_t        e_cell_id[OGS_MAX_NUM_OF_CELL_ID];
    uint8_t         num_of_e_cell_id;
} mme_pgw_t;

#define MME_SGSAP_IS_CONNECTED(__mME) \
    ((__mME) && ((__mME)->csmap) && ((__mME)->csmap->vlr) && \
     (OGS_FSM_CHECK(&(__mME)->csmap->vlr->sm, sgsap_state_connected)))

typedef struct mme_vlr_s {
    ogs_lnode_t     lnode;

    ogs_fsm_t       sm;          /* A state machine */

    ogs_timer_t     *t_conn;     /* client timer to connect to server */

    int             max_num_of_ostreams;/* SCTP Max num of outbound streams */
    uint16_t        ostream_id;     /* vlr_ostream_id generator */

    ogs_sockaddr_t  *sa_list;   /* VLR SGsAP Socket Address List */
    ogs_sockaddr_t  *local_sa_list;   /* VLR SGsAP Socket Local Address List */

    ogs_sock_t      *sock;      /* VLR SGsAP Socket */
    ogs_sockopt_t   *option;    /* VLR SGsAP Socket Option */
    ogs_poll_t      *poll;      /* VLR SGsAP Poll */
} mme_vlr_t;

typedef struct mme_csmap_s {
    ogs_lnode_t     lnode;

    ogs_nas_eps_tai_t tai;
    ogs_nas_lai_t   lai;

    mme_vlr_t       *vlr;
} mme_csmap_t;

typedef struct mme_hssmap_s {
    ogs_lnode_t     lnode;

    ogs_plmn_id_t   plmn_id;
    char            *realm;
    char            *host;
} mme_hssmap_t;

typedef struct mme_enb_s {
    ogs_lnode_t     lnode;
    ogs_pool_id_t   id;

    ogs_fsm_t       sm;         /* A state machine */

    bool            enb_id_presence;
    uint32_t        enb_id;     /* eNB_ID received from eNB */
    ogs_plmn_id_t   plmn_id;    /* eNB PLMN-ID received from eNB */
    ogs_sctp_sock_t sctp;       /* SCTP socket */

    struct {
        bool s1_setup_success;  /* eNB S1AP Setup complete successfuly */
    } state;

    int             max_num_of_ostreams;/* SCTP Max num of outbound streams */
    uint16_t        ostream_id;         /* enb_ostream_id generator */

    int             num_of_supported_ta_list;
    ogs_eps_tai_t   supported_ta_list[OGS_MAX_NUM_OF_SUPPORTED_TA];

    ogs_pkbuf_t     *s1_reset_ack; /* Reset message */

    ogs_list_t      enb_ue_list;

} mme_enb_t;

struct enb_ue_s {
    ogs_lnode_t     lnode;
    ogs_pool_id_t   id;
    uint32_t        index;

    /* UE identity */
#define INVALID_UE_S1AP_ID      0xffffffff /* Initial value of enb_ue_s1ap_id */
    uint32_t        enb_ue_s1ap_id; /* eNB-UE-S1AP-ID received from eNB */
    uint32_t        mme_ue_s1ap_id; /* MME-UE-S1AP-ID received from MME */

    uint16_t        enb_ostream_id; /* SCTP output stream id for eNB */

    /* Handover Info */
    S1AP_HandoverType_t handover_type;
    ogs_pool_id_t source_ue_id;
    ogs_pool_id_t target_ue_id;

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

    /* UEContextReleaseRequest or InitialContextSetupFailure */
    struct {
        S1AP_Cause_PR group;
        long cause;
    } relcause;

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
    ogs_pool_id_t   enb_id;
    ogs_pool_id_t   mme_ue_id;
};

struct sgw_ue_s {
    ogs_lnode_t     lnode;
    ogs_pool_id_t   id;

    ogs_pool_id_t   source_ue_id;
    ogs_pool_id_t   target_ue_id;

    /* UE identity */
    uint32_t        sgw_s11_teid;   /* SGW-S11-TEID is received from SGW */

    /* S11 Holding timer for removing this context */
    ogs_timer_t     *t_s11_holding;

    /* Related Context */
    union {
        mme_sgw_t       *sgw;
        ogs_gtp_node_t  *gnode;
    };
    ogs_pool_id_t mme_ue_id;
};

struct mme_ue_s {
    ogs_lnode_t     lnode;
    ogs_pool_id_t   id;
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
        ogs_nas_eps_attach_type_t attach;
        ogs_nas_eps_update_type_t update;
        ogs_nas_service_type_t service;
        ogs_nas_detach_type_t detach;
    } nas_eps;

#define MME_TAU_TYPE_INITIAL_UE_MESSAGE    1
#define MME_TAU_TYPE_UPLINK_NAS_TRANPORT   2
#define MME_TAU_TYPE_UNPROTECTED_INGERITY  3
    uint8_t tracking_area_update_request_type;

    /* 1. MME initiated detach request to the UE.
     *    (nas_eps.type = MME_EPS_TYPE_DETACH_REQUEST_TO_UE)
     * 2. If UE is IDLE, Paging sent to the UE
     * 3. If UE is wake-up, UE will send Server Request.
     *    (nas_eps.type = MME_EPS_TYPE_SERVICE_REQUEST)
     *
     * So, we will lose the MME_EPS_TYPE_DETACH_REQUEST_TO_UE.
     *
     * We need more variable(detach_type)
     * to keep Detach-Type whether UE-initiated or MME-initiaed.  */
#define MME_DETACH_TYPE_REQUEST_FROM_UE             1
#define MME_DETACH_TYPE_MME_EXPLICIT                2
#define MME_DETACH_TYPE_HSS_EXPLICIT                3
#define MME_DETACH_TYPE_MME_IMPLICIT                4
#define MME_DETACH_TYPE_HSS_IMPLICIT                5
    uint8_t     detach_type;

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

    struct {
        ogs_pool_id_t   *mme_gn_teid_node; /* A node of MME-Gn-TEID */
        uint32_t        mme_gn_teid;   /* MME-Gn-TEID is derived from NODE */
        uint32_t        sgsn_gn_teid;
        ogs_ip_t        sgsn_gn_ip;
        ogs_ip_t        sgsn_gn_ip_alt;
        /* Unnamed timer in 3GPP TS 23.401 D.3.5 step 2), see also 3GPP TS 23.060 6.9.1.2.2 */
        ogs_timer_t     *t_gn_holding;
        ogs_pool_id_t   gtp_xact_id; /* 2g->4g SGSN Context Req/Resp/Ack gtp1c xact */
    } gn;

    struct {
#define MME_NEXT_GUTI_IS_AVAILABLE(__mME) ((__mME)->next.m_tmsi)
#define MME_CURRENT_GUTI_IS_AVAILABLE(__mME) ((__mME)->current.m_tmsi)
        mme_m_tmsi_t *m_tmsi;
        ogs_nas_eps_guti_t guti;
#define MME_NEXT_P_TMSI_IS_AVAILABLE(__mME) \
    (MME_SGSAP_IS_CONNECTED(__mME) && (__mME)->next.p_tmsi)
#define MME_CURRENT_P_TMSI_IS_AVAILABLE(__mME) \
    (MME_SGSAP_IS_CONNECTED(__mME) && (__mME)->current.p_tmsi)
        mme_p_tmsi_t    p_tmsi;
    } current, next;

    ogs_pool_id_t   *mme_s11_teid_node; /* A node of MME-S11-TEID */
    uint32_t        mme_s11_teid;   /* MME-S11-TEID is derived from NODE */

    uint16_t        vlr_ostream_id; /* SCTP output stream id for VLR */

    /* UE Info */
    uint16_t        enb_ostream_id;
    ogs_eps_tai_t   tai;
    ogs_e_cgi_t     e_cgi;
    ogs_time_t      ue_location_timestamp;
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
    uint32_t        nonceue, noncemme;
    uint8_t         gprs_ciphering_key_sequence_number;

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
    uint8_t         charging_characteristics[OGS_CHRGCHARS_LEN]; /* Subscription Level Charging Characteristics */
    bool            charging_characteristics_presence;

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
    ((__mME) && \
     ((__mME)->enb_ue_id >= OGS_MIN_POOL_ID) && \
     ((__mME)->enb_ue_id <= OGS_MAX_POOL_ID) && \
     (enb_ue_find_by_id((__mME)->enb_ue_id)))
#define ECM_IDLE(__mME) \
    ((__mME) && \
     (((__mME)->enb_ue_id < OGS_MIN_POOL_ID) || \
      ((__mME)->enb_ue_id > OGS_MAX_POOL_ID) || \
      (enb_ue_find_by_id((__mME)->enb_ue_id) == NULL)))
    ogs_pool_id_t   enb_ue_id;

#define HOLDING_S1_CONTEXT(__mME) \
    do { \
        enb_ue_t *enb_ue_holding = NULL; \
        \
        (__mME)->enb_ue_holding_id = OGS_INVALID_POOL_ID; \
        \
        enb_ue_holding = enb_ue_find_by_id((__mME)->enb_ue_id); \
        if (enb_ue_holding) { \
            enb_ue_deassociate(enb_ue_holding); \
            \
            ogs_warn("[%s] Holding S1 Context", (__mME)->imsi_bcd); \
            ogs_warn("[%s]    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]", \
                    (__mME)->imsi_bcd, \
                    enb_ue_holding->enb_ue_s1ap_id, \
                    enb_ue_holding->mme_ue_s1ap_id); \
            \
            enb_ue_holding->ue_ctx_rel_action = \
                S1AP_UE_CTX_REL_S1_CONTEXT_REMOVE; \
            ogs_timer_start(enb_ue_holding->t_s1_holding, \
                    mme_timer_cfg(MME_TIMER_S1_HOLDING)->duration); \
            \
            (__mME)->enb_ue_holding_id = (__mME)->enb_ue_id; \
        } else \
            ogs_error("[%s] S1 Context has already been removed", \
                    (__mME)->imsi_bcd); \
    } while(0)
#define CLEAR_S1_CONTEXT(__mME) \
    do { \
        enb_ue_t *enb_ue_holding = NULL; \
        \
        enb_ue_holding = enb_ue_find_by_id((__mME)->enb_ue_holding_id); \
        if (enb_ue_holding) { \
            int r; \
            ogs_warn("[%s] Clear S1 Context", (__mME)->imsi_bcd); \
            ogs_warn("[%s]    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]", \
                    (__mME)->imsi_bcd, \
                    enb_ue_holding->enb_ue_s1ap_id, \
                    enb_ue_holding->mme_ue_s1ap_id); \
            \
            r = s1ap_send_ue_context_release_command( \
                    enb_ue_holding, \
                    S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release, \
                    S1AP_UE_CTX_REL_S1_CONTEXT_REMOVE, 0); \
            ogs_expect(r == OGS_OK); \
            ogs_assert(r != OGS_ERROR); \
        } \
        (__mME)->enb_ue_holding_id = OGS_INVALID_POOL_ID; \
    } while(0)
    ogs_pool_id_t   enb_ue_holding_id;

    struct {
#define MME_CLEAR_PAGING_INFO(__mME) \
    do { \
        ogs_assert(__mME); \
        ogs_debug("[%s] Clear Paging Info", (__mME)->imsi_bcd); \
        (__mME)->paging.type = 0; \
        (__mME)->paging.failed = false; \
    } while(0)

#define MME_STORE_PAGING_INFO(__mME, __tYPE, __dATA) \
    do { \
        ogs_assert(__mME); \
        ogs_assert(__tYPE); \
        ogs_debug("[%s] Store Paging Info", mme_ue->imsi_bcd); \
        (__mME)->paging.type = __tYPE; \
        (__mME)->paging.data = OGS_UINT_TO_POINTER(__dATA); \
    } while(0)

#define MME_PAGING_ONGOING(__mME) ((__mME) && ((__mME)->paging.type))

#define MME_PAGING_TYPE_DOWNLINK_DATA_NOTIFICATION 1
#define MME_PAGING_TYPE_CREATE_BEARER 2
#define MME_PAGING_TYPE_UPDATE_BEARER 3
#define MME_PAGING_TYPE_DELETE_BEARER 4
#define MME_PAGING_TYPE_CS_CALL_SERVICE 5
#define MME_PAGING_TYPE_SMS_SERVICE 6
#define MME_PAGING_TYPE_DETACH_TO_UE 7
        int type;
        void *data;
        bool failed;
    } paging;

    /* SGW UE context */
    ogs_pool_id_t sgw_ue_id;

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
        CLEAR_MME_UE_TIMER((__mME)->t_mobile_reachable); \
        CLEAR_MME_UE_TIMER((__mME)->t_implicit_detach); \
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
    } t3413, t3422, t3450, t3460, t3470, t_mobile_reachable,
        t_implicit_detach;

#define CLEAR_SERVICE_INDICATOR(__mME) \
    do { \
        ogs_assert((__mME)); \
        (__mME)->service_indicator = 0; \
    } while(0);

#define CS_CALL_SERVICE_INDICATOR(__mME) \
    (MME_CURRENT_P_TMSI_IS_AVAILABLE(__mME) && \
     ((__mME)->service_indicator) == SGSAP_CS_CALL_SERVICE_INDICATOR)
#define SMS_SERVICE_INDICATOR(__mME) \
    (MME_CURRENT_P_TMSI_IS_AVAILABLE(__mME) && \
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

#define GTP_COUNTER_CREATE_SESSION_BY_PATH_SWITCH               1
#define GTP_COUNTER_DELETE_SESSION_BY_PATH_SWITCH               2
    struct {
        uint8_t request;
        uint8_t response;
    } gtp_counter[MAX_NUM_OF_GTP_COUNTER];

    ogs_list_t      bearer_to_modify_list;

    mme_csmap_t     *csmap;
    mme_hssmap_t    *hssmap;
};

#define SESSION_CONTEXT_IS_AVAILABLE(__mME) \
    ((__mME) && \
     ((__mME)->sgw_ue_id >= OGS_MIN_POOL_ID) && \
     ((__mME)->sgw_ue_id <= OGS_MAX_POOL_ID) && \
     (sgw_ue_find_by_id((__mME)->sgw_ue_id)) && \
     (sgw_ue_find_by_id((__mME)->sgw_ue_id)->sgw_s11_teid))

#define CLEAR_SESSION_CONTEXT(__mME) \
    do { \
        sgw_ue_t *sgw_ue = NULL; \
        ogs_assert((__mME)); \
        sgw_ue = sgw_ue_find_by_id((__mME)->sgw_ue_id); \
        if (sgw_ue) sgw_ue->sgw_s11_teid = 0; \
    } while(0)

#define MME_SESS_CLEAR(__sESS) \
    do { \
        mme_ue_t *mme_ue = NULL; \
        ogs_assert(__sESS); \
        mme_ue = mme_ue_find_by_id((__sESS)->mme_ue_id); \
        ogs_assert(mme_ue); \
        ogs_info("Removed Session: UE IMSI:[%s] APN:[%s]", \
                mme_ue->imsi_bcd, \
                (__sESS)->session ? (__sESS)->session->name : "Unknown"); \
        if (mme_sess_count(mme_ue) == 1) /* Last Session */ \
            CLEAR_SESSION_CONTEXT(mme_ue); \
        mme_sess_remove(__sESS); \
    } while(0)

#define ACTIVE_EPS_BEARERS_IS_AVAIABLE(__mME) \
    (mme_ue_have_active_eps_bearers(__mME))
#define MME_SESSION_RELEASE_PENDING(__mME) \
    (mme_ue_have_session_release_pending(__mME))
typedef struct mme_sess_s {
    ogs_lnode_t     lnode;
    ogs_pool_id_t   id;

    uint8_t         pti;        /* Procedure Trasaction Identity */

    uint32_t        pgw_s5c_teid;
    ogs_ip_t        pgw_s5c_ip;

    /* PDN Connectivity Request */
    ogs_nas_request_type_t ue_request_type;

    /* mme_bearer_first(sess) : Default Bearer Context */
    ogs_list_t      bearer_list;

    /* Related Context */
    ogs_pool_id_t   mme_ue_id;

    ogs_session_t   *session;

    /* PDN Address Allocation (PAA) */
    ogs_paa_t       paa;

    /* Save Protocol Configuration Options from UE */
    struct {
        uint8_t length;
        uint8_t *buffer;
    } ue_pco;

    /* Save Extended Protocol Configuration Options from UE */
    struct {
        uint16_t length;
        uint8_t *buffer;
    } ue_epco;

    /* Save Protocol Configuration Options from PGW */
    ogs_tlv_octet_t pgw_pco;

    /* Save Extended Protocol Configuration Options from PGW */
    ogs_tlv_octet_t pgw_epco;
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
    ogs_lnode_t     to_modify_node;

    ogs_pool_id_t   id;

    ogs_fsm_t       sm;             /* State Machine */

    uint8_t         *ebi_node;      /* Pool-Node for EPS Bearer ID */
    uint8_t         ebi;            /* EPS Bearer ID */

    uint32_t        enb_s1u_teid;
    ogs_ip_t        enb_s1u_ip;
    uint32_t        sgw_s1u_teid;
    ogs_ip_t        sgw_s1u_ip;
    uint32_t        pgw_s5u_teid;
    ogs_ip_t        pgw_s5u_ip;

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
    ogs_pool_id_t   mme_ue_id;
    ogs_pool_id_t   sess_id;

    /*
     * Issues #3240
     *
     * SMF->SGW-C->MME: First Update Bearer Request
     * MME->UE:         First Modify EPS bearer context request
     * SMF->SGW-C->MME: Second Update Bearer Request
     * MME->UE:         Second Modify EPS bearer context request
     * UE->MME:         First Modify EPS bearer context accept
     * MME->SGW-C->SMF: First Update Bearer Response
     * UE->MME:         Second Modify EPS bearer context accept
     * MME->SGW-C->SMF: Second Update Bearer Response
     *
     * We'll start by managing only Update Bearer Request/Response
     * as a list so that we can manage multiple of them.
     */
    struct {
        ogs_pool_id_t  xact_id;
    } create, delete, notify;
    struct {
        ogs_list_t  xact_list;
    } update;
} mme_bearer_t;

void mme_context_init(void);
void mme_context_final(void);
mme_context_t *mme_self(void);

int mme_context_parse_config(void);

mme_sgsn_t *mme_sgsn_add(ogs_sockaddr_t *addr);
void mme_sgsn_remove(mme_sgsn_t *sgsn);
void mme_sgsn_remove_all(void);
mme_sgsn_t *mme_sgsn_find_by_addr(const ogs_sockaddr_t *addr);
mme_sgsn_t *mme_sgsn_find_by_routing_address(const ogs_nas_rai_t *rai, uint16_t cell_id);
mme_sgsn_t *mme_sgsn_find_by_default_routing_address(void);

mme_sgw_t *mme_sgw_add(ogs_sockaddr_t *addr);
void mme_sgw_remove(mme_sgw_t *sgw);
void mme_sgw_remove_all(void);
mme_sgw_t *mme_sgw_find_by_addr(const ogs_sockaddr_t *addr);

mme_pgw_t *mme_pgw_add(ogs_sockaddr_t *addr);
void mme_pgw_remove(mme_pgw_t *pgw);
void mme_pgw_remove_all(void);
ogs_sockaddr_t *mme_pgw_addr_find_by_apn_enb(
        ogs_list_t *list, int family, const mme_sess_t *sess);

mme_vlr_t *mme_vlr_add(
        ogs_sockaddr_t *sa_list,
        ogs_sockaddr_t *local_sa_list,
        ogs_sockopt_t *option);
void mme_vlr_remove(mme_vlr_t *vlr);
void mme_vlr_remove_all(void);
void mme_vlr_close(mme_vlr_t *vlr);
mme_vlr_t *mme_vlr_find_by_sock(const ogs_sock_t *sock);

mme_csmap_t *mme_csmap_add(mme_vlr_t *vlr);
void mme_csmap_remove(mme_csmap_t *csmap);
void mme_csmap_remove_all(void);

mme_csmap_t *mme_csmap_find_by_tai(const ogs_eps_tai_t *tai);
mme_csmap_t *mme_csmap_find_by_nas_lai(const ogs_nas_lai_t *lai);

mme_hssmap_t *mme_hssmap_add(ogs_plmn_id_t *plmn_id, const char *realm,
                             const char *host);
void mme_hssmap_remove(mme_hssmap_t *hssmap);
void mme_hssmap_remove_all(void);

mme_hssmap_t *mme_hssmap_find_by_imsi_bcd(const char *imsi_bcd);

mme_enb_t *mme_enb_add(ogs_sock_t *sock, ogs_sockaddr_t *addr);
int mme_enb_remove(mme_enb_t *enb);
int mme_enb_remove_all(void);
mme_enb_t *mme_enb_find_by_addr(const ogs_sockaddr_t *addr);
mme_enb_t *mme_enb_find_by_enb_id(uint32_t enb_id);
int mme_enb_set_enb_id(mme_enb_t *enb, uint32_t enb_id);
int mme_enb_sock_type(ogs_sock_t *sock);
mme_enb_t *mme_enb_find_by_id(ogs_pool_id_t id);

enb_ue_t *enb_ue_add(mme_enb_t *enb, uint32_t enb_ue_s1ap_id);
void enb_ue_remove(enb_ue_t *enb_ue);
void enb_ue_switch_to_enb(enb_ue_t *enb_ue, mme_enb_t *new_enb);
enb_ue_t *enb_ue_find_by_enb_ue_s1ap_id(
        const mme_enb_t *enb, uint32_t enb_ue_s1ap_id);
enb_ue_t *enb_ue_find(uint32_t index);
enb_ue_t *enb_ue_find_by_mme_ue_s1ap_id(uint32_t mme_ue_s1ap_id);
enb_ue_t *enb_ue_find_by_id(ogs_pool_id_t id);

sgw_ue_t *sgw_ue_add(mme_sgw_t *sgw);
void sgw_ue_remove(sgw_ue_t *sgw_ue);
void sgw_ue_switch_to_sgw(sgw_ue_t *sgw_ue, mme_sgw_t *new_sgw);
sgw_ue_t *sgw_ue_find_by_id(ogs_pool_id_t id);

typedef enum {
    SGW_WITHOUT_RELOCATION = 1,
    SGW_WITH_RELOCATION = 2,
    SGW_HAS_ALREADY_BEEN_RELOCATED = 3,
} sgw_relocation_e;
sgw_relocation_e sgw_ue_check_if_relocated(mme_ue_t *mme_ue);

void mme_ue_new_guti(mme_ue_t *mme_ue);
void mme_ue_confirm_guti(mme_ue_t *mme_ue);

#define INVALID_P_TMSI 0
void mme_ue_set_p_tmsi(
        mme_ue_t *mme_ue,
        ogs_nas_mobile_identity_tmsi_t *nas_mobile_identity_tmsi);
void mme_ue_confirm_p_tmsi(mme_ue_t *mme_ue);

mme_ue_t *mme_ue_add(enb_ue_t *enb_ue);
void mme_ue_remove(mme_ue_t *mme_ue);
void mme_ue_remove_all(void);
mme_ue_t *mme_ue_find_by_id(ogs_pool_id_t id);

void mme_ue_fsm_init(mme_ue_t *mme_ue);
void mme_ue_fsm_fini(mme_ue_t *mme_ue);

mme_ue_t *mme_ue_find_by_imsi(const uint8_t *imsi, int imsi_len);
mme_ue_t *mme_ue_find_by_imsi_bcd(const char *imsi_bcd);
mme_ue_t *mme_ue_find_by_guti(const ogs_nas_eps_guti_t *nas_guti);
mme_ue_t *mme_ue_find_by_s11_local_teid(uint32_t teid);
mme_ue_t *mme_ue_find_by_gn_local_teid(uint32_t teid);

mme_ue_t *mme_ue_find_by_message(const ogs_nas_eps_message_t *message);
int mme_ue_set_imsi(mme_ue_t *mme_ue, char *imsi_bcd);

bool mme_ue_have_indirect_tunnel(mme_ue_t *mme_ue);
void mme_ue_clear_indirect_tunnel(mme_ue_t *mme_ue);

bool mme_ue_have_active_eps_bearers(mme_ue_t *mme_ue);
bool mme_sess_have_active_eps_bearers(mme_sess_t *sess);
bool mme_ue_have_session_release_pending(mme_ue_t *mme_ue);
bool mme_sess_have_session_release_pending(mme_sess_t *sess);

int mme_ue_xact_count(mme_ue_t *mme_ue, uint8_t org);

/*
 * o RECV Initial UE-Message : S-TMSI
 * o RECV Attach Request : IMSI, GUTI
 * o RECV TAU Request : GUTI
 * ### MME_UE_ASSOCIATE_ENB_UE() ###
 * ### MME_UE_ECM_CONNECTED() ###
 *
 * o RECV Initial Context Setup Failure in EMM Registered State
 * ### ENB_UE_DEASSOCIATE_MME_UE() ###
 * ### ENB_UE_REMOVE() ###
 * ### ENB_UE_UNLINK() ###
 *
 * o SEND UE Context Release Command with S1_REMOVE_AND_UNLINK
 *   - RECV UE Context Release Complete
 * ### ENB_UE_REMOVE() ###
 * ### ENB_UE_UNLINK() ###
 *
 * o SEND UE Context Release Command with UE_CONTEXT_REMOVE
 *   - RECV UE Context Release Complete
 * ### ENB_UE_REMOVE() ###
 * ### MME_UE_REMOVE() ###
 *
 *
 * o RECV Handover Required
 * ### ENB_UE_SOURCE_ASSOCIATE_TARGET() ####
 *   - SEND Handover Request
 *
 * o RECV Handover Notify
 * ### MME_UE_ASSOCIATE_ENB_UE(TARGET) ###
 * ### MME_UE_ECM_CONNECTED(TARGET) ###
 *   - Modify Bearer Request/Response
 *   - UE Context Release Command/Complete
 * ### ENB_UE_SOURCE_DEASSOCIATE_TARGET() ####
 * ### ENB_UE_REMOVE() ####
 *   - Delete Indirect Data Forwarding Tunnel Request/Response
 *
 * o RECV Handover Cancel
 *   - UE Context Release Command/Complete
 * ### ENB_UE_SOURCE_DEASSOCIATE_TARGET() ####
 * ### ENB_UE_REMOVE() ####
 *   - Delete Indirect Data Forwarding Tunnel Request/Response
 *
 * o RECV Handover Failure
 *   - UE Context Release Command/Complete
 * ### ENB_UE_SOURCE_DEASSOCIATE_TARGET() ####
 * ### ENB_UE_REMOVE() ####
 *   - Delete Indirect Data Forwarding Tunnel Request/Response
 */
void enb_ue_associate_mme_ue(enb_ue_t *enb_ue, mme_ue_t *mme_ue);
void enb_ue_deassociate(enb_ue_t *enb_ue);
void enb_ue_unlink(mme_ue_t *mme_ue);
void enb_ue_source_associate_target(enb_ue_t *source_ue, enb_ue_t *target_ue);
void enb_ue_source_deassociate_target(enb_ue_t *enb_ue);

void sgw_ue_associate_mme_ue(sgw_ue_t *sgw_ue, mme_ue_t *mme_ue);
void sgw_ue_deassociate(sgw_ue_t *sgw_ue);
void sgw_ue_unlink(mme_ue_t *mme_ue);
void sgw_ue_source_associate_target(sgw_ue_t *source_ue, sgw_ue_t *target_ue);
void sgw_ue_source_deassociate_target(sgw_ue_t *sgw_ue);

mme_sess_t *mme_sess_add(mme_ue_t *mme_ue, uint8_t pti);
void mme_sess_remove(mme_sess_t *sess);
void mme_sess_remove_all(mme_ue_t *mme_ue);
mme_sess_t *mme_sess_find_by_pti(const mme_ue_t *mme_ue, uint8_t pti);
mme_sess_t *mme_sess_find_by_ebi(const mme_ue_t *mme_ue, uint8_t ebi);
mme_sess_t *mme_sess_find_by_apn(const mme_ue_t *mme_ue, const char *apn);
mme_sess_t *mme_sess_find_by_id(ogs_pool_id_t id);

mme_sess_t *mme_sess_first(const mme_ue_t *mme_ue);
mme_sess_t *mme_sess_next(mme_sess_t *sess);
unsigned int mme_sess_count(const mme_ue_t *mme_ue);

mme_bearer_t *mme_bearer_add(mme_sess_t *sess);
void mme_bearer_remove(mme_bearer_t *bearer);
void mme_bearer_remove_all(mme_sess_t *sess);
mme_bearer_t *mme_bearer_find_by_sess_ebi(const mme_sess_t *sess, uint8_t ebi);
mme_bearer_t *mme_bearer_find_by_ue_ebi(const mme_ue_t *mme_ue, uint8_t ebi);
mme_bearer_t *mme_bearer_find_or_add_by_message(
        mme_ue_t *mme_ue, ogs_nas_eps_message_t *message, int create_action);
mme_bearer_t *mme_default_bearer_in_sess(mme_sess_t *sess);
mme_bearer_t *mme_linked_bearer(mme_bearer_t *bearer);
mme_bearer_t *mme_bearer_first(const mme_sess_t *sess);
mme_bearer_t *mme_bearer_next(mme_bearer_t *bearer);
mme_bearer_t *mme_bearer_find_by_id(ogs_pool_id_t id);

void mme_session_remove_all(mme_ue_t *mme_ue);
ogs_session_t *mme_session_find_by_apn(mme_ue_t *mme_ue, const char *apn);
ogs_session_t *mme_default_session(mme_ue_t *mme_ue);

int mme_find_served_tai(ogs_eps_tai_t *tai);

mme_m_tmsi_t *mme_m_tmsi_alloc(void);
int mme_m_tmsi_free(mme_m_tmsi_t *tmsi);

void mme_ebi_pool_init(mme_ue_t *mme_ue);
void mme_ebi_pool_final(mme_ue_t *mme_ue);
void mme_ebi_pool_clear(mme_ue_t *mme_ue);

uint8_t mme_selected_int_algorithm(mme_ue_t *mme_ue);
uint8_t mme_selected_enc_algorithm(mme_ue_t *mme_ue);

#ifdef __cplusplus
}
#endif

#endif /* MME_CONTEXT_H */

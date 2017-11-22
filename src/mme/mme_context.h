#ifndef __MME_CONTEXT__
#define __MME_CONTEXT__

#include "core_list.h"
#include "core_index.h"
#include "core_errno.h"
#include "core_net.h"
#include "core_sha2.h"
#include "core_hash.h"

#include "types.h"
#include "s1ap_message.h"
#include "nas_message.h"
#include "gtp_xact.h"
#include "s6a_message.h"

#include "mme_sm.h"

/* S1AP */
#include "S1ap-Cause.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_PLMN_ID                 6
#define GRP_PER_MME                 256    /* According to spec it is 65535 */
#define CODE_PER_MME                256    /* According to spec it is 256 */

#define MAX_NUM_OF_SERVED_TAI       16
#define MAX_NUM_OF_SERVED_GUMMEI    8
#define MAX_NUM_OF_ALGORITHM        8

#define MAX_NUM_OF_TAC              256
#define MAX_NUM_OF_BPLMN            6

typedef struct _enb_ue_t enb_ue_t;
typedef struct _mme_ue_t mme_ue_t;
typedef gtp_node_t mme_sgw_t;

typedef struct _served_gummei {
    c_uint32_t      num_of_plmn_id;
    plmn_id_t       plmn_id[MAX_PLMN_ID];

    c_uint32_t      num_of_mme_gid;
    c_uint16_t      mme_gid[GRP_PER_MME];
    c_uint32_t      num_of_mme_code;
    c_uint8_t       mme_code[CODE_PER_MME];
} served_gummei_t;

typedef struct _mme_context_t {
    const char      *fd_conf_path;  /* MME freeDiameter conf path */

    c_uint32_t      s1ap_addr;      /* MME S1AP local address */
    c_uint16_t      s1ap_port;      /* MME S1AP local port */
    sock_id         s1ap_sock;      /* MME S1AP local listen socket */

    c_uint32_t      gtpc_addr;      /* MME GTPC local address */
    c_uint16_t      gtpc_port;      /* MME GTPC local port */
    net_sock_t      *gtpc_sock;     /* MME GTPC local listen socket */

    c_uint32_t      s5c_addr;       /* PGW S5C remote address */
    c_uint16_t      s5c_port;       /* PGW S5C remote port */

    msgq_id         queue_id;       /* Queue for processing MME control plane */
    tm_service_t    tm_service;     /* Timer Service */

    /* Generator for unique identification */
    c_uint32_t      mme_ue_s1ap_id; /* mme_ue_s1ap_id generator */
    c_uint32_t      m_tmsi;         /* m_tmsi generator */
    /* Iterator for SGW round-robin */
    mme_sgw_t       *sgw;

    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EEA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EEA2    2
     * #define NAS_SECURITY_ALGORITHMS_128_EEA3    3 */
    c_uint8_t       num_of_ciphering_order;
    c_uint8_t       ciphering_order[MAX_NUM_OF_ALGORITHM];
    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    2
     * #define NAS_SECURITY_ALGORITHMS_128_EIA3    3 */
    c_uint8_t       num_of_integrity_order;
    c_uint8_t       integrity_order[MAX_NUM_OF_ALGORITHM];

    /* S1SetupRequest */
    c_uint8_t       max_num_of_served_tai;
    tai_t           served_tai[MAX_NUM_OF_SERVED_TAI];

    /* S1SetupResponse */
    c_uint8_t       max_num_of_served_gummei;
    served_gummei_t served_gummei[MAX_NUM_OF_SERVED_GUMMEI];
    c_uint8_t       relative_capacity;

    /* Timer value */
    c_uint32_t      t3413_value; /* Paging retry timer */

    list_t          sgw_list;  /* SGW GTP Node List */

    hash_t          *s1ap_sock_hash;        /* hash table for S1AP IP address */
    hash_t          *enb_id_hash;           /* hash table for ENB-ID */
    hash_t          *mme_ue_s1ap_id_hash;   /* hash table for MME-UE-S1AP-ID */
    hash_t          *imsi_ue_hash;          /* hash table (IMSI : MME_UE) */
    hash_t          *guti_ue_hash;          /* hash table (GUTI : MME_UE) */
} mme_context_t;

typedef struct _mme_enb_t {
    lnode_t         node;   /* A node of list_t */
    index_t         index;  /* An index of this node */
    fsm_t           sm;     /* A state machine */

    c_uint32_t      enb_id;     /* eNB_ID received from eNB */
    c_uint32_t      s1ap_addr;  /* eNB S1AP IP address */
    c_uint16_t      s1ap_port;  /* eNB S1AP Port */
    sock_id         s1ap_sock;  /* eNB S1AP Socket */

    c_uint8_t       num_of_tai;
    tai_t           tai[MAX_NUM_OF_TAC * MAX_NUM_OF_BPLMN];

    list_t          enb_ue_list;

} mme_enb_t;

struct _enb_ue_t {
    lnode_t         node;   /* A node of list_t */
    index_t         index;  /* An index of this node */

    /* UE identity */
    c_uint32_t      enb_ue_s1ap_id; /* eNB-UE-S1AP-ID received from eNB */
    c_uint32_t      mme_ue_s1ap_id; /* MME-UE-S1AP-ID received from MME */

    /* Handover Info */
    S1ap_HandoverType_t handover_type;
    enb_ue_t        *source_ue;
    enb_ue_t        *target_ue;

    /* Use mme_ue->tai, mme_ue->e_cgi.
     * Do not access enb_ue->nas.tai enb_ue->nas.e_cgi.
     * 
     * Save TAI and ECGI. And then, this will copy 'mme_ue_t' context later */
    struct {
        tai_t           tai;
        e_cgi_t         e_cgi;
    } nas;

    /* Related Context */
    mme_enb_t       *enb;
    mme_ue_t        *mme_ue;
}; 

struct _mme_ue_t {
    lnode_t         node;   /* A node of list_t */
    index_t         index;  /* An index of this node */
    fsm_t           sm;     /* A state machine */

    struct {
#define MME_EPS_TYPE_ATTACH_REQUEST                 1
#define MME_EPS_TYPE_TAU_REQUEST                    2
#define MME_EPS_TYPE_SERVICE_REQUEST                3
        c_uint8_t   type;
        union {
            nas_eps_attach_type_t attach;
            nas_eps_update_type_t update;
        };
    } nas_eps;

    /* UE identity */
#define MME_UE_HAVE_IMSI(__mME) \
    ((__mME) && ((__mME)->imsi_len))
    c_uint8_t       imsi[MAX_IMSI_LEN];
    int             imsi_len;
    c_int8_t        imsi_bcd[MAX_IMSI_BCD_LEN+1];
    guti_t          guti;

    /* IMPORTANT!
     * MME-S11-TEID is same with an index */
    c_uint32_t      mme_s11_teid;
    c_uint32_t      mme_s11_addr;

    c_uint32_t      sgw_s11_teid;
    c_uint32_t      sgw_s11_addr;

    /* UE Info */
    tai_t           tai;
    e_cgi_t         e_cgi;
    plmn_id_t       visited_plmn_id;

#define SECURITY_CONTEXT_IS_VALID(__mME) \
    ((__mME) && \
    ((__mME)->security_context_available == 1) && ((__mME)->mac_failed == 0))
#define CLEAR_SECURITY_CONTEXT(__mME) \
    do { \
        d_assert((__mME), break, "Null param"); \
        (__mME)->security_context_available = 0; \
        (__mME)->mac_failed = 0; \
    } while(0)
    int             security_context_available;
    int             mac_failed;

    /* Security Context */
    nas_ue_network_capability_t ue_network_capability;
    nas_ms_network_capability_t ms_network_capability;
    c_uint8_t       xres[MAX_RES_LEN];
    c_uint8_t       xres_len;
    c_uint8_t       kasme[SHA256_DIGEST_SIZE];
    c_uint8_t       knas_int[SHA256_DIGEST_SIZE/2]; 
    c_uint8_t       knas_enc[SHA256_DIGEST_SIZE/2];
    c_uint32_t      dl_count;
    union {
        struct {
        ED3(c_uint8_t spare;,
            c_uint16_t overflow;,
            c_uint8_t sqn;)
        } __attribute__ ((packed));
        c_uint32_t i32;
    } ul_count;
    c_uint8_t       kenb[SHA256_DIGEST_SIZE];

    struct {
    ED2(c_uint8_t nhcc_spare:5;,
        c_uint8_t nhcc:3;) /* Next Hop Channing Counter */
    };
    c_uint8_t       nh[SHA256_DIGEST_SIZE]; /* NH Security Key */

    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EEA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EEA2    2
     * #define NAS_SECURITY_ALGORITHMS_128_EEA3    3 */
    c_uint8_t       selected_enc_algorithm;
    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    2
     * #define NAS_SECURITY_ALGORITHMS_128_EIA3    3 */
    c_uint8_t       selected_int_algorithm;

    /* HSS Info */
    s6a_subscription_data_t subscription_data;

    /* ESM Info */
#define MIN_EPS_BEARER_ID           5
#define MAX_EPS_BEARER_ID           15

#define CLEAR_EPS_BEARER_ID(__mME) \
    do { \
        d_assert((__mME), break, "Null param"); \
        (__mME)->ebi = MIN_EPS_BEARER_ID - 1; \
    } while(0)
    c_uint8_t       ebi; /* EPS Bearer ID generator */
    list_t          sess_list;

    /* eNB UE context */
    enb_ue_t        *enb_ue;

    /* Save PDN Connectivity Request */
    nas_esm_message_container_t pdn_connectivity_request;

    /* Paging */
#define CLEAR_PAGING_INFO(__mME) \
    do { \
        d_assert((__mME), break, "Null param"); \
        \
        tm_stop((__mME)->t3413); \
        if ((__mME)->last_paging_msg) \
        { \
            pkbuf_free((__mME)->last_paging_msg); \
            (__mME)->last_paging_msg = NULL; \
        } \
        (__mME)->max_paging_retry = 0; \
    } while(0);
    pkbuf_t         *last_paging_msg;
    tm_block_id     t3413;
#define MAX_NUM_OF_PAGING           2
    c_uint32_t      max_paging_retry;

    /* UE Radio Capability */
    void            *radio_capa;

    /* Detach Request */
    nas_detach_type_t detach_type;

    /* S1AP Transparent Container */
    OCTET_STRING_t container;

    /* GTP Request/Response Counter */
#define GTP_COUNTER_INCREMENT(__mME, __tYPE) \
        do { \
            d_assert((__mME), break,); \
            ((__mME)->gtp_counter[__tYPE].request)++; \
        } while(0);

#define GTP_COUNTER_CHECK(__mME, __tYPE, __eXPR) \
        do { \
            d_assert((__mME), break,); \
            if ((__mME)->gtp_counter[__tYPE].request == 0) break; \
            ((__mME)->gtp_counter[__tYPE].response)++; \
            if (((__mME)->gtp_counter[__tYPE].request) == \
                ((__mME)->gtp_counter[__tYPE].response)) \
            { \
                ((__mME)->gtp_counter[__tYPE].request) = 0; \
                ((__mME)->gtp_counter[__tYPE].response) = 0; \
                __eXPR \
            } \
        } while(0);

#define MAX_NUM_OF_GTP_COUNTER                                  16

#define GTP_COUNTER_DELETE_SESSION                              0
#define GTP_COUNTER_MODIFY_BEARER_BY_PATH_SWITCH                1
#define GTP_COUNTER_MODIFY_BEARER_BY_HANDOVER_NOTIFY            2
    struct {
        c_uint8_t request;
        c_uint8_t response;
    } gtp_counter[MAX_NUM_OF_GTP_COUNTER];

#define CONNECT_SGW_GTP_NODE(__mME) \
    do { \
        d_assert((__mME), break, "Null param"); \
        (__mME)->sgw = self.sgw; \
        d_assert(((__mME)->sgw), break, "Null param"); \
        self.sgw = mme_sgw_next((__mME)->sgw); \
        if (!self.sgw) self.sgw = mme_sgw_first(); \
        d_assert(self.sgw, break, "Null param"); \
    } while(0)
    mme_sgw_t       *sgw;
};

#define MME_HAVE_SGW_S1U_PATH(__sESS) \
    ((__sESS) && (mme_bearer_first(__sESS)) && \
     ((mme_default_bearer_in_sess(__sESS)->sgw_s1u_teid) && \
      (mme_default_bearer_in_sess(__sESS)->sgw_s1u_addr)))

#define MME_HAVE_SGW_S11_PATH(__mME) \
     ((__mME) && ((__mME)->sgw_s11_teid) && ((__mME)->sgw_s11_addr))

#define CLEAR_SGW_S11_PATH(__mME) \
    do { \
        d_assert((__mME), break, "Null param"); \
        (__mME)->sgw_s11_teid = 0; \
        (__mME)->sgw_s11_addr = 0; \
    } while(0)
typedef struct _mme_sess_t {
    lnode_t         node;       /* A node of list_t */
    index_t         index;      /* An index of this node */

    c_uint8_t       pti;        /* Procedure Trasaction Identity */

    /* mme_bearer_first(sess) : Default Bearer Context */
    list_t          bearer_list;

    /* Related Context */
    mme_ue_t        *mme_ue;

#define MME_UE_HAVE_APN(__mME) \
    ((__mME) && (mme_sess_first(__mME)) && \
    ((mme_sess_first(__mME))->pdn))
#define MME_GET_PGW_IPV4_ADDR(__sESS) \
    (((__sESS) && ((__sESS)->pdn) && (((__sESS)->pdn)->pgw.ipv4_addr)) ? \
      (((__sESS)->pdn)->pgw.ipv4_addr) : (mme_self()->s5c_addr))
    pdn_t           *pdn;

    /* Save Protocol Configuration Options from UE */
    struct {
        c_uint8_t length;
        c_uint8_t *buffer;
    } ue_pco; 

    /* Save Protocol Configuration Options from PGW */
    tlv_octet_t     pgw_pco;
} mme_sess_t;

#define MME_HAVE_ENB_S1U_PATH(__bEARER) \
    ((__bEARER) && ((__bEARER)->enb_s1u_teid) && ((__bEARER)->enb_s1u_addr))

#define MME_HAVE_ENB_DL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->enb_dl_teid) && ((__bEARER)->enb_dl_addr))
#define MME_HAVE_ENB_UL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->enb_ul_teid) && ((__bEARER)->enb_ul_addr))
#define MME_HAVE_SGW_DL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->sgw_dl_teid) && ((__bEARER)->sgw_dl_addr))
#define MME_HAVE_SGW_UL_INDIRECT_TUNNEL(__bEARER) \
    ((__bEARER) && ((__bEARER)->sgw_ul_teid) && ((__bEARER)->sgw_ul_addr))
#define CLEAR_INDIRECT_TUNNEL(__bEARER) \
    do { \
        d_assert((__bEARER), break, "Null param"); \
        (__bEARER)->enb_dl_teid = 0; \
        (__bEARER)->enb_dl_addr = 0; \
        (__bEARER)->enb_ul_teid = 0; \
        (__bEARER)->enb_ul_addr = 0; \
        (__bEARER)->sgw_dl_teid = 0; \
        (__bEARER)->sgw_dl_addr = 0; \
        (__bEARER)->sgw_ul_teid = 0; \
        (__bEARER)->sgw_ul_addr = 0; \
    } while(0)
typedef struct _mme_bearer_t {
    lnode_t         node;           /* A node of list_t */
    index_t         index;          /* An index of this node */
    fsm_t           sm;             /* State Machine */

    c_uint8_t       ebi;            /* EPS Bearer ID */    

    c_uint32_t      enb_s1u_teid;
    c_uint32_t      enb_s1u_addr;
    c_uint32_t      sgw_s1u_teid;
    c_uint32_t      sgw_s1u_addr;

    c_uint32_t      target_s1u_teid;  /* Target S1U TEID from HO-Req-Ack */
    c_uint32_t      target_s1u_addr;  /* Target S1U ADDR from HO-Req-Ack */

    c_uint32_t      enb_dl_teid;
    c_uint32_t      enb_dl_addr;
    c_uint32_t      enb_ul_teid;
    c_uint32_t      enb_ul_addr;

    c_uint32_t      sgw_dl_teid;
    c_uint32_t      sgw_dl_addr;
    c_uint32_t      sgw_ul_teid;
    c_uint32_t      sgw_ul_addr;

    qos_t           qos;
    tlv_octet_t     tft;   /* Saved TFT */

    /* Related Context */
    mme_ue_t        *mme_ue;
    mme_sess_t      *sess;
    gtp_xact_t      *xact;
} mme_bearer_t;

CORE_DECLARE(status_t)      mme_context_init(void);
CORE_DECLARE(status_t)      mme_context_final(void);
CORE_DECLARE(mme_context_t*) mme_self(void);

CORE_DECLARE(status_t)      mme_context_parse_config(void);
CORE_DECLARE(status_t)      mme_context_setup_trace_module(void);

CORE_DECLARE(mme_sgw_t*)    mme_sgw_add(void);
CORE_DECLARE(status_t)      mme_sgw_remove(mme_sgw_t *sgw);
CORE_DECLARE(status_t)      mme_sgw_remove_all(void);
CORE_DECLARE(mme_sgw_t*)    mme_sgw_first(void);
CORE_DECLARE(mme_sgw_t*)    mme_sgw_next(mme_sgw_t *sgw);

CORE_DECLARE(mme_enb_t*)    mme_enb_add(sock_id sock);
CORE_DECLARE(status_t)      mme_enb_remove(mme_enb_t *enb);
CORE_DECLARE(status_t)      mme_enb_remove_all(void);
CORE_DECLARE(mme_enb_t*)    mme_enb_find(index_t index);
CORE_DECLARE(mme_enb_t*)    mme_enb_find_by_sock(sock_id sock);
CORE_DECLARE(mme_enb_t*)    mme_enb_find_by_enb_id(c_uint32_t enb_id);
CORE_DECLARE(status_t)      mme_enb_set_enb_id(
        mme_enb_t *enb, c_uint32_t enb_id);
CORE_DECLARE(hash_index_t *) mme_enb_first();
CORE_DECLARE(hash_index_t *) mme_enb_next(hash_index_t *hi);
CORE_DECLARE(mme_enb_t *)    mme_enb_this(hash_index_t *hi);

CORE_DECLARE(mme_ue_t*)     mme_ue_add(enb_ue_t *enb_ue);
CORE_DECLARE(status_t)      mme_ue_remove(mme_ue_t *mme_ue);
CORE_DECLARE(status_t)      mme_ue_remove_all();

CORE_DECLARE(mme_ue_t*)     mme_ue_find(index_t index);
CORE_DECLARE(mme_ue_t*)     mme_ue_find_by_imsi(c_uint8_t *imsi, int imsi_len);
CORE_DECLARE(mme_ue_t*)     mme_ue_find_by_imsi_bcd(c_int8_t *imsi_bcd);
CORE_DECLARE(mme_ue_t*)     mme_ue_find_by_guti(guti_t *guti);
CORE_DECLARE(mme_ue_t*)     mme_ue_find_by_teid(c_uint32_t teid);

CORE_DECLARE(mme_ue_t*)     mme_ue_find_by_message(nas_message_t *message);
CORE_DECLARE(status_t)      mme_ue_set_imsi(
                                mme_ue_t *mme_ue, c_int8_t *imsi_bcd);
CORE_DECLARE(status_t)      mme_ue_associate_enb_ue(
                                mme_ue_t *mme_ue, enb_ue_t *enb_ue);
CORE_DECLARE(status_t)      mme_ue_deassociate_enb_ue(enb_ue_t *enb_ue);
CORE_DECLARE(status_t)      source_ue_associate_target_ue(
                                enb_ue_t *source_ue, enb_ue_t *target_ue);
CORE_DECLARE(status_t)      source_ue_deassociate_target_ue(enb_ue_t *enb_ue);

CORE_DECLARE(hash_index_t *) mme_ue_first();
CORE_DECLARE(hash_index_t *) mme_ue_next(hash_index_t *hi);
CORE_DECLARE(mme_ue_t *)    mme_ue_this(hash_index_t *hi);

CORE_DECLARE(mme_sess_t*)   mme_sess_add(mme_ue_t *mme_ue, c_uint8_t pti);
CORE_DECLARE(status_t )     mme_sess_remove(mme_sess_t *sess);
CORE_DECLARE(status_t )     mme_sess_remove_all(mme_ue_t *mme_ue);
CORE_DECLARE(mme_sess_t*)   mme_sess_find(index_t index);
CORE_DECLARE(mme_sess_t*)   mme_sess_find_by_pti(
                                mme_ue_t *mme_ue, c_uint8_t pti);
CORE_DECLARE(mme_sess_t*)   mme_sess_find_by_ebi(
                                mme_ue_t *mme_ue, c_uint8_t ebi);
CORE_DECLARE(mme_sess_t*)   mme_sess_find_by_apn(
                                mme_ue_t *mme_ue, c_int8_t *apn);
CORE_DECLARE(mme_sess_t*)   mme_sess_first(mme_ue_t *mme_ue);
CORE_DECLARE(mme_sess_t*)   mme_sess_next(mme_sess_t *sess);

CORE_DECLARE(mme_bearer_t*) mme_bearer_add(mme_sess_t *sess);
CORE_DECLARE(status_t)      mme_bearer_remove(mme_bearer_t *bearer);
CORE_DECLARE(status_t)      mme_bearer_remove_all(mme_sess_t *sess);
CORE_DECLARE(mme_bearer_t*) mme_bearer_find(index_t index);
CORE_DECLARE(mme_bearer_t*) mme_bearer_find_by_sess_ebi(
                                mme_sess_t *sess, c_uint8_t ebi);
CORE_DECLARE(mme_bearer_t*) mme_bearer_find_by_ue_ebi(
                                mme_ue_t *mme_ue, c_uint8_t ebi);
CORE_DECLARE(mme_bearer_t*) mme_bearer_find_or_add_by_message(
                                mme_ue_t *mme_ue, nas_message_t *message);
CORE_DECLARE(mme_bearer_t*) mme_default_bearer_in_sess(mme_sess_t *sess);
CORE_DECLARE(mme_bearer_t*) mme_linked_bearer(mme_bearer_t *bearer);
CORE_DECLARE(mme_bearer_t*) mme_bearer_first(mme_sess_t *sess);
CORE_DECLARE(mme_bearer_t*) mme_bearer_next(mme_bearer_t *bearer);

CORE_DECLARE(pdn_t*)        mme_pdn_add(mme_ue_t *mme_ue, c_int8_t *apn);
CORE_DECLARE(status_t)      mme_pdn_remove_all(mme_ue_t *mme_ue);
CORE_DECLARE(pdn_t*)        mme_pdn_find_by_apn(
                                mme_ue_t *mme_ue, c_int8_t *apn);

CORE_DECLARE(enb_ue_t*)     enb_ue_add(mme_enb_t *enb);
CORE_DECLARE(unsigned int)  enb_ue_count();
CORE_DECLARE(status_t)      enb_ue_remove(enb_ue_t *enb_ue);
CORE_DECLARE(status_t)      enb_ue_remove_in_enb(mme_enb_t *enb);
CORE_DECLARE(status_t)      enb_ue_switch_to_enb(enb_ue_t *enb_ue, 
                                mme_enb_t *new_enb);
CORE_DECLARE(enb_ue_t*)     enb_ue_find(index_t index);
CORE_DECLARE(enb_ue_t*)     enb_ue_find_by_enb_ue_s1ap_id(mme_enb_t *enb, 
                                c_uint32_t enb_ue_s1ap_id);
CORE_DECLARE(enb_ue_t*)     enb_ue_find_by_mme_ue_s1ap_id(c_uint32_t mme_ue_s1ap_id);
CORE_DECLARE(enb_ue_t*)     enb_ue_first_in_enb(mme_enb_t *enb);
CORE_DECLARE(enb_ue_t*)     enb_ue_next_in_enb(enb_ue_t *enb_ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_CONTEXT__ */

#ifndef __MME_CTX_H__
#define __MME_CTX_H__

#include "core_list.h"
#include "core_errno.h"
#include "core_net.h"
#include "core_sha2.h"

#include "3gpp_defs.h"
#include "3gpp_types.h"
#include "nas_types.h"
#include "gtp_xact.h"

#include "mme_sm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_PLMN_ID                 6
#define GRP_PER_MME                 256    /* According to spec it is 65535 */
#define CODE_PER_MME                256    /* According to spec it is 256 */

typedef list_t ue_list_t;
typedef list_t erab_list_t;
typedef list_t s11_list_t;

typedef struct _served_gummei {
    c_uint32_t      num_of_plmn_id;
    plmn_id_t       plmn_id[MAX_PLMN_ID];

    c_uint32_t      num_of_mme_gid;
    c_uint16_t      mme_gid[GRP_PER_MME];
    c_uint32_t      num_of_mme_code;
    c_uint8_t       mme_code[CODE_PER_MME];
} srvd_gummei_t;

typedef struct _mme_ctx_t {
    c_uint32_t      s1ap_addr;  /* MME S1AP local address */
    c_uint16_t      s1ap_port;  /* MME S1AP local port */
    net_sock_t      *s1ap_sock; /* MME S1AP local listen socket */

    c_uint32_t      s11_addr;   /* MME S11 local address */
    c_uint16_t      s11_port;   /* MME S11 local port */
    net_sock_t      *s11_sock;  /* MME S11 local listen socket */

    msgq_id         queue_id;       /* Queue for processing MME control plane */
    tm_service_t    tm_service;     /* Timer Service */
    gtp_xact_ctx_t  gtp_xact_ctx;   /* GTP Transaction Context for MME */

    c_uint32_t      mme_ue_s1ap_id; /** mme_ue_s1ap_id generator */
    plmn_id_t       plmn_id;

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

    /* S1SetupRequest */
    c_uint16_t      tracking_area_code;
    c_uint16_t      default_paging_drx;

    /* S1SetupResponse */
    srvd_gummei_t   srvd_gummei;
    c_uint8_t       relative_capacity;
} mme_ctx_t;

typedef struct _sgw_ctx_t {
    gtp_node_t      gnode; /* SGW S11 remote GTPv2-C node */

} sgw_ctx_t;

typedef struct _enb_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint32_t      enb_id; /** eNB_ID received from eNB */

    s1ap_sm_t       s1ap_sm;
    net_sock_t      *s1ap_sock;

    ue_list_t       ue_list;

} enb_ctx_t;

typedef struct _ue_ctx_t {
    lnode_t         node; /**< A node of list_t */

    /* State Machine */
    emm_sm_t        emm_sm;
    esm_sm_t        esm_sm;

    /* UE identity */
    c_uint32_t      enb_ue_s1ap_id; /** eNB-UE-S1AP-ID received from eNB */
    c_uint32_t      mme_ue_s1ap_id; /** MME-UE-S1AP-ID received from MME */
    c_uint8_t       imsi[MAX_IMSI_LEN+1];
    c_uint8_t       imsi_len;

    /* Security Context */
    int             security_context_available;
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

    /* Related Context for UE */
    erab_list_t     erab_list;
    s11_list_t      s11_list;

    enb_ctx_t       *enb;
} ue_ctx_t;

/**
 * This structure represents E-RAB */
typedef struct _erab_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint32_t      erab_id;
    c_uint32_t      teid;

    ue_ctx_t        *ue;
} erab_ctx_t;

/**
 * This structure represents S11 */
typedef struct _s11_ctx_t {
    lnode_t         node; /**< A node of list_t */

    /* State Machine */
    s11_sm_t        s11_sm;

    c_uint32_t      teid;

    ue_ctx_t        *ue;
} s11_ctx_t;

CORE_DECLARE(status_t)      mme_ctx_init(void);
CORE_DECLARE(status_t)      mme_ctx_final(void);

CORE_DECLARE(mme_ctx_t*)    mme_self(void);

CORE_DECLARE(sgw_ctx_t*)    mme_ctx_sgw_add(void);
CORE_DECLARE(status_t)      mme_ctx_sgw_remove(sgw_ctx_t *sgw);
CORE_DECLARE(status_t)      mme_ctx_sgw_remove_all(void);
CORE_DECLARE(sgw_ctx_t*)    mme_ctx_sgw_find_by_node(gtp_node_t *gnode);
CORE_DECLARE(sgw_ctx_t*)    mme_ctx_sgw_first(void);
CORE_DECLARE(sgw_ctx_t*)    mme_ctx_sgw_next(sgw_ctx_t *sgw);

CORE_DECLARE(enb_ctx_t*)    mme_ctx_enb_add(void);
CORE_DECLARE(status_t)      mme_ctx_enb_remove(enb_ctx_t *enb);
CORE_DECLARE(status_t)      mme_ctx_enb_remove_all(void);
CORE_DECLARE(enb_ctx_t*)    mme_ctx_enb_find_by_sock(net_sock_t *sock);
CORE_DECLARE(enb_ctx_t*)    mme_ctx_enb_find_by_enb_id(c_uint32_t enb_id);
CORE_DECLARE(enb_ctx_t*)    mme_ctx_enb_first(void);
CORE_DECLARE(enb_ctx_t*)    mme_ctx_enb_next(enb_ctx_t *enb);

CORE_DECLARE(ue_ctx_t*)     mme_ctx_ue_add(enb_ctx_t *enb);
CORE_DECLARE(status_t)      mme_ctx_ue_remove(ue_ctx_t *ue);
CORE_DECLARE(status_t)      mme_ctx_ue_remove_all(enb_ctx_t *enb);
CORE_DECLARE(ue_ctx_t*)     mme_ctx_ue_find_by_enb_ue_s1ap_id(
                                enb_ctx_t *enb, c_uint32_t enb_ue_s1ap_id);
CORE_DECLARE(ue_ctx_t*)     mme_ctx_ue_first(enb_ctx_t *enb);
CORE_DECLARE(ue_ctx_t*)     mme_ctx_ue_next(ue_ctx_t *ue);

CORE_DECLARE(erab_ctx_t*)   mme_ctx_erab_add(ue_ctx_t *ue);
CORE_DECLARE(status_t)      mme_ctx_erab_remove(erab_ctx_t *erab);
CORE_DECLARE(status_t)      mme_ctx_erab_remove_all(ue_ctx_t *ue);
CORE_DECLARE(erab_ctx_t*)   mme_ctx_erab_find_by_erab_id(
                                ue_ctx_t *ue, c_uint32_t erab_id);
CORE_DECLARE(erab_ctx_t*)   mme_ctx_erab_find_by_teid(
                                ue_ctx_t *ue, c_uint32_t teid);
CORE_DECLARE(erab_ctx_t*)   mme_ctx_erab_first(ue_ctx_t *ue);
CORE_DECLARE(erab_ctx_t*)   mme_ctx_erab_next(erab_ctx_t *erab);

CORE_DECLARE(s11_ctx_t*)    mme_ctx_s11_add(ue_ctx_t *ue);
CORE_DECLARE(status_t)      mme_ctx_s11_remove(s11_ctx_t *s11);
CORE_DECLARE(status_t)      mme_ctx_s11_remove_all(ue_ctx_t *ue);
CORE_DECLARE(s11_ctx_t*)    mme_ctx_s11_find_by_teid(
                                ue_ctx_t *ue, c_uint32_t teid);
CORE_DECLARE(s11_ctx_t*)    mme_ctx_s11_first(ue_ctx_t *ue);
CORE_DECLARE(s11_ctx_t*)    mme_ctx_s11_next(s11_ctx_t *s11);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_CTX_H__ */

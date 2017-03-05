#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "core_list.h"
#include "core_errno.h"
#include "core_net.h"

#include "plmn_id.h"
#include "sm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_PLMN_ID                 6
#define GRP_PER_MME                 256    /* According to spec it is 65535 */
#define CODE_PER_MME                256    /* According to spec it is 256 */

typedef list_t ue_list_t;
typedef list_t rab_list_t;

typedef struct _served_gummei {
    c_uint32_t      num_of_plmn_id;
    plmn_id_t       plmn_id[MAX_PLMN_ID];

    c_uint32_t      num_of_mme_gid;
    c_uint16_t      mme_gid[GRP_PER_MME];
    c_uint32_t      num_of_mme_code;
    c_uint8_t       mme_code[CODE_PER_MME];
} srvd_gummei_t;

typedef struct _mme_ctx_t {
    net_sock_t      *enb_s1ap_sock;
    c_uint16_t      enb_s1ap_port;
    c_uint32_t      enb_local_addr; /** Network byte order */

    c_uint32_t      mme_ue_s1ap_id;

    plmn_id_t       plmn_id;

    /* S1SetupRequest */
    c_uint16_t      tracking_area_code;
    c_uint16_t      default_paging_drx;

    /* S1SetupResponse */
    srvd_gummei_t   srvd_gummei;
    c_uint8_t       relative_capacity;
} mme_ctx_t;

typedef struct _enb_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint32_t      enb_id; /** eNB_ID received from eNB */

    enb_s1ap_sm_t   s1ap_sm;
    net_sock_t      *s1ap_sock;

    ue_list_t       ue_list;

} enb_ctx_t;

typedef struct _ue_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint32_t      enb_ue_s1ap_id; /** eNB-UE-S1AP-ID received from eNB */
    c_uint32_t      mme_ue_s1ap_id; /** MME-UE-S1AP-ID received from MME */

    rab_list_t      rab_list;

    enb_ctx_t       *enb;
} ue_ctx_t;

/**
 * This structure represents RAB */
typedef struct _rab_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint32_t      e_rab_id;

    ue_ctx_t        *ue;
} rab_ctx_t;

CORE_DECLARE(status_t)      mme_ctx_init(void);
CORE_DECLARE(status_t)      mme_ctx_final(void);

CORE_DECLARE(mme_ctx_t*)    mme_self(void);

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

CORE_DECLARE(rab_ctx_t*)    mme_ctx_rab_add(ue_ctx_t *enb);
CORE_DECLARE(status_t)      mme_ctx_rab_remove(rab_ctx_t *ue);
CORE_DECLARE(status_t)      mme_ctx_rab_remove_all(ue_ctx_t *enb);
CORE_DECLARE(rab_ctx_t*)    mme_ctx_rab_find_by_e_rab_id(
                                ue_ctx_t *ue, c_uint32_t e_rab_id);
CORE_DECLARE(rab_ctx_t*)    mme_ctx_rab_first(ue_ctx_t *enb);
CORE_DECLARE(rab_ctx_t*)    mme_ctx_rab_next(rab_ctx_t *ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__CONTEXT_H__ */

#ifndef __CELLWIRE_CONTEXT_H__
#define __CELLWIRE_CONTEXT_H__

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
#define CODE_PER_MME                256    /* According to spec it is 256*/

#define CELL_PER_ENB                8
#define UE_PER_ENB                  128
#define RAB_PER_UE                  16

#define SIZE_OF_ENB_POOL            128
#define SIZE_OF_UE_POOL             (SIZE_OF_ENB_POOL * UE_PER_ENB)
#define SIZE_OF_RAB_POOL            (SIZE_OF_UE_POOL * RAB_PER_UE)

typedef list_t rab_list_t;

typedef struct _served_gummei {
    c_uint32_t      num_of_plmn_id;
    plmn_id_t       plmn_id[MAX_PLMN_ID];

    c_uint32_t      num_of_mme_gid;
    c_uint16_t      mme_gid[GRP_PER_MME];
    c_uint32_t      num_of_mme_code;
    c_uint8_t       mme_code[CODE_PER_MME];
} srvd_gummei_t;

/**
 * This structure represents HypcerCell */
typedef struct _mme_ctx_t {
    net_sock_t      *enb_s1_sock;
    c_uint16_t      enb_s1_port;
    c_uint32_t      enb_local_addr; /** Network byte order */

    plmn_id_t       plmn_id;

    /* S1SetupRequest */
    c_uint16_t      tracking_area_code;
    c_uint16_t      default_paging_drx;

    /* S1SetupResponse */
    srvd_gummei_t   srvd_gummei;
    c_uint8_t       relative_capacity;
} mme_ctx_t;

/**
 * This structure represents eNB */
typedef struct _enb_ctx_t {
    lnode_t         node; /**< A node of list_t */

    enb_s1_sm_t     s1_sm; /**< eNB S1 state machine */
    net_sock_t      *s1_sock;

    c_uint32_t      id;
} enb_ctx_t;

/**
 * This structure represents UE-S1 */
typedef struct _ue_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint32_t      enb_id; /** eNB-UE-S1AP-ID received from eNB */
    c_uint32_t      mme_id; /** MME-UE-S1AP-ID received from MME */

    rab_list_t      rab_list;

    enb_ctx_t       *enb;
    mme_ctx_t       *mme;
} ue_ctx_t;

/**
 * This structure represents RAB */
typedef struct _rab_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint32_t      id;

    ue_ctx_t        *ue;
} rab_ctx_t;

CORE_DECLARE(status_t)      context_init(void);
CORE_DECLARE(status_t)      context_final(void);

CORE_DECLARE(status_t)      context_read_lock(void);
CORE_DECLARE(status_t)      context_write_lock(void);
CORE_DECLARE(status_t)      context_unlock(void);

CORE_DECLARE(void)          context_post_cpath(void);
CORE_DECLARE(int)           context_fetch_cpath(void);

CORE_DECLARE(mme_ctx_t*)    mme_self(void);

CORE_DECLARE(enb_ctx_t*)    enb_ctx_add(void);
CORE_DECLARE(status_t)      enb_ctx_remove(enb_ctx_t *enb);
CORE_DECLARE(status_t)      enb_ctx_remove_all(void);
CORE_DECLARE(enb_ctx_t*)    enb_ctx_find_by_sock(net_sock_t *sock);
CORE_DECLARE(enb_ctx_t*)    enb_ctx_find_by_id(c_uint32_t id);
CORE_DECLARE(enb_ctx_t*)    enb_ctx_first(void);
CORE_DECLARE(enb_ctx_t*)    enb_ctx_next(enb_ctx_t *enb);
#define self() mme_self()

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__CELLWIRE_CONTEXT_H__ */

#define TRACE_MODULE _ctx
#include "core_debug.h"
#include "core_pool.h"
#include "core_rwlock.h"
#include "core_index.h"

#include "cellwire.h"
#include "context.h"
#include "s1ap_message.h"

static mme_ctx_t self;

pool_declare(enb_pool, enb_ctx_t, SIZE_OF_ENB_POOL);
pool_declare(ue_pool, ue_ctx_t, SIZE_OF_UE_POOL);
pool_declare(rab_pool, rab_ctx_t, SIZE_OF_RAB_POOL);

static int g_mme_ctx_initialized = 0;

static list_t g_enb_list;

static rwlock_id g_rwlock;
static int g_cpath_post_q;

status_t context_init()
{
    d_assert(g_mme_ctx_initialized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    pool_init(&enb_pool, SIZE_OF_ENB_POOL);
    pool_init(&ue_pool, SIZE_OF_UE_POOL);
    pool_init(&rab_pool, SIZE_OF_RAB_POOL);

    list_init(&g_enb_list);

    rwlock_create(&g_rwlock);
    d_assert(g_rwlock, return CORE_ERROR, "RW-lock creation failed");

    /* Initialize MME context */
    memset(&self, 0, sizeof(mme_ctx_t));

    self.enb_local_addr = inet_addr("127.0.0.1");
    self.enb_s1_port = S1_SCTP_PORT;

    self.plmn_id.mnc_len = 2;
    self.plmn_id.mcc = 1; /* 001 */
    self.plmn_id.mnc = 1; /* 01 */
    self.tracking_area_code = 12345;
    self.default_paging_drx = S1ap_PagingDRX_v64;
    self.relative_capacity = 0xff;

    self.srvd_gummei.num_of_plmn_id = 1;
    self.srvd_gummei.plmn_id[0].mnc_len = 2;
    self.srvd_gummei.plmn_id[0].mcc = 1; /* 001 */
    self.srvd_gummei.plmn_id[0].mnc = 1; /* 01 */

    self.srvd_gummei.num_of_mme_gid = 1;
    self.srvd_gummei.mme_gid[0] = 2;
    self.srvd_gummei.num_of_mme_code = 1;
    self.srvd_gummei.mme_code[0] = 1;

    g_mme_ctx_initialized = 1;

    return CORE_OK;
}

status_t context_final()
{
    d_assert(g_mme_ctx_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    enb_ctx_remove_all();

    pool_final(&enb_pool);
    pool_final(&ue_pool);
    pool_final(&rab_pool);

    g_mme_ctx_initialized = 0;

    return CORE_OK;
}

status_t context_read_lock()
{
    return rwlock_rdlock(g_rwlock);
}

status_t context_write_lock()
{
    return rwlock_wrlock(g_rwlock);
}

status_t context_unlock()
{
    return rwlock_unlock(g_rwlock);
}

void context_post_cpath()
{
    g_cpath_post_q = 1;
}

int context_fetch_cpath()
{
    if (g_cpath_post_q)
    {
        g_cpath_post_q = 0;
        return 1;
    }

    return 0;
}

mme_ctx_t* mme_self()
{
    return &self;
}

enb_ctx_t* enb_ctx_add()
{

    enb_ctx_t *enb = NULL;

    /* Allocate new eNB context */
    pool_alloc_node(&enb_pool, &enb);
    d_assert(enb, return NULL, "eNB context allocation failed");

    /* Initialize eNB context */
    memset(enb, 0, sizeof(enb_ctx_t));

    /* Add new eNB context to list */
    list_append(&g_enb_list, enb);
    
    return enb;
}

status_t enb_ctx_remove(enb_ctx_t *enb)
{
    d_assert(enb, return CORE_ERROR, "Null param");

    list_remove(&g_enb_list, enb);
    pool_free_node(&enb_pool, enb);

    return CORE_OK;
}

status_t enb_ctx_remove_all()
{
    enb_ctx_t *enb = NULL, *next_enb = NULL;
    
    enb = list_first(&g_enb_list);
    while (enb)
    {
        next_enb = list_next(enb);

        enb_ctx_remove(enb);

        enb = next_enb;
    }

    return CORE_OK;
}

enb_ctx_t* enb_ctx_find_by_sock(net_sock_t *sock)
{
    enb_ctx_t *enb = NULL;
    
    enb = list_first(&g_enb_list);
    while (enb)
    {
        if (sock == enb->s1_sock)
            break;

        enb = list_next(enb);
    }

    return enb;
}

enb_ctx_t* enb_ctx_find_by_id(c_uint32_t id)
{
    enb_ctx_t *enb = NULL;
    
    enb = list_first(&g_enb_list);
    while (enb)
    {
        if (id == enb->id)
            break;

        enb = list_next(enb);
    }

    return enb;
}

enb_ctx_t* enb_ctx_first()
{
    return list_first(&g_enb_list);
}

enb_ctx_t* enb_ctx_next(enb_ctx_t *enb)
{
    return list_next(enb);
}

#define TRACE_MODULE _ctx
#include "core_debug.h"
#include "core_pool.h"
#include "core_rwlock.h"
#include "core_index.h"

#include "cellwire.h"
#include "context.h"

static mme_ctx_t self;

static int g_mme_ctx_initialized = 0;

static rwlock_id g_rwlock;
static int g_cpath_post_q;

status_t context_init()
{
    d_assert(g_mme_ctx_initialized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    rwlock_create(&g_rwlock);
    d_assert(g_rwlock, return CORE_ERROR, "RW-lock creation failed");

    /* Initialize MME context */
    memset(&self, 0, sizeof(mme_ctx_t));

    self.plmn_id.mnc_len = 2;
    self.plmn_id.mcc = 1; /* 001 */
    self.plmn_id.mnc = 1; /* 01 */
    self.relative_capacity = 0xff;

    self.srvd_gummei.num_of_plmn_id = 1;
    self.srvd_gummei.plmn_id[0].mnc_len = 2;
    self.srvd_gummei.plmn_id[0].mcc = 1; /* 001 */
    self.srvd_gummei.plmn_id[0].mnc = 1; /* 01 */

    self.srvd_gummei.num_of_grp_id = 1;
    self.srvd_gummei.grp_id[0] = 2;
    self.srvd_gummei.num_of_code = 1;
    self.srvd_gummei.code[0] = 1;

    g_mme_ctx_initialized = 1;

    return CORE_OK;
}

status_t context_final()
{
    d_assert(g_mme_ctx_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

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

#define TRACE_MODULE _s6a_hss_ctx

#include "core_debug.h"
#include "core_pool.h"

#include "hss_ctx.h"

pool_declare(ue_pool, ue_ctx_t, SIZE_OF_UE_POOL);

static list_t g_ue_list;

status_t hss_ctx_init(void)
{
    pool_init(&ue_pool, SIZE_OF_UE_POOL);
	
	return CORE_OK;
}

void hss_ctx_final(void)
{
    pool_final(&ue_pool);
	
	return;
}

ue_ctx_t* hss_ue_ctx_add()
{
    ue_ctx_t *ue = NULL;

    /* Allocate new eNB context */
    pool_alloc_node(&ue_pool, &ue);
    d_assert(ue, return NULL, "HSS-UE context allocation failed");

    /* Initialize eNB context */
    memset(ue, 0, sizeof(ue_ctx_t));

    /* Add new eNB context to list */
    list_append(&g_ue_list, ue);
    
    return ue;
}

status_t hss_ue_ctx_remove(ue_ctx_t *ue)
{
    d_assert(ue, return CORE_ERROR, "Null param");

    list_remove(&g_ue_list, ue);
    pool_free_node(&ue_pool, ue);

    return CORE_OK;
}

status_t hss_ue_ctx_remove_all()
{
    ue_ctx_t *ue = NULL, *next_ue = NULL;
    
    ue = list_first(&g_ue_list);
    while (ue)
    {
        next_ue = list_next(ue);

        hss_ue_ctx_remove(ue);

        ue = next_ue;
    }

    return CORE_OK;
}

ue_ctx_t* hss_ue_ctx_find_by_imsi(c_uint8_t *imsi)
{
    ue_ctx_t *ue = NULL;
    
    ue = list_first(&g_ue_list);
    while (ue)
    {
        if (memcmp(ue->imsi, imsi, ue->imsi_len) == 0)
            break;

        ue = list_next(ue);
    }

    return ue;
}

ue_ctx_t* hss_ue_ctx_first()
{
    return list_first(&g_ue_list);
}

ue_ctx_t* hss_ue_ctx_next(ue_ctx_t *ue)
{
    return list_next(ue);
}

#define TRACE_MODULE _hss_ctx

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"

#include "context.h"

#define OP "5F1D289C5D354D0A140C2548F5F3E3BA"
#define OPc "E8ED289DEBA952E4283B54E88E6183CA"
#define AMF "8000"

static hss_ctx_t self;

pool_declare(ue_pool, ue_ctx_t, SIZE_OF_UE_POOL);

static list_t g_ue_list;

hss_ctx_t* hss_self()
{
    return &self;
}

status_t hss_ctx_init(void)
{
    char buf[MAX_KEY_LEN];

    pool_init(&ue_pool, SIZE_OF_UE_POOL);

    memset(&self, 0, sizeof(hss_ctx_t));

    memcpy(self.op, core_ascii_to_hex(OP, strlen(OP), buf), MAX_KEY_LEN);
    memcpy(self.amf, core_ascii_to_hex(AMF, strlen(AMF), buf), MAX_AMF_LEN);
	
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

    memcpy(ue->op, self.op, MAX_KEY_LEN);
    memcpy(ue->amf, self.amf, MAX_AMF_LEN);
    
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

ue_ctx_t* hss_ue_ctx_find_by_imsi(c_uint8_t *imsi, c_uint8_t imsi_len)
{
    ue_ctx_t *ue = NULL;
    
    ue = list_first(&g_ue_list);
    while (ue)
    {
        if (memcmp(ue->imsi, imsi, imsi_len) == 0)
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

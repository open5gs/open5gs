#define TRACE_MODULE _mme_ctx

#include "core_debug.h"
#include "core_pool.h"
#include "core_rwlock.h"
#include "core_index.h"

#include "context.h"
#include "s1ap_message.h"

#define CELL_PER_ENB                8
#define UE_PER_ENB                  128
#define RAB_PER_UE                  16

#define SIZE_OF_ENB_POOL            128
#define SIZE_OF_UE_POOL             (SIZE_OF_ENB_POOL * UE_PER_ENB)
#define SIZE_OF_RAB_POOL            (SIZE_OF_UE_POOL * RAB_PER_UE)

#define S1_SCTP_PORT                36412
#define GTP_C_UDP_PORT              2123
#define GTP_U_UDP_PORT              2152

static mme_ctx_t self;

pool_declare(enb_pool, enb_ctx_t, SIZE_OF_ENB_POOL);
pool_declare(ue_pool, ue_ctx_t, SIZE_OF_UE_POOL);
pool_declare(rab_pool, rab_ctx_t, SIZE_OF_RAB_POOL);

static int g_mme_ctx_initialized = 0;

static list_t g_enb_list;

status_t mme_ctx_init()
{
    d_assert(g_mme_ctx_initialized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    pool_init(&enb_pool, SIZE_OF_ENB_POOL);
    pool_init(&ue_pool, SIZE_OF_UE_POOL);
    pool_init(&rab_pool, SIZE_OF_RAB_POOL);

    list_init(&g_enb_list);

    /* Initialize MME context */
    memset(&self, 0, sizeof(mme_ctx_t));

    self.enb_local_addr = inet_addr("127.0.0.1");
    self.enb_s1ap_port = S1_SCTP_PORT;

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

status_t mme_ctx_final()
{
    d_assert(g_mme_ctx_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    mme_ctx_enb_remove_all();

    pool_final(&enb_pool);
    pool_final(&ue_pool);
    pool_final(&rab_pool);

    g_mme_ctx_initialized = 0;

    return CORE_OK;
}

mme_ctx_t* mme_self()
{
    return &self;
}

enb_ctx_t* mme_ctx_enb_add()
{
    enb_ctx_t *enb = NULL;

    pool_alloc_node(&enb_pool, &enb);
    d_assert(enb, return NULL, "Null param");

    memset(enb, 0, sizeof(enb_ctx_t));

    list_init(&enb->ue_list);

    list_append(&g_enb_list, enb);
    
    return enb;
}

status_t mme_ctx_enb_remove(enb_ctx_t *enb)
{
    d_assert(enb, return CORE_ERROR, "Null param");

    mme_ctx_ue_remove_all(enb);

    list_remove(&g_enb_list, enb);
    pool_free_node(&enb_pool, enb);

    return CORE_OK;
}

status_t mme_ctx_enb_remove_all()
{
    enb_ctx_t *enb = NULL, *next_enb = NULL;
    
    enb = mme_ctx_enb_first();
    while (enb)
    {
        next_enb = mme_ctx_enb_next(enb);

        mme_ctx_enb_remove(enb);

        enb = next_enb;
    }

    return CORE_OK;
}

enb_ctx_t* mme_ctx_enb_find_by_sock(net_sock_t *sock)
{
    enb_ctx_t *enb = NULL;
    
    enb = mme_ctx_enb_first();
    while (enb)
    {
        if (sock == enb->s1ap_sock)
            break;

        enb = mme_ctx_enb_next(enb);
    }

    return enb;
}

enb_ctx_t* mme_ctx_enb_find_by_enb_id(c_uint32_t enb_id)
{
    enb_ctx_t *enb = NULL;
    
    enb = list_first(&g_enb_list);
    while (enb)
    {
        if (enb_id == enb->enb_id)
            break;

        enb = list_next(enb);
    }

    return enb;
}

enb_ctx_t* mme_ctx_enb_first()
{
    return list_first(&g_enb_list);
}

enb_ctx_t* mme_ctx_enb_next(enb_ctx_t *enb)
{
    return list_next(enb);
}

ue_ctx_t* mme_ctx_ue_add(enb_ctx_t *enb)
{
    ue_ctx_t *ue = NULL;

    d_assert(enb, return NULL, "Null param");

    pool_alloc_node(&ue_pool, &ue);
    d_assert(ue, return NULL, "Null param");

    memset(ue, 0, sizeof(ue_ctx_t));

    ue->enb = enb;
    list_init(&ue->rab_list);

    list_append(&enb->ue_list, ue);
    
    return ue;
}

status_t mme_ctx_ue_remove(ue_ctx_t *ue)
{
    d_assert(ue, return CORE_ERROR, "Null param");
    d_assert(ue->enb, return CORE_ERROR, "Null param");

    mme_ctx_rab_remove_all(ue);

    list_remove(&ue->enb->ue_list, ue);
    pool_free_node(&ue_pool, ue);

    return CORE_OK;
}

status_t mme_ctx_ue_remove_all(enb_ctx_t *enb)
{
    ue_ctx_t *ue = NULL, *next_ue = NULL;
    
    ue = mme_ctx_ue_first(enb);
    while (ue)
    {
        next_ue = mme_ctx_ue_next(ue);

        mme_ctx_ue_remove(ue);

        ue = next_ue;
    }

    return CORE_OK;
}

ue_ctx_t* mme_ctx_ue_find_by_enb_ue_s1ap_id(
        enb_ctx_t *enb, c_uint32_t enb_ue_s1ap_id)
{
    ue_ctx_t *ue = NULL;
    
    ue = mme_ctx_ue_first(enb);
    while (ue)
    {
        if (enb_ue_s1ap_id == ue->enb_ue_s1ap_id)
            break;

        ue = mme_ctx_ue_next(ue);
    }

    return ue;
}

ue_ctx_t* mme_ctx_ue_first(enb_ctx_t *enb)
{
    return list_first(&enb->ue_list);
}

ue_ctx_t* mme_ctx_ue_next(ue_ctx_t *ue)
{
    return list_next(ue);
}

rab_ctx_t* mme_ctx_rab_add(ue_ctx_t *ue)
{
    rab_ctx_t *rab = NULL;

    d_assert(ue, return NULL, "Null param");

    pool_alloc_node(&rab_pool, &rab);
    d_assert(rab, return NULL, "Null param");

    memset(rab, 0, sizeof(rab_ctx_t));

    rab->ue = ue;

    list_append(&ue->rab_list, rab);
    
    return rab;
}

status_t mme_ctx_rab_remove(rab_ctx_t *rab)
{
    d_assert(rab, return CORE_ERROR, "Null param");
    d_assert(rab->ue, return CORE_ERROR, "Null param");

    list_remove(&rab->ue->rab_list, rab);
    pool_free_node(&rab_pool, rab);

    return CORE_OK;
}

status_t mme_ctx_rab_remove_all(ue_ctx_t *ue)
{
    rab_ctx_t *rab = NULL, *next_rab = NULL;
    
    rab = mme_ctx_rab_first(ue);
    while (rab)
    {
        next_rab = mme_ctx_rab_next(rab);

        mme_ctx_rab_remove(rab);

        rab = next_rab;
    }

    return CORE_OK;
}

rab_ctx_t* mme_ctx_rab_find_by_e_rab_id(
        ue_ctx_t *ue, c_uint32_t e_rab_id)
{
    rab_ctx_t *rab = NULL;
    
    rab = mme_ctx_rab_first(ue);
    while (rab)
    {
        if (e_rab_id == rab->e_rab_id)
            break;

        rab = mme_ctx_rab_next(rab);
    }

    return rab;
}

rab_ctx_t* mme_ctx_rab_first(ue_ctx_t *ue)
{
    return list_first(&ue->rab_list);
}

rab_ctx_t* mme_ctx_rab_next(rab_ctx_t *rab)
{
    return list_next(rab);
}

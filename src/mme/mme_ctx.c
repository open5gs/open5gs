#define TRACE_MODULE _mme_ctx

#include "core_debug.h"
#include "core_pool.h"

#include "3gpp_conv.h"
#include "gtp_path.h"
#include "s1ap_message.h"

#include "mme_ctx.h"

#define MAX_CELL_PER_ENB            8
#define MAX_ERAB_PER_UE             16

#define MAX_NUM_OF_SGW              8
#define MAX_NUM_OF_ERAB             (MAX_NUM_OF_UE * MAX_ERAB_PER_UE)

#define S1AP_SCTP_PORT              36412

static mme_ctx_t self;

pool_declare(sgw_pool, sgw_ctx_t, MAX_NUM_OF_SGW);
pool_declare(enb_pool, enb_ctx_t, MAX_NUM_OF_ENB);
pool_declare(ue_pool, ue_ctx_t, MAX_NUM_OF_UE);
pool_declare(erab_pool, erab_ctx_t, MAX_NUM_OF_ERAB);

static int ctx_initialized = 0;

static list_t sgw_list;
static list_t enb_list;

status_t mme_ctx_init()
{
    d_assert(ctx_initialized == 0, return CORE_ERROR,
            "MME context already has been ctx_initialized");

    pool_init(&sgw_pool, MAX_NUM_OF_SGW);
    pool_init(&enb_pool, MAX_NUM_OF_ENB);
    pool_init(&ue_pool, MAX_NUM_OF_UE);
    pool_init(&erab_pool, MAX_NUM_OF_ERAB);

    list_init(&sgw_list);
    list_init(&enb_list);

    /* Initialize MME context */
    memset(&self, 0, sizeof(mme_ctx_t));

    self.s1ap_addr = inet_addr("127.0.0.1");
    self.s1ap_port = S1AP_SCTP_PORT;

    sgw_ctx_t *sgw = mme_ctx_sgw_add();
    d_assert(sgw, return CORE_ERROR, "Can't add SGW context");

    self.s11_addr = inet_addr("127.0.0.1");
    self.s11_port = GTPV2_C_UDP_PORT;

    sgw->gnode.addr = inet_addr("127.0.0.1");
    sgw->gnode.port = GTPV2_C_UDP_PORT+1;

    /* MCC : 001, MNC : 01 */
    plmn_id_build(&self.plmn_id, 1, 1, 2); 
    self.tracking_area_code = 12345;
    self.default_paging_drx = S1ap_PagingDRX_v64;
    self.relative_capacity = 0xff;

    self.srvd_gummei.num_of_plmn_id = 1;
    /* MCC : 001, MNC : 01 */
    plmn_id_build(&self.srvd_gummei.plmn_id[0], 1, 1, 2); 

    self.srvd_gummei.num_of_mme_gid = 1;
    self.srvd_gummei.mme_gid[0] = 2;
    self.srvd_gummei.num_of_mme_code = 1;
    self.srvd_gummei.mme_code[0] = 1;

    self.selected_enc_algorithm = NAS_SECURITY_ALGORITHMS_EEA0;
    self.selected_int_algorithm = NAS_SECURITY_ALGORITHMS_128_EIA1;

    ctx_initialized = 1;

    return CORE_OK;
}

status_t mme_ctx_final()
{
    d_assert(ctx_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    mme_ctx_sgw_remove_all();
    mme_ctx_enb_remove_all();

    pool_final(&sgw_pool);
    pool_final(&enb_pool);
    pool_final(&ue_pool);
    pool_final(&erab_pool);

    ctx_initialized = 0;

    return CORE_OK;
}

mme_ctx_t* mme_self()
{
    return &self;
}

sgw_ctx_t* mme_ctx_sgw_add()
{
    sgw_ctx_t *sgw = NULL;

    pool_alloc_node(&sgw_pool, &sgw);
    d_assert(sgw, return NULL, "Null param");

    memset(sgw, 0, sizeof(sgw_ctx_t));

    list_init(&sgw->gnode.local_list);
    list_init(&sgw->gnode.remote_list);

    list_append(&sgw_list, sgw);
    
    return sgw;
}

status_t mme_ctx_sgw_remove(sgw_ctx_t *sgw)
{
    d_assert(sgw, return CORE_ERROR, "Null param");

    list_remove(&sgw_list, sgw);
    pool_free_node(&sgw_pool, sgw);

    return CORE_OK;
}

status_t mme_ctx_sgw_remove_all()
{
    sgw_ctx_t *sgw = NULL, *next_sgw = NULL;
    
    sgw = mme_ctx_sgw_first();
    while (sgw)
    {
        next_sgw = mme_ctx_sgw_next(sgw);

        mme_ctx_sgw_remove(sgw);

        sgw = next_sgw;
    }

    return CORE_OK;
}

sgw_ctx_t* mme_ctx_sgw_find_by_node(gtp_node_t *gnode)
{
    sgw_ctx_t *sgw = NULL;
    
    sgw = mme_ctx_sgw_first();
    while (sgw)
    {
        if (GTP_COMPARE_NODE(&sgw->gnode, gnode))
            break;

        sgw = mme_ctx_sgw_next(sgw);
    }

    return sgw;
}

sgw_ctx_t* mme_ctx_sgw_first()
{
    return list_first(&sgw_list);
}

sgw_ctx_t* mme_ctx_sgw_next(sgw_ctx_t *sgw)
{
    return list_next(sgw);
}

enb_ctx_t* mme_ctx_enb_add()
{
    enb_ctx_t *enb = NULL;

    pool_alloc_node(&enb_pool, &enb);
    d_assert(enb, return NULL, "Null param");

    memset(enb, 0, sizeof(enb_ctx_t));

    list_init(&enb->ue_list);

    list_append(&enb_list, enb);
    
    return enb;
}

status_t mme_ctx_enb_remove(enb_ctx_t *enb)
{
    d_assert(enb, return CORE_ERROR, "Null param");

    mme_ctx_ue_remove_all(enb);

    list_remove(&enb_list, enb);
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
    
    enb = list_first(&enb_list);
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
    return list_first(&enb_list);
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
    list_init(&ue->erab_list);
    list_init(&ue->s11_list);

    if (self.mme_ue_s1ap_id == 0) self.mme_ue_s1ap_id = 1;
    ue->mme_ue_s1ap_id = self.mme_ue_s1ap_id;

    self.mme_ue_s1ap_id = self.mme_ue_s1ap_id + 1;

    list_append(&enb->ue_list, ue);
    
    return ue;
}

status_t mme_ctx_ue_remove(ue_ctx_t *ue)
{
    d_assert(ue, return CORE_ERROR, "Null param");
    d_assert(ue->enb, return CORE_ERROR, "Null param");

    mme_ctx_erab_remove_all(ue);

    if (FSM_STATE(&ue->emm_sm))
    {
        fsm_final((fsm_t*)&ue->emm_sm, 0);
        fsm_clear((fsm_t*)&ue->emm_sm);
    }
    if (FSM_STATE(&ue->esm_sm))
    {
        fsm_final((fsm_t*)&ue->esm_sm, 0);
        fsm_clear((fsm_t*)&ue->esm_sm);
    }

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

erab_ctx_t* mme_ctx_erab_add(ue_ctx_t *ue)
{
    erab_ctx_t *erab = NULL;

    d_assert(ue, return NULL, "Null param");

    pool_alloc_node(&erab_pool, &erab);
    d_assert(erab, return NULL, "Null param");

    memset(erab, 0, sizeof(erab_ctx_t));

    erab->ue = ue;

    list_append(&ue->erab_list, erab);
    
    return erab;
}

status_t mme_ctx_erab_remove(erab_ctx_t *erab)
{
    d_assert(erab, return CORE_ERROR, "Null param");
    d_assert(erab->ue, return CORE_ERROR, "Null param");

    list_remove(&erab->ue->erab_list, erab);
    pool_free_node(&erab_pool, erab);

    return CORE_OK;
}

status_t mme_ctx_erab_remove_all(ue_ctx_t *ue)
{
    erab_ctx_t *erab = NULL, *next_erab = NULL;
    
    erab = mme_ctx_erab_first(ue);
    while (erab)
    {
        next_erab = mme_ctx_erab_next(erab);

        mme_ctx_erab_remove(erab);

        erab = next_erab;
    }

    return CORE_OK;
}

erab_ctx_t* mme_ctx_erab_find_by_erab_id(ue_ctx_t *ue, c_uint32_t erab_id)
{
    erab_ctx_t *erab = NULL;
    
    erab = mme_ctx_erab_first(ue);
    while (erab)
    {
        if (erab_id == erab->erab_id)
            break;

        erab = mme_ctx_erab_next(erab);
    }

    return erab;
}

erab_ctx_t* mme_ctx_erab_find_by_teid(ue_ctx_t *ue, c_uint32_t teid)
{
    erab_ctx_t *erab = NULL;
    
    erab = mme_ctx_erab_first(ue);
    while (erab)
    {
        if (teid == erab->teid)
            break;

        erab = mme_ctx_erab_next(erab);
    }

    return erab;
}

erab_ctx_t* mme_ctx_erab_first(ue_ctx_t *ue)
{
    return list_first(&ue->erab_list);
}

erab_ctx_t* mme_ctx_erab_next(erab_ctx_t *erab)
{
    return list_next(erab);
}

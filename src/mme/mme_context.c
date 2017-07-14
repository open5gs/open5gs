#define TRACE_MODULE _mme_context

#include "core_debug.h"
#include "core_pool.h"

#include "gtp_path.h"
#include "s1ap_message.h"

#include "context.h"
#include "mme_context.h"
#include "mme_event.h"

#define MAX_CELL_PER_ENB            8

#define MAX_NUM_OF_SGW              8

#define S1AP_SCTP_PORT              36412

#define MIN_EPS_BEARER_ID           5
#define MAX_EPS_BEARER_ID           15

static mme_context_t self;

pool_declare(mme_sgw_pool, mme_sgw_t, MAX_NUM_OF_SGW);

index_declare(mme_enb_pool, mme_enb_t, MAX_NUM_OF_ENB);
index_declare(mme_ue_pool, mme_ue_t, MAX_NUM_OF_UE);
index_declare(enb_ue_pool, enb_ue_t, MAX_NUM_OF_UE);
index_declare(mme_sess_pool, mme_sess_t, MAX_NUM_OF_UE);
index_declare(mme_bearer_pool, mme_bearer_t, MAX_NUM_OF_UE_BEARER);
pool_declare(mme_pdn_pool, pdn_t, MAX_NUM_OF_UE_PDN);

static int context_initialized = 0;

status_t mme_context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "MME context already has been context_initialized");

    /* Initialize MME context */
    memset(&self, 0, sizeof(mme_context_t));

    pool_init(&mme_sgw_pool, MAX_NUM_OF_SGW);
    list_init(&self.sgw_list);

    index_init(&mme_enb_pool, MAX_NUM_OF_ENB);
    list_init(&self.enb_list);

    index_init(&mme_ue_pool, MAX_NUM_OF_UE);
    index_init(&enb_ue_pool, MAX_NUM_OF_UE);
    index_init(&mme_sess_pool, MAX_NUM_OF_UE);
    index_init(&mme_bearer_pool, MAX_NUM_OF_UE_BEARER);
    pool_init(&mme_pdn_pool, MAX_NUM_OF_UE_PDN);

    self.mme_ue_s1ap_id_hash = hash_make();
    self.imsi_ue_hash = hash_make();
    self.guti_ue_hash = hash_make();

    context_initialized = 1;

    return CORE_OK;
}

static status_t mme_context_prepare()
{
    self.relative_capacity = 0xff;

    self.s1ap_port = S1AP_SCTP_PORT;
    self.s11_port = GTPV2_C_UDP_PORT;

    return CORE_OK;
}

static status_t mme_context_validation()
{
    if (self.s1ap_addr == 0)
    {
        d_error("No MME.NEWORK.S1AP_ADDR in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.s11_addr == 0)
    {
        d_error("No MME.NEWORK.S11_ADDR in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    mme_sgw_t *sgw = mme_sgw_first();
    if (sgw == NULL)
    {
        d_error("No SGW.NEWORK in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    while(sgw)
    {
        if (sgw->gnode.addr == 0)
        {
            d_error("No SGW.NEWORK.S11_ADDR in '%s'",
                    context_self()->config.path);
            return CORE_ERROR;
        }
        sgw = mme_sgw_next(sgw);
    }

    if (self.max_num_of_served_gummei == 0)
    {
        d_error("No MME.GUMMEI in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_plmn_id == 0)
    {
        d_error("No MME.GUMMEI.PLMN_ID in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_mme_gid == 0)
    {
        d_error("No MME.GUMMEI.MME_GID in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_mme_code == 0)
    {
        d_error("No MME.GUMMEI.MME_CODE in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.max_num_of_served_tai == 0)
    {
        d_error("No MME.TAI(PLMN_ID.MCC.MNC|TAC) in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.num_of_integrity_order == 0)
    {
        d_error("No MME.SECURITY.INTEGRITY_ORDER in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.num_of_ciphering_order == 0)
    {
        d_error("No MME.SECURITY.CIPHERING_ORDER in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t mme_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;

    char *json = config->json;
    jsmntok_t *token = config->token;

    typedef enum { 
        START, ROOT, 
        MME_START, MME_ROOT, 
        SGW_START, SGW_ROOT, 
        SKIP, STOP 
    } parse_state;
    parse_state state = START;
    parse_state stack = STOP;

    size_t root_tokens = 0;
    size_t mme_tokens = 0;
    size_t sgw_tokens = 0;
    size_t skip_tokens = 0;
    int i, j, m, n, p, q;
    int arr, size, arr1, size1;

    rv = mme_context_prepare();
    if (rv != CORE_OK) return rv;

    for (i = 0, j = 1; j > 0; i++, j--)
    {
        jsmntok_t *t = &token[i];

        j += t->size;

        switch (state)
        {
            case START:
            {
                state = ROOT;
                root_tokens = t->size;

                break;
            }
            case ROOT:
            {
                if (jsmntok_equal(json, t, "MME") == 0)
                {
                    state = MME_START;
                }
                else if (jsmntok_equal(json, t, "SGW") == 0)
                {
                    state = SGW_START;
                }
                else
                {
                    state = SKIP;
                    stack = ROOT;
                    skip_tokens = t->size;
                }

                root_tokens--;
                if (root_tokens == 0) state = STOP;
                break;
            }
            case MME_START:
            {
                state = MME_ROOT;
                mme_tokens = t->size;

                break;
            }
            case MME_ROOT:
            {
                if (jsmntok_equal(json, t, "RELATIVE_CAPACITY") == 0)
                {
                    char *v = jsmntok_to_string(json, t+1);
                    if (v) self.relative_capacity = atoi(v);
                }
                else if (jsmntok_equal(json, t, "NETWORK") == 0)
                {
                    m = 1;
                    size = 1;

                    if ((t+1)->type == JSMN_ARRAY)
                    {
                        m = 2;
                    }

                    for (arr = 0; arr < size; arr++)
                    {
                        for (n = 1; n > 0; m++, n--)
                        {
                            n += (t+m)->size;

                            if (jsmntok_equal(json, t+m, "S1AP_ADDR") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s1ap_addr = inet_addr(v);
                            }
                            else if (jsmntok_equal(json, t+m, "S1AP_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s1ap_port = atoi(v);
                            }
                            else if (jsmntok_equal(json, t+m, "S11_ADDR") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s11_addr = inet_addr(v);
                            }
                            else if (jsmntok_equal(json, t+m, "S11_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s11_port = atoi(v);
                            }
                        }
                    }
                }
                else if (jsmntok_equal(json, t, "GUMMEI") == 0)
                {
                    m = 1;
                    size = 1;

                    if ((t+1)->type == JSMN_ARRAY)
                    {
                        m = 2;
                        size = (t+1)->size;
                    }

                    self.max_num_of_served_gummei = size;
                    for (arr = 0; arr < size; arr++)
                    {
                        served_gummei_t *gummei = &self.served_gummei[arr];
                        for (n = 1; n > 0; m++, n--)
                        {
                            n += (t+m)->size;

                            if (jsmntok_equal(json, t+m, "PLMN_ID") == 0)
                            {
                                p = 1;
                                size1 = 1;

                                if ((t+m+1)->type == JSMN_ARRAY)
                                {
                                    p = 2;
                                    size1 = (t+m+1)->size;
                                }

                                for (arr1 = 0; arr1 < size1; arr1++)
                                {
                                    char *mcc = NULL, *mnc = NULL;
                                    for (q = 1; q > 0; p++, q--)
                                    {
                                        q += (t+m+p)->size;
                                        if (jsmntok_equal(
                                                    json, t+m+p, "MCC") == 0)
                                        {
                                            mcc = jsmntok_to_string(
                                                    json, t+m+p+1);
                                        }
                                        else if (jsmntok_equal(
                                                    json, t+m+p, "MNC") == 0)
                                        {
                                            mnc = jsmntok_to_string(
                                                    json, t+m+p+1);
                                        }
                                    }

                                    if (mcc && mnc)
                                    {
                                        plmn_id_build(&gummei->
                                            plmn_id[gummei->num_of_plmn_id],
                                            atoi(mcc), 
                                            atoi(mnc), strlen(mnc));
                                        gummei->num_of_plmn_id++;
                                    }
                                }
                            } 
                            else if (jsmntok_equal(json, t+m, "MME_GID") == 0)
                            {
                                p = 1;
                                size1 = 1;

                                if ((t+m+1)->type == JSMN_ARRAY)
                                {
                                    p = 2;
                                    size1 = (t+m+1)->size;
                                }

                                for (arr1 = 0; arr1 < size1; arr1++)
                                {
                                    char *v = jsmntok_to_string(json, t+m+p);
                                    if (v) 
                                    {
                                        gummei->mme_gid
                                            [gummei->num_of_mme_gid] = atoi(v);
                                        gummei->num_of_mme_gid++;
                                    }
                                    p++;
                                }
                            }
                            else if (jsmntok_equal(json, t+m, "MME_CODE") == 0)
                            {
                                p = 1;
                                size1 = 1;

                                if ((t+m+1)->type == JSMN_ARRAY)
                                {
                                    p = 2;
                                    size1 = (t+m+1)->size;
                                }

                                for (arr1 = 0; arr1 < size1; arr1++)
                                {
                                    char *v = jsmntok_to_string(json, t+m+p);
                                    if (v) 
                                    {
                                        gummei->mme_code
                                            [gummei->num_of_mme_code] = atoi(v);
                                        gummei->num_of_mme_code++;
                                    }
                                    p++;
                                }
                            }
                        }
                    }
                }
                else if (jsmntok_equal(json, t, "TAI") == 0)
                {
                    m = 1;
                    size = 1;

                    if ((t+1)->type == JSMN_ARRAY)
                    {
                        m = 2;
                        size = (t+1)->size;
                    }

                    for (arr = 0; arr < size; arr++)
                    {
                        char *mcc = NULL, *mnc = NULL, *tac = NULL;

                        for (n = 1; n > 0; m++, n--)
                        {
                            n += (t+m)->size;

                            if (jsmntok_equal(json, t+m, "PLMN_ID") == 0)
                            {
                                p = 1;
                                size1 = 1;

                                if ((t+m+1)->type == JSMN_ARRAY)
                                {
                                    p = 2;
                                }

                                for (arr1 = 0; arr1 < size1; arr1++)
                                {
                                    for (q = 1; q > 0; p++, q--)
                                    {
                                        q += (t+m+p)->size;
                                        if (jsmntok_equal(
                                                    json, t+m+p, "MCC") == 0)
                                        {
                                            mcc = jsmntok_to_string(
                                                    json, t+m+p+1);
                                        }
                                        else if (jsmntok_equal(
                                                    json, t+m+p, "MNC") == 0)
                                        {
                                            mnc = jsmntok_to_string(
                                                    json, t+m+p+1);
                                        }
                                    }
                                }
                            }
                            else if (jsmntok_equal(json, t+m, "TAC") == 0)
                            {
                                p = 1;

                                if ((t+m+1)->type == JSMN_ARRAY)
                                {
                                    p = 2;
                                }

                                tac = jsmntok_to_string(json, t+m+p);
                            }
                        }

                        if (mcc && mnc && tac)
                        {
                            tai_t *tai = &self.served_tai[
                                self.max_num_of_served_tai];
                           
                            plmn_id_build(&tai->plmn_id,
                                atoi(mcc), atoi(mnc), strlen(mnc));
                            tai->tac = atoi(tac);

                            self.max_num_of_served_tai++;
                        }
                    }
                }
                else if (jsmntok_equal(json, t, "SECURITY") == 0)
                {
                    for (m = 1, n = 1; n > 0; m++, n--)
                    {
                        n += (t+m)->size;
                        if (jsmntok_equal(json, t+m, "INTEGRITY_ORDER") == 0)
                        {
                            p = 1;
                            size = 1;

                            if ((t+m+1)->type == JSMN_ARRAY)
                            {
                                p = 2;
                                size = (t+m+1)->size;
                            }

                            for (arr = 0; arr < size; arr++)
                            {
                                char *v = jsmntok_to_string(json, t+m+p);
                                if (v) 
                                {
                                    if (strcmp(v, "EIA0") == 0)
                                    {
                                        self.integrity_order[arr] = 
                                            NAS_SECURITY_ALGORITHMS_EIA0;
                                        self.num_of_integrity_order++;
                                    }
                                    else if (strcmp(v, "EIA1") == 0)
                                    {
                                        self.integrity_order[arr] = 
                                            NAS_SECURITY_ALGORITHMS_128_EIA1;
                                        self.num_of_integrity_order++;
                                    }
                                    else if (strcmp(v, "EIA2") == 0)
                                    {
                                        self.integrity_order[arr] = 
                                            NAS_SECURITY_ALGORITHMS_128_EIA2;
                                        self.num_of_integrity_order++;
                                    }
                                    else if (strcmp(v, "EIA3") == 0)
                                    {
                                        self.integrity_order[arr] = 
                                            NAS_SECURITY_ALGORITHMS_128_EIA3;
                                        self.num_of_integrity_order++;
                                    }
                                }

                                p++;
                            }
                        }
                        else if (jsmntok_equal(json, t+m, "CIPHERING_ORDER") == 0)
                        {
                            p = 1;
                            size = 1;

                            if ((t+m+1)->type == JSMN_ARRAY)
                            {
                                p = 2;
                                size = (t+m+1)->size;
                            }

                            for (arr = 0; arr < size; arr++)
                            {
                                char *v = jsmntok_to_string(json, t+m+p);
                                if (v) 
                                {
                                    if (strcmp(v, "EEA0") == 0)
                                    {
                                        self.ciphering_order[arr] = 
                                            NAS_SECURITY_ALGORITHMS_EEA0;
                                        self.num_of_ciphering_order++;
                                    }
                                    else if (strcmp(v, "EEA1") == 0)
                                    {
                                        self.ciphering_order[arr] = 
                                            NAS_SECURITY_ALGORITHMS_128_EEA1;
                                        self.num_of_ciphering_order++;
                                    }
                                    else if (strcmp(v, "EEA2") == 0)
                                    {
                                        self.ciphering_order[arr] = 
                                            NAS_SECURITY_ALGORITHMS_128_EEA2;
                                        self.num_of_ciphering_order++;
                                    }
                                    else if (strcmp(v, "EEA3") == 0)
                                    {
                                        self.ciphering_order[arr] = 
                                            NAS_SECURITY_ALGORITHMS_128_EEA3;
                                        self.num_of_ciphering_order++;
                                    }
                                }
                                p++;
                            }
                        }
                    }
                }

                state = SKIP;
                stack = MME_ROOT;
                skip_tokens = t->size;

                mme_tokens--;
                if (mme_tokens == 0) stack = ROOT;
                break;
            }
            case SGW_START:
            {
                state = SGW_ROOT;
                sgw_tokens = t->size;

                break;
            }
            case SGW_ROOT:
            {
                if (jsmntok_equal(json, t, "NETWORK") == 0)
                {
                    m = 1;
                    size = 1;

                    if ((t+1)->type == JSMN_ARRAY)
                    {
                        m = 2;
                        size = (t+1)->size;
                    }

                    for (arr = 0; arr < size; arr++)
                    {
                        mme_sgw_t *sgw = mme_sgw_add();
                        d_assert(sgw, return CORE_ERROR, 
                                "Can't add SGW context");
                        sgw->gnode.port = GTPV2_C_UDP_PORT;

                        for (n = 1; n > 0; m++, n--)
                        {
                            n += (t+m)->size;

                            if (jsmntok_equal(json, t+m, "S11_ADDR") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) sgw->gnode.addr = inet_addr(v);
                            }
                            else if (jsmntok_equal(json, t+m, "S11_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) sgw->gnode.port = atoi(v);
                            }
                        }
                    }
                }

                state = SKIP;
                stack = SGW_ROOT;
                skip_tokens = t->size;

                sgw_tokens--;
                if (sgw_tokens == 0) stack = ROOT;
                break;
            }
            case SKIP:
            {
                skip_tokens += t->size;

                skip_tokens--;
                if (skip_tokens == 0) state = stack;
                break;
            }
            case STOP:
            {
                break;
            }
            default:
            {
                d_error("Failed to parse configuration in the state(%u)", 
                        state);
                break;
            }

        }
    }

    rv = mme_context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

status_t mme_context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "MME context already has been finalized");

    mme_sgw_remove_all();
    mme_enb_remove_all();

    d_assert(self.mme_ue_s1ap_id_hash, , "Null param");
    hash_destroy(self.mme_ue_s1ap_id_hash);

    d_assert(self.imsi_ue_hash, , "Null param");
    hash_destroy(self.imsi_ue_hash);

    d_assert(self.guti_ue_hash, , "Null param");
    hash_destroy(self.guti_ue_hash);

    pool_final(&mme_pdn_pool);
    index_final(&mme_bearer_pool);
    index_final(&mme_sess_pool);
    index_final(&mme_ue_pool);
    index_final(&enb_ue_pool);

    index_final(&mme_enb_pool);
    pool_final(&mme_sgw_pool);

    context_initialized = 0;

    return CORE_OK;
}

mme_context_t* mme_self()
{
    return &self;
}

mme_sgw_t* mme_sgw_add()
{
    mme_sgw_t *sgw = NULL;

    pool_alloc_node(&mme_sgw_pool, &sgw);
    d_assert(sgw, return NULL, "Null param");

    memset(sgw, 0, sizeof(mme_sgw_t));

    list_init(&sgw->gnode.local_list);
    list_init(&sgw->gnode.remote_list);

    list_append(&self.sgw_list, sgw);
    
    return sgw;
}

status_t mme_sgw_remove(mme_sgw_t *sgw)
{
    d_assert(sgw, return CORE_ERROR, "Null param");

    gtp_xact_delete_all(&sgw->gnode);

    list_remove(&self.sgw_list, sgw);
    pool_free_node(&mme_sgw_pool, sgw);

    return CORE_OK;
}

status_t mme_sgw_remove_all()
{
    mme_sgw_t *sgw = NULL, *next_sgw = NULL;
    
    sgw = mme_sgw_first();
    while (sgw)
    {
        next_sgw = mme_sgw_next(sgw);

        mme_sgw_remove(sgw);

        sgw = next_sgw;
    }

    return CORE_OK;
}

mme_sgw_t* mme_sgw_find_by_node(gtp_node_t *gnode)
{
    mme_sgw_t *sgw = NULL;
    
    sgw = mme_sgw_first();
    while (sgw)
    {
        if (GTP_COMPARE_NODE(&sgw->gnode, gnode))
            break;

        sgw = mme_sgw_next(sgw);
    }

    return sgw;
}

mme_sgw_t* mme_sgw_first()
{
    return list_first(&self.sgw_list);
}

mme_sgw_t* mme_sgw_next(mme_sgw_t *sgw)
{
    return list_next(sgw);
}

mme_enb_t* mme_enb_add(net_sock_t *s1ap_sock)
{
    mme_enb_t *enb = NULL;

    index_alloc(&mme_enb_pool, &enb);
    d_assert(enb, return NULL, "Null param");

    /* IMPORTANT! 
     * eNB Index is saved in net_sock_t structure */
    s1ap_sock->app_index = enb->index;

    enb->s1ap_sock = s1ap_sock;
    list_init(&enb->enb_ue_list);
    list_append(&self.enb_list, enb);

    fsm_create(&enb->sm, s1ap_state_initial, s1ap_state_final);
    fsm_init(&enb->sm, 0);
    
    return enb;
}

status_t mme_enb_remove(mme_enb_t *enb)
{
    d_assert(enb, return CORE_ERROR, "Null param");

    fsm_final(&enb->sm, 0);
    fsm_clear(&enb->sm);

    enb_ue_remove_in_enb(enb);

    net_unregister_sock(enb->s1ap_sock);
    net_close(enb->s1ap_sock);

    list_remove(&self.enb_list, enb);
    index_free(&mme_enb_pool, enb);

    return CORE_OK;
}

status_t mme_enb_remove_all()
{
    mme_enb_t *enb = NULL, *next_enb = NULL;
    
    enb = mme_enb_first();
    while (enb)
    {
        next_enb = mme_enb_next(enb);

        mme_enb_remove(enb);

        enb = next_enb;
    }

    return CORE_OK;
}

mme_enb_t* mme_enb_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&mme_enb_pool, index);
}

mme_enb_t* mme_enb_find_by_sock(net_sock_t *sock)
{
    mme_enb_t *enb = NULL;
    
    enb = mme_enb_first();
    while (enb)
    {
        if (sock == enb->s1ap_sock)
            break;

        enb = mme_enb_next(enb);
    }

    return enb;
}

mme_enb_t* mme_enb_find_by_enb_id(c_uint32_t enb_id)
{
    mme_enb_t *enb = NULL;
    
    enb = list_first(&self.enb_list);
    while (enb)
    {
        if (enb_id == enb->enb_id)
            break;

        enb = list_next(enb);
    }

    return enb;
}

mme_enb_t* mme_enb_first()
{
    return list_first(&self.enb_list);
}

mme_enb_t* mme_enb_next(mme_enb_t *enb)
{
    return list_next(enb);
}

/** enb_ue_context handling function */
enb_ue_t* enb_ue_add(mme_enb_t *enb)
{
    enb_ue_t *enb_ue = NULL;

    d_assert(self.mme_ue_s1ap_id_hash, return NULL, "Null param");
    d_assert(enb, return NULL, "Null param");

    index_alloc(&enb_ue_pool, &enb_ue);
    d_assert(enb_ue, return NULL, "Null param");

    enb_ue->mme_ue_s1ap_id = NEXT_ID(self.mme_ue_s1ap_id, 1, 0xffffffff);
    hash_set(self.mme_ue_s1ap_id_hash, &enb_ue->mme_ue_s1ap_id, 
            sizeof(enb_ue->mme_ue_s1ap_id), enb_ue);
    list_append(&enb->enb_ue_list, enb_ue);

    enb_ue->enb = enb;

    return enb_ue;

}

unsigned int enb_ue_count()
{
    d_assert(self.mme_ue_s1ap_id_hash, return 0, "Null param");
    return hash_count(self.mme_ue_s1ap_id_hash);
}

status_t enb_ue_remove(enb_ue_t *enb_ue)
{
    d_assert(self.mme_ue_s1ap_id_hash, return CORE_ERROR, "Null param");
    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ue->enb, return CORE_ERROR, "Null param");

    list_remove(&enb_ue->enb->enb_ue_list, enb_ue);
    hash_set(self.mme_ue_s1ap_id_hash, &enb_ue->mme_ue_s1ap_id, 
            sizeof(enb_ue->mme_ue_s1ap_id), NULL);

    index_free(&enb_ue_pool, enb_ue);

    if (enb_ue->mme_ue)
        enb_ue->mme_ue->enb_ue = NULL;

    return CORE_OK;
}

status_t enb_ue_remove_in_enb(mme_enb_t *enb)
{
    enb_ue_t *enb_ue = NULL, *next_enb_ue = NULL;
    
    enb_ue = enb_ue_first_in_enb(enb);
    while (enb_ue)
    {
        next_enb_ue = enb_ue_next_in_enb(enb_ue);

        enb_ue_remove(enb_ue);

        enb_ue = next_enb_ue;
    }

    return CORE_OK;
}

enb_ue_t* enb_ue_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&enb_ue_pool, index);
}

enb_ue_t* enb_ue_find_by_enb_ue_s1ap_id(
        mme_enb_t *enb, c_uint32_t enb_ue_s1ap_id)
{
    enb_ue_t *enb_ue = NULL;
    
    enb_ue = enb_ue_first_in_enb(enb);
    while (enb_ue)
    {
        if (enb_ue_s1ap_id == enb_ue->enb_ue_s1ap_id)
            break;

        enb_ue = enb_ue_next_in_enb(enb_ue);
    }

    return enb_ue;
}

enb_ue_t* enb_ue_find_by_mme_ue_s1ap_id(c_uint32_t mme_ue_s1ap_id)
{
    d_assert(self.mme_ue_s1ap_id_hash, return NULL, "Null param");
    return hash_get(self.mme_ue_s1ap_id_hash, 
            &mme_ue_s1ap_id, sizeof(mme_ue_s1ap_id));
}

enb_ue_t* enb_ue_first_in_enb(mme_enb_t *enb)
{
    return list_first(&enb->enb_ue_list);
}

enb_ue_t* enb_ue_next_in_enb(enb_ue_t *enb_ue)
{
    return list_next(enb_ue);
}

mme_ue_t* mme_ue_add(enb_ue_t *enb_ue)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(enb_ue, return NULL, "Null param");

    index_alloc(&mme_ue_pool, &mme_ue);
    d_assert(mme_ue, return NULL, "Null param");

    mme_ue->ebi = MIN_EPS_BEARER_ID - 1; /* Setup EBI Generator */

    list_init(&mme_ue->pdn_list);
    list_init(&mme_ue->sess_list);

    mme_ue->enb_ue = enb_ue;
    enb_ue->mme_ue = mme_ue;
#define MME_UE_T3_DURATION     3000 /* 3 seconds */
    mme_ue->tm_t3 = event_timer(&self.tm_service, MME_EVT_EMM_UE_T3,
                MME_UE_T3_DURATION, mme_ue->index);
#if 1 /* example code : please remove if you know the usage */
    tm_start(mme_ue->tm_t3);
    tm_stop(mme_ue->tm_t3);
#endif

    fsm_create(&mme_ue->sm, emm_state_initial, emm_state_final);
    fsm_init(&mme_ue->sm, 0);
    
    return mme_ue;
}

status_t mme_ue_remove(mme_ue_t *mme_ue)
{
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    tm_delete(mme_ue->tm_t3);

    fsm_final(&mme_ue->sm, 0);
    fsm_clear(&mme_ue->sm);

    mme_sess_remove_all(mme_ue);
    mme_pdn_remove_all(mme_ue);

    index_free(&mme_ue_pool, mme_ue);

    return CORE_OK;
}

status_t mme_ue_remove_all()
{
    hash_index_t *hi = NULL;
    mme_ue_t *mme_ue = NULL;

    for (hi = mme_ue_first(); hi; hi = mme_ue_next(hi))
    {
        mme_ue = mme_ue_this(hi);
        mme_ue_remove(mme_ue);
    }

    return CORE_OK;
}

mme_ue_t* mme_ue_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&mme_ue_pool, index);
}

mme_ue_t* mme_ue_find_by_imsi(c_uint8_t *imsi, int imsi_len)
{
    d_assert(imsi && imsi_len, return NULL,"Invalid Param");

    return (mme_ue_t *)hash_get(self.imsi_ue_hash, imsi, imsi_len);
}

mme_ue_t* mme_ue_find_by_guti(guti_t *guti)
{
    d_assert(guti, return NULL,"Invalid Param");

    return (mme_ue_t *)hash_get(self.guti_ue_hash, guti, sizeof(guti_t));
}

hash_index_t *mme_ue_first()
{
    d_assert(self.imsi_ue_hash, return NULL, "Null param");
    return hash_first(self.imsi_ue_hash);
}

hash_index_t *mme_ue_next(hash_index_t *hi)
{
    return hash_next(hi);
}

mme_ue_t *mme_ue_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
}

#if 0
unsigned int mme_ue_count()
{
    d_assert(self.mme_ue_s1ap_id_hash, return 0, "Null param");
    return hash_count(self.mme_ue_s1ap_id_hash);
}

status_t mme_ue_remove_in_enb(mme_enb_t *enb)
{
    mme_ue_t *mme_ue = NULL, *next_mme_ue = NULL;
    
    mme_ue = mme_ue_first_in_enb(enb);
    while (mme_ue)
    {
        next_mme_ue = mme_ue_next_in_enb(mme_ue);

        mme_ue_remove(mme_ue);

        mme_ue = next_mme_ue;
    }

    return CORE_OK;
}

mme_ue_t* mme_ue_find_by_enb_ue_s1ap_id(
        mme_enb_t *enb, c_uint32_t enb_ue_s1ap_id)
{
    mme_ue_t *mme_ue = NULL;
    
    mme_ue = mme_ue_first_in_enb(enb);
    while (mme_ue)
    {
        if (enb_ue_s1ap_id == mme_ue->enb_ue_s1ap_id)
            break;

        mme_ue = mme_ue_next_in_enb(mme_ue);
    }

    return mme_ue;
}

mme_ue_t* mme_ue_first_in_enb(mme_enb_t *enb)
{
    return list_first(&enb->enb_ue_list);
}

mme_ue_t* mme_ue_next_in_enb(mme_ue_t *mme_ue)
{
    return list_next(mme_ue);
}
#endif

mme_bearer_t *mme_sess_add(mme_ue_t *mme_ue, c_uint8_t pti)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    index_alloc(&mme_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->mme_s11_teid = sess->index;
    sess->mme_s11_addr = mme_self()->s11_addr;

    list_init(&sess->bearer_list);
    list_append(&mme_ue->sess_list, sess);

    sess->mme_ue = mme_ue;

    bearer = mme_bearer_add(sess, pti);
    d_assert(bearer, mme_sess_remove(sess); return NULL, 
            "Can't add default bearer context");

    return bearer;
}

status_t mme_sess_remove(mme_sess_t *sess)
{
    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(sess->mme_ue, return CORE_ERROR, "Null param");

    mme_bearer_remove_all(sess);

    list_remove(&sess->mme_ue->sess_list, sess);
    index_free(&mme_sess_pool, sess);

    return CORE_OK;
}

status_t mme_sess_remove_all(mme_ue_t *mme_ue)
{
    mme_sess_t *sess = NULL, *next_sess = NULL;
    
    sess = mme_sess_first(mme_ue);
    while (sess)
    {
        next_sess = mme_sess_next(sess);

        mme_sess_remove(sess);

        sess = next_sess;
    }

    return CORE_OK;
}

mme_sess_t* mme_sess_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&mme_sess_pool, index);
}

mme_sess_t* mme_sess_find_by_teid(c_uint32_t teid)
{
    return mme_sess_find(teid);
}

mme_sess_t* mme_sess_find_by_ebi(mme_ue_t *mme_ue, c_uint8_t ebi)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    
    sess = mme_sess_first(mme_ue);
    while (sess)
    {
        bearer = mme_bearer_first(sess);
        while (bearer)
        {
            if (ebi == bearer->ebi)
                return sess;

            bearer = mme_bearer_next(bearer);
        }

        sess = mme_sess_next(sess);
    }

    return NULL;
}

mme_sess_t* mme_sess_first(mme_ue_t *mme_ue)
{
    return list_first(&mme_ue->sess_list);
}

mme_sess_t* mme_sess_next(mme_sess_t *sess)
{
    return list_next(sess);
}

mme_bearer_t* mme_bearer_add(mme_sess_t *sess, c_uint8_t pti)
{
    mme_bearer_t *bearer = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(sess, return NULL, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return NULL, "Null param");

    index_alloc(&mme_bearer_pool, &bearer);
    d_assert(bearer, return NULL, "Null param");

    bearer->pti = pti;
    bearer->ebi = NEXT_ID(mme_ue->ebi, MIN_EPS_BEARER_ID, MAX_EPS_BEARER_ID);

    list_append(&sess->bearer_list, bearer);
    
    bearer->mme_ue = mme_ue;
    bearer->sess = sess;

    fsm_create(&bearer->sm, esm_state_initial, esm_state_final);
    fsm_init(&bearer->sm, 0);

    return bearer;
}

status_t mme_bearer_remove(mme_bearer_t *bearer)
{
    d_assert(bearer, return CORE_ERROR, "Null param");
    d_assert(bearer->sess, return CORE_ERROR, "Null param");
    
    fsm_final(&bearer->sm, 0);
    fsm_clear(&bearer->sm);

    list_remove(&bearer->sess->bearer_list, bearer);
    index_free(&mme_bearer_pool, bearer);

    return CORE_OK;
}

status_t mme_bearer_remove_all(mme_sess_t *sess)
{
    mme_bearer_t *bearer = NULL, *next_bearer = NULL;

    d_assert(sess, return CORE_ERROR, "Null param");
    
    bearer = mme_bearer_first(sess);
    while (bearer)
    {
        next_bearer = mme_bearer_next(bearer);

        mme_bearer_remove(bearer);

        bearer = next_bearer;
    }

    return CORE_OK;
}

mme_bearer_t* mme_bearer_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&mme_bearer_pool, index);
}

mme_bearer_t* mme_bearer_find_by_ue_pti(mme_ue_t *mme_ue, c_uint8_t pti)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    d_assert(mme_ue, return NULL, "Null param");

    sess = mme_sess_first(mme_ue);
    while (sess)
    {
        bearer = mme_bearer_first(sess);
        while (bearer)
        {
            if (pti == bearer->pti)
                return bearer;

            bearer = mme_bearer_next(bearer);
        }

        sess = mme_sess_next(sess);
    }

    return NULL;
}

mme_bearer_t* mme_bearer_find_by_ue_ebi(mme_ue_t *mme_ue, c_uint8_t ebi)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    d_assert(mme_ue, return NULL, "Null param");

    sess = mme_sess_first(mme_ue);
    while (sess)
    {
        bearer = mme_bearer_first(sess);
        while (bearer)
        {
            if (ebi == bearer->ebi)
                return bearer;

            bearer = mme_bearer_next(bearer);
        }

        sess = mme_sess_next(sess);
    }

    return NULL;
}

mme_bearer_t* mme_bearer_find_by_sess_ebi(mme_sess_t *sess, c_uint8_t ebi)
{
    mme_bearer_t *bearer = NULL;

    d_assert(sess, return NULL, "Null param");

    bearer = mme_bearer_first(sess);
    while (bearer)
    {
        if (ebi == bearer->ebi)
            return bearer;

        bearer = mme_bearer_next(bearer);
    }

    return NULL;
}

mme_bearer_t* mme_default_bearer_in_sess(mme_sess_t *sess)
{
    return mme_bearer_first(sess);
}

mme_bearer_t* mme_bearer_first(mme_sess_t *sess)
{
    d_assert(sess, return NULL, "Null param");

    return list_first(&sess->bearer_list);
}

mme_bearer_t* mme_bearer_next(mme_bearer_t *bearer)
{
    return list_next(bearer);
}

pdn_t* mme_pdn_add(mme_ue_t *mme_ue, c_int8_t *apn)
{
    pdn_t *pdn = NULL;

    d_assert(mme_ue, return NULL, "Null param");
    d_assert(apn, return NULL, "Null param");

    pool_alloc_node(&mme_pdn_pool, &pdn);
    d_assert(pdn, return NULL, "PDN context allocation failed");

    memset(pdn, 0, sizeof(pdn_t));
    strcpy(pdn->apn, apn);
    
    pdn->context = mme_ue;
    list_append(&mme_ue->pdn_list, pdn);

    return pdn;
}

status_t mme_pdn_remove(pdn_t *pdn)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(pdn, return CORE_ERROR, "Null param");
    mme_ue = pdn->context;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    list_remove(&mme_ue->pdn_list, pdn);
    pool_free_node(&mme_pdn_pool, pdn);

    return CORE_OK;
}

status_t mme_pdn_remove_all(mme_ue_t *mme_ue)
{
    pdn_t *pdn = NULL, *next_pdn = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    
    pdn = list_first(&mme_ue->pdn_list);
    while (pdn)
    {
        next_pdn = list_next(pdn);

        mme_pdn_remove(pdn);

        pdn = next_pdn;
    }

    return CORE_OK;
}

pdn_t* mme_pdn_find_by_apn(mme_ue_t *mme_ue, c_int8_t *apn)
{
    pdn_t *pdn = NULL;
    
    d_assert(mme_ue, return NULL, "Null param");

    pdn = list_first(&mme_ue->pdn_list);
    while (pdn)
    {
        if (strcmp(pdn->apn, apn) == 0)
            break;

        pdn = list_next(pdn);
    }

    return pdn;
}

pdn_t* mme_pdn_first(mme_ue_t *mme_ue)
{
    d_assert(mme_ue, return NULL, "Null param");
    return list_first(&mme_ue->pdn_list);
}

pdn_t* mme_pdn_next(pdn_t *pdn)
{
    return list_next(pdn);
}


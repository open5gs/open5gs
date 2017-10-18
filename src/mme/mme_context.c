#define TRACE_MODULE _mme_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"

#include "gtp_path.h"
#include "s1ap_message.h"
#include "fd_lib.h"

#include "context.h"
#include "nas_conv.h"
#include "mme_context.h"
#include "mme_event.h"

#define MAX_CELL_PER_ENB            8

#define S1AP_SCTP_PORT              36412

static mme_context_t self;

pool_declare(mme_sgw_pool, mme_sgw_t, MAX_NUM_OF_GTP_NODE);

index_declare(mme_enb_pool, mme_enb_t, MAX_NUM_OF_ENB);
index_declare(mme_ue_pool, mme_ue_t, MAX_POOL_OF_UE);
index_declare(enb_ue_pool, enb_ue_t, MAX_POOL_OF_UE);
index_declare(mme_sess_pool, mme_sess_t, MAX_POOL_OF_SESS);
index_declare(mme_bearer_pool, mme_bearer_t, MAX_POOL_OF_BEARER);

static int context_initialized = 0;

status_t mme_context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "MME context already has been context_initialized");

    /* Initialize MME context */
    memset(&self, 0, sizeof(mme_context_t));

    pool_init(&mme_sgw_pool, MAX_NUM_OF_GTP_NODE);
    list_init(&self.sgw_list);

    index_init(&mme_enb_pool, MAX_NUM_OF_ENB);
    index_init(&mme_ue_pool, MAX_POOL_OF_UE);
    index_init(&enb_ue_pool, MAX_POOL_OF_UE);
    index_init(&mme_sess_pool, MAX_POOL_OF_SESS);
    index_init(&mme_bearer_pool, MAX_POOL_OF_BEARER);

    self.s1ap_sock_hash = hash_make();
    self.enb_id_hash = hash_make();
    self.mme_ue_s1ap_id_hash = hash_make();
    self.imsi_ue_hash = hash_make();
    self.guti_ue_hash = hash_make();

    /* Timer value */
    self.t3413_value = 2; /* Paging retry Timer: 2 secs */

    context_initialized = 1;

    return CORE_OK;
}

status_t mme_context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "MME context already has been finalized");

    mme_sgw_remove_all();
    mme_enb_remove_all();
    mme_ue_remove_all();

    d_assert(self.s1ap_sock_hash, , "Null param");
    hash_destroy(self.s1ap_sock_hash);
    d_assert(self.enb_id_hash, , "Null param");
    hash_destroy(self.enb_id_hash);

    d_assert(self.mme_ue_s1ap_id_hash, , "Null param");
    hash_destroy(self.mme_ue_s1ap_id_hash);
    d_assert(self.imsi_ue_hash, , "Null param");
    hash_destroy(self.imsi_ue_hash);
    d_assert(self.guti_ue_hash, , "Null param");
    hash_destroy(self.guti_ue_hash);

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

static status_t mme_context_prepare()
{
    self.relative_capacity = 0xff;

    self.s1ap_port = S1AP_SCTP_PORT;
    self.gtpc_port = GTPV2_C_UDP_PORT;
    self.s5c_port = GTPV2_C_UDP_PORT;

    return CORE_OK;
}

static status_t mme_context_validation()
{
    if (self.fd_conf_path == NULL)
    {
        d_error("No MME.FD_CONF_PATH in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.s1ap_addr == 0)
    {
        d_error("No MME.NEWORK.S1AP_IPV4 in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.gtpc_addr == 0)
    {
        d_error("No MME.NEWORK.GTPC_IPV4 in '%s'",
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
        if (sgw->addr == 0)
        {
            d_error("No SGW.NEWORK.GTPC_IPV4 in '%s'",
                    context_self()->config.path);
            return CORE_ERROR;
        }
        sgw = mme_sgw_next(sgw);
    }
    self.sgw = mme_sgw_first();

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
        PGW_START, PGW_ROOT, 
        SKIP, STOP 
    } parse_state;
    parse_state state = START;
    parse_state stack = STOP;

    size_t root_tokens = 0;
    size_t mme_tokens = 0;
    size_t sgw_tokens = 0;
    size_t pgw_tokens = 0;
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
                else if (jsmntok_equal(json, t, "PGW") == 0)
                {
                    state = PGW_START;
                }
                else
                {
                    state = SKIP;
                    stack = ROOT;
                    skip_tokens = t->size;

                    root_tokens--;
                    if (root_tokens == 0) state = STOP;
                }

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
                else if (jsmntok_equal(json, t, "FD_CONF_PATH") == 0)
                {
                    self.fd_conf_path = jsmntok_to_string(json, t+1);
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

                            if (jsmntok_equal(json, t+m, "S1AP_IPV4") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s1ap_addr = inet_addr(v);
                            }
                            else if (jsmntok_equal(json, t+m, "S1AP_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s1ap_port = atoi(v);
                            }
                            else if (jsmntok_equal(json, t+m, "GTPC_IPV4") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.gtpc_addr = inet_addr(v);
                            }
                            else if (jsmntok_equal(json, t+m, "GTPC_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.gtpc_port = atoi(v);
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
                        sgw->port = GTPV2_C_UDP_PORT;

                        for (n = 1; n > 0; m++, n--)
                        {
                            n += (t+m)->size;

                            if (jsmntok_equal(json, t+m, "GTPC_IPV4") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) sgw->addr = inet_addr(v);
                            }
                            else if (jsmntok_equal(json, t+m, "GTPC_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) sgw->port = atoi(v);
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
            case PGW_START:
            {
                state = PGW_ROOT;
                pgw_tokens = t->size;

                break;
            }
            case PGW_ROOT:
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
                        for (n = 1; n > 0; m++, n--)
                        {
                            n += (t+m)->size;

                            if (jsmntok_equal(json, t+m, "GTPC_IPV4") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s5c_addr = inet_addr(v);
                            }
                            else if (jsmntok_equal(json, t+m, "GTPC_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s5c_port = atoi(v);
                            }
                        }
                    }
                }

                state = SKIP;
                stack = PGW_ROOT;
                skip_tokens = t->size;

                pgw_tokens--;
                if (pgw_tokens == 0) stack = ROOT;
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

status_t mme_context_setup_trace_module()
{
    int s1ap = context_self()->trace_level.s1ap;
    int nas = context_self()->trace_level.nas;
    int fd = context_self()->trace_level.fd;
    int gtp = context_self()->trace_level.gtp;
    int others = context_self()->trace_level.others;

    if (s1ap)
    {
        extern int _s1ap_sm;
        d_trace_level(&_s1ap_sm, s1ap);
        extern int _s1ap_build;
        d_trace_level(&_s1ap_build, s1ap);
        extern int _s1ap_handler;
        d_trace_level(&_s1ap_handler, s1ap);
        extern int _s1ap_path;
        d_trace_level(&_s1ap_path, s1ap);
        extern int _s1ap_recv;
        d_trace_level(&_s1ap_recv, s1ap);
        extern int _s1ap_send;
        d_trace_level(&_s1ap_send, s1ap);
        extern int _s1ap_ies_decoder;
        d_trace_level(&_s1ap_ies_decoder, s1ap);
        extern int _s1ap_ies_encoder;
        d_trace_level(&_s1ap_ies_encoder, s1ap);
    }

    if (nas)
    {
        extern int _emm_sm;
        d_trace_level(&_emm_sm, nas);
        extern int _esm_sm;
        d_trace_level(&_esm_sm, nas);
        extern int _emm_handler;
        d_trace_level(&_emm_handler, nas);
        extern int _esm_handler;
        d_trace_level(&_esm_handler, nas);
        extern int _nas_decoder;
        d_trace_level(&_nas_decoder, nas);
        extern int _nas_encoder;
        d_trace_level(&_nas_encoder, nas);
        extern int _nas_ies;
        d_trace_level(&_nas_ies, nas);
    }

    if (fd)
    {
        if (fd <= 1) fd_g_debug_lvl = FD_LOG_ERROR;
        else if (fd <= 3) fd_g_debug_lvl = FD_LOG_NOTICE;
        else if (fd <= 5) fd_g_debug_lvl = FD_LOG_DEBUG;
        else fd_g_debug_lvl = FD_LOG_ANNOYING;

        extern int _mme_fd_path;
        d_trace_level(&_mme_fd_path, fd);
        extern int _fd_init;
        d_trace_level(&_fd_init, fd);
        extern int _fd_logger;
        d_trace_level(&_fd_logger, fd);
    }

    if (gtp)
    {
        extern int _mme_s11_handler;
        d_trace_level(&_mme_s11_handler, gtp);
        extern int _gtp_path;
        d_trace_level(&_gtp_path, gtp);
        extern int _mme_s11_path;
        d_trace_level(&_mme_s11_path, gtp);
        extern int _tlv_msg;
        d_trace_level(&_tlv_msg, gtp);
        extern int _gtp_xact;
        d_trace_level(&_gtp_xact, gtp);
    }

    if (others)
    {
        extern int _mutex;
        d_trace_level(&_mutex, others);
        extern int _pkbuf;
        d_trace_level(&_pkbuf, others);

        extern int _context;
        d_trace_level(&_context, others);
        extern int _mme_context;
        d_trace_level(&_mme_context, others);
        extern int _mme_sm;
        d_trace_level(&_mme_sm, others);
    }

    return CORE_OK;
}

mme_sgw_t* mme_sgw_add()
{
    mme_sgw_t *sgw = NULL;

    pool_alloc_node(&mme_sgw_pool, &sgw);
    d_assert(sgw, return NULL, "Null param");

    memset(sgw, 0, sizeof(mme_sgw_t));

    list_init(&sgw->local_list);
    list_init(&sgw->remote_list);

    list_append(&self.sgw_list, sgw);
    
    return sgw;
}

status_t mme_sgw_remove(mme_sgw_t *sgw)
{
    d_assert(sgw, return CORE_ERROR, "Null param");

    gtp_xact_delete_all(sgw);

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

mme_sgw_t* mme_sgw_find(c_uint32_t addr, c_uint16_t port)
{
    mme_sgw_t *sgw = NULL;
    
    sgw = mme_sgw_first();
    while (sgw)
    {
        if (sgw->addr == addr && sgw->port == port)
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

mme_enb_t* mme_enb_add(net_sock_t *sock)
{
    mme_enb_t *enb = NULL;
    event_t e;

    index_alloc(&mme_enb_pool, &enb);
    d_assert(enb, return NULL, "Null param");

    /* IMPORTANT! 
     * eNB Index is saved in net_sock_t structure */
    sock->app_index = enb->index;

    enb->s1ap_sock = sock;
    list_init(&enb->enb_ue_list);

    event_set_param1(&e, (c_uintptr_t)enb->index);
    fsm_create(&enb->sm, s1ap_state_initial, s1ap_state_final);
    fsm_init(&enb->sm, &e);

    hash_set(self.s1ap_sock_hash,
            &enb->s1ap_sock, sizeof(enb->s1ap_sock), enb);
    
    return enb;
}

status_t mme_enb_remove(mme_enb_t *enb)
{
    event_t e;

    d_assert(enb, return CORE_ERROR, "Null param");
    d_assert(enb->s1ap_sock, return CORE_ERROR, "Null param");

    hash_set(self.s1ap_sock_hash,
            &enb->s1ap_sock, sizeof(enb->s1ap_sock), NULL);

    if (enb->enb_id)
        hash_set(self.enb_id_hash, &enb->enb_id, sizeof(enb->enb_id), NULL);

    event_set_param1(&e, (c_uintptr_t)enb->index);
    fsm_final(&enb->sm, &e);
    fsm_clear(&enb->sm);

    enb_ue_remove_in_enb(enb);

    net_unregister_sock(enb->s1ap_sock);
    net_close(enb->s1ap_sock);

    index_free(&mme_enb_pool, enb);

    return CORE_OK;
}

status_t mme_enb_remove_all()
{
    hash_index_t *hi = NULL;
    mme_enb_t *enb = NULL;

    for (hi = mme_enb_first(); hi; hi = mme_enb_next(hi))
    {
        enb = mme_enb_this(hi);
        mme_enb_remove(enb);
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
    d_assert(sock, return NULL,"Invalid param");
    return (mme_enb_t *)hash_get(self.s1ap_sock_hash, &sock, sizeof(sock));

    return NULL;
}

mme_enb_t* mme_enb_find_by_enb_id(c_uint32_t enb_id)
{
    d_assert(enb_id, return NULL,"Invalid param");
    return (mme_enb_t *)hash_get(self.enb_id_hash, &enb_id, sizeof(enb_id));
}

status_t mme_enb_set_enb_id(mme_enb_t *enb, c_uint32_t enb_id)
{
    d_assert(enb, return CORE_ERROR, "Invalid param");
    d_assert(enb_id, return CORE_ERROR, "Invalid param");

    enb->enb_id = enb_id;
    hash_set(self.enb_id_hash, &enb->enb_id, sizeof(enb->enb_id), enb);

    return CORE_OK;
}

hash_index_t* mme_enb_first()
{
    d_assert(self.s1ap_sock_hash, return NULL, "Null param");
    return hash_first(self.s1ap_sock_hash);
}

hash_index_t* mme_enb_next(hash_index_t *hi)
{
    return hash_next(hi);
}

mme_enb_t *mme_enb_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
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
    status_t rv;
    d_assert(self.mme_ue_s1ap_id_hash, return CORE_ERROR, "Null param");
    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ue->enb, return CORE_ERROR, "Null param");

    rv = source_ue_deassociate_target_ue(enb_ue);
    d_assert(rv == CORE_OK, return CORE_ERROR, "Null param");
    
    list_remove(&enb_ue->enb->enb_ue_list, enb_ue);
    hash_set(self.mme_ue_s1ap_id_hash, &enb_ue->mme_ue_s1ap_id, 
            sizeof(enb_ue->mme_ue_s1ap_id), NULL);

    index_free(&enb_ue_pool, enb_ue);

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

status_t enb_ue_switch_to_enb(enb_ue_t *enb_ue, mme_enb_t *new_enb)
{
    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ue->enb, return CORE_ERROR, "Null param");
    d_assert(new_enb, return CORE_ERROR, "Null param");

    /* Remove from the old enb */
    list_remove(&enb_ue->enb->enb_ue_list, enb_ue);

    /* Add to the new enb */
    list_append(&new_enb->enb_ue_list, enb_ue);

    /* Switch to enb */
    enb_ue->enb = new_enb;

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
    event_t e;

    d_assert(enb_ue, return NULL, "Null param");

    index_alloc(&mme_ue_pool, &mme_ue);
    d_assert(mme_ue, return NULL, "Null param");

    list_init(&mme_ue->sess_list);

    mme_ue->mme_s11_teid = mme_ue->index;
    mme_ue->mme_s11_addr = mme_self()->gtpc_addr;

    /* Create t3413 timer */
    mme_ue->t3413 = timer_create(&self.tm_service, MME_EVT_EMM_T3413,
            self.t3413_value * 1000);
    d_assert(mme_ue->t3413, return NULL, "Null param");
    timer_set_param1(mme_ue->t3413, mme_ue->index);

    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    fsm_create(&mme_ue->sm, emm_state_initial, emm_state_final);
    fsm_init(&mme_ue->sm, &e);

    CONNECT_SGW_GTP_NODE(mme_ue);
    
    return mme_ue;
}

status_t mme_ue_remove(mme_ue_t *mme_ue)
{
    event_t e;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    fsm_final(&mme_ue->sm, &e);
    fsm_clear(&mme_ue->sm);

    /* Clear hash table */
    if (mme_ue->guti.m_tmsi != 0)
        hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), NULL);
    if (mme_ue->imsi_len != 0)
        hash_set(self.imsi_ue_hash, mme_ue->imsi, mme_ue->imsi_len, NULL);
    
    /* Clear the saved PDN Connectivity Request */
    NAS_CLEAR_DATA(&mme_ue->pdn_connectivity_request);

    /* Clear Paging info : t3413, last_paing_msg */
    CLEAR_PAGING_INFO(mme_ue);

    /* Free UeRadioCapability */
    if (mme_ue->radio_capa)
    {
        S1ap_UERadioCapability_t *radio_capa = 
            (S1ap_UERadioCapability_t *)mme_ue->radio_capa;

        if (radio_capa->buf)
            core_free(radio_capa->buf);
        core_free(mme_ue->radio_capa);
    }

    /* Clear Transparent Container */
    S1AP_CLEAR_DATA(&mme_ue->container);

    mme_sess_remove_all(mme_ue);
    mme_pdn_remove_all(mme_ue);

    mme_ue_deassociate_enb_ue(mme_ue->enb_ue);

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

mme_ue_t* mme_ue_find_by_imsi_bcd(c_int8_t *imsi_bcd)
{
    c_uint8_t imsi[MAX_IMSI_LEN];
    int imsi_len = 0;

    d_assert(imsi_bcd, return NULL,"Invalid param");

    core_bcd_to_buffer(imsi_bcd, imsi, &imsi_len);

    return mme_ue_find_by_imsi(imsi, imsi_len);
}

mme_ue_t* mme_ue_find_by_imsi(c_uint8_t *imsi, int imsi_len)
{
    d_assert(imsi && imsi_len, return NULL,"Invalid param");

    return (mme_ue_t *)hash_get(self.imsi_ue_hash, imsi, imsi_len);
}

mme_ue_t* mme_ue_find_by_guti(guti_t *guti)
{
    d_assert(guti, return NULL,"Invalid param");

    return (mme_ue_t *)hash_get(self.guti_ue_hash, guti, sizeof(guti_t));
}

mme_ue_t* mme_ue_find_by_teid(c_uint32_t teid)
{
    return mme_ue_find(teid);
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

mme_ue_t* mme_ue_find_by_message(nas_message_t *message)
{
    mme_ue_t *mme_ue = NULL;

    switch(message->emm.h.message_type)
    {
        case NAS_ATTACH_REQUEST:
        {
            nas_attach_request_t *attach_request =
                &message->emm.attach_request;

            nas_eps_mobile_identity_t *eps_mobile_identity =
                            &attach_request->eps_mobile_identity;

            switch(eps_mobile_identity->imsi.type)
            {
                case NAS_EPS_MOBILE_IDENTITY_IMSI:
                {
                    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

                    nas_imsi_to_bcd(
                        &eps_mobile_identity->imsi, eps_mobile_identity->length,
                        imsi_bcd);


                    mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);
                    if (mme_ue)
                    {
                        d_trace(3,"known UE by IMSI[%s]\n", imsi_bcd);
                    }
                    else
                    {
                        d_trace(3,"Unknown UE by IMSI[%s]\n", imsi_bcd);
                    }
                    break;
                }
                case NAS_EPS_MOBILE_IDENTITY_GUTI:
                {
                    nas_eps_mobile_identity_guti_t *nas_guti = NULL;
                    nas_guti = &eps_mobile_identity->guti;
                    guti_t guti;

                    guti.plmn_id = nas_guti->plmn_id;
                    guti.mme_gid = nas_guti->mme_gid;
                    guti.mme_code = nas_guti->mme_code;
                    guti.m_tmsi = nas_guti->m_tmsi;

                    mme_ue = mme_ue_find_by_guti(&guti);
                    if (mme_ue)
                    {
                        d_trace(3, "Known UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]\n",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    else
                    {
                        d_warn("Unknown UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    break;
                }
                default:
                {
                    d_error("Uknown message imsi type =%d\n",
                            eps_mobile_identity->imsi.type);
                    break;
                }
            }
            break;
        }
        case NAS_DETACH_REQUEST:
        {
            /* TODO */
            break;
        }
        case NAS_TRACKING_AREA_UPDATE_REQUEST:
        {
            nas_tracking_area_update_request_t *tau_request =
                &message->emm.tracking_area_update_request;

            nas_eps_mobile_identity_t *eps_mobile_identity =
                            &tau_request->old_guti;

            switch(eps_mobile_identity->imsi.type)
            {
                case NAS_EPS_MOBILE_IDENTITY_GUTI:
                {
                    nas_eps_mobile_identity_guti_t *nas_guti = NULL;
                    nas_guti = &eps_mobile_identity->guti;
                    guti_t guti;

                    guti.plmn_id = nas_guti->plmn_id;
                    guti.mme_gid = nas_guti->mme_gid;
                    guti.mme_code = nas_guti->mme_code;
                    guti.m_tmsi = nas_guti->m_tmsi;

                    mme_ue = mme_ue_find_by_guti(&guti);
                    if (mme_ue)
                    {
                        d_trace(3, "Known UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]\n",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    else
                    {
                        d_warn("Unknown UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    break;
                }
                default:
                {
                    d_error("Uknown message imsi type =%d\n",
                            eps_mobile_identity->imsi.type);
                    break;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return mme_ue;
}

/* At this point, I'm not sure whether this function is exported or not */
static status_t mme_ue_new_guti(mme_ue_t *mme_ue)
{
    served_gummei_t *served_gummei = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Invalid param");
    d_assert(mme_self()->max_num_of_served_gummei > 0,
            return CORE_ERROR, "Invalid param");

    served_gummei = &mme_self()->served_gummei[0];

    d_assert(served_gummei->num_of_plmn_id > 0,
            return CORE_ERROR, "Invalid param");
    d_assert(served_gummei->num_of_mme_gid > 0,
            return CORE_ERROR, "Invalid param");
    d_assert(served_gummei->num_of_mme_code > 0,
            return CORE_ERROR, "Invalid param");

    if (mme_ue->guti.m_tmsi != 0)
    {
        /* MME has a VALID GUIT
         * As such, we need to remove previous GUTI in hash table */
        hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), NULL);
    }

    memset(&mme_ue->guti, 0, sizeof(guti_t));

    /* FIXME : How to generate GUTI?
     * At this point, I'll use first(0-index) Served GUMMEI in MME_CONTEXT */
    memcpy(&mme_ue->guti.plmn_id, &served_gummei->plmn_id[0], PLMN_ID_LEN);
    mme_ue->guti.mme_gid = served_gummei->mme_gid[0];
    mme_ue->guti.mme_code = served_gummei->mme_code[0];

    mme_ue->guti.m_tmsi = NEXT_ID(self.m_tmsi, 1, 0xffffffff);

    hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), mme_ue);

    return CORE_OK;
}

status_t mme_ue_set_imsi(mme_ue_t *mme_ue, c_int8_t *imsi_bcd)
{
    d_assert(mme_ue && imsi_bcd, return CORE_ERROR, "Invalid param");

    core_cpystrn(mme_ue->imsi_bcd, imsi_bcd, MAX_IMSI_BCD_LEN+1);
    core_bcd_to_buffer(mme_ue->imsi_bcd, mme_ue->imsi, &mme_ue->imsi_len);

    hash_set(self.imsi_ue_hash, mme_ue->imsi, mme_ue->imsi_len, mme_ue);

    mme_ue_new_guti(mme_ue);

    return CORE_OK;
}

/* 
 * S1AP Initial UE-Message : S-TMSI
 * NAS ATTACH_REQUEST : IMSI, GUTI
 * NAS TAU_REQUEST : GUTI
 * S1AP Handover Notification
 */
status_t mme_ue_associate_enb_ue(mme_ue_t *mme_ue, enb_ue_t *enb_ue)
{
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    mme_ue->enb_ue = enb_ue;
    enb_ue->mme_ue = mme_ue;

    return CORE_OK;
}

/* 
 * mme_ue_remove()
 *
 * Note : should not call in enb_ue_remove()
 *
 * When mme_ue is removed, enb_ue->mme_ue must be NULL.
 * However, when enb_ue is removed, mme_ue->enb_ue need not be NULL.
 * mme_ue->enb_ue will be updated again when enb_ue is added.
 */
status_t mme_ue_deassociate_enb_ue(enb_ue_t *enb_ue)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    mme_ue->enb_ue = NULL;
    enb_ue->mme_ue = NULL;

    return CORE_OK;
}

/* 
 * S1AP Handover Required
 */
status_t source_ue_associate_target_ue(
        enb_ue_t *source_ue, enb_ue_t *target_ue)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(source_ue, return CORE_ERROR, "Null param");
    d_assert(target_ue, return CORE_ERROR, "Null param");
    mme_ue = source_ue->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    target_ue->mme_ue = mme_ue;
    target_ue->source_ue = source_ue;
    source_ue->target_ue = target_ue;

    return CORE_OK;
}

/* 
 * enb_ue_remove()
 *
 * enb_ue->mme_ue->enb_ue should not be set to NULL.
 * This is because enb_ue is not known as source_ue or target_ue.
 * Therefore, when enb_ue is removed, leave enb_ue->mme_ue->enb_ue as is.
 */
status_t source_ue_deassociate_target_ue(enb_ue_t *enb_ue)
{
    enb_ue_t *source_ue = NULL;
    enb_ue_t *target_ue = NULL;
    d_assert(enb_ue, return CORE_ERROR,);

    if (enb_ue->target_ue)
    {
        source_ue = enb_ue;
        target_ue = enb_ue->target_ue;

        d_assert(source_ue->target_ue, return CORE_ERROR,);
        d_assert(target_ue->source_ue, return CORE_ERROR,);
        source_ue->target_ue = NULL;
        target_ue->source_ue = NULL;
    }
    else if (enb_ue->source_ue)
    {
        target_ue = enb_ue;
        source_ue = enb_ue->source_ue;

        d_assert(source_ue->target_ue, return CORE_ERROR,);
        d_assert(target_ue->source_ue, return CORE_ERROR,);
        source_ue->target_ue = NULL;
        target_ue->source_ue = NULL;
    }

    return CORE_OK;
}

mme_sess_t *mme_sess_add(mme_ue_t *mme_ue, c_uint8_t pti)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    d_assert(mme_ue, return NULL, "Null param");
    d_assert(pti != NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
            return NULL, "Invalid PTI(%d)", pti);

    index_alloc(&mme_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    list_init(&sess->bearer_list);
    list_append(&mme_ue->sess_list, sess);

    sess->mme_ue = mme_ue;
    sess->pti = pti;

    bearer = mme_bearer_add(sess);
    d_assert(bearer, mme_sess_remove(sess); return NULL, 
            "Can't add default bearer context");

    return sess;
}

status_t mme_sess_remove(mme_sess_t *sess)
{
    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(sess->mme_ue, return CORE_ERROR, "Null param");

    mme_bearer_remove_all(sess);

    NAS_CLEAR_DATA(&sess->ue_pco);
    TLV_CLEAR_DATA(&sess->pgw_pco);

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

mme_sess_t* mme_sess_find_by_pti(mme_ue_t *mme_ue, c_uint8_t pti)
{
    mme_sess_t *sess = NULL;

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        if (pti == sess->pti)
            return sess;

        sess = mme_sess_next(sess);
    }

    return NULL;
}

mme_sess_t* mme_sess_find_by_ebi(mme_ue_t *mme_ue, c_uint8_t ebi)
{
    mme_bearer_t *bearer = NULL;

    bearer = mme_bearer_find_by_ue_ebi(mme_ue, ebi);
    if (bearer)
        return bearer->sess;

    return NULL;
}

mme_sess_t* mme_sess_find_by_apn(mme_ue_t *mme_ue, c_int8_t *apn)
{
    mme_sess_t *sess = NULL;

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        if (sess->pdn && strcmp(sess->pdn->apn, apn) == 0)
            return sess;

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

mme_bearer_t* mme_bearer_add(mme_sess_t *sess)
{
    event_t e;

    mme_bearer_t *bearer = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(sess, return NULL, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return NULL, "Null param");

    index_alloc(&mme_bearer_pool, &bearer);
    d_assert(bearer, return NULL, "Null param");

    bearer->ebi = NEXT_ID(mme_ue->ebi, MIN_EPS_BEARER_ID, MAX_EPS_BEARER_ID);

    list_append(&sess->bearer_list, bearer);
    
    bearer->mme_ue = mme_ue;
    bearer->sess = sess;

    event_set_param1(&e, (c_uintptr_t)bearer->index);
    fsm_create(&bearer->sm, esm_state_initial, esm_state_final);
    fsm_init(&bearer->sm, &e);

    return bearer;
}

status_t mme_bearer_remove(mme_bearer_t *bearer)
{
    event_t e;

    d_assert(bearer, return CORE_ERROR, "Null param");
    d_assert(bearer->sess, return CORE_ERROR, "Null param");

    event_set_param1(&e, (c_uintptr_t)bearer->index);
    fsm_final(&bearer->sm, &e);
    fsm_clear(&bearer->sm);

    TLV_CLEAR_DATA(&bearer->tft);
    
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

mme_bearer_t* mme_bearer_find_by_sess_ebi(mme_sess_t *sess, c_uint8_t ebi)
{
    mme_bearer_t *bearer = NULL;

    bearer = mme_bearer_first(sess);
    while(bearer)
    {
        if (ebi == bearer->ebi)
            return bearer;

        bearer = mme_bearer_next(bearer);
    }

    return NULL;
}

mme_bearer_t* mme_bearer_find_by_ue_ebi(mme_ue_t *mme_ue, c_uint8_t ebi)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    
    sess = mme_sess_first(mme_ue);
    while (sess)
    {
        bearer = mme_bearer_find_by_sess_ebi(sess, ebi);
        if (bearer)
        {
            return bearer;
        }

        sess = mme_sess_next(sess);
    }

    return NULL;
}

mme_bearer_t* mme_bearer_find_or_add_by_message(
        mme_ue_t *mme_ue, nas_message_t *message)
{
    c_uint8_t pti = NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    c_uint8_t ebi = NAS_EPS_BEARER_IDENTITY_UNASSIGNED;

    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;

    d_assert(mme_ue, return NULL,);
    d_assert(message, return NULL,);

    if (message->esm.h.message_type == NAS_PDN_DISCONNECT_REQUEST)
    {
        nas_pdn_disconnect_request_t *pdn_disconnect_request = 
            &message->esm.pdn_disconnect_request;
        nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity =
            &pdn_disconnect_request->linked_eps_bearer_identity;

        bearer = mme_bearer_find_by_ue_ebi(mme_ue,
                linked_eps_bearer_identity->eps_bearer_identity);
        d_assert(bearer, return NULL,
                "Invalid pti(%d) and ebi(%d)\n", pti, ebi);
        sess = bearer->sess;
        d_assert(sess, return NULL, "Null param");
        sess->pti = pti;

        return bearer;
    }

    pti = message->esm.h.procedure_transaction_identity;
    ebi = message->esm.h.eps_bearer_identity;

    if (ebi != NAS_EPS_BEARER_IDENTITY_UNASSIGNED)
    {
        bearer = mme_bearer_find_by_ue_ebi(mme_ue, ebi);
        d_assert(bearer, return NULL,);
        return bearer;
    }
        
    d_assert(pti != NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
            return NULL, );

    if (message->esm.h.message_type == NAS_PDN_CONNECTIVITY_REQUEST)
    {
        nas_pdn_connectivity_request_t *pdn_connectivity_request =
            &message->esm.pdn_connectivity_request;
        if (pdn_connectivity_request->presencemask &
                NAS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT)
            sess = mme_sess_find_by_apn(mme_ue,
                    pdn_connectivity_request->access_point_name.apn);
        else
            sess = mme_sess_first(mme_ue);

        if (!sess)
            sess = mme_sess_add(mme_ue, pti);
        else
            sess->pti = pti;

        d_assert(sess, return NULL,);
    }
    else
    {
        sess = mme_sess_find_by_pti(mme_ue, pti);
        d_assert(sess, return NULL,);
    }

    bearer = mme_default_bearer_in_sess(sess);
    d_assert(bearer, return NULL,);
    return bearer;
}

mme_bearer_t* mme_default_bearer_in_sess(mme_sess_t *sess)
{
    return mme_bearer_first(sess);
}

mme_bearer_t* mme_linked_bearer(mme_bearer_t *bearer)
{
    mme_sess_t *sess = NULL;

    d_assert(bearer, return NULL, "Null param");
    sess = bearer->sess;
    d_assert(sess, return NULL, "Null param");

    return mme_default_bearer_in_sess(sess);
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

status_t mme_pdn_remove_all(mme_ue_t *mme_ue)
{
    s6a_subscription_data_t *subscription_data = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    subscription_data = &mme_ue->subscription_data;
    d_assert(subscription_data, return CORE_ERROR, "Null param");

    subscription_data->num_of_pdn = 0;
    
    return CORE_OK;
}

pdn_t* mme_pdn_find_by_apn(mme_ue_t *mme_ue, c_int8_t *apn)
{
    s6a_subscription_data_t *subscription_data = NULL;
    pdn_t *pdn = NULL;
    int i = 0;
    
    d_assert(mme_ue, return NULL, "Null param");
    subscription_data = &mme_ue->subscription_data;
    d_assert(subscription_data, return NULL, "Null param");

    for (i = 0; i < subscription_data->num_of_pdn; i++)
    {
        pdn = &subscription_data->pdn[i];
        if (strcmp(pdn->apn, apn) == 0)
            return pdn;
    }

    return NULL;
}

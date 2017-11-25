#define TRACE_MODULE _sgw_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"
#include "core_lib.h"
#include <mongoc.h>

#include "types.h"
#include "gtp_types.h"

#include "context.h"
#include "sgw_context.h"

pool_declare(sgw_mme_pool, sgw_mme_t, MAX_NUM_OF_GTP_CLIENT);
pool_declare(sgw_pgw_pool, sgw_pgw_t, MAX_NUM_OF_GTP_CLIENT);

static sgw_context_t self;

index_declare(sgw_ue_pool, sgw_ue_t, MAX_POOL_OF_UE);
index_declare(sgw_sess_pool, sgw_sess_t, MAX_POOL_OF_SESS);
index_declare(sgw_bearer_pool, sgw_bearer_t, MAX_POOL_OF_BEARER);
index_declare(sgw_tunnel_pool, sgw_tunnel_t, MAX_POOL_OF_TUNNEL);

static int context_initialized = 0;

status_t sgw_context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "SGW context already has been initialized");

    memset(&self, 0, sizeof(sgw_context_t));

    pool_init(&sgw_mme_pool, MAX_NUM_OF_GTP_CLIENT);
    list_init(&self.mme_list);
    pool_init(&sgw_pgw_pool, MAX_NUM_OF_GTP_CLIENT);
    list_init(&self.pgw_list);

    index_init(&sgw_ue_pool, MAX_POOL_OF_UE);
    index_init(&sgw_sess_pool, MAX_POOL_OF_SESS);
    index_init(&sgw_bearer_pool, MAX_POOL_OF_BEARER);
    index_init(&sgw_tunnel_pool, MAX_POOL_OF_TUNNEL);

    self.imsi_ue_hash = hash_make();

    context_initialized = 1;

    return CORE_OK;
}

status_t sgw_context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "SGW context already has been finalized");

    sgw_ue_remove_all();
    sgw_mme_remove_all();
    sgw_pgw_remove_all();

    d_assert(self.imsi_ue_hash, , "Null param");
    hash_destroy(self.imsi_ue_hash);

    index_final(&sgw_tunnel_pool);
    index_final(&sgw_bearer_pool);
    index_final(&sgw_sess_pool);
    index_final(&sgw_ue_pool);

    pool_final(&sgw_mme_pool);
    pool_final(&sgw_pgw_pool);

    context_initialized = 0;
    
    return CORE_OK;
}

sgw_context_t* sgw_self()
{
    return &self;
}

static status_t sgw_context_prepare()
{
    self.gtpc_port = GTPV2_C_UDP_PORT;
    self.gtpu_port = GTPV1_U_UDP_PORT;

    return CORE_OK;
}

static status_t sgw_context_validation()
{
    if (self.gtpc_addr == 0)
    {
        d_error("No SGW.NEWORK.GTPC_IPV4 in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.gtpu_addr == 0)
    {
        d_error("No SGW.NEWORK.GTPU_IPV4 in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    return CORE_OK;
}

status_t sgw_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;
    bson_iter_t iter;
    c_uint32_t length = 0;

    d_assert(config, return CORE_ERROR, );

    rv = sgw_context_prepare();
    if (rv != CORE_OK) return rv;

    if (!bson_iter_init(&iter, config->bson))
    {
        d_error("bson_iter_init failed in this document");
        return CORE_ERROR;
    }

    while(bson_iter_next(&iter))
    {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "SGW") && BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_t sgw_iter;
            bson_iter_recurse(&iter, &sgw_iter);
            while(bson_iter_next(&sgw_iter))
            {
                const char *sgw_key = bson_iter_key(&sgw_iter);
                if (!strcmp(sgw_key, "NETWORK"))
                {
                    bson_iter_t network_iter;

                    if (BSON_ITER_HOLDS_ARRAY(&sgw_iter))
                    {
                        bson_iter_t array_iter;
                        bson_iter_recurse(&sgw_iter, &array_iter);
                        if (bson_iter_next(&array_iter))
                        {
                            /* We will pick only first item of SGW.NETWORK
                             * if the type is an array */
                            bson_iter_recurse(&array_iter, &network_iter);
                        }
                    }
                    else if (BSON_ITER_HOLDS_DOCUMENT(&sgw_iter))
                    {
                        bson_iter_recurse(&sgw_iter, &network_iter);
                    }
                    else
                        d_assert(0, return CORE_ERROR,);

                    while(bson_iter_next(&network_iter))
                    {
                        const char *network_key = bson_iter_key(&network_iter);
                        if (!strcmp(network_key, "GTPC_IPV4") &&
                            BSON_ITER_HOLDS_UTF8(&network_iter))
                        {
                            const char *v =
                                bson_iter_utf8(&network_iter, &length);
                            if (v) self.gtpc_addr = inet_addr(v);
                        }
                        else if (!strcmp(network_key, "GTPC_PORT") &&
                            BSON_ITER_HOLDS_INT32(&network_iter))
                        {
                            self.gtpc_port = bson_iter_int32(&network_iter);
                        }
                        else if (!strcmp(network_key, "GTPU_IPV4") &&
                            BSON_ITER_HOLDS_UTF8(&network_iter))
                        {
                            const char *v =
                                bson_iter_utf8(&network_iter, &length);
                            if (v) self.gtpu_addr = inet_addr(v);
                        }
                        else if (!strcmp(network_key, "GTPU_PORT") &&
                            BSON_ITER_HOLDS_UTF8(&network_iter))
                        {
                            self.gtpu_port = bson_iter_int32(&network_iter);
                        }
                    }
                }
            }
        }
    }

    rv = sgw_context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

status_t sgw_context_setup_trace_module()
{
    int gtp = context_self()->trace_level.gtp;
    int others = context_self()->trace_level.others;

    if (gtp)
    {
        extern int _sgw_sm;
        d_trace_level(&_sgw_sm, gtp);
        extern int _sgw_s11_handler;
        d_trace_level(&_sgw_s11_handler, gtp);
        extern int _sgw_s11_handler;
        d_trace_level(&_sgw_s11_handler, gtp);
        extern int _gtp_path;
        d_trace_level(&_gtp_path, gtp);
        extern int _sgw_path;
        d_trace_level(&_sgw_path, gtp);
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
        extern int _sgw_context;
        d_trace_level(&_sgw_context, others);
    }

    return CORE_OK;
}

sgw_mme_t* sgw_mme_add()
{
    sgw_mme_t *mme = NULL;

    pool_alloc_node(&sgw_mme_pool, &mme);
    d_assert(mme, return NULL, "Null param");

    memset(mme, 0, sizeof(sgw_mme_t));

    list_init(&mme->local_list);
    list_init(&mme->remote_list);

    list_append(&self.mme_list, mme);
    
    return mme;
}

status_t sgw_mme_remove(sgw_mme_t *mme)
{
    d_assert(mme, return CORE_ERROR, "Null param");

    list_remove(&self.mme_list, mme);

    gtp_xact_delete_all(mme);

    pool_free_node(&sgw_mme_pool, mme);

    return CORE_OK;
}

status_t sgw_mme_remove_all()
{
    sgw_mme_t *mme = NULL, *next_mme = NULL;
    
    mme = sgw_mme_first();
    while (mme)
    {
        next_mme = sgw_mme_next(mme);

        sgw_mme_remove(mme);

        mme = next_mme;
    }

    return CORE_OK;
}

sgw_mme_t* sgw_mme_find(c_uint32_t addr)
{
    sgw_mme_t *mme = NULL;
    
    mme = sgw_mme_first();
    while (mme)
    {
        if (mme->addr.sin.sin_addr.s_addr == addr)
            break;

        mme = sgw_mme_next(mme);
    }

    return mme;
}

sgw_mme_t* sgw_mme_first()
{
    return list_first(&self.mme_list);
}

sgw_mme_t* sgw_mme_next(sgw_mme_t *mme)
{
    return list_next(mme);
}

sgw_pgw_t* sgw_pgw_add()
{
    sgw_pgw_t *pgw = NULL;

    pool_alloc_node(&sgw_pgw_pool, &pgw);
    d_assert(pgw, return NULL, "Null param");

    memset(pgw, 0, sizeof(sgw_pgw_t));

    list_init(&pgw->local_list);
    list_init(&pgw->remote_list);

    list_append(&self.pgw_list, pgw);
    
    return pgw;
}

status_t sgw_pgw_remove(sgw_pgw_t *pgw)
{
    d_assert(pgw, return CORE_ERROR, "Null param");

    list_remove(&self.pgw_list, pgw);

    gtp_xact_delete_all(pgw);

    pool_free_node(&sgw_pgw_pool, pgw);

    return CORE_OK;
}

status_t sgw_pgw_remove_all()
{
    sgw_pgw_t *pgw = NULL, *next_pgw = NULL;
    
    pgw = sgw_pgw_first();
    while (pgw)
    {
        next_pgw = sgw_pgw_next(pgw);

        sgw_pgw_remove(pgw);

        pgw = next_pgw;
    }

    return CORE_OK;
}

sgw_pgw_t* sgw_pgw_find(c_uint32_t addr)
{
    sgw_pgw_t *pgw = NULL;
    
    pgw = sgw_pgw_first();
    while (pgw)
    {
        if (pgw->addr.sin.sin_addr.s_addr == addr)
            break;

        pgw = sgw_pgw_next(pgw);
    }

    return pgw;
}

sgw_pgw_t* sgw_pgw_first()
{
    return list_first(&self.pgw_list);
}

sgw_pgw_t* sgw_pgw_next(sgw_pgw_t *pgw)
{
    return list_next(pgw);
}

sgw_ue_t* sgw_ue_add(gtp_f_teid_t *mme_s11_teid,
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn, c_uint8_t ebi)
{
    sgw_ue_t *sgw_ue = NULL;
    sgw_sess_t *sess = NULL;
    sgw_mme_t *mme = NULL;
    c_uint32_t addr = 0;

    d_assert(mme_s11_teid, return NULL, "Null param");
    d_assert(imsi, return NULL, "Null param");
    d_assert(imsi_len, return NULL, "Null param");
    d_assert(apn, return NULL, "Null param");
    d_assert(ebi, return NULL, "Null param");

    index_alloc(&sgw_ue_pool, &sgw_ue);
    d_assert(sgw_ue, return NULL, "Null param");

    sgw_ue->sgw_s11_teid = sgw_ue->index;
    sgw_ue->sgw_s11_addr = sgw_self()->gtpc_addr;

    addr = mme_s11_teid->ipv4_addr;

    mme = sgw_mme_find(addr);
    if (!mme)
    {
        mme = sgw_mme_add();
        d_assert(mme, return NULL, "Can't add MME-GTP node");

        mme->addr.sin.sin_addr.s_addr = addr;
        mme->addr.c_sa_family = AF_INET;
        mme->addr.c_sa_port = htons(GTPV2_C_UDP_PORT);
        mme->sock = sgw_self()->gtpc_sock;
    }
    CONNECT_MME_GTP_NODE(sgw_ue, mme);

    /* Set IMSI */
    sgw_ue->imsi_len = imsi_len;
    memcpy(sgw_ue->imsi, imsi, sgw_ue->imsi_len);
    core_buffer_to_bcd(sgw_ue->imsi, sgw_ue->imsi_len, sgw_ue->imsi_bcd);

    list_init(&sgw_ue->sess_list);

    sess = sgw_sess_add(sgw_ue, apn, ebi);
    d_assert(sess, index_free(&sgw_ue_pool, sgw_ue); return NULL, "Null param");

    hash_set(self.imsi_ue_hash, sgw_ue->imsi, sgw_ue->imsi_len, sgw_ue);

    return sgw_ue;
}

status_t sgw_ue_remove(sgw_ue_t *sgw_ue)
{
    d_assert(sgw_ue, return CORE_ERROR, "Null param");

    /* Clear hash table */
    if (sgw_ue->imsi_len != 0)
        hash_set(self.imsi_ue_hash, sgw_ue->imsi, sgw_ue->imsi_len, NULL);
    
    sgw_sess_remove_all(sgw_ue);

    index_free(&sgw_ue_pool, sgw_ue);

    return CORE_OK;
}

status_t sgw_ue_remove_all()
{
    hash_index_t *hi = NULL;
    sgw_ue_t *sgw_ue = NULL;

    for (hi = sgw_ue_first(); hi; hi = sgw_ue_next(hi))
    {
        sgw_ue = sgw_ue_this(hi);
        sgw_ue_remove(sgw_ue);
    }

    return CORE_OK;
}

static sgw_ue_t* sgw_ue_find(index_t index)
{
    d_assert(index, return NULL, "Invalid index = 0x%x", index);
    return index_find(&sgw_ue_pool, index);
}

sgw_ue_t* sgw_ue_find_by_imsi_bcd(c_int8_t *imsi_bcd)
{
    c_uint8_t imsi[MAX_IMSI_LEN];
    int imsi_len = 0;

    d_assert(imsi_bcd, return NULL,"Invalid param");

    core_bcd_to_buffer(imsi_bcd, imsi, &imsi_len);

    return sgw_ue_find_by_imsi(imsi, imsi_len);
}

sgw_ue_t* sgw_ue_find_by_imsi(c_uint8_t *imsi, int imsi_len)
{
    d_assert(imsi && imsi_len, return NULL,"Invalid param");

    return (sgw_ue_t *)hash_get(self.imsi_ue_hash, imsi, imsi_len);
}

sgw_ue_t* sgw_ue_find_by_teid(c_uint32_t teid)
{
    return sgw_ue_find(teid);
}

sgw_ue_t *sgw_ue_find_or_add_by_message(gtp_message_t *gtp_message)
{
    sgw_ue_t *sgw_ue = NULL;

    gtp_create_session_request_t *req = &gtp_message->create_session_request;

    if (req->imsi.presence == 0)
    {
        d_error("No IMSI");
        return NULL;
    }

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No Sender F-TEID");
        return NULL;
    }

    if (req->access_point_name.presence == 0)
    {
        d_error("No APN");
        return NULL;
    }
    if (req->bearer_contexts_to_be_created.presence == 0)
    {
        d_error("No Bearer");
        return NULL;
    }
    if (req->bearer_contexts_to_be_created.eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return NULL;
    }

    sgw_ue = sgw_ue_find_by_imsi(req->imsi.data, req->imsi.len);
    if (!sgw_ue)
    {
        c_int8_t apn[MAX_APN_LEN];
        apn_parse(apn, req->access_point_name.data, req->access_point_name.len);
        sgw_ue = sgw_ue_add(
            req->sender_f_teid_for_control_plane.data,
            req->imsi.data, req->imsi.len, apn,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);
        d_assert(sgw_ue, return NULL, "No UE Context");
    }

    return sgw_ue;
}

hash_index_t *sgw_ue_first()
{
    d_assert(self.imsi_ue_hash, return NULL, "Null param");
    return hash_first(self.imsi_ue_hash);
}

hash_index_t *sgw_ue_next(hash_index_t *hi)
{
    return hash_next(hi);
}

sgw_ue_t *sgw_ue_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
}

sgw_sess_t *sgw_sess_add(
        sgw_ue_t *sgw_ue, c_int8_t *apn, c_uint8_t ebi)
{
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;

    d_assert(sgw_ue, return NULL, "Null param");
    d_assert(ebi, return NULL, "Invalid EBI(%d)", ebi);

    index_alloc(&sgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->sgw_s5c_teid = SGW_S5C_INDEX_TO_TEID(sess->index);
    sess->sgw_s5c_addr = sgw_self()->gtpc_addr;

    /* Set APN */
    core_cpystrn(sess->pdn.apn, apn, MAX_APN_LEN+1);

    sess->sgw_ue = sgw_ue;

    list_init(&sess->bearer_list);

    bearer = sgw_bearer_add(sess);
    d_assert(bearer, sgw_sess_remove(sess); return NULL, 
            "Can't add default bearer context");
    bearer->ebi = ebi;

    list_append(&sgw_ue->sess_list, sess);

    return sess;
}

status_t sgw_sess_remove(sgw_sess_t *sess)
{
    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(sess->sgw_ue, return CORE_ERROR, "Null param");

    list_remove(&sess->sgw_ue->sess_list, sess);

    sgw_bearer_remove_all(sess);

    index_free(&sgw_sess_pool, sess);

    return CORE_OK;
}

status_t sgw_sess_remove_all(sgw_ue_t *sgw_ue)
{
    sgw_sess_t *sess = NULL, *next_sess = NULL;
    
    sess = sgw_sess_first(sgw_ue);
    while (sess)
    {
        next_sess = sgw_sess_next(sess);

        sgw_sess_remove(sess);

        sess = next_sess;
    }

    return CORE_OK;
}

static sgw_sess_t* sgw_sess_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&sgw_sess_pool, index);
}

sgw_sess_t* sgw_sess_find_by_teid(c_uint32_t teid)
{
    return sgw_sess_find(SGW_S5C_TEID_TO_INDEX(teid));
}

sgw_sess_t* sgw_sess_find_by_apn(sgw_ue_t *sgw_ue, c_int8_t *apn)
{
    sgw_sess_t *sess = NULL;
    
    sess = sgw_sess_first(sgw_ue);
    while (sess)
    {
        if (strcmp(sess->pdn.apn, apn) == 0)
            return sess;

        sess = sgw_sess_next(sess);
    }

    return NULL;
}

sgw_sess_t* sgw_sess_find_by_ebi(sgw_ue_t *sgw_ue, c_uint8_t ebi)
{
    sgw_bearer_t *bearer = NULL;

    bearer = sgw_bearer_find_by_ue_ebi(sgw_ue, ebi);
    if (bearer)
        return bearer->sess;

    return NULL;
}

sgw_sess_t* sgw_sess_first(sgw_ue_t *sgw_ue)
{
    return list_first(&sgw_ue->sess_list);
}

sgw_sess_t* sgw_sess_next(sgw_sess_t *sess)
{
    return list_next(sess);
}

sgw_bearer_t* sgw_bearer_add(sgw_sess_t *sess)
{
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL;
    sgw_ue_t *sgw_ue = NULL;

    d_assert(sess, return NULL, "Null param");
    sgw_ue = sess->sgw_ue;
    d_assert(sgw_ue, return NULL, "Null param");

    index_alloc(&sgw_bearer_pool, &bearer);
    d_assert(bearer, return NULL, "Bearer context allocation failed");

    bearer->sgw_ue = sgw_ue;
    bearer->sess = sess;

    list_init(&bearer->tunnel_list);

    tunnel = sgw_tunnel_add(bearer, GTP_F_TEID_S1_U_SGW_GTP_U);
    d_assert(tunnel, return NULL, "Tunnel context allocation failed");

    tunnel = sgw_tunnel_add(bearer, GTP_F_TEID_S5_S8_SGW_GTP_U);
    d_assert(tunnel, return NULL, "Tunnel context allocation failed");

    list_append(&sess->bearer_list, bearer);
    
    return bearer;
}

status_t sgw_bearer_remove(sgw_bearer_t *bearer)
{
    int i;

    d_assert(bearer, return CORE_ERROR, "Null param");
    d_assert(bearer->sess, return CORE_ERROR, "Null param");

    list_remove(&bearer->sess->bearer_list, bearer);

    sgw_tunnel_remove_all(bearer);

    /* Free the buffered packets */
    for (i = 0; i < bearer->num_buffered_pkt; i++)
        pkbuf_free(bearer->buffered_pkts[i]);

    index_free(&sgw_bearer_pool, bearer);

    return CORE_OK;
}

status_t sgw_bearer_remove_all(sgw_sess_t *sess)
{
    sgw_bearer_t *bearer = NULL, *next_bearer = NULL;

    d_assert(sess, return CORE_ERROR, "Null param");
    
    bearer = list_first(&sess->bearer_list);
    while (bearer)
    {
        next_bearer = list_next(bearer);

        sgw_bearer_remove(bearer);

        bearer = next_bearer;
    }

    return CORE_OK;
}

sgw_bearer_t* sgw_bearer_find(index_t index)
{
    d_assert(index && index < MAX_POOL_OF_BEARER, return NULL, 
            "Invalid Index(%d)",index);

    return index_find(&sgw_bearer_pool, index);
}

sgw_bearer_t* sgw_bearer_find_by_sgw_s5u_teid(c_uint32_t sgw_s5u_teid)
{
    return sgw_bearer_find(sgw_s5u_teid);
}

sgw_bearer_t* sgw_bearer_find_by_sess_ebi(sgw_sess_t *sess, c_uint8_t ebi)
{
    sgw_bearer_t *bearer = NULL;

    bearer = sgw_bearer_first(sess);
    while(bearer)
    {
        if (ebi == bearer->ebi)
            return bearer;

        bearer = sgw_bearer_next(bearer);
    }

    return NULL;
}

sgw_bearer_t* sgw_bearer_find_by_ue_ebi(sgw_ue_t *sgw_ue, c_uint8_t ebi)
{
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    
    sess = sgw_sess_first(sgw_ue);
    while (sess)
    {
        bearer = sgw_bearer_find_by_sess_ebi(sess, ebi);
        if (bearer)
        {
            return bearer;
        }

        sess = sgw_sess_next(sess);
    }

    return NULL;
}

sgw_bearer_t* sgw_default_bearer_in_sess(sgw_sess_t *sess)
{
    return sgw_bearer_first(sess);
}

sgw_bearer_t* sgw_bearer_first(sgw_sess_t *sess)
{
    d_assert(sess, return NULL, "Null param");
    return list_first(&sess->bearer_list);
}

sgw_bearer_t* sgw_bearer_next(sgw_bearer_t *bearer)
{
    return list_next(bearer);
}

sgw_tunnel_t* sgw_tunnel_add(sgw_bearer_t *bearer, c_uint8_t interface_type)
{
    sgw_tunnel_t *tunnel = NULL;

    d_assert(bearer, return NULL, "Null param");

    index_alloc(&sgw_tunnel_pool, &tunnel);
    d_assert(tunnel, return NULL, "Tunnel context allocation failed");

    tunnel->interface_type = interface_type;
    tunnel->local_teid = tunnel->index;
    tunnel->local_addr = sgw_self()->gtpu_addr;

    tunnel->bearer = bearer;

    list_append(&bearer->tunnel_list, tunnel);

    return tunnel;
}

status_t sgw_tunnel_remove(sgw_tunnel_t *tunnel)
{
    d_assert(tunnel, return CORE_ERROR, "Null param");
    d_assert(tunnel->bearer, return CORE_ERROR, "Null param");

    list_remove(&tunnel->bearer->tunnel_list, tunnel);
    index_free(&sgw_tunnel_pool, tunnel);

    return CORE_OK;
}

status_t sgw_tunnel_remove_all(sgw_bearer_t *bearer)
{
    sgw_tunnel_t *tunnel = NULL, *next_tunnel = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    
    tunnel = sgw_tunnel_first(bearer);
    while (tunnel)
    {
        next_tunnel = sgw_tunnel_next(tunnel);

        sgw_tunnel_remove(tunnel);

        tunnel = next_tunnel;
    }

    return CORE_OK;
}

sgw_tunnel_t* sgw_tunnel_find(index_t index)
{
    d_assert(index && index < MAX_POOL_OF_TUNNEL, return NULL, 
            "Invalid Index(%d)",index);

    return index_find(&sgw_tunnel_pool, index);
}

sgw_tunnel_t* sgw_tunnel_find_by_teid(c_uint32_t teid)
{
    return sgw_tunnel_find(teid);
}

sgw_tunnel_t* sgw_tunnel_find_by_interface_type(
        sgw_bearer_t *bearer, c_uint8_t interface_type)
{
    sgw_tunnel_t *tunnel = NULL;

    d_assert(bearer, return NULL,);

    tunnel = sgw_tunnel_first(bearer);
    while(tunnel)
    {
        if (tunnel->interface_type == interface_type)
        {
            return tunnel;
        }

        tunnel = sgw_tunnel_next(tunnel);
    }

    return NULL;
}

sgw_tunnel_t* sgw_s1u_tunnel_in_bearer(sgw_bearer_t *bearer)
{
    return sgw_tunnel_find_by_interface_type(
            bearer, GTP_F_TEID_S1_U_SGW_GTP_U);
}
sgw_tunnel_t* sgw_s5u_tunnel_in_bearer(sgw_bearer_t *bearer)
{
    return sgw_tunnel_find_by_interface_type(
            bearer, GTP_F_TEID_S5_S8_SGW_GTP_U);
}

sgw_tunnel_t* sgw_tunnel_first(sgw_bearer_t *bearer)
{
    d_assert(bearer, return NULL, "Null param");
    return list_first(&bearer->tunnel_list);
}

sgw_tunnel_t* sgw_tunnel_next(sgw_tunnel_t *tunnel)
{
    return list_next(tunnel);
}

#define TRACE_MODULE _pgw_context
#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"
#include "core_lib.h"
#include <mongoc.h>

#include "fd_lib.h"
#include "gtp_path.h"

#include "context.h"
#include "pgw_context.h"

static pgw_context_t self;

pool_declare(pgw_sgw_pool, pgw_sgw_t, MAX_NUM_OF_GTP_NODE);

index_declare(pgw_sess_pool, pgw_sess_t, MAX_POOL_OF_SESS);
index_declare(pgw_bearer_pool, pgw_bearer_t, MAX_POOL_OF_BEARER);

pool_declare(pgw_ip_pool_pool, pgw_ip_pool_t, MAX_POOL_OF_SESS);
pool_declare(pgw_pf_pool, pgw_pf_t, MAX_POOL_OF_PF);

static int context_initiaized = 0;

status_t pgw_context_init()
{
    d_assert(context_initiaized == 0, return CORE_ERROR,
            "PGW context already has been initialized");

    memset(&self, 0, sizeof(pgw_context_t));

    pool_init(&pgw_sgw_pool, MAX_NUM_OF_GTP_NODE);
    list_init(&self.sgw_list);

    index_init(&pgw_sess_pool, MAX_POOL_OF_SESS);
    index_init(&pgw_bearer_pool, MAX_POOL_OF_BEARER);

    pool_init(&pgw_ip_pool_pool, MAX_POOL_OF_SESS);
    pool_init(&pgw_pf_pool, MAX_POOL_OF_PF);

    self.sess_hash = hash_make();

    context_initiaized = 1;

    return CORE_OK;
}

status_t pgw_context_final()
{
    d_assert(context_initiaized == 1, return CORE_ERROR,
            "PGW context already has been finalized");

    pgw_sess_remove_all();
    pgw_sgw_remove_all();

    d_assert(self.sess_hash, , "Null param");
    hash_destroy(self.sess_hash);

    if (index_size(&pgw_sess_pool) != pool_avail(&pgw_sess_pool))
        d_warn("%d not freed in pgw_sess_pool[%d] in PGW-Context",
                index_size(&pgw_sess_pool) - pool_avail(&pgw_sess_pool),
                index_size(&pgw_sess_pool));
    d_trace(3, "%d not freed in pgw_sess_pool[%d] in PGW-Context\n",
            index_size(&pgw_sess_pool) - pool_avail(&pgw_sess_pool),
            index_size(&pgw_sess_pool));

    if (index_size(&pgw_ip_pool_pool) != pool_avail(&pgw_ip_pool_pool))
    {
        d_warn("%d not freed in pgw_ip_pool[%d] in PGW-Context",
                index_size(&pgw_ip_pool_pool) - pool_avail(&pgw_ip_pool_pool),
                index_size(&pgw_ip_pool_pool));
    }
    d_trace(3, "%d not freed in pgw_ip_pool[%d] in PGW-Context\n",
            index_size(&pgw_ip_pool_pool) - pool_avail(&pgw_ip_pool_pool),
            index_size(&pgw_ip_pool_pool));

    pool_final(&pgw_pf_pool);
    pool_final(&pgw_ip_pool_pool);

    index_final(&pgw_bearer_pool);
    index_final(&pgw_sess_pool);

    pool_final(&pgw_sgw_pool);

    context_initiaized = 0;
    
    return CORE_OK;
}

pgw_context_t* pgw_self()
{
    return &self;
}

static status_t pgw_context_prepare()
{
    self.gtpc_port = GTPV2_C_UDP_PORT;
    self.gtpu_port = GTPV1_U_UDP_PORT;

    return CORE_OK;
}

static status_t pgw_context_validation()
{
    if (self.fd_conf_path == NULL)
    {
        d_error("No PGW.FD_CONF_PATH in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.gtpc_addr == 0)
    {
        d_error("No PGW.NEWORK.GTPC_IPV4 in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.gtpu_addr == 0)
    {
        d_error("No PGW.NEWORK.GTPU_IPV4 in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.num_of_ue_network == 0)
    {
        d_error("No PGW.UE_NETWORK.IPV4_POOL in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.primary_dns_addr == 0)
    {
        d_error("No PGW.DNS.PRIMARY_IPV4 in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    return CORE_OK;
}

status_t pgw_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;
    bson_iter_t iter;
    c_uint32_t length = 0;

    d_assert(config, return CORE_ERROR, );

    rv = pgw_context_prepare();
    if (rv != CORE_OK) return rv;

    if (!bson_iter_init(&iter, config->bson))
    {
        d_error("bson_iter_init failed in this document");
        return CORE_ERROR;
    }

    while(bson_iter_next(&iter))
    {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "PGW") && BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_t pgw_iter;
            bson_iter_recurse(&iter, &pgw_iter);
            while(bson_iter_next(&pgw_iter))
            {
                const char *pgw_key = bson_iter_key(&pgw_iter);
                if (!strcmp(pgw_key, "FD_CONF_PATH") &&
                    BSON_ITER_HOLDS_UTF8(&pgw_iter))
                {
                    self.fd_conf_path = bson_iter_utf8(&pgw_iter, &length);
                }
                else if (!strcmp(pgw_key, "NETWORK"))
                {
                    bson_iter_t network_iter;

                    if (BSON_ITER_HOLDS_ARRAY(&pgw_iter))
                    {
                        bson_iter_t array_iter;
                        bson_iter_recurse(&pgw_iter, &array_iter);
                        if (bson_iter_next(&array_iter))
                        {
                            /* We will pick only first item of SGW.NETWORK
                             * if the type is an array */
                            bson_iter_recurse(&array_iter, &network_iter);
                        }
                    }
                    else if (BSON_ITER_HOLDS_DOCUMENT(&pgw_iter))
                    {
                        bson_iter_recurse(&pgw_iter, &network_iter);
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
                else if (!strcmp(pgw_key, "UE_NETWORK"))
                {
                    int ue_network_index = 0;
                    bson_iter_t ue_network_array;

                    if (BSON_ITER_HOLDS_ARRAY(&pgw_iter))
                    {
                        bson_iter_recurse(&pgw_iter, &ue_network_array);
                        d_assert(bson_iter_next(&ue_network_array),
                                return CORE_ERROR,);
                    }
                    else if (BSON_ITER_HOLDS_DOCUMENT(&pgw_iter))
                    {
                        memcpy(&ue_network_array, &pgw_iter,
                                sizeof(ue_network_array));
                    }
                    else
                        d_assert(0, return CORE_ERROR,);

                    do 
                    {
                        bson_iter_t ue_network_iter;
                        const char *ue_network_index_key =
                            bson_iter_key(&ue_network_array);
                        const char *if_name = NULL;
                        c_uint32_t addr = 0;
                        c_uint8_t bits = 0;

                        d_assert(ue_network_index_key, return CORE_ERROR,);
                        if (BSON_ITER_HOLDS_ARRAY(&pgw_iter))
                            ue_network_index = atoi(ue_network_index_key);
                        d_assert(ue_network_index < MAX_NUM_OF_GTP_NODE,
                                return CORE_ERROR,
                                "GTP NODE Overflow : %d", ue_network_index);

                        bson_iter_recurse(&ue_network_array, &ue_network_iter);
                        while(bson_iter_next(&ue_network_iter))
                        {
                            const char *ue_network_key =
                                bson_iter_key(&ue_network_iter);

                            if (!strcmp(ue_network_key, "IF_NAME") &&
                                BSON_ITER_HOLDS_UTF8(&ue_network_iter))
                            {
                                if_name = 
                                    bson_iter_utf8(&ue_network_iter, &length);
                            }
                            else if (!strcmp(ue_network_key, "IPV4_POOL") &&
                                    BSON_ITER_HOLDS_UTF8(&ue_network_iter))
                            {
                                char *v = (char *)bson_iter_utf8(
                                        &ue_network_iter, &length);
                                if (v)
                                {
                                    char *str = strsep(&v, "/");
                                    if (str)
                                    {
                                        addr = inet_addr(str);
                                        bits = atoi(v);
                                    }
                                }
                            }
                        }

                        if (addr && bits)
                        {
                            self.ue_network[self.num_of_ue_network].if_name =
                                if_name;
                            self.ue_network[self.num_of_ue_network].ipv4.addr =
                                addr;
                            self.ue_network[self.num_of_ue_network].ipv4.bits =
                                bits;
                            self.num_of_ue_network++;
                        }
                    } while(
                        BSON_ITER_HOLDS_ARRAY(&pgw_iter) &&
                        bson_iter_next(&ue_network_array));
                }
                else if (!strcmp(pgw_key, "DNS") &&
                    BSON_ITER_HOLDS_DOCUMENT(&pgw_iter))
                {
                    bson_iter_t dns_iter;
                    bson_iter_recurse(&pgw_iter, &dns_iter);
                    while(bson_iter_next(&dns_iter))
                    {
                        const char *dns_key = bson_iter_key(&dns_iter);
                        if (!strcmp(dns_key, "PRIMARY_IPV4") &&
                            BSON_ITER_HOLDS_UTF8(&dns_iter))
                        {
                            const char *v = bson_iter_utf8(&dns_iter, &length);
                            if (v) self.primary_dns_addr = inet_addr(v);

                        }
                        else if (!strcmp(dns_key, "SECONDARY_IPV4") &&
                            BSON_ITER_HOLDS_UTF8(&dns_iter))
                        {
                            const char *v = bson_iter_utf8(&dns_iter, &length);
                            if (v) self.secondary_dns_addr = inet_addr(v);
                        }
                    }
                }
            }
        }
    }

    rv = pgw_context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

status_t pgw_context_setup_trace_module()
{
    int fd = context_self()->trace_level.fd;
    int gtp = context_self()->trace_level.gtp;
    int others = context_self()->trace_level.others;

    if (fd)
    {
        if (fd <= 1) fd_g_debug_lvl = FD_LOG_ERROR;
        else if (fd <= 3) fd_g_debug_lvl = FD_LOG_NOTICE;
        else if (fd <= 5) fd_g_debug_lvl = FD_LOG_DEBUG;
        else fd_g_debug_lvl = FD_LOG_ANNOYING;

        extern int _pgw_fd_path;
        d_trace_level(&_pgw_fd_path, fd);
        extern int _fd_init;
        d_trace_level(&_fd_init, fd);
        extern int _fd_logger;
        d_trace_level(&_fd_logger, fd);
    }

    if (gtp)
    {
        extern int _pgw_sm;
        d_trace_level(&_pgw_sm, gtp);
        extern int _pgw_s5c_build;
        d_trace_level(&_pgw_s5c_build, gtp);
        extern int _pgw_s5c_handler;
        d_trace_level(&_pgw_s5c_handler, gtp);
        extern int _gtp_path;
        d_trace_level(&_gtp_path, gtp);
        extern int _pgw_gtp_path;
        d_trace_level(&_pgw_gtp_path, gtp);
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
        extern int _pgw_context;
        d_trace_level(&_pgw_context, others);
    }

    return CORE_OK;
}

pgw_sgw_t* pgw_sgw_add()
{
    pgw_sgw_t *sgw = NULL;

    pool_alloc_node(&pgw_sgw_pool, &sgw);
    d_assert(sgw, return NULL, "Null param");

    memset(sgw, 0, sizeof(pgw_sgw_t));

    list_init(&sgw->local_list);
    list_init(&sgw->remote_list);

    list_append(&self.sgw_list, sgw);
    
    return sgw;
}

status_t pgw_sgw_remove(pgw_sgw_t *sgw)
{
    d_assert(sgw, return CORE_ERROR, "Null param");

    list_remove(&self.sgw_list, sgw);

    gtp_xact_delete_all(sgw);

    pool_free_node(&pgw_sgw_pool, sgw);

    return CORE_OK;
}

status_t pgw_sgw_remove_all()
{
    pgw_sgw_t *sgw = NULL, *next_sgw = NULL;
    
    sgw = pgw_sgw_first();
    while (sgw)
    {
        next_sgw = pgw_sgw_next(sgw);

        pgw_sgw_remove(sgw);

        sgw = next_sgw;
    }

    return CORE_OK;
}

pgw_sgw_t* pgw_sgw_find(c_uint32_t addr)
{
    pgw_sgw_t *sgw = NULL;
    
    sgw = pgw_sgw_first();
    while (sgw)
    {
        if (sgw->addr.sin.sin_addr.s_addr == addr)
            break;

        sgw = pgw_sgw_next(sgw);
    }

    return sgw;
}

pgw_sgw_t* pgw_sgw_first()
{
    return list_first(&self.sgw_list);
}

pgw_sgw_t* pgw_sgw_next(pgw_sgw_t *sgw)
{
    return list_next(sgw);
}

static void *sess_hash_keygen(c_uint8_t *out, int *out_len,
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn)
{
    memcpy(out, imsi, imsi_len);
    core_cpystrn((char*)(out+imsi_len), apn, MAX_APN_LEN+1);
    *out_len = imsi_len+strlen((char*)(out+imsi_len));

    return out;
}

pgw_sess_t *pgw_sess_add(gtp_f_teid_t *sgw_s5c_teid,
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn, c_uint8_t ebi)
{
    pgw_sess_t *sess = NULL;
    pgw_bearer_t *bearer = NULL;
    pgw_sgw_t *sgw = NULL;
    c_uint32_t addr = 0;

    index_alloc(&pgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->pgw_s5c_teid = sess->index;  /* derived from an index */
    sess->pgw_s5c_addr = pgw_self()->gtpc_addr;

    /* Set IMSI */
    sess->imsi_len = imsi_len;
    memcpy(sess->imsi, imsi, sess->imsi_len);
    core_buffer_to_bcd(sess->imsi, sess->imsi_len, sess->imsi_bcd);

    addr = sgw_s5c_teid->ipv4_addr;
    sgw = pgw_sgw_find(addr);
    if (!sgw)
    {
        sgw = pgw_sgw_add();
        d_assert(sgw, return NULL, "Can't add SGW-GTP node");

        sgw->addr.sin.sin_addr.s_addr = addr;
        sgw->addr.c_sa_port = htons(GTPV2_C_UDP_PORT);
        sgw->addr.c_sa_family = AF_INET;
        sgw->sock = pgw_self()->gtpc_sock;
    }
    /* Setup GTP Node between PGW and SGW */
    CONNECT_SGW_GTP_NODE(sess, sgw);

    /* Set APN */
    core_cpystrn(sess->pdn.apn, apn, MAX_APN_LEN+1);

    list_init(&sess->bearer_list);

    core_cpystrn(sess->pdn.apn, apn, MAX_APN_LEN+1);

    bearer = pgw_bearer_add(sess);
    d_assert(bearer, pgw_sess_remove(sess); return NULL, 
            "Can't add default bearer context");
    bearer->ebi = ebi;

    sess->ip_pool = pgw_ip_pool_alloc();
    d_assert(sess->ip_pool, pgw_sess_remove(sess); return NULL, 
            "Can't add default bearer context");

    /* Generate Hash Key : IMSI + APN */
    sess_hash_keygen(sess->hash_keybuf, &sess->hash_keylen,
            imsi, imsi_len, apn);
    hash_set(self.sess_hash, sess->hash_keybuf, sess->hash_keylen, sess);

    return sess;
}

status_t pgw_sess_remove(pgw_sess_t *sess)
{
    d_assert(self.sess_hash, return CORE_ERROR, "Null param");
    d_assert(sess, return CORE_ERROR, "Null param");

    hash_set(self.sess_hash, sess->hash_keybuf, sess->hash_keylen, NULL);

    pgw_ip_pool_free(sess->ip_pool);

    pgw_bearer_remove_all(sess);

    index_free(&pgw_sess_pool, sess);

    return CORE_OK;
}

status_t pgw_sess_remove_all()
{
    hash_index_t *hi = NULL;
    pgw_sess_t *sess = NULL;

    for (hi = pgw_sess_first(); hi; hi = pgw_sess_next(hi))
    {
        sess = pgw_sess_this(hi);
        pgw_sess_remove(sess);
    }

    return CORE_OK;
}

pgw_sess_t* pgw_sess_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&pgw_sess_pool, index);
}

pgw_sess_t* pgw_sess_find_by_teid(c_uint32_t teid)
{
    return pgw_sess_find(teid);
}

pgw_sess_t* pgw_sess_find_by_imsi_apn(
    c_uint8_t *imsi, int imsi_len, c_int8_t *apn)
{
    c_uint8_t keybuf[MAX_IMSI_LEN+MAX_APN_LEN+1];
    int keylen = 0;

    d_assert(self.sess_hash, return NULL, "Null param");

    sess_hash_keygen(keybuf, &keylen, imsi, imsi_len, apn);
    return (pgw_sess_t *)hash_get(self.sess_hash, keybuf, keylen);
}

pgw_sess_t *pgw_sess_find_or_add_by_message(gtp_message_t *gtp_message)
{
    pgw_sess_t *sess = NULL;

    gtp_create_session_request_t *req = &gtp_message->create_session_request;
    c_int8_t apn[MAX_APN_LEN];

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

    apn_parse(apn, req->access_point_name.data, req->access_point_name.len);
    sess = pgw_sess_find_by_imsi_apn(req->imsi.data, req->imsi.len, apn);
    if (!sess)
    {
        sess = pgw_sess_add(
            req->sender_f_teid_for_control_plane.data,
            req->imsi.data, req->imsi.len, apn,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);
        d_assert(sess, return NULL, "No Session Context");
    }

    return sess;
}

hash_index_t* pgw_sess_first()
{
    d_assert(self.sess_hash, return NULL, "Null param");
    return hash_first(self.sess_hash);
}

hash_index_t* pgw_sess_next(hash_index_t *hi)
{
    return hash_next(hi);
}

pgw_sess_t *pgw_sess_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
}

pgw_bearer_t* pgw_bearer_add(pgw_sess_t *sess)
{
    pgw_bearer_t *bearer = NULL;

    d_assert(sess, return NULL, "Null param");

    index_alloc(&pgw_bearer_pool, &bearer);
    d_assert(bearer, return NULL, "Bearer context allocation failed");

    list_init(&bearer->pf_list);

    bearer->pgw_s5u_teid = bearer->index;
    bearer->pgw_s5u_addr = pgw_self()->gtpu_addr;
    
    bearer->sess = sess;
    list_append(&sess->bearer_list, bearer);

    return bearer;
}

status_t pgw_bearer_remove(pgw_bearer_t *bearer)
{
    d_assert(bearer, return CORE_ERROR, "Null param");
    d_assert(bearer->sess, return CORE_ERROR, "Null param");

    list_remove(&bearer->sess->bearer_list, bearer);

    pgw_pf_remove_all(bearer);

    index_free(&pgw_bearer_pool, bearer);

    return CORE_OK;
}

status_t pgw_bearer_remove_all(pgw_sess_t *sess)
{
    pgw_bearer_t *bearer = NULL, *next_bearer = NULL;

    d_assert(sess, return CORE_ERROR, "Null param");
    
    bearer = list_first(&sess->bearer_list);
    while (bearer)
    {
        next_bearer = list_next(bearer);

        pgw_bearer_remove(bearer);

        bearer = next_bearer;
    }

    return CORE_OK;
}

pgw_bearer_t* pgw_bearer_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&pgw_bearer_pool, index);
}

pgw_bearer_t* pgw_bearer_find_by_pgw_s5u_teid(c_uint32_t pgw_s5u_teid)
{
    return pgw_bearer_find(pgw_s5u_teid);
}

pgw_bearer_t* pgw_bearer_find_by_ebi(pgw_sess_t *sess, c_uint8_t ebi)
{
    pgw_bearer_t *bearer = NULL;
    
    d_assert(sess, return NULL, "Null param");

    bearer = pgw_bearer_first(sess);
    while (bearer)
    {
        if (bearer->ebi == ebi)
            break;

        bearer = pgw_bearer_next(bearer);
    }

    return bearer;
}

pgw_bearer_t* pgw_bearer_find_by_qci_arp(pgw_sess_t *sess, 
                                c_uint8_t qci,
                                c_uint8_t priority_level,
                                c_uint8_t pre_emption_capability,
                                c_uint8_t pre_emption_vulnerability)
{
    pgw_bearer_t *bearer = NULL;

    d_assert(sess, return NULL, "Null param");

    bearer = pgw_default_bearer_in_sess(sess);
    if (!bearer) return NULL;

    if (sess->pdn.qos.qci == qci &&
        sess->pdn.qos.arp.priority_level == priority_level &&
        sess->pdn.qos.arp.pre_emption_capability == 
            pre_emption_capability &&
        sess->pdn.qos.arp.pre_emption_vulnerability == 
            pre_emption_vulnerability)
    {
        return bearer;
    }

    bearer = pgw_bearer_next(bearer);
    while(bearer)
    {
        if (bearer->qos.qci == qci &&
            bearer->qos.arp.priority_level == priority_level &&
            bearer->qos.arp.pre_emption_capability == 
                pre_emption_capability &&
            bearer->qos.arp.pre_emption_vulnerability == 
                pre_emption_vulnerability)
        {
            return bearer;
        }
        bearer = pgw_bearer_next(bearer);
    }

    return NULL;
}

pgw_bearer_t* pgw_default_bearer_in_sess(pgw_sess_t *sess)
{
    return pgw_bearer_first(sess);
}

pgw_bearer_t* pgw_bearer_first(pgw_sess_t *sess)
{
    d_assert(sess, return NULL, "Null param");
    return list_first(&sess->bearer_list);
}

pgw_bearer_t* pgw_bearer_next(pgw_bearer_t *bearer)
{
    return list_next(bearer);
}

pgw_bearer_t* pgw_bearer_find_by_packet(pkbuf_t *pkt)
{
    pgw_bearer_t *default_bearer = NULL;
    pgw_bearer_t *bearer = NULL;
    hash_index_t *hi = NULL;
    pgw_sess_t *sess = NULL;
    struct ip *iph =  NULL;
#if 0 /* ADDR */
    char buf1[INET_ADDRSTRLEN];
    char buf2[INET_ADDRSTRLEN];
#endif

    d_assert(pkt, return NULL, "pkt is NULL");

    iph = (struct ip *)pkt->payload;

    /* FIXME : Only support IPV4 */
    if (iph->ip_v != 4) /* IPv4 */
    {
        return NULL;
    }

#if 0 /* ADDR */
    d_trace(50, "Src(%s)-> Dst(%s), Protocol: %d\n",
            INET_NTOP(&iph->ip_src.s_addr,buf1),
            INET_NTOP(&iph->ip_dst.s_addr,buf2),
            iph->ip_p);
#endif

    /* TODO: Need to use the method of FAST matching algorithm and 
     *          implementation .
     *       Until be ready, linear searching will be use to find the bearer.
     */

    for (hi = pgw_sess_first(); hi; hi = pgw_sess_next(hi))
    {
        sess = pgw_sess_this(hi);
#if 0 /* ADDR */
        d_trace(50, "Dst(%s) in Pkt : PAA(%s) in PDN\n",
                INET_NTOP(&iph->ip_dst.s_addr,buf1),
                INET_NTOP(&sess->pdn.paa.ipv4_addr, buf2));
#endif

        if (iph->ip_dst.s_addr == sess->pdn.paa.ipv4_addr)
        {
            /* Found */

            /* Save the default bearer */
            default_bearer = pgw_default_bearer_in_sess(sess);
            d_assert(default_bearer, return NULL, "No default Bearer");

            bearer = pgw_bearer_next(default_bearer);
            /* Find the bearer with matched */
            for (; bearer; bearer = pgw_bearer_next(bearer))
            {
                pgw_pf_t *pf = NULL;

                for (pf = pgw_pf_first(bearer); pf; pf = pgw_pf_next(pf))
                {
                    d_trace(50,"Dir:%d Proto:%d Src = 0x%x/0x%x (low:%d high:%d)"
                            "Dst = 0x%x/0x%x (low:%d high:%d)\n",
                            pf->direction,
                            pf->rule.proto,
                            ntohl(pf->rule.ipv4.local.addr),
                            ntohl(pf->rule.ipv4.local.mask),
                            pf->rule.port.local.low,
                            pf->rule.port.local.high,
                            ntohl(pf->rule.ipv4.remote.addr),
                            ntohl(pf->rule.ipv4.remote.mask),
                            pf->rule.port.remote.low,
                            pf->rule.port.remote.high);

                    if (pf->direction != 1)
                    {
                        continue;
                    }

                    if (((iph->ip_src.s_addr & pf->rule.ipv4.local.mask) ==
                          pf->rule.ipv4.local.addr)  &&
                        ((iph->ip_dst.s_addr & pf->rule.ipv4.remote.mask) ==
                          pf->rule.ipv4.remote.addr))
                    {
                        /* Protocol match */
                        if (pf->rule.proto == 0) /* IP */
                        {
                            /* No need to match port */
                            break;
                        }

                        if (pf->rule.proto == iph->ip_p)
                        {
                            if (pf->rule.proto == IPPROTO_TCP)
                            {
                                struct tcphdr *tcph = 
                                    (struct tcphdr *)
                                    ((char *)iph + (iph->ip_hl)*4);

                                /* Source port */
                                if (pf->rule.port.local.low && 
                                      ntohs(tcph->th_sport) < 
                                              pf->rule.port.local.low)
                                {
                                    continue;
                                }

                                if (pf->rule.port.local.high && 
                                      ntohs(tcph->th_sport) > 
                                              pf->rule.port.local.high)
                                {
                                    continue;
                                }

                                /* Dst Port*/
                                if (pf->rule.port.remote.low && 
                                      ntohs(tcph->th_dport) < 
                                              pf->rule.port.remote.low)
                                {
                                    continue;
                                }

                                if (pf->rule.port.remote.high && 
                                      ntohs(tcph->th_dport) > 
                                              pf->rule.port.remote.high)
                                {
                                    continue;
                                }

                                /* Matched */
                                break;
                            }
                            else if (pf->rule.proto == IPPROTO_UDP)
                            {
                                struct udphdr *udph = 
                                    (struct udphdr *)
                                    ((char *)iph + (iph->ip_hl)*4);

                                /* Source port */
                                if (pf->rule.port.local.low && 
                                      ntohs(udph->uh_sport) < 
                                              pf->rule.port.local.low)
                                {
                                    continue;
                                }

                                if (pf->rule.port.local.high && 
                                      ntohs(udph->uh_sport) > 
                                              pf->rule.port.local.high)
                                {
                                    continue;
                                }

                                /* Dst Port*/
                                if (pf->rule.port.remote.low && 
                                      ntohs(udph->uh_dport) < 
                                              pf->rule.port.remote.low)
                                {
                                    continue;
                                }

                                if (pf->rule.port.remote.high && 
                                      ntohs(udph->uh_dport) > 
                                              pf->rule.port.remote.high)
                                {
                                    continue;
                                }

                                /* Matched */
                                break;
                            }
                            else
                            {
                                /* No need to match port */
                                break;
                            }

                        }
                    }

                }

                if (pf)
                {
                    bearer = pf->bearer;
                    d_trace(50,"Found bearer ebi = %d\n",bearer->ebi);
                    break;
                }

            }

            return (bearer ? bearer : default_bearer);
        }
    }

    return NULL;
}

pgw_pf_t *pgw_pf_add(pgw_bearer_t *bearer, c_uint32_t precedence)
{
    pgw_pf_t *pf = NULL;

    d_assert(bearer, return NULL, "Null param");

    pool_alloc_node(&pgw_pf_pool, &pf);
    d_assert(pf, return NULL, "Null param");

    pf->identifier = NEXT_ID(bearer->pf_identifier, 1, 15);
    pf->bearer = bearer;

    list_append(&bearer->pf_list, pf);

    return pf;
}

status_t pgw_pf_remove(pgw_pf_t *pf)
{
    d_assert(pf, return CORE_ERROR, "Null param");
    d_assert(pf->bearer, return CORE_ERROR, "Null param");

    list_remove(&pf->bearer->pf_list, pf);
    pool_free_node(&pgw_pf_pool, pf);

    return CORE_OK;
}

status_t pgw_pf_remove_all(pgw_bearer_t *bearer)
{
    pgw_pf_t *pf = NULL, *next_pf = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    
    pf = pgw_pf_first(bearer);
    while (pf)
    {
        next_pf = pgw_pf_next(pf);

        pgw_pf_remove(pf);

        pf = next_pf;
    }

    return CORE_OK;
}

pgw_pf_t* pgw_pf_find_by_id(pgw_bearer_t *bearer, c_uint8_t id)
{
    pgw_pf_t *pf = NULL;
    
    pf = pgw_pf_first(bearer);
    while (pf)
    {
        if (pf->identifier == id)
            return pf;

        pf = pgw_pf_next(pf);
    }

    return CORE_OK;
}

pgw_pf_t* pgw_pf_first(pgw_bearer_t *bearer)
{
    return list_first(&bearer->pf_list);
}

pgw_pf_t* pgw_pf_next(pgw_pf_t *pf)
{
    return list_next(pf);
}

status_t pgw_ip_pool_generate()
{
    int i, j;
    int pool_index = 0;

    for (i = 0; i < self.num_of_ue_network; i++)
    {
        c_uint32_t mask = 
            htonl(0xffffffff << (32 - self.ue_network[i].ipv4.bits));
        c_uint32_t prefix = self.ue_network[i].ipv4.addr & mask;

#if 1   /* Update IP assign rule from bradon's comment */
        c_uint32_t broadcast = prefix + ~mask;

        for (j = 1; j < (0xffffffff >> self.ue_network[i].ipv4.bits) &&
                pool_index < MAX_POOL_OF_SESS; j++)
        {
            pgw_ip_pool_t *ip_pool = &pgw_ip_pool_pool.pool[pool_index];
            ip_pool->ue_addr = prefix + htonl(j);

            /* Exclude Network Address */
            if (ip_pool->ue_addr == prefix) continue;

            /* Exclude Broadcast Address */
            if (ip_pool->ue_addr == broadcast) continue;

            /* Exclude TUN IP Address */
            if (ip_pool->ue_addr == self.ue_network[i].ipv4.addr) continue;

            pool_index++;
        }
#else   /* Deprecated */
        /* Exclude X.X.X.0, X.X.X.255 addresses from ip pool */
        c_uint32_t exclude_mask[] = { 0, 255 };

        for (j = 1; j < (0xffffffff >> self.ip_pool[i].mask) && 
                pool_index < MAX_NUM_OF_SESS; j++)
        {
            int exclude = 0;
            pgw_ip_pool_t *ip_pool = &pgw_ip_pool_pool.pool[pool_index];
            ip_pool->ue_addr = prefix + htonl(j);

            for (k = 0; k < sizeof(exclude_mask)/sizeof(exclude_mask[0]); k++)
            {
                if ((htonl(ip_pool->ue_addr) & 0x000000ff) == exclude_mask[k])
                {
                    exclude = 1;
                }
            }

            if (exclude)
            {
                continue;
            }
            pool_index++;
        }
#endif
    }

    return CORE_OK;
}
pgw_ip_pool_t* pgw_ip_pool_alloc()
{
    pgw_ip_pool_t *ip_pool = NULL;

    pool_alloc_node(&pgw_ip_pool_pool, &ip_pool);
    d_assert(ip_pool, return NULL, "IP Pool context allocation failed");

    list_append(&self.ip_pool_list, ip_pool);

    return ip_pool;
}
status_t pgw_ip_pool_free(pgw_ip_pool_t *ip_pool)
{
    list_remove(&self.ip_pool_list, ip_pool);
    pool_free_node(&pgw_ip_pool_pool, ip_pool);

    return CORE_OK;
}

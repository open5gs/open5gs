#define TRACE_MODULE _pgw_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"
#include "core_lib.h"

#include "fd_lib.h"
#include "gtp_path.h"

#include "context.h"
#include "pgw_context.h"

static pgw_context_t self;

pool_declare(pgw_sgw_pool, pgw_sgw_t, MAX_NUM_OF_GTP_NODE);

index_declare(pgw_sess_pool, pgw_sess_t, MAX_POOL_OF_SESS);
index_declare(pgw_bearer_pool, pgw_bearer_t, MAX_POOL_OF_BEARER);

pool_declare(pgw_ip_pool_pool, pgw_ip_pool_t, MAX_POOL_OF_SESS);

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
    self.s5c_port = GTPV2_C_UDP_PORT;
    self.s5u_port = GTPV1_U_UDP_PORT;

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
    if (self.s5c_addr == 0)
    {
        d_error("No PGW.NEWORK.S5C_ADDR in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.s5u_addr == 0)
    {
        d_error("No PGW.NEWORK.S5U_ADDR in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.tun_dev_name == NULL)
    {
        d_error("No PGW.TUNNEL.DEV_NAME in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.num_of_ip_pool == 0)
    {
        d_error("No PGW.IP_POOL.CIDR in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.primary_dns_addr == 0)
    {
        d_error("No PGW.DNS.PRIMARY_ADDR in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    return CORE_OK;
}

status_t pgw_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;

    char *json = config->json;
    jsmntok_t *token = config->token;

    typedef enum {
        START, ROOT,
        PGW_START, PGW_ROOT,
        SKIP, STOP
    } parse_state;
    parse_state state = START;
    parse_state stack = STOP;

    size_t root_tokens = 0;
    size_t pgw_tokens = 0;
    size_t skip_tokens = 0;
    int i, j, m, n;
    int arr, size;

    rv = pgw_context_prepare();
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
                if (jsmntok_equal(json, t, "PGW") == 0)
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
            case PGW_START:
            {
                state = PGW_ROOT;
                pgw_tokens = t->size;

                break;
            }
            case PGW_ROOT:
            {
                if (jsmntok_equal(json, t, "FD_CONF_PATH") == 0)
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

                            if (jsmntok_equal(json, t+m, "S5C_ADDR") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s5c_addr = inet_addr(v);
                            }
                            else if (jsmntok_equal(json, t+m, "S5C_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s5c_port = atoi(v);
                            }
                            else if (jsmntok_equal(json, t+m, "S5U_ADDR") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s5u_addr = inet_addr(v);
                            }
                            else if (jsmntok_equal(json, t+m, "S5U_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s5u_port = atoi(v);
                            }
                        }
                    }
                }
                else if (jsmntok_equal(json, t, "TUNNEL") == 0)
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

                            if (jsmntok_equal(json, t+m, "DEV_NAME") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                self.tun_dev_name = v;
                            }
                        }
                    }
                }
                else if (jsmntok_equal(json, t, "IP_POOL") == 0)
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

                            if (jsmntok_equal(json, t+m, "CIDR") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v)
                                {
                                    char *prefix = strsep(&v, "/");
                                    if (prefix)
                                    {
                                        self.ip_pool[self.num_of_ip_pool].prefix
                                            = inet_addr(prefix);
                                        self.ip_pool[self.num_of_ip_pool].mask
                                            = atoi(v);
                                    }
                                }

                                self.num_of_ip_pool++;
                            }
                        }
                    }
                }
                else if (jsmntok_equal(json, t, "DNS") == 0)
                {
                    for (m = 1, n = 1; n > 0; m++, n--)
                    {
                        n += (t+m)->size;
                        if (jsmntok_equal(json, t+m, "PRIMARY_ADDR") == 0)
                        {
                            char *v = jsmntok_to_string(json, t+m+1);
                            if (v) self.primary_dns_addr = inet_addr(v);
                        }
                        else if (jsmntok_equal(
                                    json, t+m, "SECONDARY_ADDR") == 0)
                        {
                            char *v = jsmntok_to_string(json, t+m+1);
                            if (v) self.secondary_dns_addr = inet_addr(v);
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

    gtp_xact_delete_all(sgw);

    list_remove(&self.sgw_list, sgw);
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

pgw_sgw_t* pgw_sgw_find(c_uint32_t addr, c_uint16_t port)
{
    pgw_sgw_t *sgw = NULL;
    
    sgw = pgw_sgw_first();
    while (sgw)
    {
        if (sgw->addr == addr && sgw->port == port)
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

pgw_sess_t *pgw_sess_add(
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn, c_uint8_t id)
{
    pgw_sess_t *sess = NULL;
    pgw_bearer_t *bearer = NULL;

    index_alloc(&pgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->pgw_s5c_teid = sess->index;  /* derived from an index */
    sess->pgw_s5c_addr = pgw_self()->s5c_addr;

    /* Set IMSI */
    sess->imsi_len = imsi_len;
    memcpy(sess->imsi, imsi, sess->imsi_len);
    core_buffer_to_bcd(sess->imsi, sess->imsi_len, sess->imsi_bcd);

    /* Set APN */
    core_cpystrn(sess->pdn.apn, apn, MAX_APN_LEN+1);

    list_init(&sess->bearer_list);

    core_cpystrn(sess->pdn.apn, apn, MAX_APN_LEN+1);

    bearer = pgw_bearer_add(sess, id);
    d_assert(bearer, pgw_sess_remove(sess); return NULL, 
            "Can't add default bearer context");

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

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No IMSI");
        return NULL;
    }

    if (req->access_point_name.presence == 0)
    {
        d_error("No APN");
        return NULL;
    }

    apn_parse(apn, req->access_point_name.data, req->access_point_name.len);
    sess = pgw_sess_find_by_imsi_apn(req->imsi.data, req->imsi.len, apn);
    if (!sess)
    {
        sess = pgw_sess_add(req->imsi.data, req->imsi.len, apn,
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

pgw_bearer_t* pgw_bearer_add(pgw_sess_t *sess, c_uint8_t ebi)
{
    pgw_bearer_t *bearer = NULL;

    d_assert(sess, return NULL, "Null param");

    index_alloc(&pgw_bearer_pool, &bearer);
    d_assert(bearer, return NULL, "Bearer context allocation failed");

    bearer->ebi = ebi;
    bearer->pgw_s5u_teid = bearer->index;
    bearer->pgw_s5u_addr = pgw_self()->s5u_addr;
    
    bearer->sess = sess;
    list_append(&sess->bearer_list, bearer);

    return bearer;
}

status_t pgw_bearer_remove(pgw_bearer_t *bearer)
{
    d_assert(bearer, return CORE_ERROR, "Null param");
    d_assert(bearer->sess, return CORE_ERROR, "Null param");

    list_remove(&bearer->sess->bearer_list, bearer);
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

    bearer = list_first(&sess->bearer_list);
    while (bearer)
    {
        if (bearer->ebi == ebi)
            break;

        bearer = list_next(bearer);
    }

    return bearer;
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
    pgw_bearer_t *bearer = NULL;
    hash_index_t *hi = NULL;
    pgw_sess_t *sess = NULL;
    struct ip *iph =  NULL;
    char buf1[INET_ADDRSTRLEN];
    char buf2[INET_ADDRSTRLEN];

    d_assert(pkt, return NULL, "pkt is NULL");

    iph = (struct ip *)pkt->payload;

    /* FIXME : Only support IPV4 */
    if (iph->ip_v != 4) /* IPv4 */
    {
        return NULL;
    }

    d_trace(50, "Src(%s)-> Dst(%s), Protocol: %d\n",
            INET_NTOP(&iph->ip_src.s_addr,buf1),
            INET_NTOP(&iph->ip_dst.s_addr,buf2),
            iph->ip_p);

    /* FIXME: Need API to find the bearer with packet filter */
    /* Iterate session */
    for (hi = pgw_sess_first(); hi; hi = pgw_sess_next(hi))
    {
        sess = pgw_sess_this(hi);
        d_trace(50, "Dst(%s) in Pkt : PAA(%s) in PDN\n",
                INET_NTOP(&iph->ip_dst.s_addr,buf1),
                INET_NTOP(&sess->pdn.paa.ipv4_addr, buf2));

        if (iph->ip_dst.s_addr == sess->pdn.paa.ipv4_addr)
        {
            /* Found */
            bearer = pgw_default_bearer_in_sess(sess);
            d_assert(bearer, return NULL, "No Bearer");

            d_trace(50, "Found bearer(EBI = %d)\n", bearer->ebi);
            return bearer;
        }
    }

    return NULL;
}

status_t pgw_ip_pool_generate()
{
    int i, j, k;
    int pool_index = 0;

    for (i = 0; i < self.num_of_ip_pool; i++)
    {
        c_uint32_t mask = htonl(0xffffffff << (32 - self.ip_pool[i].mask));
        c_uint32_t prefix = self.ip_pool[i].prefix & mask;

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

#define TRACE_MODULE _pgw_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "gtp_path.h"

#include "context.h"
#include "pgw_context.h"

static pgw_context_t self;

index_declare(pgw_sess_pool, pgw_sess_t, MAX_NUM_OF_UE);
index_declare(pgw_bearer_pool, pgw_bearer_t, MAX_NUM_OF_UE_BEARER);

pool_declare(pgw_ip_pool_pool, pgw_ip_pool_t, MAX_NUM_OF_UE);

static int context_initiaized = 0;

status_t pgw_context_init()
{
    d_assert(context_initiaized == 0, return CORE_ERROR,
            "PGW context already has been initialized");

    memset(&self, 0, sizeof(pgw_context_t));

    index_init(&pgw_sess_pool, MAX_NUM_OF_UE);
    list_init(&self.sess_list);
    index_init(&pgw_bearer_pool, MAX_NUM_OF_UE_BEARER);

    pool_init(&pgw_ip_pool_pool, MAX_NUM_OF_UE);

    list_init(&self.s5c_node.local_list);
    list_init(&self.s5c_node.remote_list);

    context_initiaized = 1;

    return CORE_OK;
}

status_t pgw_context_final()
{
    d_assert(context_initiaized == 1, return CORE_ERROR,
            "PGW context already has been finalized");

    gtp_xact_delete_all(&self.s5c_node);
    pgw_sess_remove_all();

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
    self.s5c_node.port = GTPV2_C_UDP_PORT;

    self.s5u_port = GTPV1_U_UDP_PORT;

    return CORE_OK;
}

static status_t pgw_context_validation()
{
    if (self.s5c_node.addr == 0)
    {
        d_error("No SGW.NEWORK.S5C_ADDR in '%s'",
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
        SGW_START, SGW_ROOT,
        PGW_START, PGW_ROOT,
        SKIP, STOP
    } parse_state;
    parse_state state = START;
    parse_state stack = STOP;

    size_t root_tokens = 0;
    size_t sgw_tokens = 0;
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
                if (jsmntok_equal(json, t, "SGW") == 0)
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
                    }

                    for (arr = 0; arr < size; arr++)
                    {
                        for (n = 1; n > 0; m++, n--)
                        {
                            n += (t+m)->size;

                            if (jsmntok_equal(json, t+m, "S5C_ADDR") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s5c_node.addr = inet_addr(v);
                            }
                            else if (jsmntok_equal(json, t+m, "S5C_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.s5c_node.port = atoi(v);
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
    int others = context_self()->trace_level.others;
    int gtp = context_self()->trace_level.gtp;

    if (gtp)
    {
        extern int _pgw_sm;
        d_trace_level(&_pgw_sm, gtp);
        extern int _pgw_handler;
        d_trace_level(&_pgw_handler, gtp);
        extern int _gtp_path;
        d_trace_level(&_gtp_path, gtp);
        extern int _pgw_path;
        d_trace_level(&_pgw_path, gtp);
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

pgw_bearer_t *pgw_sess_add(c_int8_t *apn, c_uint8_t id)
{
    pgw_sess_t *sess = NULL;
    pgw_bearer_t *bearer = NULL;

    index_alloc(&pgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->pgw_s5c_teid = sess->index;  /* derived from an index */
    sess->pgw_s5c_addr = pgw_self()->s5c_addr;

    list_init(&sess->bearer_list);
    list_append(&self.sess_list, sess);

    strcpy(sess->apn, apn);

    bearer = pgw_bearer_add(sess, id);
    d_assert(bearer, pgw_sess_remove(sess); return NULL, 
            "Can't add default bearer context");

    sess->ip_pool = pgw_ip_pool_alloc();
    d_assert(sess->ip_pool, pgw_sess_remove(sess); return NULL, 
            "Can't add default bearer context");

    return bearer;
}

status_t pgw_sess_remove(pgw_sess_t *sess)
{
    d_assert(sess, return CORE_ERROR, "Null param");

    pgw_ip_pool_free(sess->ip_pool);

    pgw_bearer_remove_all(sess);

    list_remove(&self.sess_list, sess);
    index_free(&pgw_sess_pool, sess);

    return CORE_OK;
}

status_t pgw_sess_remove_all()
{
    pgw_sess_t *sess = NULL, *next_sess = NULL;
    
    sess = pgw_sess_first();
    while (sess)
    {
        next_sess = pgw_sess_next(sess);

        pgw_sess_remove(sess);

        sess = next_sess;
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

pgw_sess_t* pgw_sess_find_by_apn(c_int8_t *apn)
{
    pgw_sess_t *sess = NULL;
    
    sess = pgw_sess_first();
    while (sess)
    {
        if (strcmp(sess->apn, apn) == 0)
            break;

        sess = pgw_sess_next(sess);
    }

    return sess;
}

pgw_sess_t* pgw_sess_first()
{
    return list_first(&self.sess_list);
}

pgw_sess_t* pgw_sess_next(pgw_sess_t *sess)
{
    return list_next(sess);
}

pgw_bearer_t* pgw_bearer_add(pgw_sess_t *sess, c_uint8_t id)
{
    pgw_bearer_t *bearer = NULL;

    d_assert(sess, return NULL, "Null param");

    index_alloc(&pgw_bearer_pool, &bearer);
    d_assert(bearer, return NULL, "Bearer context allocation failed");

    bearer->id = id;
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

pgw_bearer_t* pgw_bearer_find_by_id(pgw_sess_t *sess, c_uint8_t id)
{
    pgw_bearer_t *bearer = NULL;
    
    d_assert(sess, return NULL, "Null param");

    bearer = list_first(&sess->bearer_list);
    while (bearer)
    {
        if (bearer->id == id)
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
    for (sess = pgw_sess_first(); sess; sess = pgw_sess_next(sess))
    {
        d_trace(50, "Dst(%s) in Pkt : PAA(%s) in PDN\n",
                INET_NTOP(&iph->ip_dst.s_addr,buf1),
                INET_NTOP(&sess->paa.ipv4_addr, buf2));

        if (iph->ip_dst.s_addr == sess->paa.ipv4_addr)
        {
            /* Found */
            bearer = pgw_default_bearer_in_sess(sess);
            d_assert(bearer, return NULL, "No Bearer");

            d_trace(50, "Found bearer(id = %d)\n",bearer->id);
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
                pool_index < MAX_NUM_OF_UE; j++)
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

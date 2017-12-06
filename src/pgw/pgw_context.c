#define TRACE_MODULE _pgw_context
#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"
#include "core_lib.h"

#include <mongoc.h>
#include <yaml.h>
#include "yaml_helper.h"

#include "gtp_types.h"
#include "gtp_conv.h"
#include "gtp_node.h"
#include "gtp_path.h"
#include "gtp_xact.h"

#include "fd_lib.h"

#include "context.h"
#include "pgw_context.h"

static pgw_context_t self;

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

    list_init(&self.gtpc_list);
    list_init(&self.gtpc_list6);

    gtp_node_init();
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

    gtp_remove_all_nodes(&self.sgw_list);
    gtp_node_final();

    sock_remove_all_nodes(&self.gtpc_list);
    sock_remove_all_nodes(&self.gtpc_list6);

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
        d_error("No pgw.freeDiameter in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (list_first(&self.gtpc_list) == NULL &&
        list_first(&self.gtpc_list6) == NULL)
    {
        d_error("No pgw.gtpc in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.gtpu_addr == 0)
    {
        d_error("No pgw.gtpu in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.num_of_ue_network == 0)
    {
        d_error("No pgw.pdn.addr in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.dns.primary == 0)
    {
        d_error("No pgw.dns in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    return CORE_OK;
}

status_t pgw_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;
    yaml_document_t *document = NULL;
    yaml_iter_t root_iter;

    d_assert(config, return CORE_ERROR,);
    document = config->document;
    d_assert(document, return CORE_ERROR,);

    rv = pgw_context_prepare();
    if (rv != CORE_OK) return rv;

    yaml_iter_init(&root_iter, document);
    while(yaml_iter_next(&root_iter))
    {
        const char *root_key = yaml_iter_key(&root_iter);
        d_assert(root_key, return CORE_ERROR,);
        if (!strcmp(root_key, "pgw"))
        {
            yaml_iter_t pgw_iter;
            yaml_iter_recurse(&root_iter, &pgw_iter);
            while(yaml_iter_next(&pgw_iter))
            {
                const char *pgw_key = yaml_iter_key(&pgw_iter);
                d_assert(pgw_key, return CORE_ERROR,);
                if (!strcmp(pgw_key, "freeDiameter"))
                {
                    self.fd_conf_path = yaml_iter_value(&pgw_iter);
                }
                else if (!strcmp(pgw_key, "gtpc"))
                {
                    yaml_iter_t gtpc_array, gtpc_iter;
                    yaml_iter_recurse(&pgw_iter, &gtpc_array);
                    do
                    {
                        int family = AF_UNSPEC;
                        int i, num = 0;
#define MAX_NUM_OF_HOSTNAME     16
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        c_uint16_t port = self.gtpc_port;
                        c_sockaddr_t *list = NULL;
                        sock_node_t *node = NULL;

                        if (yaml_iter_type(&gtpc_array) == YAML_MAPPING_NODE)
                        {
                            memcpy(&gtpc_iter, &gtpc_array,
                                    sizeof(yaml_iter_t));
                        }
                        else if (yaml_iter_type(&gtpc_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!yaml_iter_next(&gtpc_array))
                                break;
                            yaml_iter_recurse(&gtpc_array, &gtpc_iter);
                        }
                        else if (yaml_iter_type(&gtpc_array) ==
                            YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            d_assert(0, return CORE_ERROR,);

                        while(yaml_iter_next(&gtpc_iter))
                        {
                            const char *gtpc_key =
                                yaml_iter_key(&gtpc_iter);
                            d_assert(gtpc_key,
                                    return CORE_ERROR,);
                            if (!strcmp(gtpc_key, "family"))
                            {
                                const char *v = yaml_iter_value(&gtpc_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6)
                                {
                                    d_warn("Ignore family(%d) : AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            }
                            else if (!strcmp(gtpc_key, "addr") ||
                                    !strcmp(gtpc_key, "name"))
                            {
                                yaml_iter_t hostname_iter;
                                yaml_iter_recurse(&gtpc_iter, &hostname_iter);
                                d_assert(yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE, return CORE_ERROR,);

                                do
                                {
                                    if (yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    d_assert(num <= MAX_NUM_OF_HOSTNAME,
                                            return CORE_ERROR,);
                                    hostname[num++] = 
                                        yaml_iter_value(&hostname_iter);
                                } while(
                                    yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(gtpc_key, "port"))
                            {
                                const char *v = yaml_iter_value(&gtpc_iter);
                                if (v)
                                {
                                    port = atoi(v);
                                    self.gtpc_port = port;
                                }
                            }
                            else
                                d_warn("unknown key `%s`", gtpc_key);
                        }

                        list = NULL;
                        for (i = 0; i < num; i++)
                        {
                            rv = core_addaddrinfo(&list,
                                    family, hostname[i], port, AI_PASSIVE);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }

                        if (context_self()->parameter.no_ipv4 == 0)
                        {
                            rv = sock_add_node(&self.gtpc_list,
                                    &node, list, AF_INET);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }

                        if (context_self()->parameter.no_ipv6 == 0)
                        {
                            rv = sock_add_node(&self.gtpc_list6,
                                    &node, list, AF_INET6);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }

                        core_freeaddrinfo(list);

                    } while(yaml_iter_type(&gtpc_array) == YAML_SEQUENCE_NODE);

                    if (list_first(&self.gtpc_list) == NULL &&
                        list_first(&self.gtpc_list6) == NULL)
                    {
                        rv = sock_probe_node(
                                context_self()->parameter.no_ipv4 ?
                                    NULL : &self.gtpc_list,
                                context_self()->parameter.no_ipv6 ?
                                    NULL : &self.gtpc_list6,
                                self.gtpc_port);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
                }
                else if (!strcmp(pgw_key, "gtpu"))
                {
                    yaml_iter_t gtpu_array, gtpu_iter;
                    yaml_iter_recurse(&pgw_iter, &gtpu_array);
                    do
                    {
#if 0
                        pgw_gtpu_t *gtpu = NULL;
#endif
                        int family = AF_UNSPEC;
                        const char *hostname = NULL;
                        c_uint16_t port = GTPV1_U_UDP_PORT;

                        if (yaml_iter_type(&gtpu_array) == YAML_MAPPING_NODE)
                        {
                            memcpy(&gtpu_iter, &gtpu_array,
                                    sizeof(yaml_iter_t));
                        }
                        else if (yaml_iter_type(&gtpu_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!yaml_iter_next(&gtpu_array))
                                break;
                            yaml_iter_recurse(&gtpu_array, &gtpu_iter);
                        }
                        else if (yaml_iter_type(&gtpu_array) ==
                                YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            d_assert(0, return CORE_ERROR,);

                        while(yaml_iter_next(&gtpu_iter))
                        {
                            const char *gtpu_key =
                                yaml_iter_key(&gtpu_iter);
                            d_assert(gtpu_key,
                                    return CORE_ERROR,);
                            if (!strcmp(gtpu_key, "family"))
                            {
                                const char *v = yaml_iter_value(&gtpu_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6)
                                {
                                    d_warn("Ignore family(%d) : AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            }
                            else if (!strcmp(gtpu_key, "addr") ||
                                    !strcmp(gtpu_key, "name"))
                            {
                                hostname = yaml_iter_value(&gtpu_iter);
#if 1
                                if (hostname)
                                    self.gtpu_addr = inet_addr(hostname);
#endif
                            }
                            else if (!strcmp(gtpu_key, "port"))
                            {
                                const char *v = yaml_iter_value(&gtpu_iter);
                                if (v)
                                {
                                    port = atoi(v);
                                    self.gtpu_port = port;
                                }
                            }
                            else
                                d_warn("unknown key `%s`", gtpu_key);
                        }

#if 0
                        gtpu = pgw_gtpu_add(family, hostname, port);
                        d_assert(gtpu, return CORE_ERROR,);
#endif

                    } while(yaml_iter_type(&gtpu_array) == YAML_SEQUENCE_NODE);
                }
                else if (!strcmp(pgw_key, "ue_network"))
                {
                    yaml_iter_t ue_network_array, ue_network_iter;
                    yaml_iter_recurse(&pgw_iter, &ue_network_array);
                    do
                    {
                        c_uint32_t addr = 0;
                        c_uint8_t bits = 0;
                        const char *dev = NULL;
                        const char *apn = NULL;

                        d_assert(self.num_of_ue_network <=
                                MAX_NUM_OF_UE_NETWORK, return CORE_ERROR,);
                        if (yaml_iter_type(&ue_network_array) ==
                                YAML_MAPPING_NODE)
                        {
                            memcpy(&ue_network_iter, &ue_network_array,
                                    sizeof(yaml_iter_t));
                        }
                        else if (yaml_iter_type(&ue_network_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!yaml_iter_next(&ue_network_array))
                                break;
                            yaml_iter_recurse(&ue_network_array,
                                    &ue_network_iter);
                        }
                        else if (yaml_iter_type(&ue_network_array) ==
                                YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            d_assert(0, return CORE_ERROR,);

                        while(yaml_iter_next(&ue_network_iter))
                        {
                            const char *ue_network_key =
                                yaml_iter_key(&ue_network_iter);
                            d_assert(ue_network_key,
                                    return CORE_ERROR,);
                            if (!strcmp(ue_network_key, "addr"))
                            {
                                yaml_iter_t addr_iter;
                                yaml_iter_recurse(&ue_network_iter, &addr_iter);
                                d_assert(yaml_iter_type(&addr_iter) !=
                                    YAML_MAPPING_NODE, return CORE_ERROR,);

                                do
                                {
                                    char *v = NULL;

#if 0
                                    d_assert(ue_network->num_of_addr <=
                                            MAX_NUM_OF_PDN_ADDR,
                                            return CORE_ERROR,);
#endif

                                    if (yaml_iter_type(&addr_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!yaml_iter_next(&addr_iter))
                                            break;
                                    }

                                    v = (char *)yaml_iter_value(&addr_iter);
                                    if (v)
                                    {
                                        char *str = strsep(&v, "/");
                                        if (str)
                                        {
                                            addr = inet_addr(str);
                                            bits = atoi(v);
                                        }
                                    }
                                } while(
                                    yaml_iter_type(&addr_iter) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(ue_network_key, "dev"))
                            {
                                dev = yaml_iter_value(&ue_network_iter);
                            }
                            else if (!strcmp(ue_network_key, "apn"))
                            {
                                apn = yaml_iter_value(&ue_network_iter);
                                d_warn("Not implemented apn=%s", apn);
                            }
                            else
                                d_warn("unknown key `%s`", ue_network_key);
                        }

                        if (addr && bits)
                        {
                            self.ue_network[self.num_of_ue_network].ipv4.addr =
                                addr;
                            self.ue_network[self.num_of_ue_network].ipv4.bits =
                                bits;
                            self.ue_network[self.num_of_ue_network].if_name =
                                dev;
                            self.num_of_ue_network++;
                        }
                        else
                        {
                            d_warn("Ignore ue_network : addr(0x%x), bits(%d)",
                                    addr, bits);
                        }
                    } while(yaml_iter_type(&ue_network_array) ==
                            YAML_SEQUENCE_NODE);
                }
                else if (!strcmp(pgw_key, "dns"))
                {
                    int count = 0;
                    yaml_iter_t dns_iter;
                    yaml_iter_recurse(&pgw_iter, &dns_iter);
                    d_assert(yaml_iter_type(&dns_iter) !=
                        YAML_MAPPING_NODE, return CORE_ERROR,);

                    do
                    {
                        const char *v = NULL;

                        if (yaml_iter_type(&dns_iter) ==
                                YAML_SEQUENCE_NODE)
                        {
                            if (!yaml_iter_next(&dns_iter))
                                break;
                        }

                        v = yaml_iter_value(&dns_iter);
                        if (v)
                        {
                            if (count == 0)
                            {
                                self.dns.primary = inet_addr(v);
                            }
                            else if (count == 1)
                            {
                                self.dns.secondary = inet_addr(v);
                            }
                            else
                                d_warn("Ignored %d DNS(%s)", count, v);
                        }

                        count++;
                    } while(
                        yaml_iter_type(&dns_iter) ==
                            YAML_SEQUENCE_NODE);
                }
                else if (!strcmp(pgw_key, "dns6"))
                {
                    int count = 0;
                    yaml_iter_t dns6_iter;
                    yaml_iter_recurse(&pgw_iter, &dns6_iter);
                    d_assert(yaml_iter_type(&dns6_iter) !=
                        YAML_MAPPING_NODE, return CORE_ERROR,);

                    do
                    {
                        const char *v = NULL;

                        if (yaml_iter_type(&dns6_iter) ==
                                YAML_SEQUENCE_NODE)
                        {
                            if (!yaml_iter_next(&dns6_iter))
                                break;
                        }

                        v = yaml_iter_value(&dns6_iter);
                        if (v)
                        {
                            if (count == 0)
                            {
                            }
                            else if (count == 1)
                            {
                            }
                            else
                                d_warn("Ignored %d DNS(%s)", count, v);
                        }

                        count++;
                    } while(
                        yaml_iter_type(&dns6_iter) ==
                            YAML_SEQUENCE_NODE);
                }
                else
                    d_warn("unknown key `%s`", pgw_key);

            }
        }
    }

    rv = pgw_context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

status_t pgw_context_setup_trace_module()
{
    int diameter = context_self()->logger.trace.diameter;
    int gtp = context_self()->logger.trace.gtp;
    int others = context_self()->logger.trace.others;

    if (diameter)
    {
        if (diameter <= 1) fd_g_debug_lvl = FD_LOG_ERROR;
        else if (diameter <= 3) fd_g_debug_lvl = FD_LOG_NOTICE;
        else if (diameter <= 5) fd_g_debug_lvl = FD_LOG_DEBUG;
        else fd_g_debug_lvl = FD_LOG_ANNOYING;

        extern int _pgw_fd_path;
        d_trace_level(&_pgw_fd_path, diameter);
        extern int _fd_init;
        d_trace_level(&_fd_init, diameter);
        extern int _fd_logger;
        d_trace_level(&_fd_logger, diameter);
    }

    if (gtp)
    {
        extern int _pgw_sm;
        d_trace_level(&_pgw_sm, gtp);
        extern int _pgw_s5c_build;
        d_trace_level(&_pgw_s5c_build, gtp);
        extern int _pgw_s5c_handler;
        d_trace_level(&_pgw_s5c_handler, gtp);
        extern int _gtp_node;
        d_trace_level(&_gtp_node, gtp);
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

static void *sess_hash_keygen(c_uint8_t *out, int *out_len,
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn)
{
    memcpy(out, imsi, imsi_len);
    core_cpystrn((char*)(out+imsi_len), apn, MAX_APN_LEN+1);
    *out_len = imsi_len+strlen((char*)(out+imsi_len));

    return out;
}

pgw_sess_t *pgw_sess_add(
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn, c_uint8_t ebi)
{
    pgw_sess_t *sess = NULL;
    pgw_bearer_t *bearer = NULL;

    index_alloc(&pgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->pgw_s5c_teid = sess->index;  /* derived from an index */
    sess->pgw_s5c_ipv4 = pgw_self()->gtpc_addr;
    sess->pgw_s5c_ipv6 = pgw_self()->gtpc_addr6;

    /* Set IMSI */
    sess->imsi_len = imsi_len;
    memcpy(sess->imsi, imsi, sess->imsi_len);
    core_buffer_to_bcd(sess->imsi, sess->imsi_len, sess->imsi_bcd);

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
    status_t rv;
    pgw_sess_t *sess = NULL;
    gtp_node_t *sgw = NULL;

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
        gtp_f_teid_t *sgw_s5c_teid = NULL;

        sgw_s5c_teid = req->sender_f_teid_for_control_plane.data;
        d_assert(sgw_s5c_teid, return NULL,);
        sgw = gtp_find_by_ip(&pgw_self()->sgw_list, &sgw_s5c_teid->ip);
        if (!sgw)
        {
            sgw = gtp_add_node_by_f_teid(
                &pgw_self()->sgw_list, sgw_s5c_teid, pgw_self()->gtpc_port,
                context_self()->parameter.no_ipv4,
                context_self()->parameter.no_ipv6,
                context_self()->parameter.prefer_ipv4);
            d_assert(sgw, return NULL,);

            rv = gtp_client(sgw);
            d_assert(rv == CORE_OK, return NULL,);
        }

        sess = pgw_sess_add(req->imsi.data, req->imsi.len, apn,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);
        d_assert(sess, return NULL, "No Session Context");

        /* Setup GTP Node between PGW and SGW */
        CONNECT_SGW_GTP_NODE(sess, sgw);
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

        if (iph->ip_dst.s_addr == sess->pdn.paa.addr)
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

#define TRACE_MODULE _pgw_context
#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"
#include "core_lib.h"
#include "core_network.h"

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

pool_declare(pgw_pf_pool, pgw_pf_t, MAX_POOL_OF_PF);

typedef struct _ue_pool_t {
    int head, tail;
    int size, avail;
    mutex_id mut;
    pgw_ue_ip_t *free[MAX_POOL_OF_SESS], pool[MAX_POOL_OF_SESS];
} ue_pool_t;

#define INVALID_POOL_INDEX       MAX_NUM_OF_UE_POOL
static ue_pool_t ue_ip_pool[MAX_NUM_OF_UE_POOL];

static int context_initiaized = 0;

status_t pgw_context_init()
{
    int i;

    d_assert(context_initiaized == 0, return CORE_ERROR,
            "PGW context already has been initialized");

    memset(&self, 0, sizeof(pgw_context_t));

    list_init(&self.gtpc_list);
    list_init(&self.gtpc_list6);
    list_init(&self.gtpu_list);
    list_init(&self.gtpu_list6);

    gtp_node_init();
    list_init(&self.sgw_s5c_list);
    list_init(&self.sgw_s5u_list);

    index_init(&pgw_sess_pool, MAX_POOL_OF_SESS);
    index_init(&pgw_bearer_pool, MAX_POOL_OF_BEARER);

    pool_init(&pgw_pf_pool, MAX_POOL_OF_PF);

    for (i = 0; i < MAX_NUM_OF_UE_POOL; i++)
        pool_init(&ue_ip_pool[i], MAX_POOL_OF_UE);

    self.sess_hash = hash_make();

    context_initiaized = 1;

    return CORE_OK;
}

status_t pgw_context_final()
{
    int i;

    d_assert(context_initiaized == 1, return CORE_ERROR,
            "PGW context already has been finalized");

    pgw_sess_remove_all();

    d_assert(self.sess_hash, , "Null param");
    hash_destroy(self.sess_hash);

    if (index_used(&pgw_sess_pool))
        d_warn("%d not freed in pgw_sess_pool[%d] in PGW-Context",
                index_used(&pgw_sess_pool), index_size(&pgw_sess_pool));
    d_trace(3, "%d not freed in pgw_sess_pool[%d] in PGW-Context\n",
            index_used(&pgw_sess_pool), index_size(&pgw_sess_pool));

    pool_final(&pgw_pf_pool);

    for (i = 0; i < MAX_NUM_OF_UE_POOL; i++)
    {
        if (pool_used(&ue_ip_pool[i]))
        {
            d_warn("[%d] %d not freed in ue_ip_pool[%d] in PGW-Context",
                    i, pool_used(&ue_ip_pool[i]), index_size(&ue_ip_pool[i]));
        }
        d_trace(3, "[%d] %d not freed in ue_ip_pool[%d] in PGW-Context\n",
                i, pool_used(&ue_ip_pool[i]), index_size(&ue_ip_pool[i]));

        pool_final(&ue_ip_pool[i]);
    }

    index_final(&pgw_bearer_pool);
    index_final(&pgw_sess_pool);

    gtp_remove_all_nodes(&self.sgw_s5c_list);
    gtp_remove_all_nodes(&self.sgw_s5u_list);
    gtp_node_final();

    sock_remove_all_nodes(&self.gtpc_list);
    sock_remove_all_nodes(&self.gtpc_list6);
    sock_remove_all_nodes(&self.gtpu_list);
    sock_remove_all_nodes(&self.gtpu_list6);

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

    self.tun_ifname = "pgwtun";

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
    if (list_first(&self.gtpu_list) == NULL &&
        list_first(&self.gtpu_list6) == NULL)
    {
        d_error("No pgw.gtpu in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.dns[0] == NULL && self.dns6[0] == NULL)
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
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        c_uint16_t port = self.gtpc_port;
                        const char *dev = NULL;
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
                            else if (!strcmp(gtpc_key, "dev"))
                            {
                                dev = yaml_iter_value(&gtpc_iter);
                            }
                            else
                                d_warn("unknown key `%s`", gtpc_key);
                        }

                        list = NULL;
                        for (i = 0; i < num; i++)
                        {
                            rv = core_addaddrinfo(&list,
                                    family, hostname[i], port, 0);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }

                        if (list)
                        {
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
                        }

                        if (dev)
                        {
                            rv = sock_probe_node(
                                    context_self()->parameter.no_ipv4 ?
                                        NULL : &self.gtpc_list,
                                    context_self()->parameter.no_ipv6 ?
                                        NULL : &self.gtpc_list6,
                                    dev, self.gtpc_port);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }

                    } while(yaml_iter_type(&gtpc_array) == YAML_SEQUENCE_NODE);

                    if (list_first(&self.gtpc_list) == NULL &&
                        list_first(&self.gtpc_list6) == NULL)
                    {
                        rv = sock_probe_node(
                                context_self()->parameter.no_ipv4 ?
                                    NULL : &self.gtpc_list,
                                context_self()->parameter.no_ipv6 ?
                                    NULL : &self.gtpc_list6,
                                NULL, self.gtpc_port);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
                }
                else if (!strcmp(pgw_key, "gtpu"))
                {
                    yaml_iter_t gtpu_array, gtpu_iter;
                    yaml_iter_recurse(&pgw_iter, &gtpu_array);
                    do
                    {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        c_uint16_t port = self.gtpu_port;
                        const char *dev = NULL;
                        c_sockaddr_t *list = NULL;
                        sock_node_t *node = NULL;

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
                                yaml_iter_t hostname_iter;
                                yaml_iter_recurse(&gtpu_iter, &hostname_iter);
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
                            else if (!strcmp(gtpu_key, "port"))
                            {
                                const char *v = yaml_iter_value(&gtpu_iter);
                                if (v)
                                {
                                    port = atoi(v);
                                    self.gtpu_port = port;
                                }
                            }
                            else if (!strcmp(gtpu_key, "dev"))
                            {
                                dev = yaml_iter_value(&gtpu_iter);
                            }
                            else
                                d_warn("unknown key `%s`", gtpu_key);
                        }

                        list = NULL;
                        for (i = 0; i < num; i++)
                        {
                            rv = core_addaddrinfo(&list,
                                    family, hostname[i], port, 0);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }

                        if (list)
                        {
                            if (context_self()->parameter.no_ipv4 == 0)
                            {
                                rv = sock_add_node(&self.gtpu_list,
                                        &node, list, AF_INET);
                                d_assert(rv == CORE_OK, return CORE_ERROR,);
                            }

                            if (context_self()->parameter.no_ipv6 == 0)
                            {
                                rv = sock_add_node(&self.gtpu_list6,
                                        &node, list, AF_INET6);
                                d_assert(rv == CORE_OK, return CORE_ERROR,);
                            }

                            core_freeaddrinfo(list);
                        }

                        if (dev)
                        {
                            rv = sock_probe_node(
                                    context_self()->parameter.no_ipv4 ?
                                        NULL : &self.gtpu_list,
                                    context_self()->parameter.no_ipv6 ?
                                        NULL : &self.gtpu_list6,
                                    dev, self.gtpu_port);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }

                    } while(yaml_iter_type(&gtpu_array) == YAML_SEQUENCE_NODE);

                    if (list_first(&self.gtpu_list) == NULL &&
                        list_first(&self.gtpu_list6) == NULL)
                    {
                        rv = sock_probe_node(
                                context_self()->parameter.no_ipv4 ?
                                    NULL : &self.gtpu_list,
                                context_self()->parameter.no_ipv6 ?
                                    NULL : &self.gtpu_list6,
                                NULL, self.gtpu_port);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
                }
                else if (!strcmp(pgw_key, "ue_pool"))
                {
                    yaml_iter_t ue_pool_array, ue_pool_iter;
                    yaml_iter_recurse(&pgw_iter, &ue_pool_array);
                    do
                    {
                        const char *ipstr = NULL;
                        const char *mask_or_numbits = NULL;
                        const char *apn = NULL;

                        d_assert(self.num_of_ue_pool <=
                                MAX_NUM_OF_UE_POOL, return CORE_ERROR,);
                        if (yaml_iter_type(&ue_pool_array) ==
                                YAML_MAPPING_NODE)
                        {
                            memcpy(&ue_pool_iter, &ue_pool_array,
                                    sizeof(yaml_iter_t));
                        }
                        else if (yaml_iter_type(&ue_pool_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!yaml_iter_next(&ue_pool_array))
                                break;
                            yaml_iter_recurse(&ue_pool_array,
                                    &ue_pool_iter);
                        }
                        else if (yaml_iter_type(&ue_pool_array) ==
                                YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            d_assert(0, return CORE_ERROR,);

                        while(yaml_iter_next(&ue_pool_iter))
                        {
                            const char *ue_pool_key =
                                yaml_iter_key(&ue_pool_iter);
                            d_assert(ue_pool_key,
                                    return CORE_ERROR,);
                            if (!strcmp(ue_pool_key, "addr"))
                            {
                                char *v =
                                    (char *)yaml_iter_value(&ue_pool_iter);
                                if (v)
                                {
                                    ipstr = (const char *)strsep(&v, "/");
                                    if (ipstr)
                                    {
                                        mask_or_numbits = (const char *)v;
                                    }
                                }
                            }
                            else if (!strcmp(ue_pool_key, "apn"))
                            {
                                apn = yaml_iter_value(&ue_pool_iter);
                            }
                            else
                                d_warn("unknown key `%s`", ue_pool_key);
                        }

                        if (ipstr && mask_or_numbits)
                        {
                            self.ue_pool[self.num_of_ue_pool].ipstr = ipstr;
                            self.ue_pool[self.num_of_ue_pool].mask_or_numbits =
                                mask_or_numbits;
                            self.ue_pool[self.num_of_ue_pool].apn = apn;
                            self.num_of_ue_pool++;
                        }
                        else
                        {
                            d_warn("Ignore : addr(%s/%s), apn(%s)",
                                    ipstr, mask_or_numbits, apn);
                        }
                    } while(yaml_iter_type(&ue_pool_array) ==
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
                            ipsubnet_t ipsub;
                            rv = core_ipsubnet(&ipsub, v, NULL);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);

                            if (ipsub.family == AF_INET)
                            {
                                if (self.dns[0] && self.dns[1])
                                    d_warn("Ignore DNS : %s", v);
                                else if (self.dns[0]) self.dns[1] = v;
                                else self.dns[0] = v;
                            }
                            else if (ipsub.family == AF_INET6)
                            {
                                if (self.dns6[0] && self.dns6[1])
                                    d_warn("Ignore DNS : %s", v);
                                else if (self.dns6[0]) self.dns6[1] = v;
                                else self.dns6[0] = v;
                            }
                            else
                                d_warn("Ignore DNS : %s", v);
                        }

                        count++;
                    } while(
                        yaml_iter_type(&dns_iter) ==
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
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn, 
        c_uint8_t pdn_type, c_uint8_t ebi)
{
    pgw_sess_t *sess = NULL;
    pgw_bearer_t *bearer = NULL;

    index_alloc(&pgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->pgw_s5c_teid = sess->index;  /* derived from an index */

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

    sess->pdn.paa.pdn_type = pdn_type;
    if (pdn_type == GTP_PDN_TYPE_IPV4)
    {
        sess->ipv4 = pgw_ue_ip_alloc(AF_INET, apn);
        d_assert(sess->ipv4, pgw_sess_remove(sess); return NULL, 
                "Can't allocate IPv4 Pool");
        sess->pdn.paa.addr = sess->ipv4->addr[0];
    }
    else if (pdn_type == GTP_PDN_TYPE_IPV6)
    {
        sess->ipv6 = pgw_ue_ip_alloc(AF_INET6, apn);
        d_assert(sess->ipv6, pgw_sess_remove(sess); return NULL, 
                "Can't allocate IPv6 Pool");

        sess->pdn.paa.len = pgw_ue_ip_prefixlen(sess->ipv6);
        memcpy(sess->pdn.paa.addr6, sess->ipv6->addr, IPV6_LEN);
    }
    else if (pdn_type == GTP_PDN_TYPE_IPV4V6)
    {
        sess->ipv4 = pgw_ue_ip_alloc(AF_INET, apn);
        d_assert(sess->ipv4, pgw_sess_remove(sess); return NULL, 
                "Can't allocate IPv4 Pool");
        sess->ipv6 = pgw_ue_ip_alloc(AF_INET6, apn);
        d_assert(sess->ipv6, pgw_sess_remove(sess); return NULL, 
                "Can't allocate IPv6 Pool");

        sess->pdn.paa.both.addr = sess->ipv4->addr[0];
        sess->pdn.paa.both.len = pgw_ue_ip_prefixlen(sess->ipv6);
        memcpy(sess->pdn.paa.both.addr6, sess->ipv6->addr, IPV6_LEN);
    }
    else
        d_assert(0, return NULL, "Unsupported PDN Type(%d)", pdn_type);

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

    if (sess->ipv4)
        pgw_ue_ip_free(sess->ipv4);
    if (sess->ipv6)
        pgw_ue_ip_free(sess->ipv6);

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

gtp_node_t *pgw_sgw_add_by_message(gtp_message_t *message)
{
    gtp_node_t *sgw = NULL;
    gtp_f_teid_t *sgw_s5c_teid = NULL;

    gtp_create_session_request_t *req = &message->create_session_request;

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No Sender F-TEID");
        return NULL;
    }

    sgw_s5c_teid = req->sender_f_teid_for_control_plane.data;
    d_assert(sgw_s5c_teid, return NULL,);
    sgw = gtp_find_node(&pgw_self()->sgw_s5c_list, sgw_s5c_teid);
    if (!sgw)
    {
        sgw = gtp_connect_to_node(&pgw_self()->sgw_s5c_list, sgw_s5c_teid,
            pgw_self()->gtpc_port,
            context_self()->parameter.no_ipv4,
            context_self()->parameter.no_ipv6,
            context_self()->parameter.prefer_ipv4);
        d_assert(sgw, return NULL,);
    }

    return sgw;
}
pgw_sess_t *pgw_sess_add_by_message(gtp_message_t *message)
{
    pgw_sess_t *sess = NULL;
    c_int8_t apn[MAX_APN_LEN];

    gtp_create_session_request_t *req = &message->create_session_request;

    if (req->imsi.presence == 0)
    {
        d_error("No IMSI");
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
    if (req->pdn_type.presence == 0)
    {
        d_error("No PDN Type");
        return NULL;
    }

    apn_parse(apn, req->access_point_name.data, req->access_point_name.len);
    sess = pgw_sess_find_by_imsi_apn(req->imsi.data, req->imsi.len, apn);
    if (!sess)
    {
        sess = pgw_sess_add(req->imsi.data, req->imsi.len, apn,
            req->pdn_type.u8,
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
    struct ip *ip_h =  NULL;
    struct ip6_hdr *ip6_h =  NULL;
    c_uint32_t *src_addr = NULL;
    c_uint32_t *dst_addr = NULL;
    int addr_len = 0;
    c_uint8_t proto = 0;
    int ip_hlen = 0;
    char buf[CORE_ADDRSTRLEN];

    d_assert(pkt, return NULL, "pkt is NULL");

    ip_h = (struct ip *)pkt->payload;
    if (ip_h->ip_v == 4)
    {
        ip_h = (struct ip *)pkt->payload;
        ip6_h = NULL;

        proto = ip_h->ip_p;
        ip_hlen = (ip_h->ip_hl)*4;

        src_addr = &ip_h->ip_src.s_addr;
        dst_addr = &ip_h->ip_dst.s_addr;
        addr_len = 4;
    }
    else if (ip_h->ip_v == 6)
    {
        ip_h = NULL;
        ip6_h = (struct ip6_hdr *)pkt->payload;

        proto = ip6_h->ip6_nxt;
        ip_hlen = 40;  /* TODO */

        src_addr = (c_uint32_t *)ip6_h->ip6_src.s6_addr;
        dst_addr = (c_uint32_t *)ip6_h->ip6_dst.s6_addr;
        addr_len = 16;

    }
    else
        d_error("Invalid IP version = %d\n", ip_h->ip_v);

    d_trace(50, "PROTO:%d SRC:%08x %08x %08x %08x\n",
            proto, ntohl(src_addr[0]), ntohl(src_addr[1]),
            ntohl(src_addr[2]), ntohl(src_addr[3]));
    d_trace(50, "HLEN:%d  DST:%08x %08x %08x %08x\n",
            ip_hlen, ntohl(dst_addr[0]), ntohl(dst_addr[1]),
            ntohl(dst_addr[2]), ntohl(dst_addr[3]));


    /* TODO: Need to use the method of FAST matching algorithm and 
     *          implementation .
     *       Until be ready, linear searching will be use to find the bearer.
     */

    for (hi = pgw_sess_first(); hi; hi = pgw_sess_next(hi))
    {
        sess = pgw_sess_this(hi);

        if (sess->ipv4)
            d_trace(50, "PAA IPv4:%s\n", INET_NTOP(&sess->ipv4->addr, buf));
        if (sess->ipv6)
            d_trace(50, "PAA IPv6:%s\n", INET6_NTOP(&sess->ipv6->addr, buf));

        if ((sess->ipv4 && memcmp(dst_addr, sess->ipv4->addr, addr_len) == 0) ||
            (sess->ipv6 && memcmp(dst_addr, sess->ipv6->addr, addr_len) == 0))
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

                if (bearer->ebi == 0)
                {
                    /* Create Bearer Response is not received */
                    continue;
                }

                for (pf = pgw_pf_first(bearer); pf; pf = pgw_pf_next(pf))
                {
                    int k;
                    c_uint32_t src_mask[4];
                    c_uint32_t dst_mask[4];

                    d_trace(50, "DIR:%d PROTO:%d SRC:%d-%d DST:%d-%d\n",
                            pf->direction, pf->rule.proto,
                            pf->rule.port.local.low,
                            pf->rule.port.local.high,
                            pf->rule.port.remote.low,
                            pf->rule.port.remote.high);
                    d_trace(50, "SRC:%08x %08x %08x %08x/%08x %08x %08x %08x\n",
                            ntohl(pf->rule.ip.local.addr[0]),
                            ntohl(pf->rule.ip.local.addr[1]),
                            ntohl(pf->rule.ip.local.addr[2]),
                            ntohl(pf->rule.ip.local.addr[3]),
                            ntohl(pf->rule.ip.local.mask[0]),
                            ntohl(pf->rule.ip.local.mask[1]),
                            ntohl(pf->rule.ip.local.mask[2]),
                            ntohl(pf->rule.ip.local.mask[3]));
                    d_trace(50, "DST:%08x %08x %08x %08x/%08x %08x %08x %08x\n",
                            ntohl(pf->rule.ip.remote.addr[0]),
                            ntohl(pf->rule.ip.remote.addr[1]),
                            ntohl(pf->rule.ip.remote.addr[2]),
                            ntohl(pf->rule.ip.remote.addr[3]),
                            ntohl(pf->rule.ip.remote.mask[0]),
                            ntohl(pf->rule.ip.remote.mask[1]),
                            ntohl(pf->rule.ip.remote.mask[2]),
                            ntohl(pf->rule.ip.remote.mask[3]));

                    if (pf->direction != 1)
                    {
                        continue;
                    }

                    for (k = 0; k < 3; k++)
                    {
                        src_mask[k] = src_addr[k] & pf->rule.ip.local.mask[k];
                        dst_mask[k] = dst_addr[k] & pf->rule.ip.remote.mask[k];
                    }

                    if (memcmp(src_mask, pf->rule.ip.local.addr,
                                addr_len) == 0 &&
                        memcmp(dst_mask, pf->rule.ip.remote.addr,
                                addr_len) == 0)
                    {
                        /* Protocol match */
                        if (pf->rule.proto == 0) /* IP */
                        {
                            /* No need to match port */
                            break;
                        }

                        if (pf->rule.proto == proto)
                        {
                            if (pf->rule.proto == IPPROTO_TCP)
                            {
                                struct tcphdr *tcph = 
                                    (struct tcphdr *)
                                    ((char *)pkt->payload + ip_hlen);

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
                                    ((char *)pkt->payload + ip_hlen);

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
                    d_trace(50,"FOUND Bearer EBI = %d\n", bearer->ebi);
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

status_t pgw_ue_pool_generate()
{
    status_t rv;
    int i, j;

    for (i = 0; i < self.num_of_ue_pool; i++)
    {
        int index = 0;
        ipsubnet_t ipaddr, ipsub;
        c_uint32_t prefixlen;
        c_uint32_t mask_count;
        c_uint32_t broadcast[4];

        rv = core_ipsubnet(&ipaddr, self.ue_pool[i].ipstr, NULL);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        rv = core_ipsubnet(&ipsub,
                self.ue_pool[i].ipstr, self.ue_pool[i].mask_or_numbits);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        d_assert(self.ue_pool[i].mask_or_numbits, return CORE_ERROR,);
        prefixlen = atoi(self.ue_pool[i].mask_or_numbits);
        if (ipsub.family == AF_INET)
        {
            if (prefixlen == 32)
                mask_count = 1;
            else if (prefixlen < 32)
                mask_count = (0xffffffff >> prefixlen) + 1;
            else
                d_assert(0, return CORE_ERROR,);
        }
        else if (ipsub.family == AF_INET6)
        {
            if (prefixlen == 128)
                mask_count = 1;
            else if (prefixlen > 96 && prefixlen < 128)
                mask_count = (0xffffffff >> (prefixlen - 96)) + 1;
            else if (prefixlen <= 96)
                mask_count = 0xffffffff;
            else
                d_assert(0, return CORE_ERROR,);
        }
        else
            d_assert(0, return CORE_ERROR,);

        self.ue_pool[i].family = ipsub.family;
        
        for (j = 0; j < 4; j++)
        {
            broadcast[j] = ipsub.sub[j] + ~ipsub.mask[j];
        }

        for (j = 0; j < mask_count && index < MAX_POOL_OF_SESS; j++)
        {
            pgw_ue_ip_t *ue_ip = NULL;
            int maxbytes = 0;
            int lastindex = 0;

            ue_ip = &ue_ip_pool[i].pool[index];
            d_assert(ue_ip, return CORE_ERROR,);
            memset(ue_ip, 0, sizeof *ue_ip);

            if (ipsub.family == AF_INET)
            {
                maxbytes = 4;
                lastindex = 0;
            }
            else if (ipsub.family == AF_INET6)
            {
                maxbytes = 16;
                lastindex = 3;
            }

            memcpy(ue_ip->addr, ipsub.sub, maxbytes);
            ue_ip->addr[lastindex] += htonl(j);

            /* Exclude Network Address */
            if (memcmp(ue_ip->addr, ipsub.sub, maxbytes) == 0) continue;

            /* Exclude Broadcast Address */
            if (memcmp(ue_ip->addr, broadcast, maxbytes) == 0) continue;

            /* Exclude TUN IP Address */
            if (memcmp(ue_ip->addr, ipaddr.sub, maxbytes) == 0) continue;

            index++;
        }
        ue_ip_pool[i].size = ue_ip_pool[i].avail = index;
    }

    return CORE_OK;
}

static c_uint8_t find_ue_pool_index(int family, const char *apn)
{
    int i;
    c_uint8_t pool_index = INVALID_POOL_INDEX;

    d_assert(apn, return INVALID_POOL_INDEX,);
    d_assert(family == AF_INET || family == AF_INET6,
            return INVALID_POOL_INDEX,);

    for (i = 0; i < self.num_of_ue_pool; i++)
    {
        if (self.ue_pool[i].apn)
        {
            if (self.ue_pool[i].family == family &&
                strcmp(self.ue_pool[i].apn, apn) == 0 &&
                pool_avail(&ue_ip_pool[i]))
            {
                pool_index = i;
                break;
            }
        }
    }

    if (pool_index == INVALID_POOL_INDEX)
    {
        for (i = 0; i < self.num_of_ue_pool; i++)
        {
            if (self.ue_pool[i].apn == NULL)
            {
                if (self.ue_pool[i].family == family &&
                    pool_avail(&ue_ip_pool[i]))
                {
                    pool_index = i;
                    break;
                }
            }
        }
    }

    if (pool_index == INVALID_POOL_INDEX)
    {
        d_error("CHECK CONFIGURATION: Cannot find UE Pool");
        return INVALID_POOL_INDEX;
    }

    return pool_index;
}

pgw_ue_ip_t *pgw_ue_ip_alloc(int family, const char *apn)
{
    c_uint8_t pool_index = INVALID_POOL_INDEX;
    pgw_ue_ip_t *ue_ip = NULL;

    d_assert(apn, return NULL,);

    pool_index = find_ue_pool_index(family, apn);
    d_assert(pool_index < MAX_NUM_OF_UE_POOL, return NULL,);

    pool_alloc_node(&ue_ip_pool[pool_index], &ue_ip);
    d_assert(ue_ip, return NULL,);
    ue_ip->index = pool_index;

    return ue_ip;
}

status_t pgw_ue_ip_free(pgw_ue_ip_t *ue_ip)
{
    c_uint8_t pool_index;

    d_assert(ue_ip, return CORE_ERROR,);
    pool_index = ue_ip->index;

    d_assert(pool_index < MAX_NUM_OF_UE_POOL, return CORE_ERROR,);
    pool_free_node(&ue_ip_pool[pool_index], ue_ip);

    return CORE_OK;
}

c_uint8_t pgw_ue_ip_prefixlen(pgw_ue_ip_t *ue_ip)
{
    d_assert(ue_ip, return CORE_ERROR,);

    c_uint8_t index = ue_ip->index;
    d_assert(index < MAX_NUM_OF_UE_POOL, return CORE_ERROR,);
    d_assert(pgw_self()->ue_pool[index].mask_or_numbits,
            return CORE_ERROR,);
    return atoi(pgw_self()->ue_pool[index].mask_or_numbits);
}

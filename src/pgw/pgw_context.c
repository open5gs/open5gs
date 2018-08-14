#define TRACE_MODULE _pgw_context
#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"
#include "core_lib.h"
#include "core_network.h"

#include <mongoc.h>
#include <yaml.h>
#include "common/yaml_helper.h"

#include "gtp/gtp_types.h"
#include "gtp/gtp_conv.h"
#include "gtp/gtp_node.h"
#include "gtp/gtp_path.h"
#include "gtp/gtp_xact.h"

#include "fd/fd_lib.h"

#include "common/context.h"
#include "pgw_context.h"

static pgw_context_t self;
static fd_config_t g_fd_conf;

pool_declare(pgw_dev_pool, pgw_dev_t, MAX_NUM_OF_DEV);
pool_declare(pgw_subnet_pool, pgw_subnet_t, MAX_NUM_OF_SUBNET);

index_declare(pgw_sess_pool, pgw_sess_t, MAX_POOL_OF_SESS);
index_declare(pgw_bearer_pool, pgw_bearer_t, MAX_POOL_OF_BEARER);

pool_declare(pgw_pf_pool, pgw_pf_t, MAX_POOL_OF_PF);

static int context_initiaized = 0;

status_t pgw_context_init()
{
    d_assert(context_initiaized == 0, return CORE_ERROR,
            "PGW context already has been initialized");

    /* Initial FreeDiameter Config */
    memset(&g_fd_conf, 0, sizeof(fd_config_t));

    /* Initialize PGW context */
    memset(&self, 0, sizeof(pgw_context_t));
    self.fd_config = &g_fd_conf;

    list_init(&self.gtpc_list);
    list_init(&self.gtpc_list6);
    list_init(&self.gtpu_list);
    list_init(&self.gtpu_list6);

    gtp_node_init();
    list_init(&self.sgw_s5c_list);
    list_init(&self.sgw_s5u_list);

    list_init(&self.dev_list);
    pool_init(&pgw_dev_pool, MAX_NUM_OF_DEV);
    list_init(&self.subnet_list);
    pool_init(&pgw_subnet_pool, MAX_NUM_OF_SUBNET);

    index_init(&pgw_sess_pool, MAX_POOL_OF_SESS);
    index_init(&pgw_bearer_pool, MAX_POOL_OF_BEARER);

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

    if (index_used(&pgw_sess_pool))
        d_error("%d not freed in pgw_sess_pool[%d] in PGW-Context",
                index_used(&pgw_sess_pool), index_size(&pgw_sess_pool));
    d_trace(9, "%d not freed in pgw_sess_pool[%d] in PGW-Context\n",
            index_used(&pgw_sess_pool), index_size(&pgw_sess_pool));

    pgw_dev_remove_all();
    pgw_subnet_remove_all();

    index_final(&pgw_bearer_pool);
    index_final(&pgw_sess_pool);
    pool_final(&pgw_pf_pool);

    pool_final(&pgw_dev_pool);
    pool_final(&pgw_subnet_pool);

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
    self.fd_config->cnf_port = DIAMETER_PORT;
    self.fd_config->cnf_port_tls = DIAMETER_SECURE_PORT;

    self.tun_ifname = "pgwtun";

    return CORE_OK;
}

static status_t pgw_context_validation()
{
    if (self.fd_conf_path == NULL &&
        (self.fd_config->cnf_diamid == NULL ||
        self.fd_config->cnf_diamrlm == NULL ||
        self.fd_config->cnf_addr == NULL))
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
                    yaml_node_t *node = 
                        yaml_document_get_node(document, pgw_iter.pair->value);
                    d_assert(node, return CORE_ERROR,);
                    if (node->type == YAML_SCALAR_NODE)
                    {
                        self.fd_conf_path = yaml_iter_value(&pgw_iter);
                    }
                    else if (node->type == YAML_MAPPING_NODE)
                    {
                        yaml_iter_t fd_iter;
                        yaml_iter_recurse(&pgw_iter, &fd_iter);

                        while(yaml_iter_next(&fd_iter))
                        {
                            const char *fd_key = yaml_iter_key(&fd_iter);
                            d_assert(fd_key, return CORE_ERROR,);
                            if (!strcmp(fd_key, "identity"))
                            {
                                self.fd_config->cnf_diamid = 
                                    yaml_iter_value(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "realm"))
                            {
                                self.fd_config->cnf_diamrlm = 
                                    yaml_iter_value(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "port"))
                            {
                                const char *v = yaml_iter_value(&fd_iter);
                                if (v) self.fd_config->cnf_port = atoi(v);
                            }
                            else if (!strcmp(fd_key, "sec_port"))
                            {
                                const char *v = yaml_iter_value(&fd_iter);
                                if (v) self.fd_config->cnf_port_tls = atoi(v);
                            }
                            else if (!strcmp(fd_key, "no_sctp"))
                            {
                                self.fd_config->cnf_flags.no_sctp =
                                    yaml_iter_bool(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "listen_on"))
                            {
                                self.fd_config->cnf_addr = 
                                    yaml_iter_value(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "load_extension"))
                            {
                                yaml_iter_t ext_array, ext_iter;
                                yaml_iter_recurse(&fd_iter, &ext_array);
                                do
                                {
                                    const char *module = NULL;
                                    const char *conf = NULL;

                                    if (yaml_iter_type(&ext_array) ==
                                        YAML_MAPPING_NODE)
                                    {
                                        memcpy(&ext_iter, &ext_array,
                                                sizeof(yaml_iter_t));
                                    }
                                    else if (yaml_iter_type(&ext_array) ==
                                        YAML_SEQUENCE_NODE)
                                    {
                                        if (!yaml_iter_next(&ext_array))
                                            break;
                                        yaml_iter_recurse(
                                                &ext_array, &ext_iter);
                                    }
                                    else if (yaml_iter_type(&ext_array) ==
                                        YAML_SCALAR_NODE)
                                    {
                                        break;
                                    }
                                    else
                                        d_assert(0, return CORE_ERROR,);

                                    while(yaml_iter_next(&ext_iter))
                                    {
                                        const char *ext_key =
                                            yaml_iter_key(&ext_iter);
                                        d_assert(ext_key,
                                                return CORE_ERROR,);
                                        if (!strcmp(ext_key, "module"))
                                        {
                                            module = yaml_iter_value(&ext_iter);
                                        }
                                        else if (!strcmp(ext_key, "conf"))
                                        {
                                            conf = yaml_iter_value(&ext_iter);
                                        }
                                        else
                                            d_warn("unknown key `%s`", ext_key);
                                    }

                                    if (module)
                                    {
                                        self.fd_config->
                                            ext[self.fd_config->num_of_ext].
                                                module = module;
                                        self.fd_config->
                                            ext[self.fd_config->num_of_ext].
                                                conf = conf;
                                        self.fd_config->num_of_ext++;
                                    }
                                } while(yaml_iter_type(&ext_array) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(fd_key, "connect"))
                            {
                                yaml_iter_t conn_array, conn_iter;
                                yaml_iter_recurse(&fd_iter, &conn_array);
                                do
                                {
                                    const char *identity = NULL;
                                    const char *addr = NULL;
                                    c_uint16_t port = 0;

                                    if (yaml_iter_type(&conn_array) ==
                                        YAML_MAPPING_NODE)
                                    {
                                        memcpy(&conn_iter, &conn_array,
                                                sizeof(yaml_iter_t));
                                    }
                                    else if (yaml_iter_type(&conn_array) ==
                                        YAML_SEQUENCE_NODE)
                                    {
                                        if (!yaml_iter_next(&conn_array))
                                            break;
                                        yaml_iter_recurse(&conn_array, &conn_iter);
                                    }
                                    else if (yaml_iter_type(&conn_array) ==
                                        YAML_SCALAR_NODE)
                                    {
                                        break;
                                    }
                                    else
                                        d_assert(0, return CORE_ERROR,);

                                    while(yaml_iter_next(&conn_iter))
                                    {
                                        const char *conn_key =
                                            yaml_iter_key(&conn_iter);
                                        d_assert(conn_key,
                                                return CORE_ERROR,);
                                        if (!strcmp(conn_key, "identity"))
                                        {
                                            identity = yaml_iter_value(&conn_iter);
                                        }
                                        else if (!strcmp(conn_key, "addr"))
                                        {
                                            addr = yaml_iter_value(&conn_iter);
                                        }
                                        else if (!strcmp(conn_key, "port"))
                                        {
                                            const char *v =
                                                yaml_iter_value(&conn_iter);
                                            if (v) port = atoi(v);
                                        }
                                        else
                                            d_warn("unknown key `%s`", conn_key);
                                    }

                                    if (identity && addr)
                                    {
                                        self.fd_config->
                                            conn[self.fd_config->num_of_conn].
                                                identity = identity;
                                        self.fd_config->
                                            conn[self.fd_config->num_of_conn].
                                                addr = addr;
                                        self.fd_config->
                                            conn[self.fd_config->num_of_conn].
                                                port = port;
                                        self.fd_config->num_of_conn++;
                                    }
                                } while(yaml_iter_type(&conn_array) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else
                                d_warn("unknown key `%s`", fd_key);
                        }
                    }
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
                        pgw_subnet_t *subnet = NULL;
                        const char *ipstr = NULL;
                        const char *mask_or_numbits = NULL;
                        const char *apn = NULL;
                        const char *dev = self.tun_ifname;

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
                            else if (!strcmp(ue_pool_key, "dev"))
                            {
                                dev = yaml_iter_value(&ue_pool_iter);
                            }
                            else
                                d_warn("unknown key `%s`", ue_pool_key);
                        }

                        if (ipstr && mask_or_numbits)
                        {
                            subnet = pgw_subnet_add(
                                    ipstr, mask_or_numbits, apn, dev);
                            d_assert(subnet, return CORE_ERROR,);
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

                    } while(
                        yaml_iter_type(&dns_iter) ==
                            YAML_SEQUENCE_NODE);
                }
                else if (!strcmp(pgw_key, "p-cscf"))
                {
                    yaml_iter_t dns_iter;
                    yaml_iter_recurse(&pgw_iter, &dns_iter);
                    d_assert(yaml_iter_type(&dns_iter) !=
                        YAML_MAPPING_NODE, return CORE_ERROR,);

                    self.num_of_p_cscf = 0;
                    self.num_of_p_cscf6 = 0;
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
                                if (self.num_of_p_cscf >= MAX_NUM_OF_P_CSCF)
                                    d_warn("Ignore P-CSCF : %s", v);
                                else self.p_cscf[self.num_of_p_cscf++] = v;
                            }
                            else if (ipsub.family == AF_INET6)
                            {
                                if (self.num_of_p_cscf6 >= MAX_NUM_OF_P_CSCF)
                                    d_warn("Ignore P-CSCF : %s", v);
                                else self.p_cscf6[self.num_of_p_cscf6++] = v;
                            }
                            else
                                d_warn("Ignore P-CSCF : %s", v);
                        }

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
    int app = context_self()->logger.trace.app;
    int diameter = context_self()->logger.trace.diameter;
    int gtpv2 = context_self()->logger.trace.gtpv2;
    int gtp = context_self()->logger.trace.gtp;

    if (app)
    {
        extern int _pgw_context;
        d_trace_level(&_pgw_context, app);
    }

    if (diameter)
    {
        extern int _pgw_fd_path;
        d_trace_level(&_pgw_fd_path, diameter);
        extern int _fd_init;
        d_trace_level(&_fd_init, diameter);
        extern int _fd_logger;
        d_trace_level(&_fd_logger, diameter);
    }

    if (gtpv2)
    {
        extern int _pgw_sm;
        d_trace_level(&_pgw_sm, gtpv2);
        extern int _pgw_s5c_build;
        d_trace_level(&_pgw_s5c_build, gtpv2);
        extern int _pgw_s5c_handler;
        d_trace_level(&_pgw_s5c_handler, gtpv2);

        extern int _gtp_node;
        d_trace_level(&_gtp_node, gtpv2);
        extern int _gtp_message;
        d_trace_level(&_gtp_message, gtpv2);
        extern int _gtp_path;
        d_trace_level(&_gtp_path, gtpv2);
        extern int _gtp_xact;
        d_trace_level(&_gtp_xact, gtpv2);

        extern int _tlv_msg;
        d_trace_level(&_tlv_msg, gtpv2);
    }

    if (gtp)
    {
        extern int _pgw_gtp_path;
        d_trace_level(&_pgw_gtp_path, gtp);
        extern int _pgw_ipfw;
        d_trace_level(&_pgw_ipfw, gtp);
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
    char buf1[CORE_ADDRSTRLEN];
    char buf2[CORE_ADDRSTRLEN];
    pgw_sess_t *sess = NULL;
    pgw_bearer_t *bearer = NULL;
    pgw_subnet_t *subnet6 = NULL;

    index_alloc(&pgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->gnode = NULL;

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

        subnet6 = sess->ipv6->subnet;
        d_assert(subnet6, pgw_sess_remove(sess); return NULL, 
                "No IPv6 subnet");

        sess->pdn.paa.len = subnet6->prefixlen;
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

        subnet6 = sess->ipv6->subnet;
        d_assert(subnet6, pgw_sess_remove(sess); return NULL, 
                "No IPv6 subnet");

        sess->pdn.paa.both.addr = sess->ipv4->addr[0];
        sess->pdn.paa.both.len = subnet6->prefixlen;
        memcpy(sess->pdn.paa.both.addr6, sess->ipv6->addr, IPV6_LEN);
    }
    else
        d_assert(0, return NULL, "Unsupported PDN Type(%d)", pdn_type);

    d_trace(1, "UE IPv4:[%s] IPv6:[%s]\n",
            sess->ipv4 ?  INET_NTOP(&sess->ipv4->addr, buf1) : "",
            sess->ipv6 ?  INET6_NTOP(&sess->ipv6->addr, buf2) : "");

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
    status_t rv;
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
        sgw = gtp_add_node_with_teid(&pgw_self()->sgw_s5c_list, sgw_s5c_teid,
            pgw_self()->gtpc_port,
            context_self()->parameter.no_ipv4,
            context_self()->parameter.no_ipv6,
            context_self()->parameter.prefer_ipv4);
        d_assert(sgw, return NULL,);

        rv = gtp_client(sgw);
        d_assert(rv == CORE_OK, return NULL,);
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

    d_trace(9, "pgw_sess_add_by_message() [APN:%s, PDN:%d, EDI:%d]\n",
            apn, req->pdn_type.u8,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);

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

    bearer->name = NULL;

    list_init(&bearer->pf_list);

    bearer->pgw_s5u_teid = bearer->index;
    
    bearer->sess = sess;
    bearer->gnode = NULL;

    list_append(&sess->bearer_list, bearer);

    return bearer;
}

status_t pgw_bearer_remove(pgw_bearer_t *bearer)
{
    d_assert(bearer, return CORE_ERROR, "Null param");
    d_assert(bearer->sess, return CORE_ERROR, "Null param");

    list_remove(&bearer->sess->bearer_list, bearer);

    if (bearer->name)
        CORE_FREE(bearer->name);

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

pgw_bearer_t* pgw_bearer_find_by_name(pgw_sess_t *sess, c_int8_t *name)
{
    pgw_bearer_t *bearer = NULL;
    
    d_assert(sess, return NULL, "Null param");
    d_assert(name, return NULL, "Null param");

    bearer = pgw_bearer_first(sess);
    while (bearer)
    {
        if (bearer->name && strcmp(bearer->name, name) == 0)
            return bearer;

        bearer = pgw_bearer_next(bearer);
    }

    return NULL;
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
    int j;
    pgw_subnet_t *subnet = NULL;

    for (subnet = pgw_subnet_first(); subnet; subnet = pgw_subnet_next(subnet))
    {
        int index = 0;
        c_uint32_t mask_count;
        c_uint32_t broadcast[4];

        if (subnet->family == AF_INET)
        {
            if (subnet->prefixlen == 32)
                mask_count = 1;
            else if (subnet->prefixlen < 32)
                mask_count = (0xffffffff >> subnet->prefixlen) + 1;
            else
                d_assert(0, return CORE_ERROR,);
        }
        else if (subnet->family == AF_INET6)
        {
            if (subnet->prefixlen == 128)
                mask_count = 1;
            else if (subnet->prefixlen > 96 && subnet->prefixlen < 128)
                mask_count = (0xffffffff >> (subnet->prefixlen - 96)) + 1;
            else if (subnet->prefixlen <= 96)
                mask_count = 0xffffffff;
            else
                d_assert(0, return CORE_ERROR,);
        }
        else
            d_assert(0, return CORE_ERROR,);
        
        for (j = 0; j < 4; j++)
        {
            broadcast[j] = subnet->sub.sub[j] + ~subnet->sub.mask[j];
        }

        for (j = 0; j < mask_count && index < MAX_POOL_OF_SESS; j++)
        {
            pgw_ue_ip_t *ue_ip = NULL;
            int maxbytes = 0;
            int lastindex = 0;

            ue_ip = &subnet->pool.pool[index];
            d_assert(ue_ip, return CORE_ERROR,);
            memset(ue_ip, 0, sizeof *ue_ip);

            if (subnet->family == AF_INET)
            {
                maxbytes = 4;
                lastindex = 0;
            }
            else if (subnet->family == AF_INET6)
            {
                maxbytes = 16;
                lastindex = 3;
            }

            memcpy(ue_ip->addr, subnet->sub.sub, maxbytes);
            ue_ip->addr[lastindex] += htonl(j);
            ue_ip->subnet = subnet;

            /* Exclude Network Address */
            if (memcmp(ue_ip->addr, subnet->sub.sub, maxbytes) == 0) continue;

            /* Exclude Broadcast Address */
            if (memcmp(ue_ip->addr, broadcast, maxbytes) == 0) continue;

            /* Exclude TUN IP Address */
            if (memcmp(ue_ip->addr, subnet->gw.sub, maxbytes) == 0) continue;

            index++;
        }
        subnet->pool.size = subnet->pool.avail = index;
    }

    return CORE_OK;
}

static pgw_subnet_t *find_subnet(int family, const char *apn)
{
    pgw_subnet_t *subnet = NULL;

    d_assert(apn, return NULL,);
    d_assert(family == AF_INET || family == AF_INET6, return NULL,);

    for (subnet = pgw_subnet_first(); subnet; subnet = pgw_subnet_next(subnet))
    {
        if (strlen(subnet->apn))
        {
            if (subnet->family == family && strcmp(subnet->apn, apn) == 0 &&
                pool_avail(&subnet->pool))
            {
                return subnet;
            }
        }
    }

    for (subnet = pgw_subnet_first(); subnet; subnet = pgw_subnet_next(subnet))
    {
        if (strlen(subnet->apn) == 0)
        {
            if (subnet->family == family &&
                pool_avail(&subnet->pool))
            {
                return subnet;
            }
        }
    }

    if (subnet == NULL)
        d_error("CHECK CONFIGURATION: Cannot find UE Pool");

    return subnet;
}

pgw_ue_ip_t *pgw_ue_ip_alloc(int family, const char *apn)
{
    pgw_subnet_t *subnet = NULL;
    pgw_ue_ip_t *ue_ip = NULL;

    d_assert(apn, return NULL,);

    subnet = find_subnet(family, apn);
    d_assert(subnet, return NULL,);

    pool_alloc_node(&subnet->pool, &ue_ip);
    d_assert(ue_ip, return NULL,);

    return ue_ip;
}

status_t pgw_ue_ip_free(pgw_ue_ip_t *ue_ip)
{
    pgw_subnet_t *subnet = NULL;

    d_assert(ue_ip, return CORE_ERROR,);
    subnet = ue_ip->subnet;

    d_assert(subnet, return CORE_ERROR,);
    pool_free_node(&subnet->pool, ue_ip);

    return CORE_OK;
}

pgw_dev_t *pgw_dev_add(const char *ifname)
{
    pgw_dev_t *dev = NULL;

    d_assert(ifname, return NULL,);

    pool_alloc_node(&pgw_dev_pool, &dev);
    d_assert(dev, return NULL,);
    memset(dev, 0, sizeof *dev);

    strcpy(dev->ifname, ifname);

    list_append(&self.dev_list, dev);

    return dev;
}

status_t pgw_dev_remove(pgw_dev_t *dev)
{
    d_assert(dev, return CORE_ERROR, "Null param");

    list_remove(&self.dev_list, dev);

    if (dev->link_local_addr)
        core_freeaddrinfo(dev->link_local_addr);

    pool_free_node(&pgw_dev_pool, dev);

    return CORE_OK;
}

status_t pgw_dev_remove_all()
{
    pgw_dev_t *dev = NULL, *next_dev = NULL;

    dev = pgw_dev_first();
    while (dev)
    {
        next_dev = pgw_dev_next(dev);

        pgw_dev_remove(dev);

        dev = next_dev;
    }

    return CORE_OK;
}

pgw_dev_t* pgw_dev_find_by_ifname(const char *ifname)
{
    pgw_dev_t *dev = NULL;

    d_assert(ifname, return NULL,);
    
    dev = pgw_dev_first();
    while (dev)
    {
        if (strcmp(dev->ifname, ifname) == 0)
            return dev;

        dev = pgw_dev_next(dev);
    }

    return CORE_OK;
}

pgw_dev_t* pgw_dev_first()
{
    return list_first(&self.dev_list);
}

pgw_dev_t* pgw_dev_next(pgw_dev_t *dev)
{
    return list_next(dev);
}

pgw_subnet_t *pgw_subnet_add(
        const char *ipstr, const char *mask_or_numbits,
        const char *apn, const char *ifname)
{
    status_t rv;
    pgw_dev_t *dev = NULL;
    pgw_subnet_t *subnet = NULL;

    d_assert(ipstr, return NULL,);
    d_assert(mask_or_numbits, return NULL,);
    d_assert(ifname, return NULL,);

    dev = pgw_dev_find_by_ifname(ifname);
    if (!dev)
        dev = pgw_dev_add(ifname);
    d_assert(dev, return NULL,);

    pool_alloc_node(&pgw_subnet_pool, &subnet);
    d_assert(subnet, return NULL,);
    memset(subnet, 0, sizeof *subnet);

    subnet->dev = dev;

    rv = core_ipsubnet(&subnet->gw, ipstr, NULL);
    d_assert(rv == CORE_OK, return NULL,);

    rv = core_ipsubnet(&subnet->sub, ipstr, mask_or_numbits);
    d_assert(rv == CORE_OK, return NULL,);

    if (apn)
        strcpy(subnet->apn, apn);

    subnet->family = subnet->gw.family;
    subnet->prefixlen = atoi(mask_or_numbits);

    pool_init(&subnet->pool, MAX_POOL_OF_SESS);

    list_append(&self.subnet_list, subnet);

    return subnet;
}

status_t pgw_subnet_remove(pgw_subnet_t *subnet)
{
    d_assert(subnet, return CORE_ERROR, "Null param");

    list_remove(&self.subnet_list, subnet);

    if (pool_used(&subnet->pool))
    {
        d_warn("%d not freed in ue_ip_pool[%d] in PGW-Context",
                pool_used(&subnet->pool), pool_size(&subnet->pool));
    }
    d_trace(9, "%d not freed in ue_ip_pool[%d] in PGW-Context\n",
            pool_used(&subnet->pool), pool_size(&subnet->pool));
    pool_final(&subnet->pool);

    pool_free_node(&pgw_subnet_pool, subnet);

    return CORE_OK;
}

status_t pgw_subnet_remove_all()
{
    pgw_subnet_t *subnet = NULL, *next_subnet = NULL;

    subnet = pgw_subnet_first();
    while (subnet)
    {
        next_subnet = pgw_subnet_next(subnet);

        pgw_subnet_remove(subnet);

        subnet = next_subnet;
    }

    return CORE_OK;
}

pgw_subnet_t* pgw_subnet_first()
{
    return list_first(&self.subnet_list);
}

pgw_subnet_t* pgw_subnet_next(pgw_subnet_t *subnet)
{
    return list_next(subnet);
}

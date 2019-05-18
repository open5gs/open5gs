#include <mongoc.h>
#include <yaml.h>

#include "gtp/gtp_types.h"
#include "gtp/gtp_conv.h"
#include "gtp/gtp_node.h"
#include "gtp/gtp_path.h"
#include "gtp/gtp_xact.h"

#include "fd/fd_lib.h"

#include "app/context.h"
#include "pgw_context.h"

static pgw_context_t self;
static fd_config_t g_fd_conf;

int __pgw_log_domain;

static OGS_POOL(pgw_dev_pool, pgw_dev_t);
static OGS_POOL(pgw_subnet_pool, pgw_subnet_t);

static OGS_POOL(pgw_sess_pool, pgw_sess_t);
static OGS_POOL(pgw_bearer_pool, pgw_bearer_t);

static OGS_POOL(pgw_pf_pool, pgw_pf_t);

static int context_initiaized = 0;

void pgw_context_init()
{
    ogs_assert(context_initiaized == 0);

    /* Initial FreeDiameter Config */
    memset(&g_fd_conf, 0, sizeof(fd_config_t));

    /* Initialize PGW context */
    memset(&self, 0, sizeof(pgw_context_t));
    self.fd_config = &g_fd_conf;

    ogs_log_install_domain(&__pgw_log_domain, "pgw", ogs_core()->log.level);

    ogs_list_init(&self.gtpc_list);
    ogs_list_init(&self.gtpc_list6);
    ogs_list_init(&self.gtpu_list);
    ogs_list_init(&self.gtpu_list6);

    gtp_node_init();
    ogs_list_init(&self.sgw_s5c_list);
    ogs_list_init(&self.sgw_s5u_list);

    ogs_list_init(&self.dev_list);
    ogs_pool_init(&pgw_dev_pool, MAX_NUM_OF_DEV);
    ogs_list_init(&self.subnet_list);
    ogs_pool_init(&pgw_subnet_pool, MAX_NUM_OF_SUBNET);

    ogs_pool_init(&pgw_sess_pool, MAX_POOL_OF_SESS);
    ogs_pool_init(&pgw_bearer_pool, MAX_POOL_OF_BEARER);

    ogs_pool_init(&pgw_pf_pool, MAX_POOL_OF_PF);

    self.sess_hash = ogs_hash_make();

    context_initiaized = 1;
}

void pgw_context_final()
{
    ogs_assert(context_initiaized == 1);

    pgw_sess_remove_all();

    ogs_assert(self.sess_hash);
    ogs_hash_destroy(self.sess_hash);

    pgw_dev_remove_all();
    pgw_subnet_remove_all();

    ogs_pool_final(&pgw_bearer_pool);
    ogs_pool_final(&pgw_sess_pool);
    ogs_pool_final(&pgw_pf_pool);

    ogs_pool_final(&pgw_dev_pool);
    ogs_pool_final(&pgw_subnet_pool);

    gtp_remove_all_nodes(&self.sgw_s5c_list);
    gtp_remove_all_nodes(&self.sgw_s5u_list);
    gtp_node_final();

    ogs_sock_remove_all_nodes(&self.gtpc_list);
    ogs_sock_remove_all_nodes(&self.gtpc_list6);
    ogs_sock_remove_all_nodes(&self.gtpu_list);
    ogs_sock_remove_all_nodes(&self.gtpu_list6);

    context_initiaized = 0;
}

pgw_context_t* pgw_self()
{
    return &self;
}

static int pgw_context_prepare()
{
    self.gtpc_port = GTPV2_C_UDP_PORT;
    self.gtpu_port = GTPV1_U_UDP_PORT;
    self.fd_config->cnf_port = DIAMETER_PORT;
    self.fd_config->cnf_port_tls = DIAMETER_SECURE_PORT;

    self.tun_ifname = "pgwtun";

    return OGS_OK;
}

static int pgw_context_validation()
{
    if (self.fd_conf_path == NULL &&
        (self.fd_config->cnf_diamid == NULL ||
        self.fd_config->cnf_diamrlm == NULL ||
        self.fd_config->cnf_addr == NULL))
    {
        ogs_error("No pgw.freeDiameter in '%s'",
                context_self()->config.path);
        return OGS_ERROR;
    }
    if (ogs_list_first(&self.gtpc_list) == NULL &&
        ogs_list_first(&self.gtpc_list6) == NULL)
    {
        ogs_error("No pgw.gtpc in '%s'",
                context_self()->config.path);
        return OGS_ERROR;
    }
    if (ogs_list_first(&self.gtpu_list) == NULL &&
        ogs_list_first(&self.gtpu_list6) == NULL)
    {
        ogs_error("No pgw.gtpu in '%s'",
                context_self()->config.path);
        return OGS_ERROR;
    }
    if (self.dns[0] == NULL && self.dns6[0] == NULL)
    {
        ogs_error("No pgw.dns in '%s'",
                context_self()->config.path);
        return OGS_ERROR;
    }
    return OGS_OK;
}

int pgw_context_parse_config()
{
    int rv;
    config_t *config = &context_self()->config;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    ogs_assert(config);
    document = config->document;
    ogs_assert(document);

    rv = pgw_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while(ogs_yaml_iter_next(&root_iter))
    {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "pgw"))
        {
            ogs_yaml_iter_t pgw_iter;
            ogs_yaml_iter_recurse(&root_iter, &pgw_iter);
            while(ogs_yaml_iter_next(&pgw_iter))
            {
                const char *pgw_key = ogs_yaml_iter_key(&pgw_iter);
                ogs_assert(pgw_key);
                if (!strcmp(pgw_key, "freeDiameter"))
                {
                    yaml_node_t *node = 
                        yaml_document_get_node(document, pgw_iter.pair->value);
                    ogs_assert(node);
                    if (node->type == YAML_SCALAR_NODE)
                    {
                        self.fd_conf_path = ogs_yaml_iter_value(&pgw_iter);
                    }
                    else if (node->type == YAML_MAPPING_NODE)
                    {
                        ogs_yaml_iter_t fd_iter;
                        ogs_yaml_iter_recurse(&pgw_iter, &fd_iter);

                        while(ogs_yaml_iter_next(&fd_iter))
                        {
                            const char *fd_key = ogs_yaml_iter_key(&fd_iter);
                            ogs_assert(fd_key);
                            if (!strcmp(fd_key, "identity"))
                            {
                                self.fd_config->cnf_diamid = 
                                    ogs_yaml_iter_value(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "realm"))
                            {
                                self.fd_config->cnf_diamrlm = 
                                    ogs_yaml_iter_value(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "port"))
                            {
                                const char *v = ogs_yaml_iter_value(&fd_iter);
                                if (v) self.fd_config->cnf_port = atoi(v);
                            }
                            else if (!strcmp(fd_key, "sec_port"))
                            {
                                const char *v = ogs_yaml_iter_value(&fd_iter);
                                if (v) self.fd_config->cnf_port_tls = atoi(v);
                            }
                            else if (!strcmp(fd_key, "no_sctp"))
                            {
                                self.fd_config->cnf_flags.no_sctp =
                                    ogs_yaml_iter_bool(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "listen_on"))
                            {
                                self.fd_config->cnf_addr = 
                                    ogs_yaml_iter_value(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "load_extension"))
                            {
                                ogs_yaml_iter_t ext_array, ext_iter;
                                ogs_yaml_iter_recurse(&fd_iter, &ext_array);
                                do
                                {
                                    const char *module = NULL;
                                    const char *conf = NULL;

                                    if (ogs_yaml_iter_type(&ext_array) ==
                                        YAML_MAPPING_NODE)
                                    {
                                        memcpy(&ext_iter, &ext_array,
                                                sizeof(ogs_yaml_iter_t));
                                    }
                                    else if (ogs_yaml_iter_type(&ext_array) ==
                                        YAML_SEQUENCE_NODE)
                                    {
                                        if (!ogs_yaml_iter_next(&ext_array))
                                            break;
                                        ogs_yaml_iter_recurse(
                                                &ext_array, &ext_iter);
                                    }
                                    else if (ogs_yaml_iter_type(&ext_array) ==
                                        YAML_SCALAR_NODE)
                                    {
                                        break;
                                    }
                                    else
                                        ogs_assert_if_reached();

                                    while(ogs_yaml_iter_next(&ext_iter))
                                    {
                                        const char *ext_key =
                                            ogs_yaml_iter_key(&ext_iter);
                                        ogs_assert(ext_key);
                                        if (!strcmp(ext_key, "module"))
                                        {
                                            module = ogs_yaml_iter_value(&ext_iter);
                                        }
                                        else if (!strcmp(ext_key, "conf"))
                                        {
                                            conf = ogs_yaml_iter_value(&ext_iter);
                                        }
                                        else
                                            ogs_warn("unknown key `%s`", ext_key);
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
                                } while(ogs_yaml_iter_type(&ext_array) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(fd_key, "connect"))
                            {
                                ogs_yaml_iter_t conn_array, conn_iter;
                                ogs_yaml_iter_recurse(&fd_iter, &conn_array);
                                do
                                {
                                    const char *identity = NULL;
                                    const char *addr = NULL;
                                    uint16_t port = 0;

                                    if (ogs_yaml_iter_type(&conn_array) ==
                                        YAML_MAPPING_NODE)
                                    {
                                        memcpy(&conn_iter, &conn_array,
                                                sizeof(ogs_yaml_iter_t));
                                    }
                                    else if (ogs_yaml_iter_type(&conn_array) ==
                                        YAML_SEQUENCE_NODE)
                                    {
                                        if (!ogs_yaml_iter_next(&conn_array))
                                            break;
                                        ogs_yaml_iter_recurse(&conn_array, &conn_iter);
                                    }
                                    else if (ogs_yaml_iter_type(&conn_array) ==
                                        YAML_SCALAR_NODE)
                                    {
                                        break;
                                    }
                                    else
                                        ogs_assert_if_reached();

                                    while(ogs_yaml_iter_next(&conn_iter))
                                    {
                                        const char *conn_key =
                                            ogs_yaml_iter_key(&conn_iter);
                                        ogs_assert(conn_key);
                                        if (!strcmp(conn_key, "identity"))
                                        {
                                            identity = ogs_yaml_iter_value(&conn_iter);
                                        }
                                        else if (!strcmp(conn_key, "addr"))
                                        {
                                            addr = ogs_yaml_iter_value(&conn_iter);
                                        }
                                        else if (!strcmp(conn_key, "port"))
                                        {
                                            const char *v =
                                                ogs_yaml_iter_value(&conn_iter);
                                            if (v) port = atoi(v);
                                        }
                                        else
                                            ogs_warn("unknown key `%s`", conn_key);
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
                                } while(ogs_yaml_iter_type(&conn_array) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else
                                ogs_warn("unknown key `%s`", fd_key);
                        }
                    }
                }
                else if (!strcmp(pgw_key, "gtpc"))
                {
                    ogs_yaml_iter_t gtpc_array, gtpc_iter;
                    ogs_yaml_iter_recurse(&pgw_iter, &gtpc_array);
                    do
                    {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.gtpc_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *list = NULL;
                        ogs_socknode_t *node = NULL;

                        if (ogs_yaml_iter_type(&gtpc_array) == YAML_MAPPING_NODE)
                        {
                            memcpy(&gtpc_iter, &gtpc_array,
                                    sizeof(ogs_yaml_iter_t));
                        }
                        else if (ogs_yaml_iter_type(&gtpc_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!ogs_yaml_iter_next(&gtpc_array))
                                break;
                            ogs_yaml_iter_recurse(&gtpc_array, &gtpc_iter);
                        }
                        else if (ogs_yaml_iter_type(&gtpc_array) ==
                            YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            ogs_assert_if_reached();

                        while(ogs_yaml_iter_next(&gtpc_iter))
                        {
                            const char *gtpc_key =
                                ogs_yaml_iter_key(&gtpc_iter);
                            ogs_assert(gtpc_key);
                            if (!strcmp(gtpc_key, "family"))
                            {
                                const char *v = ogs_yaml_iter_value(&gtpc_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6)
                                {
                                    ogs_warn("Ignore family(%d) : AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            }
                            else if (!strcmp(gtpc_key, "addr") ||
                                    !strcmp(gtpc_key, "name"))
                            {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&gtpc_iter, &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do
                                {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!ogs_yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num <= MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] = 
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while(
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(gtpc_key, "port"))
                            {
                                const char *v = ogs_yaml_iter_value(&gtpc_iter);
                                if (v)
                                {
                                    port = atoi(v);
                                    self.gtpc_port = port;
                                }
                            }
                            else if (!strcmp(gtpc_key, "dev"))
                            {
                                dev = ogs_yaml_iter_value(&gtpc_iter);
                            }
                            else if (!strcmp(gtpc_key, "apn"))
                            {
                                /* Skip */
                            }
                            else
                                ogs_warn("unknown key `%s`", gtpc_key);
                        }

                        list = NULL;
                        for (i = 0; i < num; i++)
                        {
                            rv = ogs_addaddrinfo(&list,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        if (list)
                        {
                            if (context_self()->config.parameter.no_ipv4 == 0)
                            {
                                rv = ogs_sock_add_node(&self.gtpc_list,
                                        &node, list, AF_INET);
                                ogs_assert(rv == OGS_OK);
                            }

                            if (context_self()->config.parameter.no_ipv6 == 0)
                            {
                                rv = ogs_sock_add_node(&self.gtpc_list6,
                                        &node, list, AF_INET6);
                                ogs_assert(rv == OGS_OK);
                            }

                            ogs_freeaddrinfo(list);
                        }

                        if (dev)
                        {
                            rv = ogs_sock_probe_node(
                                    context_self()->config.parameter.no_ipv4 ?
                                        NULL : &self.gtpc_list,
                                    context_self()->config.parameter.no_ipv6 ?
                                        NULL : &self.gtpc_list6,
                                    dev, self.gtpc_port);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while(ogs_yaml_iter_type(&gtpc_array) == YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.gtpc_list) == NULL &&
                        ogs_list_first(&self.gtpc_list6) == NULL)
                    {
                        rv = ogs_sock_probe_node(
                                context_self()->config.parameter.no_ipv4 ?
                                    NULL : &self.gtpc_list,
                                context_self()->config.parameter.no_ipv6 ?
                                    NULL : &self.gtpc_list6,
                                NULL, self.gtpc_port);
                        ogs_assert(rv == OGS_OK);
                    }
                }
                else if (!strcmp(pgw_key, "gtpu"))
                {
                    ogs_yaml_iter_t gtpu_array, gtpu_iter;
                    ogs_yaml_iter_recurse(&pgw_iter, &gtpu_array);
                    do
                    {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.gtpu_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *list = NULL;
                        ogs_socknode_t *node = NULL;

                        if (ogs_yaml_iter_type(&gtpu_array) == YAML_MAPPING_NODE)
                        {
                            memcpy(&gtpu_iter, &gtpu_array,
                                    sizeof(ogs_yaml_iter_t));
                        }
                        else if (ogs_yaml_iter_type(&gtpu_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!ogs_yaml_iter_next(&gtpu_array))
                                break;
                            ogs_yaml_iter_recurse(&gtpu_array, &gtpu_iter);
                        }
                        else if (ogs_yaml_iter_type(&gtpu_array) ==
                            YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            ogs_assert_if_reached();

                        while(ogs_yaml_iter_next(&gtpu_iter))
                        {
                            const char *gtpu_key =
                                ogs_yaml_iter_key(&gtpu_iter);
                            ogs_assert(gtpu_key);
                            if (!strcmp(gtpu_key, "family"))
                            {
                                const char *v = ogs_yaml_iter_value(&gtpu_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6)
                                {
                                    ogs_warn("Ignore family(%d) : AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            }
                            else if (!strcmp(gtpu_key, "addr") ||
                                    !strcmp(gtpu_key, "name"))
                            {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&gtpu_iter, &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do
                                {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!ogs_yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num <= MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] = 
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while(
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(gtpu_key, "port"))
                            {
                                const char *v = ogs_yaml_iter_value(&gtpu_iter);
                                if (v)
                                {
                                    port = atoi(v);
                                    self.gtpu_port = port;
                                }
                            }
                            else if (!strcmp(gtpu_key, "dev"))
                            {
                                dev = ogs_yaml_iter_value(&gtpu_iter);
                            }
                            else
                                ogs_warn("unknown key `%s`", gtpu_key);
                        }

                        list = NULL;
                        for (i = 0; i < num; i++)
                        {
                            rv = ogs_addaddrinfo(&list,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        if (list)
                        {
                            if (context_self()->config.parameter.no_ipv4 == 0)
                            {
                                rv = ogs_sock_add_node(&self.gtpu_list,
                                        &node, list, AF_INET);
                                ogs_assert(rv == OGS_OK);
                            }

                            if (context_self()->config.parameter.no_ipv6 == 0)
                            {
                                rv = ogs_sock_add_node(&self.gtpu_list6,
                                        &node, list, AF_INET6);
                                ogs_assert(rv == OGS_OK);
                            }

                            ogs_freeaddrinfo(list);
                        }

                        if (dev)
                        {
                            rv = ogs_sock_probe_node(
                                    context_self()->config.parameter.no_ipv4 ?
                                        NULL : &self.gtpu_list,
                                    context_self()->config.parameter.no_ipv6 ?
                                        NULL : &self.gtpu_list6,
                                    dev, self.gtpu_port);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while(ogs_yaml_iter_type(&gtpu_array) == YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.gtpu_list) == NULL &&
                        ogs_list_first(&self.gtpu_list6) == NULL)
                    {
                        rv = ogs_sock_probe_node(
                                context_self()->config.parameter.no_ipv4 ?
                                    NULL : &self.gtpu_list,
                                context_self()->config.parameter.no_ipv6 ?
                                    NULL : &self.gtpu_list6,
                                NULL, self.gtpu_port);
                        ogs_assert(rv == OGS_OK);
                    }
                }
                else if (!strcmp(pgw_key, "ue_pool"))
                {
                    ogs_yaml_iter_t ue_pool_array, ue_pool_iter;
                    ogs_yaml_iter_recurse(&pgw_iter, &ue_pool_array);
                    do
                    {
                        pgw_subnet_t *subnet = NULL;
                        const char *ipstr = NULL;
                        const char *mask_or_numbits = NULL;
                        const char *apn = NULL;
                        const char *dev = self.tun_ifname;

                        if (ogs_yaml_iter_type(&ue_pool_array) ==
                                YAML_MAPPING_NODE)
                        {
                            memcpy(&ue_pool_iter, &ue_pool_array,
                                    sizeof(ogs_yaml_iter_t));
                        }
                        else if (ogs_yaml_iter_type(&ue_pool_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!ogs_yaml_iter_next(&ue_pool_array))
                                break;
                            ogs_yaml_iter_recurse(&ue_pool_array,
                                    &ue_pool_iter);
                        }
                        else if (ogs_yaml_iter_type(&ue_pool_array) ==
                                YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            ogs_assert_if_reached();

                        while(ogs_yaml_iter_next(&ue_pool_iter))
                        {
                            const char *ue_pool_key =
                                ogs_yaml_iter_key(&ue_pool_iter);
                            ogs_assert(ue_pool_key);
                            if (!strcmp(ue_pool_key, "addr"))
                            {
                                char *v =
                                    (char *)ogs_yaml_iter_value(&ue_pool_iter);
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
                                apn = ogs_yaml_iter_value(&ue_pool_iter);
                            }
                            else if (!strcmp(ue_pool_key, "dev"))
                            {
                                dev = ogs_yaml_iter_value(&ue_pool_iter);
                            }
                            else
                                ogs_warn("unknown key `%s`", ue_pool_key);
                        }

                        if (ipstr && mask_or_numbits)
                        {
                            subnet = pgw_subnet_add(
                                    ipstr, mask_or_numbits, apn, dev);
                            ogs_assert(subnet);
                        }
                        else
                        {
                            ogs_warn("Ignore : addr(%s/%s), apn(%s)",
                                    ipstr, mask_or_numbits, apn);
                        }
                    } while(ogs_yaml_iter_type(&ue_pool_array) ==
                            YAML_SEQUENCE_NODE);
                }
                else if (!strcmp(pgw_key, "dns"))
                {
                    ogs_yaml_iter_t dns_iter;
                    ogs_yaml_iter_recurse(&pgw_iter, &dns_iter);
                    ogs_assert(ogs_yaml_iter_type(&dns_iter) !=
                        YAML_MAPPING_NODE);

                    do
                    {
                        const char *v = NULL;

                        if (ogs_yaml_iter_type(&dns_iter) ==
                                YAML_SEQUENCE_NODE)
                        {
                            if (!ogs_yaml_iter_next(&dns_iter))
                                break;
                        }

                        v = ogs_yaml_iter_value(&dns_iter);
                        if (v)
                        {
                            ogs_ipsubnet_t ipsub;
                            rv = ogs_ipsubnet(&ipsub, v, NULL);
                            ogs_assert(rv == OGS_OK);

                            if (ipsub.family == AF_INET)
                            {
                                if (self.dns[0] && self.dns[1])
                                    ogs_warn("Ignore DNS : %s", v);
                                else if (self.dns[0]) self.dns[1] = v;
                                else self.dns[0] = v;
                            }
                            else if (ipsub.family == AF_INET6)
                            {
                                if (self.dns6[0] && self.dns6[1])
                                    ogs_warn("Ignore DNS : %s", v);
                                else if (self.dns6[0]) self.dns6[1] = v;
                                else self.dns6[0] = v;
                            }
                            else
                                ogs_warn("Ignore DNS : %s", v);
                        }

                    } while(
                        ogs_yaml_iter_type(&dns_iter) ==
                            YAML_SEQUENCE_NODE);
                }
                else if (!strcmp(pgw_key, "p-cscf"))
                {
                    ogs_yaml_iter_t dns_iter;
                    ogs_yaml_iter_recurse(&pgw_iter, &dns_iter);
                    ogs_assert(ogs_yaml_iter_type(&dns_iter) !=
                        YAML_MAPPING_NODE);

                    self.num_of_p_cscf = 0;
                    self.num_of_p_cscf6 = 0;
                    do
                    {
                        const char *v = NULL;

                        if (ogs_yaml_iter_type(&dns_iter) ==
                                YAML_SEQUENCE_NODE)
                        {
                            if (!ogs_yaml_iter_next(&dns_iter))
                                break;
                        }

                        v = ogs_yaml_iter_value(&dns_iter);
                        if (v)
                        {
                            ogs_ipsubnet_t ipsub;
                            rv = ogs_ipsubnet(&ipsub, v, NULL);
                            ogs_assert(rv == OGS_OK);

                            if (ipsub.family == AF_INET)
                            {
                                if (self.num_of_p_cscf >= MAX_NUM_OF_P_CSCF)
                                    ogs_warn("Ignore P-CSCF : %s", v);
                                else self.p_cscf[self.num_of_p_cscf++] = v;
                            }
                            else if (ipsub.family == AF_INET6)
                            {
                                if (self.num_of_p_cscf6 >= MAX_NUM_OF_P_CSCF)
                                    ogs_warn("Ignore P-CSCF : %s", v);
                                else self.p_cscf6[self.num_of_p_cscf6++] = v;
                            }
                            else
                                ogs_warn("Ignore P-CSCF : %s", v);
                        }

                    } while(
                        ogs_yaml_iter_type(&dns_iter) ==
                            YAML_SEQUENCE_NODE);
                }
                else
                    ogs_warn("unknown key `%s`", pgw_key);
            }
        }
    }

    rv = pgw_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

static void *sess_hash_keygen(uint8_t *out, int *out_len,
        uint8_t *imsi, int imsi_len, char *apn)
{
    memcpy(out, imsi, imsi_len);
    ogs_cpystrn((char*)(out+imsi_len), apn, MAX_APN_LEN+1);
    *out_len = imsi_len+strlen((char*)(out+imsi_len));

    return out;
}

pgw_sess_t *pgw_sess_add(
        uint8_t *imsi, int imsi_len, char *apn, 
        uint8_t pdn_type, uint8_t ebi)
{
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];
    pgw_sess_t *sess = NULL;
    pgw_bearer_t *bearer = NULL;
    pgw_subnet_t *subnet6 = NULL;

    ogs_pool_alloc(&pgw_sess_pool, &sess);
    ogs_assert(sess);
    sess->index = ogs_pool_index(&pgw_sess_pool, sess);
    ogs_assert(sess->index > 0 && sess->index <= MAX_POOL_OF_SESS);

    sess->gnode = NULL;

    sess->pgw_s5c_teid = sess->index;  /* derived from an index */

    /* Set IMSI */
    sess->imsi_len = imsi_len;
    memcpy(sess->imsi, imsi, sess->imsi_len);
    ogs_buffer_to_bcd(sess->imsi, sess->imsi_len, sess->imsi_bcd);

    /* Set APN */
    ogs_cpystrn(sess->pdn.apn, apn, MAX_APN_LEN+1);

    ogs_list_init(&sess->bearer_list);

    ogs_cpystrn(sess->pdn.apn, apn, MAX_APN_LEN+1);

    bearer = pgw_bearer_add(sess);
    ogs_assert(bearer);
    bearer->ebi = ebi;

    sess->pdn.paa.pdn_type = pdn_type;
    if (pdn_type == GTP_PDN_TYPE_IPV4)
    {
        sess->ipv4 = pgw_ue_ip_alloc(AF_INET, apn);
        ogs_assert(sess->ipv4);
        sess->pdn.paa.addr = sess->ipv4->addr[0];
    }
    else if (pdn_type == GTP_PDN_TYPE_IPV6)
    {
        sess->ipv6 = pgw_ue_ip_alloc(AF_INET6, apn);
        ogs_assert(sess->ipv6);

        subnet6 = sess->ipv6->subnet;
        ogs_assert(subnet6);

        sess->pdn.paa.len = subnet6->prefixlen;
        memcpy(sess->pdn.paa.addr6, sess->ipv6->addr, IPV6_LEN);
    }
    else if (pdn_type == GTP_PDN_TYPE_IPV4V6)
    {
        sess->ipv4 = pgw_ue_ip_alloc(AF_INET, apn);
        ogs_assert(sess->ipv4);
        sess->ipv6 = pgw_ue_ip_alloc(AF_INET6, apn);
        ogs_assert(sess->ipv6);

        subnet6 = sess->ipv6->subnet;
        ogs_assert(subnet6);

        sess->pdn.paa.both.addr = sess->ipv4->addr[0];
        sess->pdn.paa.both.len = subnet6->prefixlen;
        memcpy(sess->pdn.paa.both.addr6, sess->ipv6->addr, IPV6_LEN);
    }
    else
        ogs_assert_if_reached();

    ogs_info("UE IMSI:[%s] APN:[%s] IPv4:[%s] IPv6:[%s]",
	    sess->imsi_bcd,
	    apn,
            sess->ipv4 ?  INET_NTOP(&sess->ipv4->addr, buf1) : "",
            sess->ipv6 ?  INET6_NTOP(&sess->ipv6->addr, buf2) : "");

    /* Generate Hash Key : IMSI + APN */
    sess_hash_keygen(sess->hash_keybuf, &sess->hash_keylen,
            imsi, imsi_len, apn);
    ogs_hash_set(self.sess_hash, sess->hash_keybuf, sess->hash_keylen, sess);

    return sess;
}

int pgw_sess_remove(pgw_sess_t *sess)
{
    ogs_assert(self.sess_hash);
    ogs_assert(sess);

    ogs_hash_set(self.sess_hash, sess->hash_keybuf, sess->hash_keylen, NULL);

    if (sess->ipv4)
        pgw_ue_ip_free(sess->ipv4);
    if (sess->ipv6)
        pgw_ue_ip_free(sess->ipv6);

    pgw_bearer_remove_all(sess);

    ogs_pool_free(&pgw_sess_pool, sess);

    return OGS_OK;
}

void pgw_sess_remove_all()
{
    ogs_hash_index_t *hi = NULL;
    pgw_sess_t *sess = NULL;

    for (hi = pgw_sess_first(); hi; hi = pgw_sess_next(hi))
    {
        sess = pgw_sess_this(hi);
        pgw_sess_remove(sess);
    }
}

pgw_sess_t* pgw_sess_find(uint32_t index)
{
    ogs_assert(index);
    return ogs_pool_find(&pgw_sess_pool, index);
}

pgw_sess_t* pgw_sess_find_by_teid(uint32_t teid)
{
    return pgw_sess_find(teid);
}

pgw_sess_t* pgw_sess_find_by_imsi_apn(
    uint8_t *imsi, int imsi_len, char *apn)
{
    uint8_t keybuf[MAX_IMSI_LEN+MAX_APN_LEN+1];
    int keylen = 0;

    ogs_assert(self.sess_hash);

    sess_hash_keygen(keybuf, &keylen, imsi, imsi_len, apn);
    return (pgw_sess_t *)ogs_hash_get(self.sess_hash, keybuf, keylen);
}

gtp_node_t *pgw_sgw_add_by_message(gtp_message_t *message)
{
    int rv;
    gtp_node_t *sgw = NULL;
    gtp_f_teid_t *sgw_s5c_teid = NULL;

    gtp_create_session_request_t *req = &message->create_session_request;

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        ogs_error("No Sender F-TEID");
        return NULL;
    }

    sgw_s5c_teid = req->sender_f_teid_for_control_plane.data;
    ogs_assert(sgw_s5c_teid);
    sgw = gtp_find_node(&pgw_self()->sgw_s5c_list, sgw_s5c_teid);
    if (!sgw)
    {
        sgw = gtp_add_node(&pgw_self()->sgw_s5c_list, sgw_s5c_teid,
            pgw_self()->gtpc_port,
            context_self()->config.parameter.no_ipv4,
            context_self()->config.parameter.no_ipv6,
            context_self()->config.parameter.prefer_ipv4);
        ogs_assert(sgw);

        rv = gtp_connect(pgw_self()->gtpc_sock, pgw_self()->gtpc_sock6, sgw);
        ogs_assert(rv == OGS_OK);
    }

    return sgw;
}
pgw_sess_t *pgw_sess_add_by_message(gtp_message_t *message)
{
    pgw_sess_t *sess = NULL;
    char apn[MAX_APN_LEN];

    gtp_create_session_request_t *req = &message->create_session_request;

    if (req->imsi.presence == 0)
    {
        ogs_error("No IMSI");
        return NULL;
    }
    if (req->access_point_name.presence == 0)
    {
        ogs_error("No APN");
        return NULL;
    }
    if (req->bearer_contexts_to_be_created.presence == 0)
    {
        ogs_error("No Bearer");
        return NULL;
    }
    if (req->bearer_contexts_to_be_created.eps_bearer_id.presence == 0)
    {
        ogs_error("No EPS Bearer ID");
        return NULL;
    }
    if (req->pdn_type.presence == 0)
    {
        ogs_error("No PDN Type");
        return NULL;
    }

    apn_parse(apn, req->access_point_name.data, req->access_point_name.len);

    ogs_trace("pgw_sess_add_by_message() [APN:%s, PDN:%d, EDI:%d]",
            apn, req->pdn_type.u8,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);

    sess = pgw_sess_find_by_imsi_apn(req->imsi.data, req->imsi.len, apn);
    if (!sess)
    {
        sess = pgw_sess_add(req->imsi.data, req->imsi.len, apn,
            req->pdn_type.u8,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);
        ogs_assert(sess);
    }

    return sess;
}

ogs_hash_index_t* pgw_sess_first()
{
    ogs_assert(self.sess_hash);
    return ogs_hash_first(self.sess_hash);
}

ogs_hash_index_t* pgw_sess_next(ogs_hash_index_t *hi)
{
    return ogs_hash_next(hi);
}

pgw_sess_t *pgw_sess_this(ogs_hash_index_t *hi)
{
    ogs_assert(hi);
    return ogs_hash_this_val(hi);
}

pgw_bearer_t* pgw_bearer_add(pgw_sess_t *sess)
{
    pgw_bearer_t *bearer = NULL;

    ogs_assert(sess);

    ogs_pool_alloc(&pgw_bearer_pool, &bearer);
    ogs_assert(bearer);
    bearer->index = ogs_pool_index(&pgw_bearer_pool, bearer);
    ogs_assert(bearer->index > 0 && bearer->index <= MAX_POOL_OF_BEARER);

    bearer->name = NULL;

    ogs_list_init(&bearer->pf_list);

    bearer->pgw_s5u_teid = bearer->index;
    
    bearer->sess = sess;
    bearer->gnode = NULL;

    ogs_list_add(&sess->bearer_list, bearer);

    return bearer;
}

int pgw_bearer_remove(pgw_bearer_t *bearer)
{
    ogs_assert(bearer);
    ogs_assert(bearer->sess);

    ogs_list_remove(&bearer->sess->bearer_list, bearer);

    if (bearer->name)
        ogs_free(bearer->name);

    pgw_pf_remove_all(bearer);

    ogs_pool_free(&pgw_bearer_pool, bearer);

    return OGS_OK;
}

void pgw_bearer_remove_all(pgw_sess_t *sess)
{
    pgw_bearer_t *bearer = NULL, *next_bearer = NULL;

    ogs_assert(sess);
    ogs_list_for_each_safe(&sess->bearer_list, next_bearer, bearer)
        pgw_bearer_remove(bearer);
}

pgw_bearer_t* pgw_bearer_find(uint32_t index)
{
    ogs_assert(index);
    return ogs_pool_find(&pgw_bearer_pool, index);
}

pgw_bearer_t* pgw_bearer_find_by_pgw_s5u_teid(uint32_t pgw_s5u_teid)
{
    return pgw_bearer_find(pgw_s5u_teid);
}

pgw_bearer_t* pgw_bearer_find_by_ebi(pgw_sess_t *sess, uint8_t ebi)
{
    pgw_bearer_t *bearer = NULL;
    
    ogs_assert(sess);

    bearer = pgw_bearer_first(sess);
    while (bearer)
    {
        if (bearer->ebi == ebi)
            break;

        bearer = pgw_bearer_next(bearer);
    }

    return bearer;
}

pgw_bearer_t* pgw_bearer_find_by_name(pgw_sess_t *sess, char *name)
{
    pgw_bearer_t *bearer = NULL;
    
    ogs_assert(sess);
    ogs_assert(name);

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
                                uint8_t qci,
                                uint8_t priority_level,
                                uint8_t pre_emption_capability,
                                uint8_t pre_emption_vulnerability)
{
    pgw_bearer_t *bearer = NULL;

    ogs_assert(sess);

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
    ogs_assert(sess);
    return ogs_list_first(&sess->bearer_list);
}

pgw_bearer_t* pgw_bearer_next(pgw_bearer_t *bearer)
{
    return ogs_list_next(bearer);
}

pgw_pf_t *pgw_pf_add(pgw_bearer_t *bearer, uint32_t precedence)
{
    pgw_pf_t *pf = NULL;

    ogs_assert(bearer);

    ogs_pool_alloc(&pgw_pf_pool, &pf);
    ogs_assert(pf);

    pf->identifier = NEXT_ID(bearer->pf_identifier, 1, 15);
    pf->bearer = bearer;

    ogs_list_add(&bearer->pf_list, pf);

    return pf;
}

int pgw_pf_remove(pgw_pf_t *pf)
{
    ogs_assert(pf);
    ogs_assert(pf->bearer);

    ogs_list_remove(&pf->bearer->pf_list, pf);
    ogs_pool_free(&pgw_pf_pool, pf);

    return OGS_OK;
}

void pgw_pf_remove_all(pgw_bearer_t *bearer)
{
    pgw_pf_t *pf = NULL, *next_pf = NULL;

    ogs_assert(bearer);
    ogs_list_for_each_safe(&bearer->pf_list, next_pf, pf)
        pgw_pf_remove(pf);
}

pgw_pf_t* pgw_pf_find_by_id(pgw_bearer_t *bearer, uint8_t id)
{
    pgw_pf_t *pf = NULL;
    
    pf = pgw_pf_first(bearer);
    while (pf)
    {
        if (pf->identifier == id)
            return pf;

        pf = pgw_pf_next(pf);
    }

    return OGS_OK;
}

pgw_pf_t* pgw_pf_first(pgw_bearer_t *bearer)
{
    return ogs_list_first(&bearer->pf_list);
}

pgw_pf_t* pgw_pf_next(pgw_pf_t *pf)
{
    return ogs_list_next(pf);
}

int pgw_ue_pool_generate()
{
    int j;
    pgw_subnet_t *subnet = NULL;

    for (subnet = pgw_subnet_first(); subnet; subnet = pgw_subnet_next(subnet))
    {
        int index = 0;
        uint32_t mask_count;
        uint32_t broadcast[4];

        if (subnet->family == AF_INET)
        {
            if (subnet->prefixlen == 32)
                mask_count = 1;
            else if (subnet->prefixlen < 32)
                mask_count = (0xffffffff >> subnet->prefixlen) + 1;
            else
            {
                ogs_assert_if_reached();
                return OGS_ERROR;
            }
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
            {
                ogs_assert_if_reached();
                return OGS_ERROR;
            }
        }
        else
        {
            ogs_assert_if_reached();
            return OGS_ERROR;
        }
        
        for (j = 0; j < 4; j++)
        {
            broadcast[j] = subnet->sub.sub[j] + ~subnet->sub.mask[j];
        }

        for (j = 0; j < mask_count && index < MAX_POOL_OF_SESS; j++)
        {
            pgw_ue_ip_t *ue_ip = NULL;
            int maxbytes = 0;
            int lastindex = 0;

            ue_ip = &subnet->pool.array[index];
            ogs_assert(ue_ip);
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

    return OGS_OK;
}

static pgw_subnet_t *find_subnet(int family, const char *apn)
{
    pgw_subnet_t *subnet = NULL;

    ogs_assert(apn);
    ogs_assert(family == AF_INET || family == AF_INET6);

    for (subnet = pgw_subnet_first(); subnet; subnet = pgw_subnet_next(subnet))
    {
        if (strlen(subnet->apn))
        {
            if (subnet->family == family && strcmp(subnet->apn, apn) == 0 &&
                ogs_pool_avail(&subnet->pool))
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
                ogs_pool_avail(&subnet->pool))
            {
                return subnet;
            }
        }
    }

    if (subnet == NULL)
        ogs_error("CHECK CONFIGURATION: Cannot find UE Pool");

    return subnet;
}

pgw_ue_ip_t *pgw_ue_ip_alloc(int family, const char *apn)
{
    pgw_subnet_t *subnet = NULL;
    pgw_ue_ip_t *ue_ip = NULL;

    ogs_assert(apn);

    subnet = find_subnet(family, apn);
    ogs_assert(subnet);

    ogs_pool_alloc(&subnet->pool, &ue_ip);
    ogs_assert(ue_ip);

    return ue_ip;
}

int pgw_ue_ip_free(pgw_ue_ip_t *ue_ip)
{
    pgw_subnet_t *subnet = NULL;

    ogs_assert(ue_ip);
    subnet = ue_ip->subnet;

    ogs_assert(subnet);
    ogs_pool_free(&subnet->pool, ue_ip);

    return OGS_OK;
}

pgw_dev_t *pgw_dev_add(const char *ifname)
{
    pgw_dev_t *dev = NULL;

    ogs_assert(ifname);

    ogs_pool_alloc(&pgw_dev_pool, &dev);
    ogs_assert(dev);
    memset(dev, 0, sizeof *dev);

    strcpy(dev->ifname, ifname);

    ogs_list_add(&self.dev_list, dev);

    return dev;
}

int pgw_dev_remove(pgw_dev_t *dev)
{
    ogs_assert(dev);

    ogs_list_remove(&self.dev_list, dev);

    if (dev->link_local_addr)
        ogs_freeaddrinfo(dev->link_local_addr);

    ogs_pool_free(&pgw_dev_pool, dev);

    return OGS_OK;
}

void pgw_dev_remove_all()
{
    pgw_dev_t *dev = NULL, *next_dev = NULL;

    ogs_list_for_each_safe(&self.dev_list, next_dev, dev)
        pgw_dev_remove(dev);
}

pgw_dev_t* pgw_dev_find_by_ifname(const char *ifname)
{
    pgw_dev_t *dev = NULL;

    ogs_assert(ifname);
    
    dev = pgw_dev_first();
    while (dev)
    {
        if (strcmp(dev->ifname, ifname) == 0)
            return dev;

        dev = pgw_dev_next(dev);
    }

    return OGS_OK;
}

pgw_dev_t* pgw_dev_first()
{
    return ogs_list_first(&self.dev_list);
}

pgw_dev_t* pgw_dev_next(pgw_dev_t *dev)
{
    return ogs_list_next(dev);
}

pgw_subnet_t *pgw_subnet_add(
        const char *ipstr, const char *mask_or_numbits,
        const char *apn, const char *ifname)
{
    int rv;
    pgw_dev_t *dev = NULL;
    pgw_subnet_t *subnet = NULL;

    ogs_assert(ipstr);
    ogs_assert(mask_or_numbits);
    ogs_assert(ifname);

    dev = pgw_dev_find_by_ifname(ifname);
    if (!dev)
        dev = pgw_dev_add(ifname);
    ogs_assert(dev);

    ogs_pool_alloc(&pgw_subnet_pool, &subnet);
    ogs_assert(subnet);
    memset(subnet, 0, sizeof *subnet);

    subnet->dev = dev;

    rv = ogs_ipsubnet(&subnet->gw, ipstr, NULL);
    ogs_assert(rv == OGS_OK);

    rv = ogs_ipsubnet(&subnet->sub, ipstr, mask_or_numbits);
    ogs_assert(rv == OGS_OK);

    if (apn)
        strcpy(subnet->apn, apn);

    subnet->family = subnet->gw.family;
    subnet->prefixlen = atoi(mask_or_numbits);

    ogs_pool_init(&subnet->pool, MAX_POOL_OF_SESS);

    ogs_list_add(&self.subnet_list, subnet);

    return subnet;
}

int pgw_subnet_remove(pgw_subnet_t *subnet)
{
    ogs_assert(subnet);

    ogs_list_remove(&self.subnet_list, subnet);

    ogs_pool_final(&subnet->pool);

    ogs_pool_free(&pgw_subnet_pool, subnet);

    return OGS_OK;
}

void pgw_subnet_remove_all()
{
    pgw_subnet_t *subnet = NULL, *next_subnet = NULL;

    ogs_list_for_each_safe(&self.subnet_list, next_subnet, subnet)
        pgw_subnet_remove(subnet);
}

pgw_subnet_t* pgw_subnet_first()
{
    return ogs_list_first(&self.subnet_list);
}

pgw_subnet_t* pgw_subnet_next(pgw_subnet_t *subnet)
{
    return ogs_list_next(subnet);
}

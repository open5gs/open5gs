#define TRACE_MODULE _mme_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"
#include "core_msgq.h"
#include "core_fsm.h"
#include "core_network.h"

#include <mongoc.h>
#include <yaml.h>
#include "app/yaml_helper.h"

#include "s1ap/s1ap_message.h"
#include "gtp/gtp_xact.h"
#include "gtp/gtp_node.h"
#include "gtp/gtp_path.h"
#include "fd/fd_lib.h"

#include "app/context.h"
#include "nas_conv.h"
#include "mme_context.h"
#include "mme_event.h"
#include "s1ap_path.h"
#include "mme_sm.h"

#define MAX_CELL_PER_ENB            8

static mme_context_t self;
static fd_config_t g_fd_conf;

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

    /* Initial FreeDiameter Config */
    memset(&g_fd_conf, 0, sizeof(fd_config_t));

    /* Initialize MME context */
    memset(&self, 0, sizeof(mme_context_t));
    self.fd_config = &g_fd_conf;

    list_init(&self.s1ap_list);
    list_init(&self.s1ap_list6);

    list_init(&self.gtpc_list);
    list_init(&self.gtpc_list6);

    gtp_node_init();
    list_init(&self.sgw_list);
    list_init(&self.pgw_list);

    index_init(&mme_enb_pool, MAX_NUM_OF_ENB);

    index_init(&mme_ue_pool, MAX_POOL_OF_UE);
    index_init(&enb_ue_pool, MAX_POOL_OF_UE);
    index_init(&mme_sess_pool, MAX_POOL_OF_SESS);
    index_init(&mme_bearer_pool, MAX_POOL_OF_BEARER);
    pool_init(&self.m_tmsi, MAX_POOL_OF_UE);

    self.enb_sock_hash = hash_make();
    self.enb_addr_hash = hash_make();
    self.enb_id_hash = hash_make();
    self.mme_ue_s1ap_id_hash = hash_make();
    self.imsi_ue_hash = hash_make();
    self.guti_ue_hash = hash_make();

    /* Timer value */
    self.t3413_value = 2;               /* Paging retry timer: 2 secs */
    self.s1_holding_timer_value = 30;   /* S1 holding timer: 30 secs */

    context_initialized = 1;

    return CORE_OK;
}

status_t mme_context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "MME context already has been finalized");

    mme_enb_remove_all();
    mme_ue_remove_all();

    if (pool_used(&self.m_tmsi))
    {
        d_error("%d not freed in M-TMSI pool[%d] in MME-Context",
                pool_used(&self.m_tmsi), pool_size(&self.m_tmsi));
    }
    d_trace(9, "%d not freed in M-TMSI pool[%d] in MME-Context\n",
            pool_used(&self.m_tmsi), pool_size(&self.m_tmsi));

    d_assert(self.enb_sock_hash, , "Null param");
    hash_destroy(self.enb_sock_hash);
    d_assert(self.enb_addr_hash, , "Null param");
    hash_destroy(self.enb_addr_hash);
    d_assert(self.enb_id_hash, , "Null param");
    hash_destroy(self.enb_id_hash);

    d_assert(self.mme_ue_s1ap_id_hash, , "Null param");
    hash_destroy(self.mme_ue_s1ap_id_hash);
    d_assert(self.imsi_ue_hash, , "Null param");
    hash_destroy(self.imsi_ue_hash);
    d_assert(self.guti_ue_hash, , "Null param");
    hash_destroy(self.guti_ue_hash);

    pool_final(&self.m_tmsi);
    index_final(&mme_bearer_pool);
    index_final(&mme_sess_pool);
    index_final(&mme_ue_pool);
    index_final(&enb_ue_pool);

    index_final(&mme_enb_pool);

    gtp_remove_all_nodes(&self.sgw_list);
    gtp_remove_all_nodes(&self.pgw_list);
    gtp_node_final();

    sock_remove_all_nodes(&self.s1ap_list);
    sock_remove_all_nodes(&self.s1ap_list6);
    sock_remove_all_nodes(&self.gtpc_list);
    sock_remove_all_nodes(&self.gtpc_list6);

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
    self.fd_config->cnf_port = DIAMETER_PORT;
    self.fd_config->cnf_port_tls = DIAMETER_SECURE_PORT;

    return CORE_OK;
}

static status_t mme_context_validation()
{
    if (self.fd_conf_path == NULL &&
        (self.fd_config->cnf_diamid == NULL ||
        self.fd_config->cnf_diamrlm == NULL ||
        self.fd_config->cnf_addr == NULL))
    {
        d_error("No mme.freeDiameter in '%s'", context_self()->config.path);
        return CORE_ERROR;
    }

    if (list_first(&self.s1ap_list) == NULL &&
        list_first(&self.s1ap_list6) == NULL)
    {
        d_error("No mme.s1ap in '%s'", context_self()->config.path);
        return CORE_EAGAIN;
    }

    if (list_first(&self.gtpc_list) == NULL &&
        list_first(&self.gtpc_list6) == NULL)
    {
        d_error("No mme.gtpc in '%s'", context_self()->config.path);
        return CORE_EAGAIN;
    }

    if (list_first(&self.sgw_list) == NULL)
    {
        d_error("No sgw.gtpc in '%s'", context_self()->config.path);
        return CORE_ERROR;
    }

    if (list_first(&self.pgw_list) == NULL)
    {
        d_error("No pgw.gtpc in '%s'", context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.max_num_of_served_gummei == 0)
    {
        d_error("No mme.gummei in '%s'", context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_plmn_id == 0)
    {
        d_error("No mme.gummei.plmn_id in '%s'", context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_mme_gid == 0)
    {
        d_error("No mme.gummei.mme_gid in '%s'", context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_mme_code == 0)
    {
        d_error("No mme.gummei.mme_code in '%s'", context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.num_of_served_tai == 0)
    {
        d_error("No mme.tai in '%s'", context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_tai[0].list0.tai[0].num == 0 &&
        self.served_tai[0].list2.num == 0)
    {
        d_error("No mme.tai.plmn_id|tac in '%s'", context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.num_of_integrity_order == 0)
    {
        d_error("No mme.security.integrity_order in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.num_of_ciphering_order == 0)
    {
        d_error("no mme.security.ciphering_order in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t mme_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;
    yaml_document_t *document = NULL;
    yaml_iter_t root_iter;

    d_assert(config, return CORE_ERROR,);
    document = config->document;
    d_assert(document, return CORE_ERROR,);

    rv = mme_context_prepare();
    if (rv != CORE_OK) return rv;

    yaml_iter_init(&root_iter, document);
    while(yaml_iter_next(&root_iter))
    {
        const char *root_key = yaml_iter_key(&root_iter);
        d_assert(root_key, return CORE_ERROR,);
        if (!strcmp(root_key, "mme"))
        {
            yaml_iter_t mme_iter;
            yaml_iter_recurse(&root_iter, &mme_iter);
            while(yaml_iter_next(&mme_iter))
            {
                const char *mme_key = yaml_iter_key(&mme_iter);
                d_assert(mme_key, return CORE_ERROR,);
                if (!strcmp(mme_key, "freeDiameter"))
                {
                    yaml_node_t *node = 
                        yaml_document_get_node(document, mme_iter.pair->value);
                    d_assert(node, return CORE_ERROR,);
                    if (node->type == YAML_SCALAR_NODE)
                    {
                        self.fd_conf_path = yaml_iter_value(&mme_iter);
                    }
                    else if (node->type == YAML_MAPPING_NODE)
                    {
                        yaml_iter_t fd_iter;
                        yaml_iter_recurse(&mme_iter, &fd_iter);

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
                else if (!strcmp(mme_key, "relative_capacity"))
                {
                    const char *v = yaml_iter_value(&mme_iter);
                    if (v) self.relative_capacity = atoi(v);
                }
                else if (!strcmp(mme_key, "s1ap"))
                {
                    yaml_iter_t s1ap_array, s1ap_iter;
                    yaml_iter_recurse(&mme_iter, &s1ap_array);
                    do
                    {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        c_uint16_t port = self.s1ap_port;
                        const char *dev = NULL;
                        c_sockaddr_t *list = NULL;
                        sock_node_t *node = NULL;

                        if (yaml_iter_type(&s1ap_array) == YAML_MAPPING_NODE)
                        {
                            memcpy(&s1ap_iter, &s1ap_array,
                                    sizeof(yaml_iter_t));
                        }
                        else if (yaml_iter_type(&s1ap_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!yaml_iter_next(&s1ap_array))
                                break;
                            yaml_iter_recurse(&s1ap_array, &s1ap_iter);
                        }
                        else if (yaml_iter_type(&s1ap_array) ==
                            YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            d_assert(0, return CORE_ERROR,);

                        while(yaml_iter_next(&s1ap_iter))
                        {
                            const char *s1ap_key =
                                yaml_iter_key(&s1ap_iter);
                            d_assert(s1ap_key,
                                    return CORE_ERROR,);
                            if (!strcmp(s1ap_key, "family"))
                            {
                                const char *v = yaml_iter_value(&s1ap_iter);
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
                            else if (!strcmp(s1ap_key, "addr") ||
                                    !strcmp(s1ap_key, "name"))
                            {
                                yaml_iter_t hostname_iter;
                                yaml_iter_recurse(&s1ap_iter, &hostname_iter);
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
                            else if (!strcmp(s1ap_key, "port"))
                            {
                                const char *v = yaml_iter_value(&s1ap_iter);
                                if (v)
                                {
                                    port = atoi(v);
                                    self.s1ap_port = port;
                                }
                            }
                            else if (!strcmp(s1ap_key, "dev"))
                            {
                                dev = yaml_iter_value(&s1ap_iter);
                            }
                            else
                                d_warn("unknown key `%s`", s1ap_key);
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
                                rv = sock_add_node(&self.s1ap_list,
                                        &node, list, AF_INET);
                                d_assert(rv == CORE_OK, return CORE_ERROR,);
                            }

                            if (context_self()->parameter.no_ipv6 == 0)
                            {
                                rv = sock_add_node(&self.s1ap_list6,
                                        &node, list, AF_INET6);
                                d_assert(rv == CORE_OK, return CORE_ERROR,);
                            }

                            core_freeaddrinfo(list);
                        }

                        if (dev)
                        {
                            rv = sock_probe_node(
                                    context_self()->parameter.no_ipv4 ?
                                        NULL : &self.s1ap_list,
                                    context_self()->parameter.no_ipv6 ?
                                        NULL : &self.s1ap_list6,
                                    dev, self.s1ap_port);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }

                    } while(yaml_iter_type(&s1ap_array) == YAML_SEQUENCE_NODE);

                    if (list_first(&self.s1ap_list) == NULL &&
                        list_first(&self.s1ap_list6) == NULL)
                    {
                        rv = sock_probe_node(
                                context_self()->parameter.no_ipv4 ?
                                    NULL : &self.s1ap_list,
                                context_self()->parameter.no_ipv6 ?
                                    NULL : &self.s1ap_list6,
                                NULL, self.s1ap_port);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
                }
                else if (!strcmp(mme_key, "gtpc"))
                {
                    yaml_iter_t gtpc_array, gtpc_iter;
                    yaml_iter_recurse(&mme_iter, &gtpc_array);
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
                else if (!strcmp(mme_key, "gummei"))
                {
                    yaml_iter_t gummei_array, gummei_iter;
                    yaml_iter_recurse(&mme_iter, &gummei_array);
                    do
                    {
                        served_gummei_t *gummei = NULL;
                        d_assert(self.max_num_of_served_gummei <=
                                MAX_NUM_OF_SERVED_GUMMEI, return CORE_ERROR,);
                        gummei = &self.served_gummei[
                            self.max_num_of_served_gummei];
                        d_assert(gummei, return CORE_ERROR,);

                        if (yaml_iter_type(&gummei_array) ==
                                YAML_MAPPING_NODE)
                        {
                            memcpy(&gummei_iter, &gummei_array,
                                    sizeof(yaml_iter_t));
                        }
                        else if (yaml_iter_type(&gummei_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!yaml_iter_next(&gummei_array))
                                break;
                            yaml_iter_recurse(&gummei_array,
                                    &gummei_iter);
                        }
                        else if (yaml_iter_type(&gummei_array) ==
                            YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            d_assert(0, return CORE_ERROR,);

                        while(yaml_iter_next(&gummei_iter))
                        {
                            const char *gummei_key =
                                yaml_iter_key(&gummei_iter);
                            d_assert(gummei_key,
                                    return CORE_ERROR,);
                            if (!strcmp(gummei_key, "plmn_id"))
                            {
                                yaml_iter_t plmn_id_array, plmn_id_iter;
                                yaml_iter_recurse(&gummei_iter, &plmn_id_array);
                                do
                                {
                                    plmn_id_t *plmn_id = NULL;
                                    const char *mcc = NULL, *mnc = NULL;
                                    d_assert(gummei->num_of_plmn_id <=
                                            MAX_PLMN_ID, return CORE_ERROR,);
                                    plmn_id = &gummei->plmn_id[
                                        gummei->num_of_plmn_id];
                                    d_assert(plmn_id, return CORE_ERROR,);

                                    if (yaml_iter_type(&plmn_id_array) ==
                                            YAML_MAPPING_NODE)
                                    {
                                        memcpy(&plmn_id_iter, &plmn_id_array,
                                                sizeof(yaml_iter_t));
                                    }
                                    else if (yaml_iter_type(&plmn_id_array) ==
                                        YAML_SEQUENCE_NODE)
                                    {
                                        if (!yaml_iter_next(&plmn_id_array))
                                            break;
                                        yaml_iter_recurse(&plmn_id_array,
                                                &plmn_id_iter);
                                    }
                                    else if (yaml_iter_type(&plmn_id_array) ==
                                        YAML_SCALAR_NODE)
                                    {
                                        break;
                                    }
                                    else
                                        d_assert(0, return CORE_ERROR,);

                                    while(yaml_iter_next(&plmn_id_iter))
                                    {
                                        const char *plmn_id_key =
                                            yaml_iter_key(&plmn_id_iter);
                                        d_assert(plmn_id_key,
                                                return CORE_ERROR,);
                                        if (!strcmp(plmn_id_key, "mcc"))
                                        {
                                            mcc =
                                                yaml_iter_value(&plmn_id_iter);
                                        }
                                        else if (!strcmp(plmn_id_key, "mnc"))
                                        {
                                            mnc =
                                                yaml_iter_value(&plmn_id_iter);
                                        }
                                    }

                                    if (mcc && mnc)
                                    {
                                        plmn_id_build(plmn_id,
                                            atoi(mcc), atoi(mnc), strlen(mnc));
                                        gummei->num_of_plmn_id++;
                                    }

                                } while(yaml_iter_type(&plmn_id_array) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(gummei_key, "mme_gid"))
                            {
                                yaml_iter_t mme_gid_iter;
                                yaml_iter_recurse(&gummei_iter, &mme_gid_iter);
                                d_assert(yaml_iter_type(&mme_gid_iter) !=
                                    YAML_MAPPING_NODE, return CORE_ERROR,);

                                do
                                {
                                    c_uint16_t *mme_gid = NULL;
                                    const char *v = NULL;

                                    d_assert(gummei->num_of_mme_gid <=
                                            GRP_PER_MME, return CORE_ERROR,);
                                    mme_gid = &gummei->mme_gid[
                                        gummei->num_of_mme_gid];
                                    d_assert(mme_gid, return CORE_ERROR,);

                                    if (yaml_iter_type(&mme_gid_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!yaml_iter_next(&mme_gid_iter))
                                            break;
                                    }

                                    v = yaml_iter_value(&mme_gid_iter);
                                    if (v) 
                                    {
                                        *mme_gid = atoi(v);
                                        gummei->num_of_mme_gid++;
                                    }
                                } while(
                                    yaml_iter_type(&mme_gid_iter) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(gummei_key, "mme_code"))
                            {
                                yaml_iter_t mme_code_iter;
                                yaml_iter_recurse(&gummei_iter, &mme_code_iter);
                                d_assert(yaml_iter_type(&mme_code_iter) !=
                                    YAML_MAPPING_NODE, return CORE_ERROR,);

                                do
                                {
                                    c_uint8_t *mme_code = NULL;
                                    const char *v = NULL;

                                    d_assert(gummei->num_of_mme_code <=
                                            CODE_PER_MME, return CORE_ERROR,);
                                    mme_code = &gummei->mme_code[
                                        gummei->num_of_mme_code];
                                    d_assert(mme_code, return CORE_ERROR,);

                                    if (yaml_iter_type(&mme_code_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!yaml_iter_next(&mme_code_iter))
                                            break;
                                    }

                                    v = yaml_iter_value(&mme_code_iter);
                                    if (v) 
                                    {
                                        *mme_code = atoi(v);
                                        gummei->num_of_mme_code++;
                                    }
                                } while(
                                    yaml_iter_type(&mme_code_iter) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else
                                d_warn("unknown key `%s`", gummei_key);
                        }

                        if (gummei->num_of_plmn_id &&
                            gummei->num_of_mme_gid && gummei->num_of_mme_code)
                        {
                            self.max_num_of_served_gummei++;
                        }
                        else
                        {
                            d_warn("Ignore gummei : "
                                    "plmn_id(%d), mme_gid(%d), mme_code(%d)",
                                gummei->num_of_plmn_id,
                                gummei->num_of_mme_gid, gummei->num_of_mme_code);
                            gummei->num_of_plmn_id = 0;
                            gummei->num_of_mme_gid = 0;
                            gummei->num_of_mme_code = 0;
                        }
                    } while(yaml_iter_type(&gummei_array) ==
                            YAML_SEQUENCE_NODE);
                }
                else if (!strcmp(mme_key, "tai"))
                {
                    int num_of_list0 = 0;
                    tai0_list_t *list0 = NULL;
                    tai2_list_t *list2 = NULL;

                    d_assert(self.num_of_served_tai <=
                            MAX_NUM_OF_SERVED_TAI, return CORE_ERROR,);
                    list0 = &self.served_tai[self.num_of_served_tai].list0;
                    d_assert(list0, return CORE_ERROR,);
                    list2 = &self.served_tai[self.num_of_served_tai].list2;
                    d_assert(list2, return CORE_ERROR,);

                    yaml_iter_t tai_array, tai_iter;
                    yaml_iter_recurse(&mme_iter, &tai_array);
                    do
                    {
                        const char *mcc = NULL, *mnc = NULL;
                        c_uint16_t tac[MAX_NUM_OF_TAI];
                        int num_of_tac = 0;

                        if (yaml_iter_type(&tai_array) == YAML_MAPPING_NODE)
                        {
                            memcpy(&tai_iter, &tai_array, sizeof(yaml_iter_t));
                        }
                        else if (yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!yaml_iter_next(&tai_array))
                                break;
                            yaml_iter_recurse(&tai_array,
                                    &tai_iter);
                        }
                        else if (yaml_iter_type(&tai_array) == YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            d_assert(0, return CORE_ERROR,);

                        while(yaml_iter_next(&tai_iter))
                        {
                            const char *tai_key = yaml_iter_key(&tai_iter);
                            d_assert(tai_key, return CORE_ERROR,);
                            if (!strcmp(tai_key, "plmn_id"))
                            {
                                yaml_iter_t plmn_id_iter;

                                yaml_iter_recurse(&tai_iter, &plmn_id_iter);
                                while(yaml_iter_next(&plmn_id_iter))
                                {
                                    const char *plmn_id_key =
                                        yaml_iter_key(&plmn_id_iter);
                                    d_assert(plmn_id_key,
                                            return CORE_ERROR,);
                                    if (!strcmp(plmn_id_key, "mcc"))
                                    {
                                        mcc = yaml_iter_value(&plmn_id_iter);
                                    }
                                    else if (!strcmp(plmn_id_key, "mnc"))
                                    {
                                        mnc = yaml_iter_value(&plmn_id_iter);
                                    }
                                }
                            }
                            else if (!strcmp(tai_key, "tac"))
                            {
                                yaml_iter_t tac_iter;
                                yaml_iter_recurse(&tai_iter, &tac_iter);
                                d_assert(yaml_iter_type(&tac_iter) !=
                                    YAML_MAPPING_NODE, return CORE_ERROR,);

                                do
                                {
                                    const char *v = NULL;

                                    d_assert(num_of_tac <=
                                            MAX_NUM_OF_TAI, return CORE_ERROR,);
                                    if (yaml_iter_type(&tac_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!yaml_iter_next(&tac_iter))
                                            break;
                                    }

                                    v = yaml_iter_value(&tac_iter);
                                    if (v) 
                                    {
                                        tac[num_of_tac] = atoi(v);
                                        num_of_tac++;
                                    }
                                } while(
                                    yaml_iter_type(&tac_iter) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else
                                d_warn("unknown key `%s`", tai_key);
                        }

                        if (mcc && mnc && num_of_tac)
                        {
                            if (num_of_tac == 1)
                            {
                                plmn_id_build(
                                    &list2->tai[list2->num].plmn_id,
                                    atoi(mcc), atoi(mnc), strlen(mnc));
                                list2->tai[list2->num].tac = tac[0];

                                list2->num++;
                                if (list2->num > 1)
                                    list2->type = TAI2_TYPE;
                                else
                                    list2->type = TAI1_TYPE;
                            }
                            else if (num_of_tac > 1)
                            {
                                int i;
                                plmn_id_build(
                                    &list0->tai[num_of_list0].plmn_id,
                                    atoi(mcc), atoi(mnc), strlen(mnc));
                                for (i = 0; i < num_of_tac; i++)
                                {
                                    list0->tai[num_of_list0].tac[i] = tac[i];
                                }

                                list0->tai[num_of_list0].num = num_of_tac;
                                list0->tai[num_of_list0].type = TAI0_TYPE;

                                num_of_list0++;
                            }
                        }
                        else
                        {
                            d_warn("Ignore tai : mcc(%p), mnc(%p), "
                                    "num_of_tac(%d)", mcc, mnc, num_of_tac);
                        }
                    } while(yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE);

                    if (list2->num || num_of_list0)
                    {
                        self.num_of_served_tai++;
                    }
                }
                else if (!strcmp(mme_key, "security"))
                {
                    yaml_iter_t security_iter;
                    yaml_iter_recurse(&mme_iter, &security_iter);
                    while(yaml_iter_next(&security_iter))
                    {
                        const char *security_key =
                            yaml_iter_key(&security_iter);
                        d_assert(security_key, return CORE_ERROR,);
                        if (!strcmp(security_key, "integrity_order"))
                        {
                            yaml_iter_t integrity_order_iter;
                            yaml_iter_recurse(&security_iter,
                                    &integrity_order_iter);
                            d_assert(yaml_iter_type(&integrity_order_iter) !=
                                YAML_MAPPING_NODE, return CORE_ERROR,);

                            do
                            {
                                const char *v = NULL;

                                if (yaml_iter_type(&integrity_order_iter) ==
                                        YAML_SEQUENCE_NODE)
                                {
                                    if (!yaml_iter_next(&integrity_order_iter))
                                        break;
                                }

                                v = yaml_iter_value(&integrity_order_iter);
                                if (v) 
                                {
                                    int integrity_index = 
                                        self.num_of_integrity_order;
                                    if (strcmp(v, "EIA0") == 0)
                                    {
                                        self.integrity_order[integrity_index] = 
                                            NAS_SECURITY_ALGORITHMS_EIA0;
                                        self.num_of_integrity_order++;
                                    }
                                    else if (strcmp(v, "EIA1") == 0)
                                    {
                                        self.integrity_order[integrity_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EIA1;
                                        self.num_of_integrity_order++;
                                    }
                                    else if (strcmp(v, "EIA2") == 0)
                                    {
                                        self.integrity_order[integrity_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EIA2;
                                        self.num_of_integrity_order++;
                                    }
                                    else if (strcmp(v, "EIA3") == 0)
                                    {
                                        self.integrity_order[integrity_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EIA3;
                                        self.num_of_integrity_order++;
                                    }
                                }
                            } while(
                                yaml_iter_type(&integrity_order_iter) ==
                                    YAML_SEQUENCE_NODE);
                        }
                        else if (!strcmp(security_key, "ciphering_order"))
                        {
                            yaml_iter_t ciphering_order_iter;
                            yaml_iter_recurse(&security_iter,
                                    &ciphering_order_iter);
                            d_assert(yaml_iter_type(&ciphering_order_iter) !=
                                YAML_MAPPING_NODE, return CORE_ERROR,);

                            do
                            {
                                const char *v = NULL;

                                if (yaml_iter_type(&ciphering_order_iter) ==
                                        YAML_SEQUENCE_NODE)
                                {
                                    if (!yaml_iter_next(&ciphering_order_iter))
                                        break;
                                }

                                v = yaml_iter_value(&ciphering_order_iter);
                                if (v) 
                                {
                                    int ciphering_index = 
                                        self.num_of_ciphering_order;
                                    if (strcmp(v, "EEA0") == 0)
                                    {
                                        self.ciphering_order[ciphering_index] = 
                                            NAS_SECURITY_ALGORITHMS_EEA0;
                                        self.num_of_ciphering_order++;
                                    }
                                    else if (strcmp(v, "EEA1") == 0)
                                    {
                                        self.ciphering_order[ciphering_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EEA1;
                                        self.num_of_ciphering_order++;
                                    }
                                    else if (strcmp(v, "EEA2") == 0)
                                    {
                                        self.ciphering_order[ciphering_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EEA2;
                                        self.num_of_ciphering_order++;
                                    }
                                    else if (strcmp(v, "EEA3") == 0)
                                    {
                                        self.ciphering_order[ciphering_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EEA3;
                                        self.num_of_ciphering_order++;
                                    }
                                }
                            } while(
                                yaml_iter_type(&ciphering_order_iter) ==
                                    YAML_SEQUENCE_NODE);
                        }
                    }
                }
                else if(!strcmp(mme_key, "network_name"))
                {
                    yaml_iter_t network_name_iter;
                    yaml_iter_recurse(&mme_iter, &network_name_iter);

                    while(yaml_iter_next(&network_name_iter))
                    {
                        const char *network_name_key =
                        yaml_iter_key(&network_name_iter);
                        d_assert(network_name_key,
                                    return CORE_ERROR,);
                        if (!strcmp(network_name_key, "full"))
                        {  
                            nas_network_name_t *network_full_name =
                                &self.full_name;
                            const char *c_network_name =
                                yaml_iter_value(&network_name_iter);
                            c_uint8_t size = strlen(c_network_name);
                            c_uint8_t i;
                            for(i = 0;i<size;i++)
                            {
                                /* Workaround to convert the ASCII to USC-2 */
                                network_full_name->name[i*2] = 0;
                                network_full_name->name[(i*2)+1] =
                                    c_network_name[i];

                            }
                                network_full_name->length = size*2+1;
                                network_full_name->coding_scheme = 1;
                        }
                        else if (!strcmp(network_name_key, "short"))
                        {
                            nas_network_name_t *network_short_name =
                                &self.short_name;
                            const char *c_network_name =
                                yaml_iter_value(&network_name_iter);
                            c_uint8_t size = strlen(c_network_name);
                            c_uint8_t i;
                            for(i = 0;i<size;i++)
                            {
                                /* Workaround to convert the ASCII to USC-2 */
                                network_short_name->name[i*2] = 0;
                                network_short_name->name[(i*2)+1] =
                                    c_network_name[i];

                            }
                            network_short_name->length = size*2+1;
                            network_short_name->coding_scheme = 1;
                        }
                    }
                }
                else
                    d_warn("unknown key `%s`", mme_key);
            }
        }
        else if (!strcmp(root_key, "sgw"))
        {
            yaml_iter_t mme_iter;
            yaml_iter_recurse(&root_iter, &mme_iter);
            while(yaml_iter_next(&mme_iter))
            {
                const char *mme_key = yaml_iter_key(&mme_iter);
                d_assert(mme_key, return CORE_ERROR,);
                if (!strcmp(mme_key, "gtpc"))
                {
                    yaml_iter_t gtpc_array, gtpc_iter;
                    yaml_iter_recurse(&mme_iter, &gtpc_array);
                    do
                    {
                        gtp_node_t *sgw = NULL;
                        c_sockaddr_t *list = NULL;
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        c_uint16_t port = self.gtpc_port;

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
                                if (v) port = atoi(v);
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

                        rv = gtp_add_node(&self.sgw_list, &sgw, list,
                                context_self()->parameter.no_ipv4,
                                context_self()->parameter.no_ipv6,
                                context_self()->parameter.prefer_ipv4);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);

                        core_freeaddrinfo(list);

                    } while(yaml_iter_type(&gtpc_array) == YAML_SEQUENCE_NODE);
                }
            }
        }
        else if (!strcmp(root_key, "pgw"))
        {
            yaml_iter_t mme_iter;
            yaml_iter_recurse(&root_iter, &mme_iter);
            while(yaml_iter_next(&mme_iter))
            {
                const char *mme_key = yaml_iter_key(&mme_iter);
                d_assert(mme_key, return CORE_ERROR,);
                if (!strcmp(mme_key, "gtpc"))
                {
                    yaml_iter_t gtpc_array, gtpc_iter;
                    yaml_iter_recurse(&mme_iter, &gtpc_array);
                    do
                    {
                        gtp_node_t *pgw = NULL;
                        c_sockaddr_t *list = NULL;
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        c_uint16_t port = self.gtpc_port;

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
                                if (v) port = atoi(v);
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

                        rv = gtp_add_node(&self.pgw_list, &pgw, list,
                                context_self()->parameter.no_ipv4,
                                context_self()->parameter.no_ipv6,
                                context_self()->parameter.prefer_ipv4);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);

                        core_freeaddrinfo(list);

                    } while(yaml_iter_type(&gtpc_array) == YAML_SEQUENCE_NODE);
                }
            }
        }
    }

    rv = mme_context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

status_t mme_context_setup_trace_module()
{
    int app = context_self()->logger.trace.app;
    int s1ap = context_self()->logger.trace.s1ap;
    int nas = context_self()->logger.trace.nas;
    int diameter = context_self()->logger.trace.diameter;
    int gtpv2 = context_self()->logger.trace.gtpv2;

    if (app)
    {
        extern int _mme_context;
        d_trace_level(&_mme_context, app);
        extern int _mme_sm;
        d_trace_level(&_mme_sm, app);
    }

    if (s1ap)
    {
        extern int _s1ap_sm;
        d_trace_level(&_s1ap_sm, s1ap);
        extern int _s1ap_build;
        d_trace_level(&_s1ap_build, s1ap);
        extern int _s1ap_conv;
        d_trace_level(&_s1ap_conv, s1ap);
        extern int _s1ap_handler;
        d_trace_level(&_s1ap_handler, s1ap);
        extern int _s1ap_sctp;
        d_trace_level(&_s1ap_sctp, s1ap);
        extern int _s1ap_path;
        d_trace_level(&_s1ap_path, s1ap);
        extern int _s1ap_decoder;
        d_trace_level(&_s1ap_decoder, s1ap);
        extern int _s1ap_encoder;
        d_trace_level(&_s1ap_encoder, s1ap);
    }

    if (nas)
    {
        extern int _emm_sm;
        d_trace_level(&_emm_sm, nas);
        extern int _esm_sm;
        d_trace_level(&_esm_sm, nas);
        extern int _emm_build;
        d_trace_level(&_emm_build, nas);
        extern int _esm_build;
        d_trace_level(&_esm_build, nas);
        extern int _emm_handler;
        d_trace_level(&_emm_handler, nas);
        extern int _esm_handler;
        d_trace_level(&_esm_handler, nas);
        extern int _nas_path;
        d_trace_level(&_nas_path, nas);
        extern int _nas_decoder;
        d_trace_level(&_nas_decoder, nas);
        extern int _nas_encoder;
        d_trace_level(&_nas_encoder, nas);
        extern int _nas_ies;
        d_trace_level(&_nas_ies, nas);
    }

    if (diameter)
    {
        extern int _mme_fd_path;
        d_trace_level(&_mme_fd_path, diameter);
        extern int _fd_init;
        d_trace_level(&_fd_init, diameter);
        extern int _fd_logger;
        d_trace_level(&_fd_logger, diameter);
    }

    if (gtpv2)
    {
        extern int _mme_s11_handler;
        d_trace_level(&_mme_s11_handler, gtpv2);
        extern int _mme_gtp_path;
        d_trace_level(&_mme_gtp_path, gtpv2);

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

    return CORE_OK;
}

mme_enb_t* mme_enb_add(sock_id sock, c_sockaddr_t *addr)
{
    mme_enb_t *enb = NULL;
    event_t e;

    d_assert(sock, return NULL,);
    d_assert(addr, return NULL,);

    index_alloc(&mme_enb_pool, &enb);
    d_assert(enb, return NULL, "Null param");

    enb->sock = sock;
    enb->addr = addr;
    enb->sock_type = mme_enb_sock_type(enb->sock);

    enb->outbound_streams = context_self()->parameter.sctp_streams;

    list_init(&enb->enb_ue_list);

    hash_set(self.enb_sock_hash, &enb->sock, sizeof(enb->sock), enb);
    hash_set(self.enb_addr_hash, enb->addr, sizeof(c_sockaddr_t), enb);
    
    event_set_param1(&e, (c_uintptr_t)enb->index);
    fsm_create(&enb->sm, s1ap_state_initial, s1ap_state_final);
    fsm_init(&enb->sm, &e);

    return enb;
}

status_t mme_enb_remove(mme_enb_t *enb)
{
    event_t e;

    d_assert(enb, return CORE_ERROR, "Null param");
    d_assert(enb->sock, return CORE_ERROR, "Null param");

    event_set_param1(&e, (c_uintptr_t)enb->index);
    fsm_final(&enb->sm, &e);
    fsm_clear(&enb->sm);

    hash_set(self.enb_sock_hash, &enb->sock, sizeof(enb->sock), NULL);
    hash_set(self.enb_addr_hash, enb->addr, sizeof(c_sockaddr_t), NULL);
    if (enb->enb_id)
        hash_set(self.enb_id_hash, &enb->enb_id, sizeof(enb->enb_id), NULL);

    enb_ue_remove_in_enb(enb);

    CORE_FREE(enb->addr);

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

        if (enb->sock_type == SOCK_STREAM)
            s1ap_delete(enb->sock);

        mme_enb_remove(enb);
    }

    return CORE_OK;
}

mme_enb_t* mme_enb_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&mme_enb_pool, index);
}

mme_enb_t* mme_enb_find_by_sock(sock_id sock)
{
    d_assert(sock, return NULL,"Invalid param");
    return (mme_enb_t *)hash_get(self.enb_sock_hash, &sock, sizeof(sock));

    return NULL;
}

mme_enb_t* mme_enb_find_by_addr(c_sockaddr_t *addr)
{
    d_assert(addr, return NULL,"Invalid param");
    return (mme_enb_t *)hash_get(self.enb_addr_hash,
            addr, sizeof(c_sockaddr_t));

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
    d_assert(self.enb_sock_hash, return NULL, "Null param");
    return hash_first(self.enb_sock_hash);
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

int mme_enb_sock_type(sock_id sock)
{
    sock_node_t *snode = NULL;

    d_assert(sock, return SOCK_STREAM,);

    for (snode = list_first(&mme_self()->s1ap_list);
            snode; snode = list_next(snode))
    {
        if (snode->sock == sock) return SOCK_SEQPACKET;
    }
    for (snode = list_first(&mme_self()->s1ap_list6);
            snode; snode = list_next(snode))
    {
        if (snode->sock == sock) return SOCK_SEQPACKET;
    }

    return SOCK_STREAM;
}

/** enb_ue_context handling function */
enb_ue_t* enb_ue_add(mme_enb_t *enb)
{
    enb_ue_t *enb_ue = NULL;

    d_assert(self.mme_ue_s1ap_id_hash, return NULL, "Null param");
    d_assert(enb, return NULL, "Null param");

    index_alloc(&enb_ue_pool, &enb_ue);
    d_assert(enb_ue, return NULL, "Null param");

    enb_ue->enb_ue_s1ap_id = INVALID_UE_S1AP_ID;
    enb_ue->mme_ue_s1ap_id = NEXT_ID(self.mme_ue_s1ap_id, 1, 0xffffffff);
    enb_ue->enb = enb;

    hash_set(self.mme_ue_s1ap_id_hash, &enb_ue->mme_ue_s1ap_id, 
            sizeof(enb_ue->mme_ue_s1ap_id), enb_ue);
    list_append(&enb->enb_ue_list, enb_ue);

    /* Create S1 holding timer */
    enb_ue->holding_timer = timer_create(&self.tm_service,
            MME_EVT_S1AP_S1_HOLDING_TIMER, self.s1_holding_timer_value * 1000);
    d_assert(enb_ue->holding_timer, return NULL, "Null param");
    timer_set_param1(enb_ue->holding_timer, enb_ue->index);

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

    /* De-associate S1 with NAS/EMM */
    rv = enb_ue_deassociate(enb_ue);
    d_assert(rv == CORE_OK,,);

   /* Delete All Timers */
    tm_delete(enb_ue->holding_timer);

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

    if (mme_ue->m_tmsi)
    {
        /* MME has a VALID GUIT
         * As such, we need to remove previous GUTI in hash table */
        hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), NULL);
        d_assert(mme_m_tmsi_free(mme_ue->m_tmsi) == CORE_OK,,);
    }

    memset(&mme_ue->guti, 0, sizeof(guti_t));

    /* Use the first configured plmn_id and mme group id */
    memcpy(&mme_ue->guti.plmn_id, &served_gummei->plmn_id[0], PLMN_ID_LEN);
    mme_ue->guti.mme_gid = served_gummei->mme_gid[0];
    mme_ue->guti.mme_code = served_gummei->mme_code[0];

    mme_ue->m_tmsi = mme_m_tmsi_alloc();
    d_assert(mme_ue->m_tmsi, return CORE_ERROR,);
    mme_ue->guti.m_tmsi = *(mme_ue->m_tmsi);
    hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), mme_ue);

    return CORE_OK;
}

mme_ue_t* mme_ue_add(enb_ue_t *enb_ue)
{
    mme_enb_t *enb = NULL;
    mme_ue_t *mme_ue = NULL;
    event_t e;

    d_assert(enb_ue, return NULL,);
    enb = enb_ue->enb;
    d_assert(enb, return NULL,);

    index_alloc(&mme_ue_pool, &mme_ue);
    d_assert(mme_ue, return NULL, "Null param");

    list_init(&mme_ue->sess_list);

    mme_ue->mme_s11_teid = mme_ue->index;

    /*
     * SCTP output stream identification
     * Default context_self()->parameter.sctp_streams : 30
     *   0 : Non UE signalling
     *   1-29 : UE specific association 
     */
    mme_ue->ostream_id = NEXT_ID(self.ostream_id, 1, enb->outbound_streams-1);

    /* Create New GUTI */
    mme_ue_new_guti(mme_ue);

    /* Setup SGW with round-robin manner */
    if (mme_self()->sgw == NULL)
        mme_self()->sgw = list_first(&mme_self()->sgw_list);

    SETUP_GTP_NODE(mme_ue, mme_self()->sgw);

    mme_self()->sgw = list_next(mme_self()->sgw);

    /* Create paging retry timer */
    mme_ue->t3413 = timer_create(&self.tm_service, MME_EVT_EMM_T3413,
            self.t3413_value * 1000);
    d_assert(mme_ue->t3413, return NULL, "Null param");
    timer_set_param1(mme_ue->t3413, mme_ue->index);

    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    fsm_create(&mme_ue->sm, emm_state_initial, emm_state_final);
    fsm_init(&mme_ue->sm, &e);

    return mme_ue;
}

status_t mme_ue_remove(mme_ue_t *mme_ue)
{
    status_t rv;
    event_t e;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    fsm_final(&mme_ue->sm, &e);
    fsm_clear(&mme_ue->sm);

    /* Clear hash table */
    if (mme_ue->m_tmsi)
    {
        hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), NULL);
        d_assert(mme_m_tmsi_free(mme_ue->m_tmsi) == CORE_OK,,);
    }
    if (mme_ue->imsi_len != 0)
        hash_set(self.imsi_ue_hash, mme_ue->imsi, mme_ue->imsi_len, NULL);
    
    /* Clear the saved PDN Connectivity Request */
    NAS_CLEAR_DATA(&mme_ue->pdn_connectivity_request);

    /* Clear Paging info : stop t3413, last_paing_msg */
    CLEAR_PAGING_INFO(mme_ue);

    /* Free UeRadioCapability */
#if 0
    if (mme_ue->radio_capa)
    {
        S1AP_UERadioCapability_t *radio_capa = 
            (S1AP_UERadioCapability_t *)mme_ue->radio_capa;

        if (radio_capa->buf)
            CORE_FREE(radio_capa->buf);
        CORE_FREE(mme_ue->radio_capa);
    }
#else
    S1AP_CLEAR_DATA(&mme_ue->ueRadioCapability);
#endif

    /* Clear Transparent Container */
    S1AP_CLEAR_DATA(&mme_ue->container);

    /* Delete All Timers */
    tm_delete(mme_ue->t3413);

    rv = mme_ue_deassociate(mme_ue);
    d_assert(rv == CORE_OK,,);

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
                        d_trace(9, "known UE by IMSI[%s]\n", imsi_bcd);
                    }
                    else
                    {
                        d_trace(9, "Unknown UE by IMSI[%s]\n", imsi_bcd);
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
                        d_trace(9, "Known UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]\n",
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
                        d_trace(9, "Known UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]\n",
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

status_t mme_ue_set_imsi(mme_ue_t *mme_ue, c_int8_t *imsi_bcd)
{
    d_assert(mme_ue && imsi_bcd, return CORE_ERROR, "Invalid param");

    core_cpystrn(mme_ue->imsi_bcd, imsi_bcd, MAX_IMSI_BCD_LEN+1);
    core_bcd_to_buffer(mme_ue->imsi_bcd, mme_ue->imsi, &mme_ue->imsi_len);

    hash_set(self.imsi_ue_hash, mme_ue->imsi, mme_ue->imsi_len, mme_ue);

    mme_ue->guti_present = 1;

    return CORE_OK;
}

int mme_ue_have_indirect_tunnel(mme_ue_t *mme_ue)
{
    mme_sess_t *sess = NULL;

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        mme_bearer_t *bearer = mme_bearer_first(sess);
        while(bearer)
        {
            if (MME_HAVE_ENB_DL_INDIRECT_TUNNEL(bearer) ||
                MME_HAVE_ENB_UL_INDIRECT_TUNNEL(bearer) ||
                MME_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer) ||
                MME_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer))
            {
                return 1;
            }

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

    return 0;
}

status_t mme_ue_clear_indirect_tunnel(mme_ue_t *mme_ue)
{
    mme_sess_t *sess = NULL;

    d_assert(mme_ue, return CORE_ERROR,);

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        mme_bearer_t *bearer = mme_bearer_first(sess);
        while(bearer)
        {
            CLEAR_INDIRECT_TUNNEL(bearer);

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

    return CORE_OK;
}

status_t mme_ue_associate_enb_ue(mme_ue_t *mme_ue, enb_ue_t *enb_ue)
{
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    mme_ue->enb_ue = enb_ue;
    enb_ue->mme_ue = mme_ue;

    return CORE_OK;
}

status_t enb_ue_deassociate(enb_ue_t *enb_ue)
{
    d_assert(enb_ue, return CORE_ERROR, "Null param");
    enb_ue->mme_ue = NULL;
    
    return CORE_OK;
}

status_t mme_ue_deassociate(mme_ue_t *mme_ue)
{
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    mme_ue->enb_ue = NULL;
    
    return CORE_OK;
}

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

    sess->mme_ue = mme_ue;
    sess->pti = pti;

    bearer = mme_bearer_add(sess);
    d_assert(bearer, mme_sess_remove(sess); return NULL, 
            "Can't add default bearer context");

    list_append(&mme_ue->sess_list, sess);

    return sess;
}

status_t mme_sess_remove(mme_sess_t *sess)
{
    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(sess->mme_ue, return CORE_ERROR, "Null param");
    
    list_remove(&sess->mme_ue->sess_list, sess);

    mme_bearer_remove_all(sess);

    NAS_CLEAR_DATA(&sess->ue_pco);
    TLV_CLEAR_DATA(&sess->pgw_pco);

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

unsigned int  mme_sess_count(mme_ue_t *mme_ue)
{
    unsigned int count = 0;
    mme_sess_t *sess = NULL;

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        sess = mme_sess_next(sess);
        count++;
    }

    return count;
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

    bearer->mme_ue = mme_ue;
    bearer->sess = sess;

    list_append(&sess->bearer_list, bearer);
    
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

    list_remove(&bearer->sess->bearer_list, bearer);

    TLV_CLEAR_DATA(&bearer->tft);
    
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

    pti = message->esm.h.procedure_transaction_identity;
    ebi = message->esm.h.eps_bearer_identity;

    d_trace(9, "mme_bearer_find_or_add_by_message() [PTI:%d, EBI:%d]\n",
            pti, ebi);

    if (ebi != NAS_EPS_BEARER_IDENTITY_UNASSIGNED)
    {
        bearer = mme_bearer_find_by_ue_ebi(mme_ue, ebi);
        d_assert(bearer, return NULL, "No Bearer : [EBI:%d]", ebi);
        return bearer;
    }
        
    d_assert(pti != NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
            return NULL, "Invalid param : [PTI:%d, EBI:%d]", pti, ebi);

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

        d_assert(sess, return NULL, "No Session : [PTI:%d]", pti);
    }
    else if (message->esm.h.message_type == NAS_PDN_DISCONNECT_REQUEST)
    {
        nas_pdn_disconnect_request_t *pdn_disconnect_request = 
            &message->esm.pdn_disconnect_request;
        nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity =
            &pdn_disconnect_request->linked_eps_bearer_identity;

        bearer = mme_bearer_find_by_ue_ebi(mme_ue,
                linked_eps_bearer_identity->eps_bearer_identity);
        d_assert(bearer, return NULL,
                "No Bearer : [Linked-EBI:%d, PTI:%d, EBI:%d]",
                linked_eps_bearer_identity->eps_bearer_identity,
                pti, ebi);
        sess = bearer->sess;
        d_assert(sess, return NULL, "No Session : [PTI:%d]", pti);
        sess->pti = pti;

        return bearer;
    }
    else
    {
        sess = mme_sess_find_by_pti(mme_ue, pti);
        d_assert(sess, return NULL, "No Session : [PTI:%d]", pti);
    }

    bearer = mme_default_bearer_in_sess(sess);
    d_assert(bearer, return NULL, "No Bearer : [EBI:%d]", ebi);
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

int mme_bearer_is_inactive(mme_ue_t *mme_ue)
{
    mme_sess_t *sess = NULL;
    d_assert(mme_ue, return 1,);

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        mme_bearer_t *bearer = mme_bearer_first(sess);
        while(bearer)
        {
            if (MME_HAVE_ENB_S1U_PATH(bearer))
            {
                return 0;
            }

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

    return 1;
}

status_t mme_bearer_set_inactive(mme_ue_t *mme_ue)
{
    mme_sess_t *sess = NULL;
    d_assert(mme_ue, return CORE_ERROR,);

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        mme_bearer_t *bearer = mme_bearer_first(sess);
        while(bearer)
        {
            CLEAR_ENB_S1U_PATH(bearer);

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

    return CORE_OK;
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

pdn_t* mme_default_pdn(mme_ue_t *mme_ue)
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
        if (pdn->context_identifier == subscription_data->context_identifier)
            return pdn;
    }

    return NULL;
}

int mme_find_served_tai(tai_t *tai)
{
    int i = 0, j = 0, k = 0;

    d_assert(tai, return -1,);

    for (i = 0; i < self.num_of_served_tai; i++)
    {
        tai0_list_t *list0 = &self.served_tai[i].list0;
        d_assert(list0, return -1,);
        tai2_list_t *list2 = &self.served_tai[i].list2;
        d_assert(list2, return -1,);

        for (j = 0; list0->tai[j].num; j++)
        {
            d_assert(list0->tai[j].type == TAI0_TYPE,
                return -1, "type = %d", list0->tai[j].type);
            d_assert(list0->tai[j].num < MAX_NUM_OF_TAI,
                    return -1, "num = %d", list0->tai[j].num);

            for (k = 0; k < list0->tai[j].num; k++) 
            {
                if (memcmp(&list0->tai[j].plmn_id,
                            &tai->plmn_id, PLMN_ID_LEN) == 0 && 
                    list0->tai[j].tac[k] == tai->tac)
                {
                    return i;
                }
            }
        }

        if (list2->num)
        {
            d_assert(list2->type == TAI1_TYPE || list2->type == TAI2_TYPE,
                return -1, "type = %d", list2->type);
            d_assert(list2->num < MAX_NUM_OF_TAI,
                    return -1, "num = %d", list2->num);

            for (j = 0; j < list2->num; j++) 
            {
                if (memcmp(&list2->tai[j].plmn_id,
                            &tai->plmn_id, PLMN_ID_LEN) == 0 && 
                    list2->tai[j].tac == tai->tac)
                {
                    return i;
                }
            }
        }
    }

    return -1;
}

status_t mme_m_tmsi_pool_generate()
{
    status_t rv;
    int i, j;
    int index = 0;

    d_trace(9, "M-TMSI Pool try to generate...\n");
    for (i = 0; index < MAX_POOL_OF_UE; i++)
    {
        mme_m_tmsi_t *m_tmsi = NULL;
        int conflict = 0;

        m_tmsi = &self.m_tmsi.pool[index];
        rv = core_generate_random_bytes((c_uint8_t *)m_tmsi, sizeof(*m_tmsi));
        d_assert(rv == CORE_OK, return CORE_ERROR, "Cannot generate random");

        /* for mapped-GUTI */
        *m_tmsi |= 0xc0000000;
        *m_tmsi &= 0xff00ffff;

        for (j = 0; j < index; j++)
        {
            if (*m_tmsi == self.m_tmsi.pool[j])
            {
                conflict = 1;
                d_trace(11, "[M-TMSI CONFLICT]  %d:0x%x == %d:0x%x\n",
                        index, *m_tmsi, j, self.m_tmsi.pool[j]);
                break;
            }
        }
        if (conflict == 1) continue;

        index++;
    }
    self.m_tmsi.size = index;
    d_trace(9, "M-TMSI Pool generate...done\n");

    return CORE_OK;
}

mme_m_tmsi_t *mme_m_tmsi_alloc()
{
    mme_m_tmsi_t *m_tmsi = NULL;

    pool_alloc_node(&self.m_tmsi, &m_tmsi);
    d_assert(m_tmsi, return NULL,);

    return m_tmsi;
}

status_t mme_m_tmsi_free(mme_m_tmsi_t *m_tmsi)
{
    d_assert(m_tmsi, return CORE_ERROR,);
    pool_free_node(&self.m_tmsi, m_tmsi);

    return CORE_OK;
}


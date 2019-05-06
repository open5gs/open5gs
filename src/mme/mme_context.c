#include <mongoc.h>
#include <yaml.h>

#include "asn1c/s1ap_message.h"
#include "gtp/gtp_xact.h"
#include "gtp/gtp_node.h"
#include "gtp/gtp_path.h"
#include "fd/fd_lib.h"

#include "app/context.h"
#include "nas_conv.h"
#include "mme_context.h"
#include "mme_event.h"
#include "s1ap_path.h"
#include "s1ap_handler.h"
#include "mme_sm.h"

#define MAX_CELL_PER_ENB            8

static mme_context_t self;
static fd_config_t g_fd_conf;

int __mme_log_domain;
int __emm_log_domain;
int __esm_log_domain;

static OGS_POOL(mme_sgw_pool, mme_sgw_t);
static OGS_POOL(mme_pgw_pool, mme_pgw_t);

static OGS_POOL(mme_enb_pool, mme_enb_t);
static OGS_POOL(mme_ue_pool, mme_ue_t);
static OGS_POOL(enb_ue_pool, enb_ue_t);
static OGS_POOL(mme_sess_pool, mme_sess_t);
static OGS_POOL(mme_bearer_pool, mme_bearer_t);

static int context_initialized = 0;

int mme_context_init()
{
    ogs_assert(context_initialized == 0);

    /* Initial FreeDiameter Config */
    memset(&g_fd_conf, 0, sizeof(fd_config_t));

    /* Initialize MME context */
    memset(&self, 0, sizeof(mme_context_t));
    self.fd_config = &g_fd_conf;

    ogs_log_install_domain(&__mme_log_domain, "mme", ogs_core()->log.level);
    ogs_log_install_domain(&__emm_log_domain, "emm", ogs_core()->log.level);
    ogs_log_install_domain(&__esm_log_domain, "esm", ogs_core()->log.level);

    ogs_list_init(&self.s1ap_list);
    ogs_list_init(&self.s1ap_list6);

    ogs_list_init(&self.gtpc_list);
    ogs_list_init(&self.gtpc_list6);

    gtp_node_init();
    ogs_list_init(&self.sgw_list);
    ogs_list_init(&self.pgw_list);

    ogs_pool_init(&mme_sgw_pool, MAX_NUM_OF_SGW);
    ogs_pool_init(&mme_pgw_pool, MAX_NUM_OF_PGW);

    ogs_pool_init(&mme_enb_pool, MAX_NUM_OF_ENB);

    ogs_pool_init(&mme_ue_pool, MAX_POOL_OF_UE);
    ogs_pool_init(&enb_ue_pool, MAX_POOL_OF_UE);
    ogs_pool_init(&mme_sess_pool, MAX_POOL_OF_SESS);
    ogs_pool_init(&mme_bearer_pool, MAX_POOL_OF_BEARER);
    ogs_pool_init(&self.m_tmsi, MAX_POOL_OF_UE);

    self.enb_sock_hash = ogs_hash_make();
    self.enb_addr_hash = ogs_hash_make();
    self.enb_id_hash = ogs_hash_make();
    self.mme_ue_s1ap_id_hash = ogs_hash_make();
    self.imsi_ue_hash = ogs_hash_make();
    self.guti_ue_hash = ogs_hash_make();

    /* Timer value */
    self.t3413_value = ogs_time_from_sec(2); /* Paging retry timer: 2 secs */

    context_initialized = 1;

    return OGS_OK;
}

int mme_context_final()
{
    ogs_assert(context_initialized == 1);

    mme_enb_remove_all();
    mme_ue_remove_all();

    ogs_assert(self.enb_sock_hash);
    ogs_hash_destroy(self.enb_sock_hash);
    ogs_assert(self.enb_addr_hash);
    ogs_hash_destroy(self.enb_addr_hash);
    ogs_assert(self.enb_id_hash);
    ogs_hash_destroy(self.enb_id_hash);

    ogs_assert(self.mme_ue_s1ap_id_hash);
    ogs_hash_destroy(self.mme_ue_s1ap_id_hash);
    ogs_assert(self.imsi_ue_hash);
    ogs_hash_destroy(self.imsi_ue_hash);
    ogs_assert(self.guti_ue_hash);
    ogs_hash_destroy(self.guti_ue_hash);

    ogs_pool_final(&self.m_tmsi);
    ogs_pool_final(&mme_bearer_pool);
    ogs_pool_final(&mme_sess_pool);
    ogs_pool_final(&mme_ue_pool);
    ogs_pool_final(&enb_ue_pool);

    ogs_pool_final(&mme_enb_pool);

    mme_sgw_remove_all();
    mme_pgw_remove_all();

    ogs_pool_final(&mme_sgw_pool);
    ogs_pool_final(&mme_pgw_pool);

    gtp_node_final();

    ogs_sock_remove_all_nodes(&self.s1ap_list);
    ogs_sock_remove_all_nodes(&self.s1ap_list6);
    ogs_sock_remove_all_nodes(&self.gtpc_list);
    ogs_sock_remove_all_nodes(&self.gtpc_list6);

    context_initialized = 0;

    return OGS_OK;
}

mme_context_t* mme_self()
{
    return &self;
}

static int mme_context_prepare()
{
    self.relative_capacity = 0xff;

    self.s1ap_port = S1AP_SCTP_PORT;
    self.gtpc_port = GTPV2_C_UDP_PORT;
    self.fd_config->cnf_port = DIAMETER_PORT;
    self.fd_config->cnf_port_tls = DIAMETER_SECURE_PORT;

    return OGS_OK;
}

static int mme_context_validation()
{
    if (self.fd_conf_path == NULL &&
        (self.fd_config->cnf_diamid == NULL ||
        self.fd_config->cnf_diamrlm == NULL ||
        self.fd_config->cnf_addr == NULL))
    {
        ogs_error("No mme.freeDiameter in '%s'", context_self()->config.path);
        return OGS_ERROR;
    }

    if (ogs_list_first(&self.s1ap_list) == NULL &&
        ogs_list_first(&self.s1ap_list6) == NULL)
    {
        ogs_error("No mme.s1ap in '%s'", context_self()->config.path);
        return OGS_RETRY;
    }

    if (ogs_list_first(&self.gtpc_list) == NULL &&
        ogs_list_first(&self.gtpc_list6) == NULL)
    {
        ogs_error("No mme.gtpc in '%s'", context_self()->config.path);
        return OGS_RETRY;
    }

    if (ogs_list_first(&self.sgw_list) == NULL)
    {
        ogs_error("No sgw.gtpc in '%s'", context_self()->config.path);
        return OGS_ERROR;
    }

    if (ogs_list_first(&self.pgw_list) == NULL)
    {
        ogs_error("No pgw.gtpc in '%s'", context_self()->config.path);
        return OGS_ERROR;
    }

    if (self.max_num_of_served_gummei == 0)
    {
        ogs_error("No mme.gummei in '%s'", context_self()->config.path);
        return OGS_ERROR;
    }

    if (self.served_gummei[0].num_of_plmn_id == 0)
    {
        ogs_error("No mme.gummei.plmn_id in '%s'", context_self()->config.path);
        return OGS_ERROR;
    }

    if (self.served_gummei[0].num_of_mme_gid == 0)
    {
        ogs_error("No mme.gummei.mme_gid in '%s'", context_self()->config.path);
        return OGS_ERROR;
    }

    if (self.served_gummei[0].num_of_mme_code == 0)
    {
        ogs_error("No mme.gummei.mme_code in '%s'", context_self()->config.path);
        return OGS_ERROR;
    }

    if (self.num_of_served_tai == 0)
    {
        ogs_error("No mme.tai in '%s'", context_self()->config.path);
        return OGS_ERROR;
    }

    if (self.served_tai[0].list0.tai[0].num == 0 &&
        self.served_tai[0].list2.num == 0)
    {
        ogs_error("No mme.tai.plmn_id|tac in '%s'", context_self()->config.path);
        return OGS_ERROR;
    }

    if (self.num_of_integrity_order == 0)
    {
        ogs_error("No mme.security.integrity_order in '%s'",
                context_self()->config.path);
        return OGS_ERROR;
    }
    if (self.num_of_ciphering_order == 0)
    {
        ogs_error("no mme.security.ciphering_order in '%s'",
                context_self()->config.path);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int mme_context_parse_config()
{
    int rv;
    config_t *config = &context_self()->config;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    ogs_assert(config);
    document = config->document;
    ogs_assert(document);

    rv = mme_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while(ogs_yaml_iter_next(&root_iter))
    {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "mme"))
        {
            ogs_yaml_iter_t mme_iter;
            ogs_yaml_iter_recurse(&root_iter, &mme_iter);
            while(ogs_yaml_iter_next(&mme_iter))
            {
                const char *mme_key = ogs_yaml_iter_key(&mme_iter);
                ogs_assert(mme_key);
                if (!strcmp(mme_key, "freeDiameter"))
                {
                    yaml_node_t *node = 
                        yaml_document_get_node(document, mme_iter.pair->value);
                    ogs_assert(node);
                    if (node->type == YAML_SCALAR_NODE)
                    {
                        self.fd_conf_path = ogs_yaml_iter_value(&mme_iter);
                    }
                    else if (node->type == YAML_MAPPING_NODE)
                    {
                        ogs_yaml_iter_t fd_iter;
                        ogs_yaml_iter_recurse(&mme_iter, &fd_iter);

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
                else if (!strcmp(mme_key, "relative_capacity"))
                {
                    const char *v = ogs_yaml_iter_value(&mme_iter);
                    if (v) self.relative_capacity = atoi(v);
                }
                else if (!strcmp(mme_key, "s1ap"))
                {
                    ogs_yaml_iter_t s1ap_array, s1ap_iter;
                    ogs_yaml_iter_recurse(&mme_iter, &s1ap_array);
                    do
                    {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.s1ap_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *list = NULL;
                        ogs_socknode_t *node = NULL;

                        if (ogs_yaml_iter_type(&s1ap_array) == YAML_MAPPING_NODE)
                        {
                            memcpy(&s1ap_iter, &s1ap_array,
                                    sizeof(ogs_yaml_iter_t));
                        }
                        else if (ogs_yaml_iter_type(&s1ap_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!ogs_yaml_iter_next(&s1ap_array))
                                break;
                            ogs_yaml_iter_recurse(&s1ap_array, &s1ap_iter);
                        }
                        else if (ogs_yaml_iter_type(&s1ap_array) ==
                            YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            ogs_assert_if_reached();

                        while(ogs_yaml_iter_next(&s1ap_iter))
                        {
                            const char *s1ap_key =
                                ogs_yaml_iter_key(&s1ap_iter);
                            ogs_assert(s1ap_key);
                            if (!strcmp(s1ap_key, "family"))
                            {
                                const char *v = ogs_yaml_iter_value(&s1ap_iter);
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
                            else if (!strcmp(s1ap_key, "addr") ||
                                    !strcmp(s1ap_key, "name"))
                            {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&s1ap_iter, &hostname_iter);
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
                            else if (!strcmp(s1ap_key, "port"))
                            {
                                const char *v = ogs_yaml_iter_value(&s1ap_iter);
                                if (v)
                                {
                                    port = atoi(v);
                                    self.s1ap_port = port;
                                }
                            }
                            else if (!strcmp(s1ap_key, "dev"))
                            {
                                dev = ogs_yaml_iter_value(&s1ap_iter);
                            }
                            else
                                ogs_warn("unknown key `%s`", s1ap_key);
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
                                rv = ogs_sock_add_node(&self.s1ap_list,
                                        &node, list, AF_INET);
                                ogs_assert(rv == OGS_OK);
                            }

                            if (context_self()->config.parameter.no_ipv6 == 0)
                            {
                                rv = ogs_sock_add_node(&self.s1ap_list6,
                                        &node, list, AF_INET6);
                                ogs_assert(rv == OGS_OK);
                            }

                            ogs_freeaddrinfo(list);
                        }

                        if (dev)
                        {
                            rv = ogs_sock_probe_node(
                                    context_self()->config.parameter.no_ipv4 ?
                                        NULL : &self.s1ap_list,
                                    context_self()->config.parameter.no_ipv6 ?
                                        NULL : &self.s1ap_list6,
                                    dev, self.s1ap_port);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while(ogs_yaml_iter_type(&s1ap_array) == YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.s1ap_list) == NULL &&
                        ogs_list_first(&self.s1ap_list6) == NULL)
                    {
                        rv = ogs_sock_probe_node(
                                context_self()->config.parameter.no_ipv4 ?
                                    NULL : &self.s1ap_list,
                                context_self()->config.parameter.no_ipv6 ?
                                    NULL : &self.s1ap_list6,
                                NULL, self.s1ap_port);
                        ogs_assert(rv == OGS_OK);
                    }
                }
                else if (!strcmp(mme_key, "gtpc"))
                {
                    ogs_yaml_iter_t gtpc_array, gtpc_iter;
                    ogs_yaml_iter_recurse(&mme_iter, &gtpc_array);
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
                else if (!strcmp(mme_key, "gummei"))
                {
                    ogs_yaml_iter_t gummei_array, gummei_iter;
                    ogs_yaml_iter_recurse(&mme_iter, &gummei_array);
                    do
                    {
                        served_gummei_t *gummei = NULL;
                        ogs_assert(self.max_num_of_served_gummei <=
                                MAX_NUM_OF_SERVED_GUMMEI);
                        gummei = &self.served_gummei[
                            self.max_num_of_served_gummei];
                        ogs_assert(gummei);

                        if (ogs_yaml_iter_type(&gummei_array) ==
                                YAML_MAPPING_NODE)
                        {
                            memcpy(&gummei_iter, &gummei_array,
                                    sizeof(ogs_yaml_iter_t));
                        }
                        else if (ogs_yaml_iter_type(&gummei_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!ogs_yaml_iter_next(&gummei_array))
                                break;
                            ogs_yaml_iter_recurse(&gummei_array,
                                    &gummei_iter);
                        }
                        else if (ogs_yaml_iter_type(&gummei_array) ==
                            YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            ogs_assert_if_reached();

                        while(ogs_yaml_iter_next(&gummei_iter))
                        {
                            const char *gummei_key =
                                ogs_yaml_iter_key(&gummei_iter);
                            ogs_assert(gummei_key);
                            if (!strcmp(gummei_key, "plmn_id"))
                            {
                                ogs_yaml_iter_t plmn_id_array, plmn_id_iter;
                                ogs_yaml_iter_recurse(&gummei_iter, &plmn_id_array);
                                do
                                {
                                    plmn_id_t *plmn_id = NULL;
                                    const char *mcc = NULL, *mnc = NULL;
                                    ogs_assert(gummei->num_of_plmn_id <=
                                            MAX_PLMN_ID);
                                    plmn_id = &gummei->plmn_id[
                                        gummei->num_of_plmn_id];
                                    ogs_assert(plmn_id);

                                    if (ogs_yaml_iter_type(&plmn_id_array) ==
                                            YAML_MAPPING_NODE)
                                    {
                                        memcpy(&plmn_id_iter, &plmn_id_array,
                                                sizeof(ogs_yaml_iter_t));
                                    }
                                    else if (ogs_yaml_iter_type(&plmn_id_array) ==
                                        YAML_SEQUENCE_NODE)
                                    {
                                        if (!ogs_yaml_iter_next(&plmn_id_array))
                                            break;
                                        ogs_yaml_iter_recurse(&plmn_id_array,
                                                &plmn_id_iter);
                                    }
                                    else if (ogs_yaml_iter_type(&plmn_id_array) ==
                                        YAML_SCALAR_NODE)
                                    {
                                        break;
                                    }
                                    else
                                        ogs_assert_if_reached();

                                    while(ogs_yaml_iter_next(&plmn_id_iter))
                                    {
                                        const char *plmn_id_key =
                                            ogs_yaml_iter_key(&plmn_id_iter);
                                        ogs_assert(plmn_id_key);
                                        if (!strcmp(plmn_id_key, "mcc"))
                                        {
                                            mcc =
                                                ogs_yaml_iter_value(&plmn_id_iter);
                                        }
                                        else if (!strcmp(plmn_id_key, "mnc"))
                                        {
                                            mnc =
                                                ogs_yaml_iter_value(&plmn_id_iter);
                                        }
                                    }

                                    if (mcc && mnc)
                                    {
                                        plmn_id_build(plmn_id,
                                            atoi(mcc), atoi(mnc), strlen(mnc));
                                        gummei->num_of_plmn_id++;
                                    }

                                } while(ogs_yaml_iter_type(&plmn_id_array) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(gummei_key, "mme_gid"))
                            {
                                ogs_yaml_iter_t mme_gid_iter;
                                ogs_yaml_iter_recurse(&gummei_iter, &mme_gid_iter);
                                ogs_assert(ogs_yaml_iter_type(&mme_gid_iter) !=
                                    YAML_MAPPING_NODE);

                                do
                                {
                                    uint16_t *mme_gid = NULL;
                                    const char *v = NULL;

                                    ogs_assert(gummei->num_of_mme_gid <=
                                            GRP_PER_MME);
                                    mme_gid = &gummei->mme_gid[
                                        gummei->num_of_mme_gid];
                                    ogs_assert(mme_gid);

                                    if (ogs_yaml_iter_type(&mme_gid_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!ogs_yaml_iter_next(&mme_gid_iter))
                                            break;
                                    }

                                    v = ogs_yaml_iter_value(&mme_gid_iter);
                                    if (v) 
                                    {
                                        *mme_gid = atoi(v);
                                        gummei->num_of_mme_gid++;
                                    }
                                } while(
                                    ogs_yaml_iter_type(&mme_gid_iter) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(gummei_key, "mme_code"))
                            {
                                ogs_yaml_iter_t mme_code_iter;
                                ogs_yaml_iter_recurse(&gummei_iter, &mme_code_iter);
                                ogs_assert(ogs_yaml_iter_type(&mme_code_iter) !=
                                    YAML_MAPPING_NODE);

                                do
                                {
                                    uint8_t *mme_code = NULL;
                                    const char *v = NULL;

                                    ogs_assert(gummei->num_of_mme_code <=
                                            CODE_PER_MME);
                                    mme_code = &gummei->mme_code[
                                        gummei->num_of_mme_code];
                                    ogs_assert(mme_code);

                                    if (ogs_yaml_iter_type(&mme_code_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!ogs_yaml_iter_next(&mme_code_iter))
                                            break;
                                    }

                                    v = ogs_yaml_iter_value(&mme_code_iter);
                                    if (v) 
                                    {
                                        *mme_code = atoi(v);
                                        gummei->num_of_mme_code++;
                                    }
                                } while(
                                    ogs_yaml_iter_type(&mme_code_iter) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else
                                ogs_warn("unknown key `%s`", gummei_key);
                        }

                        if (gummei->num_of_plmn_id &&
                            gummei->num_of_mme_gid && gummei->num_of_mme_code)
                        {
                            self.max_num_of_served_gummei++;
                        }
                        else
                        {
                            ogs_warn("Ignore gummei : "
                                    "plmn_id(%d), mme_gid(%d), mme_code(%d)",
                                gummei->num_of_plmn_id,
                                gummei->num_of_mme_gid, gummei->num_of_mme_code);
                            gummei->num_of_plmn_id = 0;
                            gummei->num_of_mme_gid = 0;
                            gummei->num_of_mme_code = 0;
                        }
                    } while(ogs_yaml_iter_type(&gummei_array) ==
                            YAML_SEQUENCE_NODE);
                }
                else if (!strcmp(mme_key, "tai"))
                {
                    int num_of_list0 = 0;
                    tai0_list_t *list0 = NULL;
                    tai2_list_t *list2 = NULL;

                    ogs_assert(self.num_of_served_tai <=
                            MAX_NUM_OF_SERVED_TAI);
                    list0 = &self.served_tai[self.num_of_served_tai].list0;
                    ogs_assert(list0);
                    list2 = &self.served_tai[self.num_of_served_tai].list2;
                    ogs_assert(list2);

                    ogs_yaml_iter_t tai_array, tai_iter;
                    ogs_yaml_iter_recurse(&mme_iter, &tai_array);
                    do
                    {
                        const char *mcc = NULL, *mnc = NULL;
                        uint16_t tac[MAX_NUM_OF_TAI];
                        int num_of_tac = 0;

                        if (ogs_yaml_iter_type(&tai_array) == YAML_MAPPING_NODE)
                        {
                            memcpy(&tai_iter, &tai_array, sizeof(ogs_yaml_iter_t));
                        }
                        else if (ogs_yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE)
                        {
                            if (!ogs_yaml_iter_next(&tai_array))
                                break;
                            ogs_yaml_iter_recurse(&tai_array,
                                    &tai_iter);
                        }
                        else if (ogs_yaml_iter_type(&tai_array) == YAML_SCALAR_NODE)
                        {
                            break;
                        }
                        else
                            ogs_assert_if_reached();

                        while(ogs_yaml_iter_next(&tai_iter))
                        {
                            const char *tai_key = ogs_yaml_iter_key(&tai_iter);
                            ogs_assert(tai_key);
                            if (!strcmp(tai_key, "plmn_id"))
                            {
                                ogs_yaml_iter_t plmn_id_iter;

                                ogs_yaml_iter_recurse(&tai_iter, &plmn_id_iter);
                                while(ogs_yaml_iter_next(&plmn_id_iter))
                                {
                                    const char *plmn_id_key =
                                        ogs_yaml_iter_key(&plmn_id_iter);
                                    ogs_assert(plmn_id_key);
                                    if (!strcmp(plmn_id_key, "mcc"))
                                    {
                                        mcc = ogs_yaml_iter_value(&plmn_id_iter);
                                    }
                                    else if (!strcmp(plmn_id_key, "mnc"))
                                    {
                                        mnc = ogs_yaml_iter_value(&plmn_id_iter);
                                    }
                                }
                            }
                            else if (!strcmp(tai_key, "tac"))
                            {
                                ogs_yaml_iter_t tac_iter;
                                ogs_yaml_iter_recurse(&tai_iter, &tac_iter);
                                ogs_assert(ogs_yaml_iter_type(&tac_iter) !=
                                    YAML_MAPPING_NODE);

                                do
                                {
                                    const char *v = NULL;

                                    ogs_assert(num_of_tac <=
                                            MAX_NUM_OF_TAI);
                                    if (ogs_yaml_iter_type(&tac_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!ogs_yaml_iter_next(&tac_iter))
                                            break;
                                    }

                                    v = ogs_yaml_iter_value(&tac_iter);
                                    if (v) 
                                    {
                                        tac[num_of_tac] = atoi(v);
                                        num_of_tac++;
                                    }
                                } while(
                                    ogs_yaml_iter_type(&tac_iter) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else
                                ogs_warn("unknown key `%s`", tai_key);
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
                            ogs_warn("Ignore tai : mcc(%p), mnc(%p), "
                                    "num_of_tac(%d)", mcc, mnc, num_of_tac);
                        }
                    } while(ogs_yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE);

                    if (list2->num || num_of_list0)
                    {
                        self.num_of_served_tai++;
                    }
                }
                else if (!strcmp(mme_key, "security"))
                {
                    ogs_yaml_iter_t security_iter;
                    ogs_yaml_iter_recurse(&mme_iter, &security_iter);
                    while(ogs_yaml_iter_next(&security_iter))
                    {
                        const char *security_key =
                            ogs_yaml_iter_key(&security_iter);
                        ogs_assert(security_key);
                        if (!strcmp(security_key, "integrity_order"))
                        {
                            ogs_yaml_iter_t integrity_order_iter;
                            ogs_yaml_iter_recurse(&security_iter,
                                    &integrity_order_iter);
                            ogs_assert(ogs_yaml_iter_type(&integrity_order_iter) !=
                                YAML_MAPPING_NODE);

                            do
                            {
                                const char *v = NULL;

                                if (ogs_yaml_iter_type(&integrity_order_iter) ==
                                        YAML_SEQUENCE_NODE)
                                {
                                    if (!ogs_yaml_iter_next(&integrity_order_iter))
                                        break;
                                }

                                v = ogs_yaml_iter_value(&integrity_order_iter);
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
                                ogs_yaml_iter_type(&integrity_order_iter) ==
                                    YAML_SEQUENCE_NODE);
                        }
                        else if (!strcmp(security_key, "ciphering_order"))
                        {
                            ogs_yaml_iter_t ciphering_order_iter;
                            ogs_yaml_iter_recurse(&security_iter,
                                    &ciphering_order_iter);
                            ogs_assert(ogs_yaml_iter_type(&ciphering_order_iter) !=
                                YAML_MAPPING_NODE);

                            do
                            {
                                const char *v = NULL;

                                if (ogs_yaml_iter_type(&ciphering_order_iter) ==
                                        YAML_SEQUENCE_NODE)
                                {
                                    if (!ogs_yaml_iter_next(&ciphering_order_iter))
                                        break;
                                }

                                v = ogs_yaml_iter_value(&ciphering_order_iter);
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
                                ogs_yaml_iter_type(&ciphering_order_iter) ==
                                    YAML_SEQUENCE_NODE);
                        }
                    }
                }
                else if(!strcmp(mme_key, "network_name"))
                {
                    ogs_yaml_iter_t network_name_iter;
                    ogs_yaml_iter_recurse(&mme_iter, &network_name_iter);

                    while(ogs_yaml_iter_next(&network_name_iter))
                    {
                        const char *network_name_key =
                        ogs_yaml_iter_key(&network_name_iter);
                        ogs_assert(network_name_key);
                        if (!strcmp(network_name_key, "full"))
                        {  
                            nas_network_name_t *network_full_name =
                                &self.full_name;
                            const char *c_network_name =
                                ogs_yaml_iter_value(&network_name_iter);
                            uint8_t size = strlen(c_network_name);
                            uint8_t i;
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
                                ogs_yaml_iter_value(&network_name_iter);
                            uint8_t size = strlen(c_network_name);
                            uint8_t i;
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
                    ogs_warn("unknown key `%s`", mme_key);
            }
        }
        else if (!strcmp(root_key, "sgw"))
        {
            ogs_yaml_iter_t mme_iter;
            ogs_yaml_iter_recurse(&root_iter, &mme_iter);
            while(ogs_yaml_iter_next(&mme_iter))
            {
                const char *mme_key = ogs_yaml_iter_key(&mme_iter);
                ogs_assert(mme_key);
                if (!strcmp(mme_key, "gtpc"))
                {
                    ogs_yaml_iter_t gtpc_array, gtpc_iter;
                    ogs_yaml_iter_recurse(&mme_iter, &gtpc_array);
                    do
                    {
                        mme_sgw_t *sgw = NULL;
                        ogs_sockaddr_t *list = NULL;
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.gtpc_port;
                        uint16_t tac[MAX_NUM_OF_TAI] = {0,};
                        uint8_t num_of_tac = 0;

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
                                if (v) port = atoi(v);
                            }
                            else if (!strcmp(gtpc_key, "tac"))
                            {
                                ogs_yaml_iter_t tac_iter;
                                ogs_yaml_iter_recurse(&gtpc_iter, &tac_iter);
                                ogs_assert(ogs_yaml_iter_type(&tac_iter) !=
                                    YAML_MAPPING_NODE);

                                do
                                {
                                    const char *v = NULL;

                                    ogs_assert(num_of_tac <=
                                            MAX_NUM_OF_TAI);
                                    if (ogs_yaml_iter_type(&tac_iter) ==
                                            YAML_SEQUENCE_NODE)
                                    {
                                        if (!ogs_yaml_iter_next(&tac_iter))
                                            break;
                                    }

                                    v = ogs_yaml_iter_value(&tac_iter);
                                    if (v) 
                                    {
                                        tac[num_of_tac] = atoi(v);
                                        num_of_tac++;
                                    }
                                } while(
                                    ogs_yaml_iter_type(&tac_iter) ==
                                        YAML_SEQUENCE_NODE);
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

                        sgw = mme_sgw_add(list,
                                context_self()->config.parameter.no_ipv4,
                                context_self()->config.parameter.no_ipv6,
                                context_self()->config.parameter.prefer_ipv4);
                        ogs_assert(sgw);

                        sgw->num_of_tac = num_of_tac;
                        if (num_of_tac != 0)
                            memcpy(sgw->tac, tac, sizeof(sgw->tac));

                        ogs_freeaddrinfo(list);

                    } while(ogs_yaml_iter_type(&gtpc_array) == YAML_SEQUENCE_NODE);
                }
                else if(!strcmp(mme_key, "selection_mode"))
                {
                    const char *selection_mode =
                    ogs_yaml_iter_value(&mme_iter);

                    if (!strcmp(selection_mode, "rr"))
                        self.sgw_selection = SGW_SELECT_RR;
                    else if (!strcmp(selection_mode, "tac"))
                        self.sgw_selection = SGW_SELECT_TAC;
                    else
                        ogs_warn("unknown sgw_selection mode `%s`",
                                selection_mode);
                }
            }
        }
        else if (!strcmp(root_key, "pgw"))
        {
            ogs_yaml_iter_t mme_iter;
            ogs_yaml_iter_recurse(&root_iter, &mme_iter);
            while(ogs_yaml_iter_next(&mme_iter))
            {
                const char *mme_key = ogs_yaml_iter_key(&mme_iter);
                ogs_assert(mme_key);
                if (!strcmp(mme_key, "gtpc"))
                {
                    ogs_yaml_iter_t gtpc_array, gtpc_iter;
                    ogs_yaml_iter_recurse(&mme_iter, &gtpc_array);
                    do
                    {
                        mme_pgw_t *pgw = NULL;
                        ogs_sockaddr_t *list = NULL;
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.gtpc_port;

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
                                if (v) port = atoi(v);
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

                        pgw = mme_pgw_add(list,
                                context_self()->config.parameter.no_ipv4,
                                context_self()->config.parameter.no_ipv6,
                                context_self()->config.parameter.prefer_ipv4);
                        ogs_assert(pgw);

                        ogs_freeaddrinfo(list);

                    } while(ogs_yaml_iter_type(&gtpc_array) == YAML_SEQUENCE_NODE);
                }
            }
        }
    }

    rv = mme_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

mme_sgw_t *mme_sgw_add(
        ogs_sockaddr_t *all_list, int no_ipv4, int no_ipv6, int prefer_ipv4)
{
    int rv;
    mme_sgw_t *sgw = NULL;

    ogs_assert(all_list);

    ogs_pool_alloc(&mme_sgw_pool, &sgw);
    ogs_assert(sgw);
    memset(sgw, 0, sizeof *sgw);

    rv = gtp_create_node(&sgw->gnode, all_list, no_ipv4, no_ipv6, prefer_ipv4);
    ogs_assert(rv == OGS_OK);
    if (sgw->gnode == NULL)
    {
        ogs_error("Invalid Parameter : "
                "no_ipv4[%d], no_ipv6[%d], prefer_ipv4[%d]",
                no_ipv4, no_ipv6, prefer_ipv4);
        return NULL;
    }

    ogs_list_add(&self.sgw_list, sgw);

    return sgw;
}

int mme_sgw_remove(mme_sgw_t *sgw)
{
    int rv;
    ogs_assert(sgw);

    ogs_list_remove(&self.sgw_list, sgw);

    rv = gtp_delete_node(sgw->gnode);
    ogs_assert(rv == OGS_OK);

    ogs_pool_free(&mme_sgw_pool, sgw);

    return OGS_OK;
}

void mme_sgw_remove_all()
{
    mme_sgw_t *sgw = NULL, *next_sgw = NULL;

    ogs_list_for_each_safe(&self.sgw_list, next_sgw, sgw)
        mme_sgw_remove(sgw);
}

mme_pgw_t *mme_pgw_add(
        ogs_sockaddr_t *all_list, int no_ipv4, int no_ipv6, int prefer_ipv4)
{
    int rv;
    mme_pgw_t *pgw = NULL;

    ogs_assert(all_list);

    ogs_pool_alloc(&mme_pgw_pool, &pgw);
    ogs_assert(pgw);

    rv = gtp_create_node(&pgw->gnode, all_list, no_ipv4, no_ipv6, prefer_ipv4);
    ogs_assert(rv == OGS_OK);
    if (pgw->gnode == NULL)
    {
        ogs_error("Invalid Parameter : "
                "no_ipv4[%d], no_ipv6[%d], prefer_ipv4[%d]",
                no_ipv4, no_ipv6, prefer_ipv4);
        return NULL;
    }

    ogs_list_add(&self.pgw_list, pgw);

    return pgw;
}

int mme_pgw_remove(mme_pgw_t *pgw)
{
    int rv;
    ogs_assert(pgw);

    ogs_list_remove(&self.pgw_list, pgw);

    rv = gtp_delete_node(pgw->gnode);
    ogs_assert(rv == OGS_OK);

    ogs_pool_free(&mme_pgw_pool, pgw);

    return OGS_OK;
}

void mme_pgw_remove_all()
{
    mme_pgw_t *pgw = NULL, *next_pgw = NULL;

    ogs_list_for_each_safe(&self.pgw_list, next_pgw, pgw)
        mme_pgw_remove(pgw);
}

mme_enb_t *mme_enb_add(ogs_sock_t *sock, ogs_sockaddr_t *addr)
{
    mme_enb_t *enb = NULL;
    mme_event_t e;

    ogs_assert(sock);
    ogs_assert(addr);

    ogs_pool_alloc(&mme_enb_pool, &enb);
    ogs_assert(enb);

    enb->sock = sock;
    enb->addr = addr;
    enb->sock_type = mme_enb_sock_type(enb->sock);

    enb->outbound_streams = context_self()->config.parameter.sctp_streams;

    ogs_list_init(&enb->enb_ue_list);

    if (enb->sock_type == SOCK_STREAM)
        ogs_hash_set(self.enb_sock_hash, enb->sock, sizeof(ogs_sock_t), enb);
    ogs_hash_set(self.enb_addr_hash, enb->addr, sizeof(ogs_sockaddr_t), enb);

#if HAVE_USRSCTP != 1
    enb->poll = ogs_pollset_add(mme_self()->pollset,
        OGS_POLLIN, sock->fd, s1ap_recv_handler, sock);
    ogs_assert(enb->poll);
#endif
    
    e.enb = enb;
    ogs_fsm_create(&enb->sm, s1ap_state_initial, s1ap_state_final);
    ogs_fsm_init(&enb->sm, &e);

    return enb;
}

int mme_enb_remove(mme_enb_t *enb)
{
    mme_event_t e;

    ogs_assert(enb);
    ogs_assert(enb->sock);

    e.enb = enb;
    ogs_fsm_fini(&enb->sm, &e);
    ogs_fsm_delete(&enb->sm);

    if (enb->sock_type == SOCK_STREAM)
        ogs_hash_set(self.enb_sock_hash, enb->sock, sizeof(ogs_sock_t), NULL);
    ogs_hash_set(self.enb_addr_hash, enb->addr, sizeof(ogs_sockaddr_t), NULL);
    ogs_hash_set(self.enb_id_hash, &enb->enb_id, sizeof(enb->enb_id), NULL);

    enb_ue_remove_in_enb(enb);

#if HAVE_USRSCTP != 1
    ogs_pollset_remove(enb->poll);
#endif

    if (enb->sock_type == SOCK_STREAM)
        s1ap_closesocket(enb->sock);

    ogs_free(enb->addr);

    ogs_pool_free(&mme_enb_pool, enb);

    return OGS_OK;
}

int mme_enb_remove_all()
{
    ogs_hash_index_t *hi = NULL;
    mme_enb_t *enb = NULL;

    for (hi = mme_enb_first(); hi; hi = mme_enb_next(hi))
    {
        enb = mme_enb_this(hi);
        mme_enb_remove(enb);
    }

    return OGS_OK;
}

mme_enb_t* mme_enb_find_by_sock(ogs_sock_t *sock)
{
    ogs_assert(sock);
    return (mme_enb_t *)ogs_hash_get(self.enb_sock_hash, sock, sizeof(ogs_sock_t));

    return NULL;
}

mme_enb_t* mme_enb_find_by_addr(ogs_sockaddr_t *addr)
{
    ogs_assert(addr);
    return (mme_enb_t *)ogs_hash_get(self.enb_addr_hash,
            addr, sizeof(ogs_sockaddr_t));

    return NULL;
}

mme_enb_t* mme_enb_find_by_enb_id(uint32_t enb_id)
{
    return (mme_enb_t *)ogs_hash_get(self.enb_id_hash, &enb_id, sizeof(enb_id));
}

int mme_enb_set_enb_id(mme_enb_t *enb, uint32_t enb_id)
{
    ogs_assert(enb);

    enb->enb_id = enb_id;
    ogs_hash_set(self.enb_id_hash, &enb->enb_id, sizeof(enb->enb_id), enb);

    return OGS_OK;
}

ogs_hash_index_t* mme_enb_first()
{
    ogs_assert(self.enb_sock_hash);
    return ogs_hash_first(self.enb_sock_hash);
}

ogs_hash_index_t* mme_enb_next(ogs_hash_index_t *hi)
{
    return ogs_hash_next(hi);
}

mme_enb_t *mme_enb_this(ogs_hash_index_t *hi)
{
    ogs_assert(hi);
    return ogs_hash_this_val(hi);
}

int mme_enb_sock_type(ogs_sock_t *sock)
{
    ogs_socknode_t *snode = NULL;

    ogs_assert(sock);

    ogs_list_for_each(&mme_self()->s1ap_list, snode)
        if (snode->sock == sock) return SOCK_SEQPACKET;

    ogs_list_for_each(&mme_self()->s1ap_list6, snode)
        if (snode->sock == sock) return SOCK_SEQPACKET;

    return SOCK_STREAM;
}

/** enb_ue_context handling function */
enb_ue_t* enb_ue_add(mme_enb_t *enb)
{
    enb_ue_t *enb_ue = NULL;

    ogs_assert(self.mme_ue_s1ap_id_hash);
    ogs_assert(enb);

    ogs_pool_alloc(&enb_ue_pool, &enb_ue);
    ogs_assert(enb_ue);

    enb_ue->enb_ue_s1ap_id = INVALID_UE_S1AP_ID;
    enb_ue->mme_ue_s1ap_id = NEXT_ID(self.mme_ue_s1ap_id, 1, 0xffffffff);
    enb_ue->enb = enb;

    ogs_hash_set(self.mme_ue_s1ap_id_hash, &enb_ue->mme_ue_s1ap_id, 
            sizeof(enb_ue->mme_ue_s1ap_id), enb_ue);
    ogs_list_add(&enb->enb_ue_list, enb_ue);

    return enb_ue;
}

unsigned int enb_ue_count()
{
    ogs_assert(self.mme_ue_s1ap_id_hash);
    return ogs_hash_count(self.mme_ue_s1ap_id_hash);
}

int enb_ue_remove(enb_ue_t *enb_ue)
{
    int rv;

    ogs_assert(self.mme_ue_s1ap_id_hash);
    ogs_assert(enb_ue);
    ogs_assert(enb_ue->enb);

    /* De-associate S1 with NAS/EMM */
    rv = enb_ue_deassociate(enb_ue);
    ogs_assert(rv == OGS_OK);

    ogs_list_remove(&enb_ue->enb->enb_ue_list, enb_ue);
    ogs_hash_set(self.mme_ue_s1ap_id_hash, &enb_ue->mme_ue_s1ap_id, 
            sizeof(enb_ue->mme_ue_s1ap_id), NULL);

    ogs_pool_free(&enb_ue_pool, enb_ue);

    return OGS_OK;
}

int enb_ue_remove_in_enb(mme_enb_t *enb)
{
    enb_ue_t *enb_ue = NULL, *next_enb_ue = NULL;
    
    enb_ue = enb_ue_first_in_enb(enb);
    while (enb_ue)
    {
        next_enb_ue = enb_ue_next_in_enb(enb_ue);

        enb_ue_remove(enb_ue);

        enb_ue = next_enb_ue;
    }

    return OGS_OK;
}

int enb_ue_switch_to_enb(enb_ue_t *enb_ue, mme_enb_t *new_enb)
{
    ogs_assert(enb_ue);
    ogs_assert(enb_ue->enb);
    ogs_assert(new_enb);

    /* Remove from the old enb */
    ogs_list_remove(&enb_ue->enb->enb_ue_list, enb_ue);

    /* Add to the new enb */
    ogs_list_add(&new_enb->enb_ue_list, enb_ue);

    /* Switch to enb */
    enb_ue->enb = new_enb;

    return OGS_OK;
}

enb_ue_t* enb_ue_find_by_enb_ue_s1ap_id(
        mme_enb_t *enb, uint32_t enb_ue_s1ap_id)
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

enb_ue_t* enb_ue_find_by_mme_ue_s1ap_id(uint32_t mme_ue_s1ap_id)
{
    ogs_assert(self.mme_ue_s1ap_id_hash);
    return ogs_hash_get(self.mme_ue_s1ap_id_hash, 
            &mme_ue_s1ap_id, sizeof(mme_ue_s1ap_id));
}

enb_ue_t* enb_ue_first_in_enb(mme_enb_t *enb)
{
    return ogs_list_first(&enb->enb_ue_list);
}

enb_ue_t* enb_ue_next_in_enb(enb_ue_t *enb_ue)
{
    return ogs_list_next(enb_ue);
}

static int mme_ue_new_guti(mme_ue_t *mme_ue)
{
    served_gummei_t *served_gummei = NULL;

    ogs_assert(mme_ue);
    ogs_assert(mme_self()->max_num_of_served_gummei > 0);

    served_gummei = &mme_self()->served_gummei[0];

    ogs_assert(served_gummei->num_of_plmn_id > 0);
    ogs_assert(served_gummei->num_of_mme_gid > 0);
    ogs_assert(served_gummei->num_of_mme_code > 0);

    if (mme_ue->m_tmsi)
    {
        /* MME has a VALID GUIT
         * As such, we need to remove previous GUTI in hash table */
        ogs_hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), NULL);
        ogs_assert(mme_m_tmsi_free(mme_ue->m_tmsi) == OGS_OK);
    }

    memset(&mme_ue->guti, 0, sizeof(guti_t));

    /* Use the first configured plmn_id and mme group id */
    memcpy(&mme_ue->guti.plmn_id, &served_gummei->plmn_id[0], PLMN_ID_LEN);
    mme_ue->guti.mme_gid = served_gummei->mme_gid[0];
    mme_ue->guti.mme_code = served_gummei->mme_code[0];

    mme_ue->m_tmsi = mme_m_tmsi_alloc();
    ogs_assert(mme_ue->m_tmsi);
    mme_ue->guti.m_tmsi = *(mme_ue->m_tmsi);
    ogs_hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), mme_ue);

    return OGS_OK;
}

mme_ue_t* mme_ue_add(enb_ue_t *enb_ue)
{
    mme_enb_t *enb = NULL;
    mme_ue_t *mme_ue = NULL;
    mme_event_t e;

    ogs_assert(enb_ue);
    enb = enb_ue->enb;
    ogs_assert(enb);

    ogs_pool_alloc(&mme_ue_pool, &mme_ue);
    ogs_assert(mme_ue);

    ogs_list_init(&mme_ue->sess_list);

    mme_ue->mme_s11_teid = ogs_pool_index(&mme_ue_pool, mme_ue);
    ogs_assert(mme_ue->mme_s11_teid > 0 &&
            mme_ue->mme_s11_teid <= MAX_POOL_OF_UE);

    /*
     * SCTP output stream identification
     * Default context_self()->config.parameter.sctp_streams : 30
     *   0 : Non UE signalling
     *   1-29 : UE specific association 
     */
    mme_ue->ostream_id = NEXT_ID(self.ostream_id, 1, enb->outbound_streams-1);

    /* Create New GUTI */
    mme_ue_new_guti(mme_ue);

    if (mme_self()->sgw_selection == SGW_SELECT_RR)
    {
        /* Setup SGW with round-robin manner */
        if (mme_self()->sgw == NULL)
            mme_self()->sgw = ogs_list_first(&mme_self()->sgw_list);

        ogs_assert(mme_self()->sgw);
        SETUP_GTP_NODE(mme_ue, mme_self()->sgw->gnode);

        mme_self()->sgw = ogs_list_next(mme_self()->sgw);
    }
    else if (mme_self()->sgw_selection == SGW_SELECT_TAC)
    {
        /* Select SGW by eNB TAC */
        int i, found = 0;

        mme_self()->sgw = ogs_list_first(&mme_self()->sgw_list);
        while(mme_self()->sgw && !found)
        {
            for (i = 0; i < mme_self()->sgw->num_of_tac && !found; i++)
                found = mme_self()->sgw->tac[i] == enb_ue->nas.tai.tac ? 1: 0;

            if (!found)
                mme_self()->sgw = ogs_list_next(mme_self()->sgw);
        }

        ogs_assert(mme_self()->sgw);
        SETUP_GTP_NODE(mme_ue, mme_self()->sgw->gnode);
    }
    else
        ogs_assert_if_reached();
        

    /* Create paging retry timer */
    mme_ue->t3413 = ogs_timer_add(self.timer_mgr, s1ap_t3413_timeout, mme_ue);
    ogs_assert(mme_ue->t3413);

    e.mme_ue = mme_ue;
    ogs_fsm_create(&mme_ue->sm, emm_state_initial, emm_state_final);
    ogs_fsm_init(&mme_ue->sm, &e);

    return mme_ue;
}

int mme_ue_remove(mme_ue_t *mme_ue)
{
    int rv;
    mme_event_t e;

    ogs_assert(mme_ue);

    e.mme_ue = mme_ue;
    ogs_fsm_fini(&mme_ue->sm, &e);
    ogs_fsm_delete(&mme_ue->sm);

    /* Clear hash table */
    if (mme_ue->m_tmsi)
    {
        ogs_hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), NULL);
        ogs_assert(mme_m_tmsi_free(mme_ue->m_tmsi) == OGS_OK);
    }
    if (mme_ue->imsi_len != 0)
        ogs_hash_set(self.imsi_ue_hash, mme_ue->imsi, mme_ue->imsi_len, NULL);
    
    /* Clear the saved PDN Connectivity Request */
    NAS_CLEAR_DATA(&mme_ue->pdn_connectivity_request);

    /* Clear Paging info : stop t3413, last_paing_msg */
    CLEAR_PAGING_INFO(mme_ue);

    /* Free UeRadioCapability */
    S1AP_CLEAR_DATA(&mme_ue->ueRadioCapability);

    /* Clear Transparent Container */
    S1AP_CLEAR_DATA(&mme_ue->container);

    /* Delete All Timers */
    ogs_timer_delete(mme_ue->t3413);

    rv = mme_ue_deassociate(mme_ue);
    ogs_assert(rv == OGS_OK);

    mme_sess_remove_all(mme_ue);
    mme_pdn_remove_all(mme_ue);

    ogs_pool_free(&mme_ue_pool, mme_ue);

    return OGS_OK;
}

int mme_ue_remove_all()
{
    ogs_hash_index_t *hi = NULL;
    mme_ue_t *mme_ue = NULL;

    for (hi = mme_ue_first(); hi; hi = mme_ue_next(hi))
    {
        mme_ue = mme_ue_this(hi);
        mme_ue_remove(mme_ue);
    }

    return OGS_OK;
}

mme_ue_t* mme_ue_find_by_imsi_bcd(char *imsi_bcd)
{
    uint8_t imsi[MAX_IMSI_LEN];
    int imsi_len = 0;

    ogs_assert(imsi_bcd);

    ogs_bcd_to_buffer(imsi_bcd, imsi, &imsi_len);

    return mme_ue_find_by_imsi(imsi, imsi_len);
}

mme_ue_t* mme_ue_find_by_imsi(uint8_t *imsi, int imsi_len)
{
    ogs_assert(imsi && imsi_len);

    return (mme_ue_t *)ogs_hash_get(self.imsi_ue_hash, imsi, imsi_len);
}

mme_ue_t* mme_ue_find_by_guti(guti_t *guti)
{
    ogs_assert(guti);

    return (mme_ue_t *)ogs_hash_get(self.guti_ue_hash, guti, sizeof(guti_t));
}

mme_ue_t* mme_ue_find_by_teid(uint32_t teid)
{
    return ogs_pool_find(&mme_ue_pool, teid);
}

ogs_hash_index_t *mme_ue_first()
{
    ogs_assert(self.imsi_ue_hash);
    return ogs_hash_first(self.imsi_ue_hash);
}

ogs_hash_index_t *mme_ue_next(ogs_hash_index_t *hi)
{
    return ogs_hash_next(hi);
}

mme_ue_t *mme_ue_this(ogs_hash_index_t *hi)
{
    ogs_assert(hi);
    return ogs_hash_this_val(hi);
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
                    char imsi_bcd[MAX_IMSI_BCD_LEN+1];

                    nas_imsi_to_bcd(
                        &eps_mobile_identity->imsi, eps_mobile_identity->length,
                        imsi_bcd);


                    mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);
                    if (mme_ue)
                    {
                        ogs_trace("known UE by IMSI[%s]", imsi_bcd);
                    }
                    else
                    {
                        ogs_trace("Unknown UE by IMSI[%s]", imsi_bcd);
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
                        ogs_trace("Known UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    else
                    {
                        ogs_warn("Unknown UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    break;
                }
                default:
                {
                    ogs_error("Uknown message imsi type =%d",
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
                        ogs_trace("Known UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    else
                    {
                        ogs_warn("Unknown UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    break;
                }
                default:
                {
                    ogs_error("Uknown message imsi type =%d",
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

int mme_ue_set_imsi(mme_ue_t *mme_ue, char *imsi_bcd)
{
    ogs_assert(mme_ue && imsi_bcd);

    ogs_cpystrn(mme_ue->imsi_bcd, imsi_bcd, MAX_IMSI_BCD_LEN+1);
    ogs_bcd_to_buffer(mme_ue->imsi_bcd, mme_ue->imsi, &mme_ue->imsi_len);

    ogs_hash_set(self.imsi_ue_hash, mme_ue->imsi, mme_ue->imsi_len, mme_ue);

    mme_ue->guti_present = 1;

    return OGS_OK;
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

int mme_ue_clear_indirect_tunnel(mme_ue_t *mme_ue)
{
    mme_sess_t *sess = NULL;

    ogs_assert(mme_ue);

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

    return OGS_OK;
}

int mme_ue_associate_enb_ue(mme_ue_t *mme_ue, enb_ue_t *enb_ue)
{
    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    mme_ue->enb_ue = enb_ue;
    enb_ue->mme_ue = mme_ue;

    return OGS_OK;
}

int enb_ue_deassociate(enb_ue_t *enb_ue)
{
    ogs_assert(enb_ue);
    enb_ue->mme_ue = NULL;
    
    return OGS_OK;
}

int mme_ue_deassociate(mme_ue_t *mme_ue)
{
    ogs_assert(mme_ue);
    mme_ue->enb_ue = NULL;
    
    return OGS_OK;
}

int source_ue_associate_target_ue(
        enb_ue_t *source_ue, enb_ue_t *target_ue)
{
    mme_ue_t *mme_ue = NULL;

    ogs_assert(source_ue);
    ogs_assert(target_ue);
    mme_ue = source_ue->mme_ue;
    ogs_assert(mme_ue);

    target_ue->mme_ue = mme_ue;
    target_ue->source_ue = source_ue;
    source_ue->target_ue = target_ue;

    return OGS_OK;
}

int source_ue_deassociate_target_ue(enb_ue_t *enb_ue)
{
    enb_ue_t *source_ue = NULL;
    enb_ue_t *target_ue = NULL;
    ogs_assert(enb_ue);

    if (enb_ue->target_ue)
    {
        source_ue = enb_ue;
        target_ue = enb_ue->target_ue;

        ogs_assert(source_ue->target_ue);
        ogs_assert(target_ue->source_ue);
        source_ue->target_ue = NULL;
        target_ue->source_ue = NULL;
    }
    else if (enb_ue->source_ue)
    {
        target_ue = enb_ue;
        source_ue = enb_ue->source_ue;

        ogs_assert(source_ue->target_ue);
        ogs_assert(target_ue->source_ue);
        source_ue->target_ue = NULL;
        target_ue->source_ue = NULL;
    }

    return OGS_OK;
}

mme_sess_t *mme_sess_add(mme_ue_t *mme_ue, uint8_t pti)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    ogs_assert(mme_ue);
    ogs_assert(pti != NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED);

    ogs_pool_alloc(&mme_sess_pool, &sess);
    ogs_assert(sess);

    ogs_list_init(&sess->bearer_list);

    sess->mme_ue = mme_ue;
    sess->pti = pti;

    bearer = mme_bearer_add(sess);
    ogs_assert(bearer);

    ogs_list_add(&mme_ue->sess_list, sess);

    return sess;
}

int mme_sess_remove(mme_sess_t *sess)
{
    ogs_assert(sess);
    ogs_assert(sess->mme_ue);
    
    ogs_list_remove(&sess->mme_ue->sess_list, sess);

    mme_bearer_remove_all(sess);

    NAS_CLEAR_DATA(&sess->ue_pco);
    TLV_CLEAR_DATA(&sess->pgw_pco);

    ogs_pool_free(&mme_sess_pool, sess);

    return OGS_OK;
}

int mme_sess_remove_all(mme_ue_t *mme_ue)
{
    mme_sess_t *sess = NULL, *next_sess = NULL;
    
    sess = mme_sess_first(mme_ue);
    while (sess)
    {
        next_sess = mme_sess_next(sess);

        mme_sess_remove(sess);

        sess = next_sess;
    }

    return OGS_OK;
}

mme_sess_t* mme_sess_find_by_pti(mme_ue_t *mme_ue, uint8_t pti)
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

mme_sess_t* mme_sess_find_by_ebi(mme_ue_t *mme_ue, uint8_t ebi)
{
    mme_bearer_t *bearer = NULL;

    bearer = mme_bearer_find_by_ue_ebi(mme_ue, ebi);
    if (bearer)
        return bearer->sess;

    return NULL;
}

mme_sess_t* mme_sess_find_by_apn(mme_ue_t *mme_ue, char *apn)
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
    return ogs_list_first(&mme_ue->sess_list);
}

mme_sess_t* mme_sess_next(mme_sess_t *sess)
{
    return ogs_list_next(sess);
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
    mme_event_t e;

    mme_bearer_t *bearer = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    ogs_pool_alloc(&mme_bearer_pool, &bearer);
    ogs_assert(bearer);

    bearer->ebi = NEXT_ID(mme_ue->ebi, MIN_EPS_BEARER_ID, MAX_EPS_BEARER_ID);

    bearer->mme_ue = mme_ue;
    bearer->sess = sess;

    ogs_list_add(&sess->bearer_list, bearer);
    
    e.bearer = bearer;
    ogs_fsm_create(&bearer->sm, esm_state_initial, esm_state_final);
    ogs_fsm_init(&bearer->sm, &e);

    return bearer;
}

int mme_bearer_remove(mme_bearer_t *bearer)
{
    mme_event_t e;

    ogs_assert(bearer);
    ogs_assert(bearer->sess);

    e.bearer = bearer;
    ogs_fsm_fini(&bearer->sm, &e);
    ogs_fsm_delete(&bearer->sm);

    ogs_list_remove(&bearer->sess->bearer_list, bearer);

    TLV_CLEAR_DATA(&bearer->tft);
    
    ogs_pool_free(&mme_bearer_pool, bearer);

    return OGS_OK;
}

int mme_bearer_remove_all(mme_sess_t *sess)
{
    mme_bearer_t *bearer = NULL, *next_bearer = NULL;

    ogs_assert(sess);
    
    bearer = mme_bearer_first(sess);
    while (bearer)
    {
        next_bearer = mme_bearer_next(bearer);

        mme_bearer_remove(bearer);

        bearer = next_bearer;
    }

    return OGS_OK;
}

mme_bearer_t* mme_bearer_find_by_sess_ebi(mme_sess_t *sess, uint8_t ebi)
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

mme_bearer_t* mme_bearer_find_by_ue_ebi(mme_ue_t *mme_ue, uint8_t ebi)
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
    uint8_t pti = NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    uint8_t ebi = NAS_EPS_BEARER_IDENTITY_UNASSIGNED;

    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;

    ogs_assert(mme_ue);
    ogs_assert(message);

    pti = message->esm.h.procedure_transaction_identity;
    ebi = message->esm.h.eps_bearer_identity;

    ogs_trace("mme_bearer_find_or_add_by_message() [PTI:%d, EBI:%d]",
            pti, ebi);

    if (ebi != NAS_EPS_BEARER_IDENTITY_UNASSIGNED)
    {
        bearer = mme_bearer_find_by_ue_ebi(mme_ue, ebi);
        ogs_assert(bearer);
        return bearer;
    }
        
    ogs_assert(pti != NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED);

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

        ogs_assert(sess);
    }
    else if (message->esm.h.message_type == NAS_PDN_DISCONNECT_REQUEST)
    {
        nas_pdn_disconnect_request_t *pdn_disconnect_request = 
            &message->esm.pdn_disconnect_request;
        nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity =
            &pdn_disconnect_request->linked_eps_bearer_identity;

        bearer = mme_bearer_find_by_ue_ebi(mme_ue,
                linked_eps_bearer_identity->eps_bearer_identity);
        ogs_assert(bearer);
        sess = bearer->sess;
        ogs_assert(sess);
        sess->pti = pti;

        return bearer;
    }
    else
    {
        sess = mme_sess_find_by_pti(mme_ue, pti);
        ogs_assert(sess);
    }

    bearer = mme_default_bearer_in_sess(sess);
    ogs_assert(bearer);
    return bearer;
}

mme_bearer_t* mme_default_bearer_in_sess(mme_sess_t *sess)
{
    return mme_bearer_first(sess);
}

mme_bearer_t* mme_linked_bearer(mme_bearer_t *bearer)
{
    mme_sess_t *sess = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    return mme_default_bearer_in_sess(sess);
}

mme_bearer_t* mme_bearer_first(mme_sess_t *sess)
{
    ogs_assert(sess);

    return ogs_list_first(&sess->bearer_list);
}

mme_bearer_t* mme_bearer_next(mme_bearer_t *bearer)
{
    return ogs_list_next(bearer);
}

int mme_bearer_is_inactive(mme_ue_t *mme_ue)
{
    mme_sess_t *sess = NULL;
    ogs_assert(mme_ue);

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

int mme_bearer_set_inactive(mme_ue_t *mme_ue)
{
    mme_sess_t *sess = NULL;
    ogs_assert(mme_ue);

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

    return OGS_OK;
}

int mme_pdn_remove_all(mme_ue_t *mme_ue)
{
    s6a_subscription_data_t *subscription_data = NULL;

    ogs_assert(mme_ue);
    subscription_data = &mme_ue->subscription_data;
    ogs_assert(subscription_data);

    subscription_data->num_of_pdn = 0;
    
    return OGS_OK;
}

pdn_t* mme_pdn_find_by_apn(mme_ue_t *mme_ue, char *apn)
{
    s6a_subscription_data_t *subscription_data = NULL;
    pdn_t *pdn = NULL;
    int i = 0;
    
    ogs_assert(mme_ue);
    subscription_data = &mme_ue->subscription_data;
    ogs_assert(subscription_data);

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
    
    ogs_assert(mme_ue);
    subscription_data = &mme_ue->subscription_data;
    ogs_assert(subscription_data);

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

    ogs_assert(tai);

    for (i = 0; i < self.num_of_served_tai; i++)
    {
        tai0_list_t *list0 = &self.served_tai[i].list0;
        ogs_assert(list0);
        tai2_list_t *list2 = &self.served_tai[i].list2;
        ogs_assert(list2);

        for (j = 0; list0->tai[j].num; j++)
        {
            ogs_assert(list0->tai[j].type == TAI0_TYPE);
            ogs_assert(list0->tai[j].num < MAX_NUM_OF_TAI);

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
            ogs_assert(list2->type == TAI1_TYPE || list2->type == TAI2_TYPE);
            ogs_assert(list2->num < MAX_NUM_OF_TAI);

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

int mme_m_tmsi_pool_generate()
{
    int i, j;
    int index = 0;

    ogs_trace("M-TMSI Pool try to generate...");
    for (i = 0; index < MAX_POOL_OF_UE; i++)
    {
        mme_m_tmsi_t *m_tmsi = NULL;
        int conflict = 0;

        m_tmsi = &self.m_tmsi.array[index];
        ogs_assert(m_tmsi);
        *m_tmsi = ogs_random32();

        /* for mapped-GUTI */
        *m_tmsi |= 0xc0000000;
        *m_tmsi &= 0xff00ffff;

        for (j = 0; j < index; j++)
        {
            if (*m_tmsi == self.m_tmsi.array[j])
            {
                conflict = 1;
                ogs_trace("[M-TMSI CONFLICT]  %d:0x%x == %d:0x%x",
                        index, *m_tmsi, j, self.m_tmsi.array[j]);
                break;
            }
        }
        if (conflict == 1)
        {
            continue;
        }

        index++;
    }
    self.m_tmsi.size = index;
    ogs_trace("M-TMSI Pool generate...done");

    return OGS_OK;
}

mme_m_tmsi_t *mme_m_tmsi_alloc()
{
    mme_m_tmsi_t *m_tmsi = NULL;

    ogs_pool_alloc(&self.m_tmsi, &m_tmsi);
    ogs_assert(m_tmsi);

    return m_tmsi;
}

int mme_m_tmsi_free(mme_m_tmsi_t *m_tmsi)
{
    ogs_assert(m_tmsi);
    ogs_pool_free(&self.m_tmsi, m_tmsi);

    return OGS_OK;
}


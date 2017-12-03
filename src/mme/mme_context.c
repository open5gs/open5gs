#define TRACE_MODULE _mme_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"
#include "core_msgq.h"
#include "core_fsm.h"

#include <mongoc.h>
#include <yaml.h>
#include "yaml_helper.h"

#include "s1ap_message.h"
#include "gtp_xact.h"
#include "gtp_node.h"
#include "fd_lib.h"

#include "context.h"
#include "nas_conv.h"
#include "mme_context.h"
#include "mme_event.h"
#include "s1ap_path.h"
#include "mme_sm.h"

#define MAX_CELL_PER_ENB            8

static mme_context_t self;

pool_declare(mme_s1ap_pool, mme_s1ap_t, MAX_NUM_OF_S1AP_SERVER);

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

    /* Initialize MME context */
    memset(&self, 0, sizeof(mme_context_t));

    pool_init(&mme_s1ap_pool, MAX_NUM_OF_S1AP_SERVER);
    list_init(&self.s1ap_list);

    list_init(&self.gtpc4_list);
    list_init(&self.gtpc6_list);

    gtp_node_init();
    list_init(&self.sgw_list);
    list_init(&self.pgw_list);

    index_init(&mme_enb_pool, MAX_NUM_OF_ENB);
    index_init(&mme_ue_pool, MAX_POOL_OF_UE);
    index_init(&enb_ue_pool, MAX_POOL_OF_UE);
    index_init(&mme_sess_pool, MAX_POOL_OF_SESS);
    index_init(&mme_bearer_pool, MAX_POOL_OF_BEARER);

    self.enb_sock_hash = hash_make();
    self.enb_addr_hash = hash_make();
    self.enb_id_hash = hash_make();
    self.mme_ue_s1ap_id_hash = hash_make();
    self.imsi_ue_hash = hash_make();
    self.guti_ue_hash = hash_make();

    /* Timer value */
    self.t3413_value = 2; /* Paging retry Timer: 2 secs */

    context_initialized = 1;

    return CORE_OK;
}

status_t mme_context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "MME context already has been finalized");

    mme_s1ap_remove_all();

    mme_enb_remove_all();
    mme_ue_remove_all();

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

    index_final(&mme_bearer_pool);
    index_final(&mme_sess_pool);
    index_final(&mme_ue_pool);
    index_final(&enb_ue_pool);

    index_final(&mme_enb_pool);

    gtp_remove_all_nodes(&self.sgw_list);
    gtp_remove_all_nodes(&self.pgw_list);
    gtp_node_final();

    sock_remove_all_nodes(&self.gtpc4_list);
    sock_remove_all_nodes(&self.gtpc6_list);

    pool_final(&mme_s1ap_pool);

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

    self.gtpc_port = GTPV2_C_UDP_PORT;

    return CORE_OK;
}

static status_t mme_context_validation()
{
    if (self.fd_conf_path == NULL)
    {
        d_error("No mme.freeDiameter in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (mme_s1ap_first() == NULL)
    {
        d_error("No mme.s1ap in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (list_first(&self.gtpc4_list) == NULL &&
        list_first(&self.gtpc6_list) == NULL)
    {
        d_error("No mme.gtpc in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (list_first(&self.sgw_list) == NULL)
    {
        d_error("No sgw.gtpc in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    self.sgw = list_first(&self.sgw_list);

    if (list_first(&self.pgw_list) == NULL)
    {
        d_error("No pgw.gtpc in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    self.pgw = list_first(&self.pgw_list);

    if (self.max_num_of_served_gummei == 0)
    {
        d_error("No mme.gummei in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_plmn_id == 0)
    {
        d_error("No mme.gummei.plmn_id in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_mme_gid == 0)
    {
        d_error("No mme.gummei.mme_gid in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_mme_code == 0)
    {
        d_error("No mme.gummei.mme_code in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.max_num_of_served_tai == 0)
    {
        d_error("No mme.tai in '%s'",
                context_self()->config.path);
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
                    self.fd_conf_path = yaml_iter_value(&mme_iter);
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
                        mme_s1ap_t *s1ap = NULL;
                        int family = AF_UNSPEC;
                        const char *hostname = NULL;
                        c_uint16_t port = S1AP_SCTP_PORT;

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
                                    d_warn("Ignore family(%d) : AF_UNSPEC(0), "
                                        "AF_INET(2), AF_INET6(30) ", family);
                                    family = AF_UNSPEC;
                                }
                            }
                            else if (!strcmp(s1ap_key, "addr") ||
                                    !strcmp(s1ap_key, "name"))
                            {
                                hostname = yaml_iter_value(&s1ap_iter);
                            }
                            else if (!strcmp(s1ap_key, "port"))
                            {
                                const char *v = yaml_iter_value(&s1ap_iter);
                                if (v) port = atoi(v);
                            }
                            else
                                d_warn("unknown key `%s`", s1ap_key);
                        }

                        s1ap = mme_s1ap_add(family, hostname, port);
                        d_assert(s1ap, return CORE_ERROR,);

                    } while(yaml_iter_type(&s1ap_array) == YAML_SEQUENCE_NODE);
                }
                else if (!strcmp(mme_key, "gtpc"))
                {
                    yaml_iter_t gtpc_array, gtpc_iter;
                    yaml_iter_recurse(&mme_iter, &gtpc_array);
                    do
                    {
                        sock_node_t *node = NULL;
                        int family = AF_UNSPEC;
                        const char *hostname = NULL;
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
                                    d_warn("Ignore family(%d) : AF_UNSPEC(0), "
                                        "AF_INET(2), AF_INET6(30) ", family);
                                    family = AF_UNSPEC;
                                }
                            }
                            else if (!strcmp(gtpc_key, "addr") ||
                                    !strcmp(gtpc_key, "name"))
                            {
                                hostname = yaml_iter_value(&gtpc_iter);
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

                        if (context_self()->parameter.no_ipv4 == 0)
                        {
                            node = sock_add_node(&self.gtpc4_list,
                                family, hostname, port);
                            d_assert(node, return CORE_ERROR,);
                            rv = sock_filter_node(
                                    &self.gtpc4_list, AF_INET);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }
                        
                        if (context_self()->parameter.no_ipv6 == 0)
                        {
                            node = sock_add_node(&self.gtpc6_list,
                                family, hostname, port);
                            d_assert(node, return CORE_ERROR,);
                            rv = sock_filter_node(
                                    &self.gtpc6_list, AF_INET6);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }
                    } while(yaml_iter_type(&gtpc_array) == YAML_SEQUENCE_NODE);

                    if (list_first(&self.gtpc4_list) == NULL &&
                        list_first(&self.gtpc6_list) == NULL)
                    {
                        if (context_self()->parameter.no_ipv4 == 0)
                        {
                            rv = sock_get_all_nodes(
                                    &self.gtpc4_list, self.gtpc_port);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                            rv = sock_filter_node(
                                    &self.gtpc4_list, AF_INET);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }

                        if (context_self()->parameter.no_ipv6 == 0)
                        {
                            rv = sock_get_all_nodes(
                                    &self.gtpc6_list, self.gtpc_port);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                            rv = sock_filter_node(&self.gtpc6_list,
                                    AF_INET6);
                            d_assert(rv == CORE_OK, return CORE_ERROR,);
                        }
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
                    yaml_iter_t tai_array, tai_iter;
                    yaml_iter_recurse(&mme_iter, &tai_array);
                    do
                    {
                        tai_t *tai = NULL;
                        plmn_id_t *plmn_id = NULL;
                        const char *mcc = NULL, *mnc = NULL;
                        c_uint16_t tac = 0;

                        d_assert(self.max_num_of_served_tai <=
                                MAX_NUM_OF_SERVED_TAI, return CORE_ERROR,);
                        tai = &self.served_tai[
                            self.max_num_of_served_tai];
                        d_assert(tai, return CORE_ERROR,);
                        plmn_id = &tai->plmn_id;
                        d_assert(plmn_id, return CORE_ERROR,);

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
                                }
                            }
                            else if (!strcmp(tai_key, "tac"))
                            {
                                const char *v = yaml_iter_value(&tai_iter);
                                if (v) tac = atoi(v);
                            }
                            else
                                d_warn("unknown key `%s`", tai_key);
                        }

                        if (mcc && mnc && tac)
                        {
                            self.max_num_of_served_tai++;
                        }
                        else
                        {
                            d_warn("Ignore tai : mcc(%p), mnc(%p), tac(%d)",
                                mcc, mnc, tac);
                        }
                    } while(yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE);
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
                        int family = AF_UNSPEC;
                        const char *hostname = NULL;
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
                                    d_warn("Ignore family(%d) : AF_UNSPEC(0), "
                                        "AF_INET(2), AF_INET6(30) ", family);
                                    family = AF_UNSPEC;
                                }
                            }
                            else if (!strcmp(gtpc_key, "addr") ||
                                    !strcmp(gtpc_key, "name"))
                            {
                                hostname = yaml_iter_value(&gtpc_iter);
                            }
                            else if (!strcmp(gtpc_key, "port"))
                            {
                                const char *v = yaml_iter_value(&gtpc_iter);
                                if (v) port = atoi(v);
                            }
                            else
                                d_warn("unknown key `%s`", gtpc_key);
                        }

                        sgw = gtp_add_node(&self.sgw_list,
                                family, hostname, port);
                        d_assert(sgw, return CORE_ERROR,);

                    } while(yaml_iter_type(&gtpc_array) == YAML_SEQUENCE_NODE);

                    if (context_self()->parameter.no_ipv4 == 1)
                    {
                        rv = gtp_filter_node(&self.sgw_list, AF_INET6);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
                    if (context_self()->parameter.no_ipv6 == 1)
                    {
                        rv = gtp_filter_node(&self.sgw_list, AF_INET);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
                    if (context_self()->parameter.prefer_ipv4 == 1)
                    {
                        rv = gtp_sort_node(&self.sgw_list, AF_INET);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
                    else
                    {
                        rv = gtp_sort_node(&self.sgw_list, AF_INET6);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
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
                        int family = AF_UNSPEC;
                        const char *hostname = NULL;
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
                                    d_warn("Ignore family(%d) : AF_UNSPEC(0), "
                                        "AF_INET(2), AF_INET6(30) ", family);
                                    family = AF_UNSPEC;
                                }
                            }
                            else if (!strcmp(gtpc_key, "addr") ||
                                    !strcmp(gtpc_key, "name"))
                            {
                                hostname = yaml_iter_value(&gtpc_iter);
                            }
                            else if (!strcmp(gtpc_key, "port"))
                            {
                                const char *v = yaml_iter_value(&gtpc_iter);
                                if (v) port = atoi(v);
                            }
                            else
                                d_warn("unknown key `%s`", gtpc_key);
                        }

                        pgw = gtp_add_node(&self.pgw_list,
                                family, hostname, port);
                        d_assert(pgw, return CORE_ERROR,);

                    } while(yaml_iter_type(&gtpc_array) == YAML_SEQUENCE_NODE);

                    if (context_self()->parameter.no_ipv4 == 1)
                    {
                        rv = gtp_filter_node(&self.pgw_list, AF_INET6);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
                    if (context_self()->parameter.no_ipv6 == 1)
                    {
                        rv = gtp_filter_node(&self.pgw_list, AF_INET);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
                    if (context_self()->parameter.prefer_ipv4 == 1)
                    {
                        rv = gtp_sort_node(&self.pgw_list, AF_INET);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
                    else
                    {
                        rv = gtp_sort_node(&self.pgw_list, AF_INET6);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                    }
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
    int s1ap = context_self()->logger.trace.s1ap;
    int nas = context_self()->logger.trace.nas;
    int diameter = context_self()->logger.trace.diameter;
    int gtp = context_self()->logger.trace.gtp;
    int others = context_self()->logger.trace.others;

    if (s1ap)
    {
        extern int _s1ap_sm;
        d_trace_level(&_s1ap_sm, s1ap);
        extern int _s1ap_build;
        d_trace_level(&_s1ap_build, s1ap);
        extern int _s1ap_handler;
        d_trace_level(&_s1ap_handler, s1ap);
        extern int _s1ap_sctp;
        d_trace_level(&_s1ap_sctp, s1ap);
        extern int _s1ap_path;
        d_trace_level(&_s1ap_path, s1ap);
        extern int _s1ap_recv;
        d_trace_level(&_s1ap_recv, s1ap);
        extern int _s1ap_send;
        d_trace_level(&_s1ap_send, s1ap);
        extern int _s1ap_ies_decoder;
        d_trace_level(&_s1ap_ies_decoder, s1ap);
        extern int _s1ap_ies_encoder;
        d_trace_level(&_s1ap_ies_encoder, s1ap);
    }

    if (nas)
    {
        extern int _emm_sm;
        d_trace_level(&_emm_sm, nas);
        extern int _esm_sm;
        d_trace_level(&_esm_sm, nas);
        extern int _emm_handler;
        d_trace_level(&_emm_handler, nas);
        extern int _esm_handler;
        d_trace_level(&_esm_handler, nas);
        extern int _nas_decoder;
        d_trace_level(&_nas_decoder, nas);
        extern int _nas_encoder;
        d_trace_level(&_nas_encoder, nas);
        extern int _nas_ies;
        d_trace_level(&_nas_ies, nas);
    }

    if (diameter)
    {
        if (diameter <= 1) fd_g_debug_lvl = FD_LOG_ERROR;
        else if (diameter <= 3) fd_g_debug_lvl = FD_LOG_NOTICE;
        else if (diameter <= 5) fd_g_debug_lvl = FD_LOG_DEBUG;
        else fd_g_debug_lvl = FD_LOG_ANNOYING;

        extern int _mme_fd_path;
        d_trace_level(&_mme_fd_path, diameter);
        extern int _fd_init;
        d_trace_level(&_fd_init, diameter);
        extern int _fd_logger;
        d_trace_level(&_fd_logger, diameter);
    }

    if (gtp)
    {
        extern int _mme_s11_handler;
        d_trace_level(&_mme_s11_handler, gtp);
        extern int _gtp_node;
        d_trace_level(&_gtp_node, gtp);
        extern int _gtp_path;
        d_trace_level(&_gtp_path, gtp);
        extern int _mme_s11_path;
        d_trace_level(&_mme_s11_path, gtp);
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
        extern int _mme_context;
        d_trace_level(&_mme_context, others);
        extern int _mme_sm;
        d_trace_level(&_mme_sm, others);
    }

    return CORE_OK;
}

mme_s1ap_t* mme_s1ap_add(
        int family, const char *hostname, c_uint16_t port)
{
    mme_s1ap_t *s1ap = NULL;

    pool_alloc_node(&mme_s1ap_pool, &s1ap);
    d_assert(s1ap, return NULL, "Null param");
    memset(s1ap, 0, sizeof(mme_s1ap_t));

    s1ap->family = family;
    s1ap->hostname = hostname;
    s1ap->port = port;

    list_append(&self.s1ap_list, s1ap);
    
    return s1ap;
}

status_t mme_s1ap_remove(mme_s1ap_t *s1ap)
{
    d_assert(s1ap, return CORE_ERROR, "Null param");

    list_remove(&self.s1ap_list, s1ap);

    pool_free_node(&mme_s1ap_pool, s1ap);

    return CORE_OK;
}

status_t mme_s1ap_remove_all()
{
    mme_s1ap_t *s1ap = NULL, *next_s1ap = NULL;
    
    s1ap = mme_s1ap_first();
    while (s1ap)
    {
        next_s1ap = mme_s1ap_next(s1ap);

        mme_s1ap_remove(s1ap);

        s1ap = next_s1ap;
    }

    return CORE_OK;
}

mme_s1ap_t* mme_s1ap_first()
{
    return list_first(&self.s1ap_list);
}

mme_s1ap_t* mme_s1ap_next(mme_s1ap_t *s1ap)
{
    return list_next(s1ap);
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

    if (enb->sock_type == SOCK_STREAM)
        s1ap_delete(enb->sock);
    core_free(enb->addr);

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
    mme_s1ap_t *s1ap = NULL;

    d_assert(sock, return 0,);

    for (s1ap = mme_s1ap_first(); s1ap; s1ap = mme_s1ap_next(s1ap))
    {
        if (s1ap->sock == sock) return SOCK_SEQPACKET;
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

    enb_ue->mme_ue_s1ap_id = NEXT_ID(self.mme_ue_s1ap_id, 1, 0xffffffff);
    enb_ue->enb = enb;

    hash_set(self.mme_ue_s1ap_id_hash, &enb_ue->mme_ue_s1ap_id, 
            sizeof(enb_ue->mme_ue_s1ap_id), enb_ue);
    list_append(&enb->enb_ue_list, enb_ue);

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

    list_remove(&enb_ue->enb->enb_ue_list, enb_ue);
    hash_set(self.mme_ue_s1ap_id_hash, &enb_ue->mme_ue_s1ap_id, 
            sizeof(enb_ue->mme_ue_s1ap_id), NULL);

    rv = source_ue_deassociate_target_ue(enb_ue);
    d_assert(rv == CORE_OK, return CORE_ERROR, "Null param");

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

mme_ue_t* mme_ue_add(enb_ue_t *enb_ue)
{
    mme_ue_t *mme_ue = NULL;
    event_t e;

    d_assert(enb_ue, return NULL, "Null param");

    index_alloc(&mme_ue_pool, &mme_ue);
    d_assert(mme_ue, return NULL, "Null param");

    list_init(&mme_ue->sess_list);

    mme_ue->mme_s11_teid = mme_ue->index;
    mme_ue->mme_s11_ipv4 = mme_self()->gtpc4_addr;
    mme_ue->mme_s11_ipv6 = mme_self()->gtpc6_addr;

    CONNECT_SGW_GTP_NODE(mme_ue);
    
    /* Create t3413 timer */
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
    event_t e;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    fsm_final(&mme_ue->sm, &e);
    fsm_clear(&mme_ue->sm);

    /* Clear hash table */
    if (mme_ue->guti.m_tmsi != 0)
        hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), NULL);
    if (mme_ue->imsi_len != 0)
        hash_set(self.imsi_ue_hash, mme_ue->imsi, mme_ue->imsi_len, NULL);
    
    /* Clear the saved PDN Connectivity Request */
    NAS_CLEAR_DATA(&mme_ue->pdn_connectivity_request);

    /* Clear Paging info : t3413, last_paing_msg */
    CLEAR_PAGING_INFO(mme_ue);

    /* Free UeRadioCapability */
    if (mme_ue->radio_capa)
    {
        S1ap_UERadioCapability_t *radio_capa = 
            (S1ap_UERadioCapability_t *)mme_ue->radio_capa;

        if (radio_capa->buf)
            core_free(radio_capa->buf);
        core_free(mme_ue->radio_capa);
    }

    /* Clear Transparent Container */
    S1AP_CLEAR_DATA(&mme_ue->container);

    mme_sess_remove_all(mme_ue);
    mme_pdn_remove_all(mme_ue);

    mme_ue_deassociate_enb_ue(mme_ue->enb_ue);

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
                        d_trace(3,"known UE by IMSI[%s]\n", imsi_bcd);
                    }
                    else
                    {
                        d_trace(3,"Unknown UE by IMSI[%s]\n", imsi_bcd);
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
                        d_trace(3, "Known UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]\n",
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
                        d_trace(3, "Known UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]\n",
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

/* At this point, I'm not sure whether this function is exported or not */
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

    if (mme_ue->guti.m_tmsi != 0)
    {
        /* MME has a VALID GUIT
         * As such, we need to remove previous GUTI in hash table */
        hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), NULL);
    }

    memset(&mme_ue->guti, 0, sizeof(guti_t));

    /* FIXME : How to generate GUTI?
     * At this point, I'll use first(0-index) Served GUMMEI in MME_CONTEXT */
    memcpy(&mme_ue->guti.plmn_id, &served_gummei->plmn_id[0], PLMN_ID_LEN);
    mme_ue->guti.mme_gid = served_gummei->mme_gid[0];
    mme_ue->guti.mme_code = served_gummei->mme_code[0];

    mme_ue->guti.m_tmsi = NEXT_ID(self.m_tmsi, 1, 0xffffffff);

    hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), mme_ue);

    return CORE_OK;
}

status_t mme_ue_set_imsi(mme_ue_t *mme_ue, c_int8_t *imsi_bcd)
{
    d_assert(mme_ue && imsi_bcd, return CORE_ERROR, "Invalid param");

    core_cpystrn(mme_ue->imsi_bcd, imsi_bcd, MAX_IMSI_BCD_LEN+1);
    core_bcd_to_buffer(mme_ue->imsi_bcd, mme_ue->imsi, &mme_ue->imsi_len);

    hash_set(self.imsi_ue_hash, mme_ue->imsi, mme_ue->imsi_len, mme_ue);

    mme_ue_new_guti(mme_ue);

    return CORE_OK;
}

/* 
 * S1AP Initial UE-Message : S-TMSI
 * NAS ATTACH_REQUEST : IMSI, GUTI
 * NAS TAU_REQUEST : GUTI
 * S1AP Handover Notification
 */
status_t mme_ue_associate_enb_ue(mme_ue_t *mme_ue, enb_ue_t *enb_ue)
{
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    mme_ue->enb_ue = enb_ue;
    enb_ue->mme_ue = mme_ue;

    return CORE_OK;
}

/* 
 * mme_ue_remove()
 *
 * Note : should not call in enb_ue_remove()
 *
 * When mme_ue is removed, enb_ue->mme_ue must be NULL.
 * However, when enb_ue is removed, mme_ue->enb_ue need not be NULL.
 * mme_ue->enb_ue will be updated again when enb_ue is added.
 */
status_t mme_ue_deassociate_enb_ue(enb_ue_t *enb_ue)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    mme_ue->enb_ue = NULL;
    enb_ue->mme_ue = NULL;

    return CORE_OK;
}

/* 
 * S1AP Handover Required
 */
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

/* 
 * enb_ue_remove()
 *
 * enb_ue->mme_ue->enb_ue should not be set to NULL.
 * This is because enb_ue is not known as source_ue or target_ue.
 * Therefore, when enb_ue is removed, leave enb_ue->mme_ue->enb_ue as is.
 */
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

    if (message->esm.h.message_type == NAS_PDN_DISCONNECT_REQUEST)
    {
        nas_pdn_disconnect_request_t *pdn_disconnect_request = 
            &message->esm.pdn_disconnect_request;
        nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity =
            &pdn_disconnect_request->linked_eps_bearer_identity;

        bearer = mme_bearer_find_by_ue_ebi(mme_ue,
                linked_eps_bearer_identity->eps_bearer_identity);
        d_assert(bearer, return NULL,
                "Invalid pti(%d) and ebi(%d)\n", pti, ebi);
        sess = bearer->sess;
        d_assert(sess, return NULL, "Null param");
        sess->pti = pti;

        return bearer;
    }

    pti = message->esm.h.procedure_transaction_identity;
    ebi = message->esm.h.eps_bearer_identity;

    if (ebi != NAS_EPS_BEARER_IDENTITY_UNASSIGNED)
    {
        bearer = mme_bearer_find_by_ue_ebi(mme_ue, ebi);
        d_assert(bearer, return NULL,);
        return bearer;
    }
        
    d_assert(pti != NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
            return NULL, );

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

        d_assert(sess, return NULL,);
    }
    else
    {
        sess = mme_sess_find_by_pti(mme_ue, pti);
        d_assert(sess, return NULL,);
    }

    bearer = mme_default_bearer_in_sess(sess);
    d_assert(bearer, return NULL,);
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

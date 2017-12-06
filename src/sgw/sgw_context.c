#define TRACE_MODULE _sgw_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"
#include "core_lib.h"

#include <mongoc.h>
#include <yaml.h>
#include "yaml_helper.h"

#include "types.h"
#include "gtp_types.h"
#include "gtp_conv.h"
#include "gtp_node.h"
#include "gtp_path.h"
#include "gtp_xact.h"

#include "context.h"
#include "sgw_context.h"

static sgw_context_t self;

index_declare(sgw_ue_pool, sgw_ue_t, MAX_POOL_OF_UE);
index_declare(sgw_sess_pool, sgw_sess_t, MAX_POOL_OF_SESS);
index_declare(sgw_bearer_pool, sgw_bearer_t, MAX_POOL_OF_BEARER);
index_declare(sgw_tunnel_pool, sgw_tunnel_t, MAX_POOL_OF_TUNNEL);

static int context_initialized = 0;

status_t sgw_context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "SGW context already has been initialized");

    memset(&self, 0, sizeof(sgw_context_t));

    list_init(&self.gtpc_list);
    list_init(&self.gtpc_list6);

    gtp_node_init();
    list_init(&self.mme_list);
    list_init(&self.pgw_list);

    index_init(&sgw_ue_pool, MAX_POOL_OF_UE);
    index_init(&sgw_sess_pool, MAX_POOL_OF_SESS);
    index_init(&sgw_bearer_pool, MAX_POOL_OF_BEARER);
    index_init(&sgw_tunnel_pool, MAX_POOL_OF_TUNNEL);

    self.imsi_ue_hash = hash_make();

    context_initialized = 1;

    return CORE_OK;
}

status_t sgw_context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "SGW context already has been finalized");

    sgw_ue_remove_all();

    d_assert(self.imsi_ue_hash, , "Null param");
    hash_destroy(self.imsi_ue_hash);

    index_final(&sgw_tunnel_pool);
    index_final(&sgw_bearer_pool);
    index_final(&sgw_sess_pool);
    index_final(&sgw_ue_pool);

    gtp_remove_all_nodes(&self.mme_list);
    gtp_remove_all_nodes(&self.pgw_list);
    gtp_node_final();

    sock_remove_all_nodes(&self.gtpc_list);
    sock_remove_all_nodes(&self.gtpc_list6);

    context_initialized = 0;
    
    return CORE_OK;
}

sgw_context_t* sgw_self()
{
    return &self;
}

static status_t sgw_context_prepare()
{
    self.gtpc_port = GTPV2_C_UDP_PORT;
    self.gtpu_port = GTPV1_U_UDP_PORT;

    return CORE_OK;
}

static status_t sgw_context_validation()
{
    if (list_first(&self.gtpc_list) == NULL &&
        list_first(&self.gtpc_list6) == NULL)
    {
        d_error("No sgw.gtpc in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.gtpu_addr == 0)
    {
        d_error("No sgw.gtpu in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    return CORE_OK;
}

status_t sgw_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;
    yaml_document_t *document = NULL;
    yaml_iter_t root_iter;

    d_assert(config, return CORE_ERROR,);
    document = config->document;
    d_assert(document, return CORE_ERROR,);

    rv = sgw_context_prepare();
    if (rv != CORE_OK) return rv;

    yaml_iter_init(&root_iter, document);
    while(yaml_iter_next(&root_iter))
    {
        const char *root_key = yaml_iter_key(&root_iter);
        d_assert(root_key, return CORE_ERROR,);
        if (!strcmp(root_key, "sgw"))
        {
            yaml_iter_t sgw_iter;
            yaml_iter_recurse(&root_iter, &sgw_iter);
            while(yaml_iter_next(&sgw_iter))
            {
                const char *sgw_key = yaml_iter_key(&sgw_iter);
                d_assert(sgw_key, return CORE_ERROR,);
                if (!strcmp(sgw_key, "gtpc"))
                {
                    yaml_iter_t gtpc_array, gtpc_iter;
                    yaml_iter_recurse(&sgw_iter, &gtpc_array);
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
                else if (!strcmp(sgw_key, "gtpu"))
                {
                    yaml_iter_t gtpu_array, gtpu_iter;
                    yaml_iter_recurse(&sgw_iter, &gtpu_array);
                    do
                    {
#if 0
                        sgw_gtpu_t *gtpu = NULL;
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
                        gtpu = sgw_gtpu_add(family, hostname, port);
                        d_assert(gtpu, return CORE_ERROR,);
#endif

                    } while(yaml_iter_type(&gtpu_array) == YAML_SEQUENCE_NODE);
                }
                else
                    d_warn("unknown key `%s`", sgw_key);
            }
        }
    }

    rv = sgw_context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

status_t sgw_context_setup_trace_module()
{
    int gtp = context_self()->logger.trace.gtp;
    int others = context_self()->logger.trace.others;

    if (gtp)
    {
        extern int _sgw_sm;
        d_trace_level(&_sgw_sm, gtp);
        extern int _sgw_s11_handler;
        d_trace_level(&_sgw_s11_handler, gtp);
        extern int _sgw_s11_handler;
        d_trace_level(&_sgw_s11_handler, gtp);
        extern int _gtp_node;
        d_trace_level(&_gtp_node, gtp);
        extern int _gtp_path;
        d_trace_level(&_gtp_path, gtp);
        extern int _sgw_path;
        d_trace_level(&_sgw_path, gtp);
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
        extern int _sgw_context;
        d_trace_level(&_sgw_context, others);
    }

    return CORE_OK;
}

sgw_ue_t* sgw_ue_add(
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn, c_uint8_t ebi)
{
    sgw_ue_t *sgw_ue = NULL;
    sgw_sess_t *sess = NULL;

    d_assert(imsi, return NULL, "Null param");
    d_assert(imsi_len, return NULL, "Null param");
    d_assert(apn, return NULL, "Null param");
    d_assert(ebi, return NULL, "Null param");

    index_alloc(&sgw_ue_pool, &sgw_ue);
    d_assert(sgw_ue, return NULL, "Null param");

    sgw_ue->sgw_s11_teid = sgw_ue->index;
    sgw_ue->sgw_s11_ipv4 = sgw_self()->gtpc_addr;
    sgw_ue->sgw_s11_ipv6 = sgw_self()->gtpc_addr6;

    /* Set IMSI */
    sgw_ue->imsi_len = imsi_len;
    memcpy(sgw_ue->imsi, imsi, sgw_ue->imsi_len);
    core_buffer_to_bcd(sgw_ue->imsi, sgw_ue->imsi_len, sgw_ue->imsi_bcd);

    list_init(&sgw_ue->sess_list);

    sess = sgw_sess_add(sgw_ue, apn, ebi);
    d_assert(sess, index_free(&sgw_ue_pool, sgw_ue); return NULL, "Null param");

    hash_set(self.imsi_ue_hash, sgw_ue->imsi, sgw_ue->imsi_len, sgw_ue);

    return sgw_ue;
}

status_t sgw_ue_remove(sgw_ue_t *sgw_ue)
{
    d_assert(sgw_ue, return CORE_ERROR, "Null param");

    /* Clear hash table */
    if (sgw_ue->imsi_len != 0)
        hash_set(self.imsi_ue_hash, sgw_ue->imsi, sgw_ue->imsi_len, NULL);
    
    sgw_sess_remove_all(sgw_ue);

    index_free(&sgw_ue_pool, sgw_ue);

    return CORE_OK;
}

status_t sgw_ue_remove_all()
{
    hash_index_t *hi = NULL;
    sgw_ue_t *sgw_ue = NULL;

    for (hi = sgw_ue_first(); hi; hi = sgw_ue_next(hi))
    {
        sgw_ue = sgw_ue_this(hi);
        sgw_ue_remove(sgw_ue);
    }

    return CORE_OK;
}

static sgw_ue_t* sgw_ue_find(index_t index)
{
    d_assert(index, return NULL, "Invalid index = 0x%x", index);
    return index_find(&sgw_ue_pool, index);
}

sgw_ue_t* sgw_ue_find_by_imsi_bcd(c_int8_t *imsi_bcd)
{
    c_uint8_t imsi[MAX_IMSI_LEN];
    int imsi_len = 0;

    d_assert(imsi_bcd, return NULL,"Invalid param");

    core_bcd_to_buffer(imsi_bcd, imsi, &imsi_len);

    return sgw_ue_find_by_imsi(imsi, imsi_len);
}

sgw_ue_t* sgw_ue_find_by_imsi(c_uint8_t *imsi, int imsi_len)
{
    d_assert(imsi && imsi_len, return NULL,"Invalid param");

    return (sgw_ue_t *)hash_get(self.imsi_ue_hash, imsi, imsi_len);
}

sgw_ue_t* sgw_ue_find_by_teid(c_uint32_t teid)
{
    return sgw_ue_find(teid);
}

sgw_ue_t *sgw_ue_find_or_add_by_message(gtp_message_t *gtp_message)
{
    status_t rv;
    sgw_ue_t *sgw_ue = NULL;
    gtp_node_t *mme = NULL;
    gtp_create_session_request_t *req = &gtp_message->create_session_request;

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

    sgw_ue = sgw_ue_find_by_imsi(req->imsi.data, req->imsi.len);
    if (!sgw_ue)
    {
        gtp_f_teid_t *mme_s11_teid = NULL;
        c_int8_t apn[MAX_APN_LEN];

        mme_s11_teid = req->sender_f_teid_for_control_plane.data;
        d_assert(mme_s11_teid, return NULL,);
        mme = gtp_find_by_ip(&sgw_self()->mme_list, &mme_s11_teid->ip);
        if (!mme)
        {
            mme = gtp_add_node_by_f_teid(
                &sgw_self()->mme_list, mme_s11_teid, sgw_self()->gtpc_port,
                context_self()->parameter.no_ipv4,
                context_self()->parameter.no_ipv6,
                context_self()->parameter.prefer_ipv4);
            d_assert(mme, return NULL,);

            rv = gtp_client(mme);
            d_assert(rv == CORE_OK, return NULL,);
        }

        apn_parse(apn, req->access_point_name.data, req->access_point_name.len);
        sgw_ue = sgw_ue_add(
            req->imsi.data, req->imsi.len, apn,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);
        d_assert(sgw_ue, return NULL,);

        CONNECT_MME_GTP_NODE(sgw_ue, mme);
    }

    return sgw_ue;
}

hash_index_t *sgw_ue_first()
{
    d_assert(self.imsi_ue_hash, return NULL, "Null param");
    return hash_first(self.imsi_ue_hash);
}

hash_index_t *sgw_ue_next(hash_index_t *hi)
{
    return hash_next(hi);
}

sgw_ue_t *sgw_ue_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
}

sgw_sess_t *sgw_sess_add(
        sgw_ue_t *sgw_ue, c_int8_t *apn, c_uint8_t ebi)
{
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;

    d_assert(sgw_ue, return NULL, "Null param");
    d_assert(ebi, return NULL, "Invalid EBI(%d)", ebi);

    index_alloc(&sgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->sgw_s5c_teid = SGW_S5C_INDEX_TO_TEID(sess->index);
    sess->sgw_s5c_ipv4 = sgw_self()->gtpc_addr;
    sess->sgw_s5c_ipv6 = sgw_self()->gtpc_addr6;

    /* Set APN */
    core_cpystrn(sess->pdn.apn, apn, MAX_APN_LEN+1);

    sess->sgw_ue = sgw_ue;

    list_init(&sess->bearer_list);

    bearer = sgw_bearer_add(sess);
    d_assert(bearer, sgw_sess_remove(sess); return NULL, 
            "Can't add default bearer context");
    bearer->ebi = ebi;

    list_append(&sgw_ue->sess_list, sess);

    return sess;
}

status_t sgw_sess_remove(sgw_sess_t *sess)
{
    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(sess->sgw_ue, return CORE_ERROR, "Null param");

    list_remove(&sess->sgw_ue->sess_list, sess);

    sgw_bearer_remove_all(sess);

    index_free(&sgw_sess_pool, sess);

    return CORE_OK;
}

status_t sgw_sess_remove_all(sgw_ue_t *sgw_ue)
{
    sgw_sess_t *sess = NULL, *next_sess = NULL;
    
    sess = sgw_sess_first(sgw_ue);
    while (sess)
    {
        next_sess = sgw_sess_next(sess);

        sgw_sess_remove(sess);

        sess = next_sess;
    }

    return CORE_OK;
}

static sgw_sess_t* sgw_sess_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&sgw_sess_pool, index);
}

sgw_sess_t* sgw_sess_find_by_teid(c_uint32_t teid)
{
    return sgw_sess_find(SGW_S5C_TEID_TO_INDEX(teid));
}

sgw_sess_t* sgw_sess_find_by_apn(sgw_ue_t *sgw_ue, c_int8_t *apn)
{
    sgw_sess_t *sess = NULL;
    
    sess = sgw_sess_first(sgw_ue);
    while (sess)
    {
        if (strcmp(sess->pdn.apn, apn) == 0)
            return sess;

        sess = sgw_sess_next(sess);
    }

    return NULL;
}

sgw_sess_t* sgw_sess_find_by_ebi(sgw_ue_t *sgw_ue, c_uint8_t ebi)
{
    sgw_bearer_t *bearer = NULL;

    bearer = sgw_bearer_find_by_ue_ebi(sgw_ue, ebi);
    if (bearer)
        return bearer->sess;

    return NULL;
}

sgw_sess_t* sgw_sess_first(sgw_ue_t *sgw_ue)
{
    return list_first(&sgw_ue->sess_list);
}

sgw_sess_t* sgw_sess_next(sgw_sess_t *sess)
{
    return list_next(sess);
}

sgw_bearer_t* sgw_bearer_add(sgw_sess_t *sess)
{
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL;
    sgw_ue_t *sgw_ue = NULL;

    d_assert(sess, return NULL, "Null param");
    sgw_ue = sess->sgw_ue;
    d_assert(sgw_ue, return NULL, "Null param");

    index_alloc(&sgw_bearer_pool, &bearer);
    d_assert(bearer, return NULL, "Bearer context allocation failed");

    bearer->sgw_ue = sgw_ue;
    bearer->sess = sess;

    list_init(&bearer->tunnel_list);

    tunnel = sgw_tunnel_add(bearer, GTP_F_TEID_S1_U_SGW_GTP_U);
    d_assert(tunnel, return NULL, "Tunnel context allocation failed");

    tunnel = sgw_tunnel_add(bearer, GTP_F_TEID_S5_S8_SGW_GTP_U);
    d_assert(tunnel, return NULL, "Tunnel context allocation failed");

    list_append(&sess->bearer_list, bearer);
    
    return bearer;
}

status_t sgw_bearer_remove(sgw_bearer_t *bearer)
{
    int i;

    d_assert(bearer, return CORE_ERROR, "Null param");
    d_assert(bearer->sess, return CORE_ERROR, "Null param");

    list_remove(&bearer->sess->bearer_list, bearer);

    sgw_tunnel_remove_all(bearer);

    /* Free the buffered packets */
    for (i = 0; i < bearer->num_buffered_pkt; i++)
        pkbuf_free(bearer->buffered_pkts[i]);

    index_free(&sgw_bearer_pool, bearer);

    return CORE_OK;
}

status_t sgw_bearer_remove_all(sgw_sess_t *sess)
{
    sgw_bearer_t *bearer = NULL, *next_bearer = NULL;

    d_assert(sess, return CORE_ERROR, "Null param");
    
    bearer = list_first(&sess->bearer_list);
    while (bearer)
    {
        next_bearer = list_next(bearer);

        sgw_bearer_remove(bearer);

        bearer = next_bearer;
    }

    return CORE_OK;
}

sgw_bearer_t* sgw_bearer_find(index_t index)
{
    d_assert(index && index < MAX_POOL_OF_BEARER, return NULL, 
            "Invalid Index(%d)",index);

    return index_find(&sgw_bearer_pool, index);
}

sgw_bearer_t* sgw_bearer_find_by_sgw_s5u_teid(c_uint32_t sgw_s5u_teid)
{
    return sgw_bearer_find(sgw_s5u_teid);
}

sgw_bearer_t* sgw_bearer_find_by_sess_ebi(sgw_sess_t *sess, c_uint8_t ebi)
{
    sgw_bearer_t *bearer = NULL;

    bearer = sgw_bearer_first(sess);
    while(bearer)
    {
        if (ebi == bearer->ebi)
            return bearer;

        bearer = sgw_bearer_next(bearer);
    }

    return NULL;
}

sgw_bearer_t* sgw_bearer_find_by_ue_ebi(sgw_ue_t *sgw_ue, c_uint8_t ebi)
{
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    
    sess = sgw_sess_first(sgw_ue);
    while (sess)
    {
        bearer = sgw_bearer_find_by_sess_ebi(sess, ebi);
        if (bearer)
        {
            return bearer;
        }

        sess = sgw_sess_next(sess);
    }

    return NULL;
}

sgw_bearer_t* sgw_default_bearer_in_sess(sgw_sess_t *sess)
{
    return sgw_bearer_first(sess);
}

sgw_bearer_t* sgw_bearer_first(sgw_sess_t *sess)
{
    d_assert(sess, return NULL, "Null param");
    return list_first(&sess->bearer_list);
}

sgw_bearer_t* sgw_bearer_next(sgw_bearer_t *bearer)
{
    return list_next(bearer);
}

sgw_tunnel_t* sgw_tunnel_add(sgw_bearer_t *bearer, c_uint8_t interface_type)
{
    sgw_tunnel_t *tunnel = NULL;

    d_assert(bearer, return NULL, "Null param");

    index_alloc(&sgw_tunnel_pool, &tunnel);
    d_assert(tunnel, return NULL, "Tunnel context allocation failed");

    tunnel->interface_type = interface_type;
    tunnel->local_teid = tunnel->index;
    tunnel->local_addr = sgw_self()->gtpu_addr;

    tunnel->bearer = bearer;

    list_append(&bearer->tunnel_list, tunnel);

    return tunnel;
}

status_t sgw_tunnel_remove(sgw_tunnel_t *tunnel)
{
    d_assert(tunnel, return CORE_ERROR, "Null param");
    d_assert(tunnel->bearer, return CORE_ERROR, "Null param");

    list_remove(&tunnel->bearer->tunnel_list, tunnel);
    index_free(&sgw_tunnel_pool, tunnel);

    return CORE_OK;
}

status_t sgw_tunnel_remove_all(sgw_bearer_t *bearer)
{
    sgw_tunnel_t *tunnel = NULL, *next_tunnel = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    
    tunnel = sgw_tunnel_first(bearer);
    while (tunnel)
    {
        next_tunnel = sgw_tunnel_next(tunnel);

        sgw_tunnel_remove(tunnel);

        tunnel = next_tunnel;
    }

    return CORE_OK;
}

sgw_tunnel_t* sgw_tunnel_find(index_t index)
{
    d_assert(index && index < MAX_POOL_OF_TUNNEL, return NULL, 
            "Invalid Index(%d)",index);

    return index_find(&sgw_tunnel_pool, index);
}

sgw_tunnel_t* sgw_tunnel_find_by_teid(c_uint32_t teid)
{
    return sgw_tunnel_find(teid);
}

sgw_tunnel_t* sgw_tunnel_find_by_interface_type(
        sgw_bearer_t *bearer, c_uint8_t interface_type)
{
    sgw_tunnel_t *tunnel = NULL;

    d_assert(bearer, return NULL,);

    tunnel = sgw_tunnel_first(bearer);
    while(tunnel)
    {
        if (tunnel->interface_type == interface_type)
        {
            return tunnel;
        }

        tunnel = sgw_tunnel_next(tunnel);
    }

    return NULL;
}

sgw_tunnel_t* sgw_s1u_tunnel_in_bearer(sgw_bearer_t *bearer)
{
    return sgw_tunnel_find_by_interface_type(
            bearer, GTP_F_TEID_S1_U_SGW_GTP_U);
}
sgw_tunnel_t* sgw_s5u_tunnel_in_bearer(sgw_bearer_t *bearer)
{
    return sgw_tunnel_find_by_interface_type(
            bearer, GTP_F_TEID_S5_S8_SGW_GTP_U);
}

sgw_tunnel_t* sgw_tunnel_first(sgw_bearer_t *bearer)
{
    d_assert(bearer, return NULL, "Null param");
    return list_first(&bearer->tunnel_list);
}

sgw_tunnel_t* sgw_tunnel_next(sgw_tunnel_t *tunnel)
{
    return list_next(tunnel);
}

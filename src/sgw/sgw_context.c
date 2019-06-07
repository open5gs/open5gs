#include <mongoc.h>
#include <yaml.h>

#include "gtp/gtp_types.h"
#include "gtp/gtp_conv.h"
#include "gtp/gtp_node.h"
#include "gtp/gtp_path.h"
#include "gtp/gtp_xact.h"

#include "app/context.h"
#include "sgw_context.h"

static sgw_context_t self;

int __sgw_log_domain;

static OGS_POOL(sgw_ue_pool, sgw_ue_t);
static OGS_POOL(sgw_sess_pool, sgw_sess_t);
static OGS_POOL(sgw_bearer_pool, sgw_bearer_t);
static OGS_POOL(sgw_tunnel_pool, sgw_tunnel_t);

static int context_initialized = 0;

void sgw_context_init()
{
    ogs_assert(context_initialized == 0);

    memset(&self, 0, sizeof(sgw_context_t));

    ogs_log_install_domain(&__sgw_log_domain, "sgw", ogs_core()->log.level);

    ogs_list_init(&self.gtpc_list);
    ogs_list_init(&self.gtpc_list6);
    ogs_list_init(&self.gtpu_list);
    ogs_list_init(&self.gtpu_list6);

    gtp_node_init();
    ogs_list_init(&self.mme_s11_list);
    ogs_list_init(&self.pgw_s5c_list);
    ogs_list_init(&self.enb_s1u_list);
    ogs_list_init(&self.pgw_s5u_list);

    ogs_pool_init(&sgw_ue_pool, context_self()->pool.ue);
    ogs_pool_init(&sgw_sess_pool, context_self()->pool.sess);
    ogs_pool_init(&sgw_bearer_pool, context_self()->pool.bearer);
    ogs_pool_init(&sgw_tunnel_pool, context_self()->pool.tunnel);

    self.imsi_ue_hash = ogs_hash_make();

    context_initialized = 1;
}

void sgw_context_final()
{
    ogs_assert(context_initialized == 1);

    sgw_ue_remove_all();

    ogs_assert(self.imsi_ue_hash);
    ogs_hash_destroy(self.imsi_ue_hash);

    ogs_pool_final(&sgw_tunnel_pool);
    ogs_pool_final(&sgw_bearer_pool);
    ogs_pool_final(&sgw_sess_pool);
    ogs_pool_final(&sgw_ue_pool);

    gtp_node_remove_all(&self.mme_s11_list);
    gtp_node_remove_all(&self.pgw_s5c_list);
    gtp_node_remove_all(&self.enb_s1u_list);
    gtp_node_remove_all(&self.pgw_s5u_list);
    gtp_node_final();

    context_initialized = 0;
}

sgw_context_t* sgw_self()
{
    return &self;
}

static int sgw_context_prepare()
{
    self.gtpc_port = GTPV2_C_UDP_PORT;
    self.gtpu_port = GTPV1_U_UDP_PORT;

    return OGS_OK;
}

static int sgw_context_validation()
{
    if (ogs_list_empty(&self.gtpc_list) &&
        ogs_list_empty(&self.gtpc_list6))
    {
        ogs_error("No sgw.gtpc in '%s'",
                context_self()->config.path);
        return OGS_ERROR;
    }
    if (ogs_list_empty(&self.gtpu_list) &&
        ogs_list_empty(&self.gtpu_list6))
    {
        ogs_error("No sgw.gtpu in '%s'",
                context_self()->config.path);
        return OGS_RETRY;
    }
    return OGS_OK;
}

int sgw_context_parse_config()
{
    int rv;
    config_t *config = &context_self()->config;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    ogs_assert(config);
    document = config->document;
    ogs_assert(document);

    rv = sgw_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "sgw")) {
            ogs_yaml_iter_t sgw_iter;
            ogs_yaml_iter_recurse(&root_iter, &sgw_iter);
            while (ogs_yaml_iter_next(&sgw_iter)) {
                const char *sgw_key = ogs_yaml_iter_key(&sgw_iter);
                ogs_assert(sgw_key);
                if (!strcmp(sgw_key, "gtpc")) {
                    ogs_yaml_iter_t gtpc_array, gtpc_iter;
                    ogs_yaml_iter_recurse(&sgw_iter, &gtpc_array);
                    do {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.gtpc_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;

                        if (ogs_yaml_iter_type(&gtpc_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&gtpc_iter, &gtpc_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&gtpc_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&gtpc_array))
                                break;
                            ogs_yaml_iter_recurse(&gtpc_array, &gtpc_iter);
                        } else if (ogs_yaml_iter_type(&gtpc_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&gtpc_iter)) {
                            const char *gtpc_key =
                                ogs_yaml_iter_key(&gtpc_iter);
                            ogs_assert(gtpc_key);
                            if (!strcmp(gtpc_key, "family"))
                            {
                                const char *v = ogs_yaml_iter_value(&gtpc_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(gtpc_key, "addr") ||
                                    !strcmp(gtpc_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&gtpc_iter, &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num <= MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] = 
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(gtpc_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&gtpc_iter);
                                if (v) {
                                    port = atoi(v);
                                    self.gtpc_port = port;
                                }
                            } else if (!strcmp(gtpc_key, "dev")) {
                                dev = ogs_yaml_iter_value(&gtpc_iter);
                            }
                            else
                                ogs_warn("unknown key `%s`", gtpc_key);
                        }

                        addr = NULL;
                        for (i = 0; i < num; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        if (addr) {
                            if (context_self()->config.parameter.no_ipv4 == 0) {
                                ogs_sockaddr_t *dup = NULL;
                                rv = ogs_copyaddrinfo(&dup, addr);
                                ogs_assert(rv == OGS_OK);
                                ogs_socknode_add(
                                        &self.gtpc_list, AF_INET, dup);
                            }

                            if (context_self()->config.parameter.no_ipv6 == 0) {
                                ogs_sockaddr_t *dup = NULL;
                                rv = ogs_copyaddrinfo(&dup, addr);
                                ogs_assert(rv == OGS_OK);
                                ogs_socknode_add(
                                        &self.gtpc_list6, AF_INET6, dup);
                            }

                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                    context_self()->config.parameter.no_ipv4 ?
                                        NULL : &self.gtpc_list,
                                    context_self()->config.parameter.no_ipv6 ?
                                        NULL : &self.gtpc_list6,
                                    dev, self.gtpc_port);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while (ogs_yaml_iter_type(&gtpc_array) == 
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_empty(&self.gtpc_list) &&
                        ogs_list_empty(&self.gtpc_list6)) {
                        rv = ogs_socknode_probe(
                                context_self()->config.parameter.no_ipv4 ?
                                    NULL : &self.gtpc_list,
                                context_self()->config.parameter.no_ipv6 ?
                                    NULL : &self.gtpc_list6,
                                NULL, self.gtpc_port);
                        ogs_assert(rv == OGS_OK);
                    }
                } else if (!strcmp(sgw_key, "gtpu")) {
                    ogs_yaml_iter_t gtpu_array, gtpu_iter;
                    ogs_yaml_iter_recurse(&sgw_iter, &gtpu_array);
                    do {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.gtpu_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;

                        if (ogs_yaml_iter_type(&gtpu_array) == 
                                YAML_MAPPING_NODE) {
                            memcpy(&gtpu_iter, &gtpu_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&gtpu_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&gtpu_array))
                                break;
                            ogs_yaml_iter_recurse(&gtpu_array, &gtpu_iter);
                        } else if (ogs_yaml_iter_type(&gtpu_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&gtpu_iter)) {
                            const char *gtpu_key =
                                ogs_yaml_iter_key(&gtpu_iter);
                            ogs_assert(gtpu_key);
                            if (!strcmp(gtpu_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&gtpu_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(gtpu_key, "addr") ||
                                    !strcmp(gtpu_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(&gtpu_iter, &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num <= MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] = 
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(gtpu_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&gtpu_iter);
                                if (v) {
                                    port = atoi(v);
                                    self.gtpu_port = port;
                                }
                            } else if (!strcmp(gtpu_key, "dev")) {
                                dev = ogs_yaml_iter_value(&gtpu_iter);
                            } else
                                ogs_warn("unknown key `%s`", gtpu_key);
                        }

                        addr = NULL;
                        for (i = 0; i < num; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        if (addr) {
                            if (context_self()->config.parameter.no_ipv4 == 0) {
                                ogs_sockaddr_t *dup = NULL;
                                rv = ogs_copyaddrinfo(&dup, addr);
                                ogs_assert(rv == OGS_OK);
                                ogs_socknode_add(
                                        &self.gtpu_list, AF_INET, dup);
                            }

                            if (context_self()->config.parameter.no_ipv6 == 0) {
                                ogs_sockaddr_t *dup = NULL;
                                rv = ogs_copyaddrinfo(&dup, addr);
                                ogs_assert(rv == OGS_OK);
                                ogs_socknode_add(
                                        &self.gtpu_list6, AF_INET6, dup);
                            }

                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                    context_self()->config.parameter.no_ipv4 ?
                                        NULL : &self.gtpu_list,
                                    context_self()->config.parameter.no_ipv6 ?
                                        NULL : &self.gtpu_list6,
                                    dev, self.gtpu_port);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while (ogs_yaml_iter_type(&gtpu_array) ==
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_empty(&self.gtpu_list) &&
                        ogs_list_empty(&self.gtpu_list6)) {
                        rv = ogs_socknode_probe(
                                context_self()->config.parameter.no_ipv4 ?
                                    NULL : &self.gtpu_list,
                                context_self()->config.parameter.no_ipv6 ?
                                    NULL : &self.gtpu_list6,
                                NULL, self.gtpu_port);
                        ogs_assert(rv == OGS_OK);
                    }
                }
                else
                    ogs_warn("unknown key `%s`", sgw_key);
            }
        }
    }

    rv = sgw_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

gtp_node_t *sgw_mme_add_by_message(gtp_message_t *message)
{
    int rv;
    gtp_node_t *mme = NULL;
    gtp_f_teid_t *mme_s11_teid = NULL;
    gtp_create_session_request_t *req = &message->create_session_request;

    if (req->sender_f_teid_for_control_plane.presence == 0) {
        ogs_error("No Sender F-TEID");
        return NULL;
    }

    mme_s11_teid = req->sender_f_teid_for_control_plane.data;
    ogs_assert(mme_s11_teid);
    mme = gtp_node_find(&sgw_self()->mme_s11_list, mme_s11_teid);
    if (!mme) {
        mme = gtp_node_add(&sgw_self()->mme_s11_list, mme_s11_teid,
            sgw_self()->gtpc_port,
            context_self()->config.parameter.no_ipv4,
            context_self()->config.parameter.no_ipv6,
            context_self()->config.parameter.prefer_ipv4);
        ogs_assert(mme);

        rv = gtp_connect(
                sgw_self()->gtpc_sock, sgw_self()->gtpc_sock6, mme);
        ogs_assert(rv == OGS_OK);
    }

    return mme;
}

sgw_ue_t *sgw_ue_add_by_message(gtp_message_t *message)
{
    sgw_ue_t *sgw_ue = NULL;
    gtp_create_session_request_t *req = &message->create_session_request;

    if (req->imsi.presence == 0) {
        ogs_error("No IMSI");
        return NULL;
    }

    ogs_trace("sgw_ue_add_by_message() - IMSI ");
    ogs_log_hexdump(OGS_LOG_TRACE, req->imsi.data, req->imsi.len);

    sgw_ue = sgw_ue_find_by_imsi(req->imsi.data, req->imsi.len);
    if (!sgw_ue) {
        sgw_ue = sgw_ue_add(req->imsi.data, req->imsi.len);
        ogs_assert(sgw_ue);
    }

    return sgw_ue;
}

sgw_ue_t *sgw_ue_add(uint8_t *imsi, int imsi_len)
{
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(imsi);
    ogs_assert(imsi_len);

    ogs_pool_alloc(&sgw_ue_pool, &sgw_ue);
    ogs_assert(sgw_ue);

    sgw_ue->sgw_s11_teid = ogs_pool_index(&sgw_ue_pool, sgw_ue);
    ogs_assert(sgw_ue->sgw_s11_teid > 0 &&
                sgw_ue->sgw_s11_teid <= context_self()->pool.ue);

    /* Set IMSI */
    sgw_ue->imsi_len = imsi_len;
    memcpy(sgw_ue->imsi, imsi, sgw_ue->imsi_len);
    ogs_buffer_to_bcd(sgw_ue->imsi, sgw_ue->imsi_len, sgw_ue->imsi_bcd);

    ogs_list_init(&sgw_ue->sess_list);

    ogs_hash_set(self.imsi_ue_hash, sgw_ue->imsi, sgw_ue->imsi_len, sgw_ue);

    return sgw_ue;
}

int sgw_ue_remove(sgw_ue_t *sgw_ue)
{
    ogs_assert(sgw_ue);

    /* Clear hash table */
    if (sgw_ue->imsi_len != 0)
        ogs_hash_set(self.imsi_ue_hash, sgw_ue->imsi, sgw_ue->imsi_len, NULL);
    
    sgw_sess_remove_all(sgw_ue);

    ogs_pool_free(&sgw_ue_pool, sgw_ue);

    return OGS_OK;
}

void sgw_ue_remove_all()
{
    ogs_hash_index_t *hi = NULL;
    sgw_ue_t *sgw_ue = NULL;

    for (hi = sgw_ue_first(); hi; hi = sgw_ue_next(hi)) {
        sgw_ue = sgw_ue_this(hi);
        sgw_ue_remove(sgw_ue);
    }
}

sgw_ue_t* sgw_ue_find_by_imsi_bcd(char *imsi_bcd)
{
    uint8_t imsi[MAX_IMSI_LEN];
    int imsi_len = 0;

    ogs_assert(imsi_bcd);

    ogs_bcd_to_buffer(imsi_bcd, imsi, &imsi_len);

    return sgw_ue_find_by_imsi(imsi, imsi_len);
}

sgw_ue_t* sgw_ue_find_by_imsi(uint8_t *imsi, int imsi_len)
{
    ogs_assert(imsi && imsi_len);

    return (sgw_ue_t *)ogs_hash_get(self.imsi_ue_hash, imsi, imsi_len);
}

sgw_ue_t* sgw_ue_find_by_teid(uint32_t teid)
{
    return ogs_pool_find(&sgw_ue_pool, teid);
}

ogs_hash_index_t *sgw_ue_first()
{
    ogs_assert(self.imsi_ue_hash);
    return ogs_hash_first(self.imsi_ue_hash);
}

ogs_hash_index_t *sgw_ue_next(ogs_hash_index_t *hi)
{
    return ogs_hash_next(hi);
}

sgw_ue_t *sgw_ue_this(ogs_hash_index_t *hi)
{
    ogs_assert(hi);
    return ogs_hash_this_val(hi);
}

sgw_sess_t *sgw_sess_add(sgw_ue_t *sgw_ue, char *apn, uint8_t ebi)
{
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;

    ogs_assert(sgw_ue);
    ogs_assert(ebi);

    ogs_pool_alloc(&sgw_sess_pool, &sess);
    ogs_assert(sess);

    sess->sgw_s5c_teid = 
        SGW_S5C_INDEX_TO_TEID(ogs_pool_index(&sgw_sess_pool, sess));

    /* Set APN */
    ogs_cpystrn(sess->pdn.apn, apn, MAX_APN_LEN+1);

    sess->sgw_ue = sgw_ue;
    sess->gnode = NULL;

    ogs_list_init(&sess->bearer_list);

    bearer = sgw_bearer_add(sess);
    ogs_assert(bearer);
    bearer->ebi = ebi;

    ogs_list_add(&sgw_ue->sess_list, sess);

    return sess;
}

int sgw_sess_remove(sgw_sess_t *sess)
{
    ogs_assert(sess);
    ogs_assert(sess->sgw_ue);

    ogs_list_remove(&sess->sgw_ue->sess_list, sess);

    sgw_bearer_remove_all(sess);

    ogs_pool_free(&sgw_sess_pool, sess);

    return OGS_OK;
}

void sgw_sess_remove_all(sgw_ue_t *sgw_ue)
{
    sgw_sess_t *sess = NULL, *next_sess = NULL;
    
    ogs_list_for_each_safe(&sgw_ue->sess_list, next_sess, sess)
        sgw_sess_remove(sess);
}

sgw_sess_t* sgw_sess_find_by_teid(uint32_t teid)
{
    return ogs_pool_find(&sgw_sess_pool, SGW_S5C_TEID_TO_INDEX(teid));
}

sgw_sess_t* sgw_sess_find_by_apn(sgw_ue_t *sgw_ue, char *apn)
{
    sgw_sess_t *sess = NULL;
    
    sess = sgw_sess_first(sgw_ue);
    while (sess) {
        if (strcmp(sess->pdn.apn, apn) == 0)
            return sess;

        sess = sgw_sess_next(sess);
    }

    return NULL;
}

sgw_sess_t* sgw_sess_find_by_ebi(sgw_ue_t *sgw_ue, uint8_t ebi)
{
    sgw_bearer_t *bearer = NULL;

    bearer = sgw_bearer_find_by_ue_ebi(sgw_ue, ebi);
    if (bearer)
        return bearer->sess;

    return NULL;
}

sgw_sess_t* sgw_sess_first(sgw_ue_t *sgw_ue)
{
    return ogs_list_first(&sgw_ue->sess_list);
}

sgw_sess_t* sgw_sess_next(sgw_sess_t *sess)
{
    return ogs_list_next(sess);
}

sgw_bearer_t* sgw_bearer_add(sgw_sess_t *sess)
{
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(sess);
    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);

    ogs_pool_alloc(&sgw_bearer_pool, &bearer);
    ogs_assert(bearer);

    bearer->sgw_ue = sgw_ue;
    bearer->sess = sess;

    ogs_list_init(&bearer->tunnel_list);

    tunnel = sgw_tunnel_add(bearer, GTP_F_TEID_S1_U_SGW_GTP_U);
    ogs_assert(tunnel);

    tunnel = sgw_tunnel_add(bearer, GTP_F_TEID_S5_S8_SGW_GTP_U);
    ogs_assert(tunnel);

    ogs_list_add(&sess->bearer_list, bearer);
    
    return bearer;
}

int sgw_bearer_remove(sgw_bearer_t *bearer)
{
    int i;

    ogs_assert(bearer);
    ogs_assert(bearer->sess);

    ogs_list_remove(&bearer->sess->bearer_list, bearer);

    sgw_tunnel_remove_all(bearer);

    /* Free the buffered packets */
    for (i = 0; i < bearer->num_buffered_pkt; i++)
        ogs_pkbuf_free(bearer->buffered_pkts[i]);

    ogs_pool_free(&sgw_bearer_pool, bearer);

    return OGS_OK;
}

void sgw_bearer_remove_all(sgw_sess_t *sess)
{
    sgw_bearer_t *bearer = NULL, *next_bearer = NULL;

    ogs_assert(sess);
    ogs_list_for_each_safe(&sess->bearer_list, next_bearer, bearer)
        sgw_bearer_remove(bearer);
}

sgw_bearer_t* sgw_bearer_find_by_sgw_s5u_teid(uint32_t sgw_s5u_teid)
{
    return ogs_pool_find(&sgw_bearer_pool, sgw_s5u_teid);
}

sgw_bearer_t* sgw_bearer_find_by_sess_ebi(sgw_sess_t *sess, uint8_t ebi)
{
    sgw_bearer_t *bearer = NULL;

    bearer = sgw_bearer_first(sess);
    while (bearer) {
        if (ebi == bearer->ebi)
            return bearer;

        bearer = sgw_bearer_next(bearer);
    }

    return NULL;
}

sgw_bearer_t* sgw_bearer_find_by_ue_ebi(sgw_ue_t *sgw_ue, uint8_t ebi)
{
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    
    sess = sgw_sess_first(sgw_ue);
    while (sess) {
        bearer = sgw_bearer_find_by_sess_ebi(sess, ebi);
        if (bearer) {
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
    ogs_assert(sess);
    return ogs_list_first(&sess->bearer_list);
}

sgw_bearer_t* sgw_bearer_next(sgw_bearer_t *bearer)
{
    return ogs_list_next(bearer);
}

sgw_tunnel_t* sgw_tunnel_add(sgw_bearer_t *bearer, uint8_t interface_type)
{
    sgw_tunnel_t *tunnel = NULL;

    ogs_assert(bearer);

    ogs_pool_alloc(&sgw_tunnel_pool, &tunnel);
    ogs_assert(tunnel);

    tunnel->interface_type = interface_type;
    tunnel->local_teid = ogs_pool_index(&sgw_tunnel_pool, tunnel);
    ogs_assert(tunnel->local_teid > 0 &&
            tunnel->local_teid <= context_self()->pool.tunnel);

    tunnel->bearer = bearer;
    tunnel->gnode = NULL;

    ogs_list_add(&bearer->tunnel_list, tunnel);

    return tunnel;
}

int sgw_tunnel_remove(sgw_tunnel_t *tunnel)
{
    ogs_assert(tunnel);
    ogs_assert(tunnel->bearer);

    ogs_list_remove(&tunnel->bearer->tunnel_list, tunnel);
    ogs_pool_free(&sgw_tunnel_pool, tunnel);

    return OGS_OK;
}

void sgw_tunnel_remove_all(sgw_bearer_t *bearer)
{
    sgw_tunnel_t *tunnel = NULL, *next_tunnel = NULL;

    ogs_assert(bearer);
    ogs_list_for_each_safe(&bearer->tunnel_list, next_tunnel, tunnel)
        sgw_tunnel_remove(tunnel);
}

sgw_tunnel_t* sgw_tunnel_find_by_teid(uint32_t teid)
{
    return ogs_pool_find(&sgw_tunnel_pool, teid);
}

sgw_tunnel_t* sgw_tunnel_find_by_interface_type(
        sgw_bearer_t *bearer, uint8_t interface_type)
{
    sgw_tunnel_t *tunnel = NULL;

    ogs_assert(bearer);

    tunnel = sgw_tunnel_first(bearer);
    while (tunnel) {
        if (tunnel->interface_type == interface_type) {
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
    ogs_assert(bearer);
    return ogs_list_first(&bearer->tunnel_list);
}

sgw_tunnel_t* sgw_tunnel_next(sgw_tunnel_t *tunnel)
{
    return ogs_list_next(tunnel);
}

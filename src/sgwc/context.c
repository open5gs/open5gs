/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <yaml.h>

#include "context.h"

static sgwc_context_t self;

int __sgwc_log_domain;

static OGS_POOL(sgwc_ue_pool, sgwc_ue_t);
static OGS_POOL(sgwc_sess_pool, sgwc_sess_t);
static OGS_POOL(sgwc_bearer_pool, sgwc_bearer_t);
static OGS_POOL(sgwc_tunnel_pool, sgwc_tunnel_t);

static int context_initialized = 0;

static int num_of_sgwc_sess = 0;

static void stats_add_sgwc_session(void);
static void stats_remove_sgwc_session(void);

void sgwc_context_init(void)
{
    ogs_assert(context_initialized == 0);

    memset(&self, 0, sizeof(sgwc_context_t));

    ogs_log_install_domain(&__sgwc_log_domain, "sgwc", ogs_core()->log.level);

    ogs_pool_init(&sgwc_ue_pool, ogs_app()->max.ue);
    ogs_pool_init(&sgwc_sess_pool, ogs_app()->pool.sess);
    ogs_pool_init(&sgwc_bearer_pool, ogs_app()->pool.bearer);
    ogs_pool_init(&sgwc_tunnel_pool, ogs_app()->pool.tunnel);

    self.imsi_ue_hash = ogs_hash_make();
    ogs_assert(self.imsi_ue_hash);

    ogs_list_init(&self.sgw_ue_list);

    context_initialized = 1;
}

void sgwc_context_final(void)
{
    ogs_assert(context_initialized == 1);

    sgwc_ue_remove_all();

    ogs_assert(self.imsi_ue_hash);
    ogs_hash_destroy(self.imsi_ue_hash);

    ogs_pool_final(&sgwc_tunnel_pool);
    ogs_pool_final(&sgwc_bearer_pool);
    ogs_pool_final(&sgwc_sess_pool);
    ogs_pool_final(&sgwc_ue_pool);

    ogs_gtp_node_remove_all(&self.mme_s11_list);
    ogs_gtp_node_remove_all(&self.pgw_s5c_list);

    context_initialized = 0;
}

sgwc_context_t *sgwc_self(void)
{
    return &self;
}

static int sgwc_context_prepare(void)
{
    return OGS_OK;
}

static int sgwc_context_validation(void)
{
    if (ogs_list_empty(&ogs_gtp_self()->gtpc_list) &&
        ogs_list_empty(&ogs_gtp_self()->gtpc_list6)) {
        ogs_error("No sgwc.gtpc in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    return OGS_OK;
}

int sgwc_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = sgwc_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "sgwc")) {
            ogs_yaml_iter_t sgwc_iter;
            ogs_yaml_iter_recurse(&root_iter, &sgwc_iter);
            while (ogs_yaml_iter_next(&sgwc_iter)) {
                const char *sgwc_key = ogs_yaml_iter_key(&sgwc_iter);
                ogs_assert(sgwc_key);
                if (!strcmp(sgwc_key, "gtpc")) {
                    /* handle config in gtp library */
                } else if (!strcmp(sgwc_key, "pfcp")) {
                    /* handle config in pfcp library */
                } else
                    ogs_warn("unknown key `%s`", sgwc_key);
            }
        }
    }

    rv = sgwc_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

sgwc_ue_t *sgwc_ue_add_by_message(ogs_gtp2_message_t *message)
{
    sgwc_ue_t *sgwc_ue = NULL;
    ogs_gtp2_create_session_request_t *req = &message->create_session_request;

    ogs_assert(message);

    req = &message->create_session_request;
    if (req->imsi.presence == 0) {
        ogs_error("No IMSI");
        return NULL;
    }

    ogs_trace("sgwc_ue_add_by_message() - IMSI ");
    ogs_log_hexdump(OGS_LOG_TRACE, req->imsi.data, req->imsi.len);

    /*
     * 7.2.1 in 3GPP TS 29.274 Release 15
     *
     * If the new Create Session Request received by the SGW collides with
     * an existing active PDN connection context (the existing PDN connection
     * context is identified with the tuple [IMSI, EPS Bearer ID], where IMSI
     * shall be replaced by TAC and SNR part of ME Identity for emergency
     * attached UE without UICC or authenticated IMSI), this Create Session
     * Request shall be treated as a request for a new session. Before creating
     * the new session, the SGW should delete:
     *
     * - the existing PDN connection context locally, if the Create Session
     *   Request is received with the TEID set to zero in the header, or
     *   if it is received with a TEID not set to zero in the header and
     *   it collides with the default bearer of an existing PDN connection
     *   context;
     * - the existing dedicated bearer context locally, if the Create Session
     *   Request collides with an existing dedicated bearer context and
     *   the message is received with a TEID not set to zero in the header.
     */
    sgwc_ue = sgwc_ue_find_by_imsi(req->imsi.data, req->imsi.len);
    if (sgwc_ue)
        sgwc_ue_remove(sgwc_ue);
    sgwc_ue = sgwc_ue_add(req->imsi.data, req->imsi.len);
    ogs_assert(sgwc_ue);

    return sgwc_ue;
}

sgwc_ue_t *sgwc_ue_add(uint8_t *imsi, int imsi_len)
{
    sgwc_ue_t *sgwc_ue = NULL;

    ogs_assert(imsi);
    ogs_assert(imsi_len);

    ogs_pool_alloc(&sgwc_ue_pool, &sgwc_ue);
    ogs_assert(sgwc_ue);
    memset(sgwc_ue, 0, sizeof *sgwc_ue);

    sgwc_ue->sgw_s11_teid = ogs_pool_index(&sgwc_ue_pool, sgwc_ue);
    ogs_assert(sgwc_ue->sgw_s11_teid > 0 &&
                sgwc_ue->sgw_s11_teid <= ogs_app()->max.ue);

    /* Set IMSI */
    sgwc_ue->imsi_len = imsi_len;
    memcpy(sgwc_ue->imsi, imsi, sgwc_ue->imsi_len);
    ogs_buffer_to_bcd(sgwc_ue->imsi, sgwc_ue->imsi_len, sgwc_ue->imsi_bcd);

    ogs_list_init(&sgwc_ue->sess_list);

    ogs_hash_set(self.imsi_ue_hash, sgwc_ue->imsi, sgwc_ue->imsi_len, sgwc_ue);

    ogs_list_add(&self.sgw_ue_list, sgwc_ue);

    ogs_info("[Added] Number of SGWC-UEs is now %d",
            ogs_list_count(&self.sgw_ue_list));

    return sgwc_ue;
}

int sgwc_ue_remove(sgwc_ue_t *sgwc_ue)
{
    ogs_assert(sgwc_ue);

    ogs_list_remove(&self.sgw_ue_list, sgwc_ue);

    ogs_hash_set(self.imsi_ue_hash, sgwc_ue->imsi, sgwc_ue->imsi_len, NULL);

    sgwc_sess_remove_all(sgwc_ue);

    ogs_pool_free(&sgwc_ue_pool, sgwc_ue);

    ogs_info("[Removed] Number of SGWC-UEs is now %d",
            ogs_list_count(&self.sgw_ue_list));

    return OGS_OK;
}

void sgwc_ue_remove_all(void)
{
    sgwc_ue_t *sgwc_ue = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.sgw_ue_list, next, sgwc_ue)
        sgwc_ue_remove(sgwc_ue);
}

sgwc_ue_t *sgwc_ue_find_by_imsi_bcd(char *imsi_bcd)
{
    uint8_t imsi[OGS_MAX_IMSI_LEN];
    int imsi_len = 0;

    ogs_assert(imsi_bcd);

    ogs_bcd_to_buffer(imsi_bcd, imsi, &imsi_len);

    return sgwc_ue_find_by_imsi(imsi, imsi_len);
}

sgwc_ue_t *sgwc_ue_find_by_imsi(uint8_t *imsi, int imsi_len)
{
    ogs_assert(imsi && imsi_len);

    return (sgwc_ue_t *)ogs_hash_get(self.imsi_ue_hash, imsi, imsi_len);
}

sgwc_ue_t *sgwc_ue_find_by_teid(uint32_t teid)
{
    return ogs_pool_find(&sgwc_ue_pool, teid);
}

sgwc_sess_t *sgwc_sess_add(sgwc_ue_t *sgwc_ue, char *apn)
{
    sgwc_sess_t *sess = NULL;

    ogs_assert(sgwc_ue);

    ogs_pool_alloc(&sgwc_sess_pool, &sess);
    if (!sess) {
        ogs_error("Maximum number of session[%lld] reached",
                    (long long)ogs_app()->pool.sess);
        return NULL;
    }
    memset(sess, 0, sizeof *sess);

    ogs_pfcp_pool_init(&sess->pfcp);

    sess->index = ogs_pool_index(&sgwc_sess_pool, sess);
    ogs_assert(sess->index > 0 && sess->index <= ogs_app()->pool.sess);

    /* Set TEID & SEID */
    sess->sgw_s5c_teid = sess->index;
    sess->sgwc_sxa_seid = sess->index;

    /* Create BAR in PFCP Session */
    ogs_pfcp_bar_new(&sess->pfcp);

    /* Set APN */
    sess->session.name = ogs_strdup(apn);
    ogs_assert(sess->session.name);

    sess->sgwc_ue = sgwc_ue;

    ogs_list_add(&sgwc_ue->sess_list, sess);

    stats_add_sgwc_session();

    return sess;
}

static bool compare_ue_info(ogs_pfcp_node_t *node, sgwc_sess_t *sess)
{
    sgwc_ue_t *sgwc_ue = NULL;
    int i;

    ogs_assert(node);
    ogs_assert(sess);
    sgwc_ue = sess->sgwc_ue;
    ogs_assert(sgwc_ue);

    ogs_assert(sess->session.name);
    for (i = 0; i < node->num_of_dnn; i++)
        if (ogs_strcasecmp(node->dnn[i], sess->session.name) == 0) return true;

    for (i = 0; i < node->num_of_e_cell_id; i++)
        if (sgwc_ue->uli_presence == true &&
            node->e_cell_id[i] == sgwc_ue->e_cgi.cell_id) return true;

    for (i = 0; i < node->num_of_tac; i++)
        if (sgwc_ue->uli_presence == true &&
            node->tac[i] == sgwc_ue->e_tai.tac) return true;

    return false;
}

static ogs_pfcp_node_t *selected_sgwu_node(
        ogs_pfcp_node_t *current, sgwc_sess_t *sess)
{
    ogs_pfcp_node_t *next, *node;

    ogs_assert(current);
    ogs_assert(sess);

    /* continue search from current position */
    next = ogs_list_next(current);
    for (node = next; node; node = ogs_list_next(node)) {
        if (OGS_FSM_CHECK(&node->sm, sgwc_pfcp_state_associated) &&
            compare_ue_info(node, sess) == true) return node;
    }
    /* cyclic search from top to current position */
    for (node = ogs_list_first(&ogs_pfcp_self()->pfcp_peer_list);
            node != next; node = ogs_list_next(node)) {
        if (OGS_FSM_CHECK(&node->sm, sgwc_pfcp_state_associated) &&
            compare_ue_info(node, sess) == true) return node;
    }

    if (ogs_app()->parameter.no_pfcp_rr_select == 0) {
        /* continue search from current position */
        next = ogs_list_next(current);
        for (node = next; node; node = ogs_list_next(node)) {
            if (OGS_FSM_CHECK(&node->sm, sgwc_pfcp_state_associated))
                return node;
        }
        /* cyclic search from top to current position */
        for (node = ogs_list_first(&ogs_pfcp_self()->pfcp_peer_list);
                node != next; node = ogs_list_next(node)) {
            if (OGS_FSM_CHECK(&node->sm, sgwc_pfcp_state_associated))
                return node;
        }
    }

    ogs_error("No SGWUs are PFCP associated that are suited to RR");
    return ogs_list_first(&ogs_pfcp_self()->pfcp_peer_list);
}

void sgwc_sess_select_sgwu(sgwc_sess_t *sess)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(sess);

    /*
     * When used for the first time, if last node is set,
     * the search is performed from the first SGW-U in a round-robin manner.
     */
    if (ogs_pfcp_self()->pfcp_node == NULL)
        ogs_pfcp_self()->pfcp_node =
            ogs_list_last(&ogs_pfcp_self()->pfcp_peer_list);

    /* setup GTP session with selected SGW-U */
    ogs_pfcp_self()->pfcp_node =
        selected_sgwu_node(ogs_pfcp_self()->pfcp_node, sess);
    ogs_assert(ogs_pfcp_self()->pfcp_node);
    OGS_SETUP_PFCP_NODE(sess, ogs_pfcp_self()->pfcp_node);
    ogs_debug("UE using SGW-U on IP[%s]",
            OGS_ADDR(&ogs_pfcp_self()->pfcp_node->addr, buf));
}

int sgwc_sess_remove(sgwc_sess_t *sess)
{
    sgwc_ue_t *sgwc_ue = NULL;

    ogs_assert(sess);
    sgwc_ue = sess->sgwc_ue;
    ogs_assert(sgwc_ue);

    ogs_list_remove(&sgwc_ue->sess_list, sess);

    sgwc_bearer_remove_all(sess);

    ogs_assert(sess->pfcp.bar);
    ogs_pfcp_bar_delete(sess->pfcp.bar);

    ogs_pfcp_pool_final(&sess->pfcp);

    ogs_assert(sess->session.name);
    ogs_free(sess->session.name);

    ogs_pool_free(&sgwc_sess_pool, sess);

    stats_remove_sgwc_session();

    return OGS_OK;
}

void sgwc_sess_remove_all(sgwc_ue_t *sgwc_ue)
{
    sgwc_sess_t *sess = NULL, *next_sess = NULL;

    ogs_assert(sgwc_ue);
    ogs_list_for_each_safe(&sgwc_ue->sess_list, next_sess, sess)
        sgwc_sess_remove(sess);
}

sgwc_sess_t *sgwc_sess_find(uint32_t index)
{
    return ogs_pool_find(&sgwc_sess_pool, index);
}

sgwc_sess_t* sgwc_sess_find_by_teid(uint32_t teid)
{
    return ogs_pool_find(&sgwc_sess_pool, teid);
}

sgwc_sess_t *sgwc_sess_find_by_seid(uint64_t seid)
{
    return sgwc_sess_find(seid);
}

sgwc_sess_t* sgwc_sess_find_by_apn(sgwc_ue_t *sgwc_ue, char *apn)
{
    sgwc_sess_t *sess = NULL;

    ogs_assert(sgwc_ue);
    ogs_assert(apn);

    ogs_list_for_each(&sgwc_ue->sess_list, sess) {
        if (!ogs_strcasecmp(sess->session.name, apn))
            return sess;
    }

    return NULL;
}

sgwc_sess_t *sgwc_sess_find_by_ebi(sgwc_ue_t *sgwc_ue, uint8_t ebi)
{
    sgwc_bearer_t *bearer = NULL;
    ogs_assert(sgwc_ue);

    bearer = sgwc_bearer_find_by_ue_ebi(sgwc_ue, ebi);
    if (bearer)
        return bearer->sess;

    return NULL;
}

sgwc_sess_t *sgwc_sess_cycle(sgwc_sess_t *sess)
{
    return ogs_pool_cycle(&sgwc_sess_pool, sess);
}

int sgwc_sess_pfcp_xact_count(
        sgwc_ue_t *sgwc_ue, uint8_t pfcp_type, uint64_t modify_flags)
{
    sgwc_sess_t *sess = NULL;
    int xact_count = 0;

    ogs_assert(sgwc_ue);

    ogs_list_for_each(&sgwc_ue->sess_list, sess) {
        ogs_pfcp_node_t *pfcp_node = sess->pfcp_node;
        ogs_pfcp_xact_t *pfcp_xact = NULL;
        ogs_assert(pfcp_node);
        ogs_list_for_each(&pfcp_node->local_list, pfcp_xact) {
            if (sess != pfcp_xact->data)
                continue;
            if (pfcp_type && pfcp_type != pfcp_xact->seq[0].type)
                continue;
            if (modify_flags && modify_flags != pfcp_xact->modify_flags)
                continue;
            xact_count++;
        }
    }

    return xact_count;
}

sgwc_bearer_t *sgwc_bearer_add(sgwc_sess_t *sess)
{
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *tunnel = NULL;
    sgwc_ue_t *sgwc_ue = NULL;

    ogs_assert(sess);
    sgwc_ue = sess->sgwc_ue;
    ogs_assert(sgwc_ue);

    ogs_pool_alloc(&sgwc_bearer_pool, &bearer);
    ogs_assert(bearer);
    memset(bearer, 0, sizeof *bearer);

    bearer->sgwc_ue = sgwc_ue;
    bearer->sess = sess;

    /* Downlink */
    tunnel = sgwc_tunnel_add(bearer, OGS_GTP2_F_TEID_S5_S8_SGW_GTP_U);
    ogs_assert(tunnel);

    /* Uplink */
    tunnel = sgwc_tunnel_add(bearer, OGS_GTP2_F_TEID_S1_U_SGW_GTP_U);
    ogs_assert(tunnel);

    ogs_list_add(&sess->bearer_list, bearer);

    return bearer;
}

int sgwc_bearer_remove(sgwc_bearer_t *bearer)
{
    ogs_assert(bearer);
    ogs_assert(bearer->sess);

    ogs_list_remove(&bearer->sess->bearer_list, bearer);

    sgwc_tunnel_remove_all(bearer);

    ogs_pool_free(&sgwc_bearer_pool, bearer);

    return OGS_OK;
}

void sgwc_bearer_remove_all(sgwc_sess_t *sess)
{
    sgwc_bearer_t *bearer = NULL, *next_bearer = NULL;

    ogs_assert(sess);
    ogs_list_for_each_safe(&sess->bearer_list, next_bearer, bearer)
        sgwc_bearer_remove(bearer);
}

sgwc_bearer_t *sgwc_bearer_find_by_sess_ebi(sgwc_sess_t *sess, uint8_t ebi)
{
    sgwc_bearer_t *bearer = NULL;

    ogs_assert(sess);
    ogs_list_for_each(&sess->bearer_list, bearer)
        if (ebi == bearer->ebi) return bearer;

    return NULL;
}

sgwc_bearer_t *sgwc_bearer_find_by_ue_ebi(sgwc_ue_t *sgwc_ue, uint8_t ebi)
{
    sgwc_sess_t *sess = NULL;
    sgwc_bearer_t *bearer = NULL;

    ogs_assert(sgwc_ue);
    ogs_list_for_each(&sgwc_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            if (ebi == bearer->ebi) return bearer;
        }
    }

    return NULL;
}

sgwc_bearer_t *sgwc_bearer_find_by_error_indication_report(
        sgwc_sess_t *sess,
        ogs_pfcp_tlv_error_indication_report_t *error_indication_report)
{
    ogs_pfcp_f_teid_t *remote_f_teid = NULL;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *tunnel = NULL;

    uint32_t teid;
    uint16_t len;  /* OGS_IPV4_LEN or OGS_IPV6_LEN */
    uint32_t addr[4];

    ogs_assert(sess);
    ogs_assert(error_indication_report);

    if (error_indication_report->presence == 0) {
        ogs_error("No Error Indication Report");
        return NULL;
    }

    if (error_indication_report->remote_f_teid.presence == 0) {
        ogs_error("No Remote F-TEID");
        return NULL;
    }

    remote_f_teid = error_indication_report->remote_f_teid.data;
    ogs_assert(remote_f_teid);

    teid = be32toh(remote_f_teid->teid);
    if (remote_f_teid->ipv4 && remote_f_teid->ipv6) {
        ogs_error("User plane should not set both IPv4 and IPv6");
        return NULL;
    } else if (remote_f_teid->ipv4) {
        len = OGS_IPV4_LEN;
        memcpy(addr, &remote_f_teid->addr, len);
    } else if (remote_f_teid->ipv6) {
        len = OGS_IPV6_LEN;
        memcpy(addr, remote_f_teid->addr6, len);
    } else {
        ogs_error("No IPv4 and IPv6");
        return NULL;
    }

    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_list_for_each(&bearer->tunnel_list, tunnel) {
            if (teid == tunnel->remote_teid) {
                if (len == OGS_IPV4_LEN && tunnel->remote_ip.ipv4 &&
                    memcmp(addr, &tunnel->remote_ip.addr, len) == 0) {
                    return bearer;
                } else if (len == OGS_IPV6_LEN && tunnel->remote_ip.ipv6 &&
                            memcmp(addr, tunnel->remote_ip.addr6, len) == 0) {
                    return bearer;
                }
            }
        }
    }

    ogs_error("Cannot find the bearer context "
            "[TEID:%d,LEN:%d,ADDR:%08x %08x %08x %08x]",
            teid, len, be32toh(addr[0]), be32toh(addr[1]),
            be32toh(addr[2]), be32toh(addr[3]));

    return NULL;
}

sgwc_bearer_t *sgwc_default_bearer_in_sess(sgwc_sess_t *sess)
{
    ogs_assert(sess);
    return ogs_list_first(&sess->bearer_list);
}

sgwc_bearer_t *sgwc_bearer_cycle(sgwc_bearer_t *bearer)
{
    return ogs_pool_cycle(&sgwc_bearer_pool, bearer);
}

sgwc_tunnel_t *sgwc_tunnel_add(
        sgwc_bearer_t *bearer, uint8_t interface_type)
{
    sgwc_sess_t *sess = NULL;
    sgwc_tunnel_t *tunnel = NULL;

    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;

    uint8_t src_if, dst_if;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    switch (interface_type) {
    /* Downlink */
    case OGS_GTP2_F_TEID_S5_S8_SGW_GTP_U:
        src_if = OGS_PFCP_INTERFACE_CORE;
        dst_if = OGS_PFCP_INTERFACE_ACCESS;
        break;

    /* Uplink */
    case OGS_GTP2_F_TEID_S1_U_SGW_GTP_U:
        src_if = OGS_PFCP_INTERFACE_ACCESS;
        dst_if = OGS_PFCP_INTERFACE_CORE;
        break;

    /* Indirect */
    case OGS_GTP2_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING:
    case OGS_GTP2_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING:
        src_if = OGS_PFCP_INTERFACE_ACCESS;
        dst_if = OGS_PFCP_INTERFACE_ACCESS;
        break;
    default:
        ogs_fatal("Invalid interface type = %d", interface_type);
    }

    ogs_pool_alloc(&sgwc_tunnel_pool, &tunnel);
    ogs_assert(tunnel);
    memset(tunnel, 0, sizeof *tunnel);

    tunnel->interface_type = interface_type;
    tunnel->index = ogs_pool_index(&sgwc_tunnel_pool, tunnel);
    ogs_assert(tunnel->index > 0 && tunnel->index <= ogs_app()->pool.tunnel);

    pdr = ogs_pfcp_pdr_add(&sess->pfcp);
    ogs_assert(pdr);

    ogs_assert(sess->session.name);
    pdr->apn = ogs_strdup(sess->session.name);
    ogs_assert(pdr->apn);

    pdr->src_if = src_if;

    far = ogs_pfcp_far_add(&sess->pfcp);
    ogs_assert(far);

    ogs_assert(sess->session.name);
    far->apn = ogs_strdup(sess->session.name);
    ogs_assert(far->apn);

    far->dst_if = dst_if;
    ogs_pfcp_pdr_associate_far(pdr, far);

    far->apply_action =
        OGS_PFCP_APPLY_ACTION_BUFF| OGS_PFCP_APPLY_ACTION_NOCP;
    ogs_assert(sess->pfcp.bar);

    ogs_assert(sess->pfcp_node);
    if (sess->pfcp_node->up_function_features.ftup) {

       /* TS 129 244 V16.5.0 8.2.3
        *
        * At least one of the V4 and V6 flags shall be set to "1",
        * and both may be set to "1" for both scenarios:
        *
        * - when the CP function is providing F-TEID, i.e.
        *   both IPv4 address field and IPv6 address field may be present;
        *   or
        * - when the UP function is requested to allocate the F-TEID,
        *   i.e. when CHOOSE bit is set to "1",
        *   and the IPv4 address and IPv6 address fields are not present.
        */

        pdr->f_teid.ipv4 = 1;
        pdr->f_teid.ipv6 = 1;
        pdr->f_teid.ch = 1;
        pdr->f_teid_len = 1;
    } else {
        ogs_gtpu_resource_t *resource = NULL;
        resource = ogs_pfcp_find_gtpu_resource(
                &sess->pfcp_node->gtpu_resource_list,
                sess->session.name, OGS_PFCP_INTERFACE_ACCESS);
        if (resource) {
            ogs_user_plane_ip_resource_info_to_sockaddr(&resource->info,
                &tunnel->local_addr, &tunnel->local_addr6);
            if (resource->info.teidri)
                tunnel->local_teid = OGS_PFCP_GTPU_INDEX_TO_TEID(
                        pdr->index, resource->info.teidri,
                        resource->info.teid_range);
            else
                tunnel->local_teid = pdr->index;
        } else {
            if (sess->pfcp_node->addr.ogs_sa_family == AF_INET)
                ogs_assert(OGS_OK ==
                    ogs_copyaddrinfo(
                        &tunnel->local_addr, &sess->pfcp_node->addr));
            else if (sess->pfcp_node->addr.ogs_sa_family == AF_INET6)
                ogs_assert(OGS_OK ==
                    ogs_copyaddrinfo(
                        &tunnel->local_addr6, &sess->pfcp_node->addr));
            else
                ogs_assert_if_reached();

            tunnel->local_teid = pdr->index;
        }

        ogs_assert(OGS_OK ==
            ogs_pfcp_sockaddr_to_f_teid(
                tunnel->local_addr, tunnel->local_addr6,
                &pdr->f_teid, &pdr->f_teid_len));
        pdr->f_teid.teid = tunnel->local_teid;
    }

    tunnel->pdr = pdr;
    tunnel->far = far;

    tunnel->bearer = bearer;

    ogs_list_add(&bearer->tunnel_list, tunnel);

    return tunnel;
}

int sgwc_tunnel_remove(sgwc_tunnel_t *tunnel)
{
    ogs_assert(tunnel);
    ogs_assert(tunnel->bearer);

    ogs_list_remove(&tunnel->bearer->tunnel_list, tunnel);

    ogs_pfcp_pdr_remove(tunnel->pdr);
    ogs_pfcp_far_remove(tunnel->far);

    if (tunnel->local_addr)
        ogs_freeaddrinfo(tunnel->local_addr);
    if (tunnel->local_addr6)
        ogs_freeaddrinfo(tunnel->local_addr6);

    ogs_pool_free(&sgwc_tunnel_pool, tunnel);

    return OGS_OK;
}

void sgwc_tunnel_remove_all(sgwc_bearer_t *bearer)
{
    sgwc_tunnel_t *tunnel = NULL, *next_tunnel = NULL;

    ogs_assert(bearer);
    ogs_list_for_each_safe(&bearer->tunnel_list, next_tunnel, tunnel)
        sgwc_tunnel_remove(tunnel);
}

sgwc_tunnel_t *sgwc_tunnel_find_by_teid(sgwc_ue_t *sgwc_ue, uint32_t teid)
{
    sgwc_sess_t *sess = NULL;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *tunnel = NULL;

    ogs_assert(sgwc_ue);

    ogs_list_for_each(&sgwc_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            ogs_list_for_each(&bearer->tunnel_list, tunnel) {
                if (tunnel->local_teid == teid) return tunnel;
            }
        }
    }

    return NULL;
}

sgwc_tunnel_t *sgwc_tunnel_find_by_interface_type(
        sgwc_bearer_t *bearer, uint8_t interface_type)
{
    sgwc_tunnel_t *tunnel = NULL;

    ogs_assert(bearer);

    ogs_list_for_each(&bearer->tunnel_list, tunnel)
        if (tunnel->interface_type == interface_type) return tunnel;

    return NULL;
}

sgwc_tunnel_t *sgwc_tunnel_find_by_pdr_id(
        sgwc_sess_t *sess, ogs_pfcp_pdr_id_t pdr_id)
{
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *tunnel = NULL;

    ogs_pfcp_pdr_t *pdr = NULL;

    ogs_assert(sess);

    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_list_for_each(&bearer->tunnel_list, tunnel) {
            pdr = tunnel->pdr;
            ogs_assert(pdr);

            if (pdr->id == pdr_id) return tunnel;
        }
    }

    return NULL;
}

sgwc_tunnel_t *sgwc_dl_tunnel_in_bearer(sgwc_bearer_t *bearer)
{
    ogs_assert(bearer);
    return sgwc_tunnel_find_by_interface_type(bearer,
            OGS_GTP2_F_TEID_S5_S8_SGW_GTP_U);
}
sgwc_tunnel_t *sgwc_ul_tunnel_in_bearer(sgwc_bearer_t *bearer)
{
    ogs_assert(bearer);
    return sgwc_tunnel_find_by_interface_type(bearer,
            OGS_GTP2_F_TEID_S1_U_SGW_GTP_U);
}

static void stats_add_sgwc_session(void)
{
    num_of_sgwc_sess = num_of_sgwc_sess + 1;
    ogs_info("[Added] Number of SGWC-Sessions is now %d", num_of_sgwc_sess);
}

static void stats_remove_sgwc_session(void)
{
    num_of_sgwc_sess = num_of_sgwc_sess - 1;
    ogs_info("[Removed] Number of SGWC-Sessions is now %d", num_of_sgwc_sess);
}

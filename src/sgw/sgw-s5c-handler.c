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

#include "sgw-event.h"
#include "sgw-context.h"
#include "sgw-gtp-path.h"
#include "sgw-s5c-handler.h"

static void timeout(ogs_gtp_xact_t *xact, void *data)
{
    sgw_sess_t *sess = data;
    sgw_ue_t *sgw_ue = NULL;
    uint8_t type = 0;

    ogs_assert(xact);
    ogs_assert(sess);
    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);

    type = xact->seq[0].type;

    ogs_error("GTP Timeout : IMSI[%s] Message-Type[%d]",
            sgw_ue->imsi_bcd, type);
}

void sgw_s5c_handle_create_session_response(ogs_gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, ogs_gtp_message_t *message)
{
    int rv;
    uint8_t cause_value;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
    ogs_gtp_node_t *pgw = NULL;
    ogs_gtp_xact_t *s11_xact = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL, *s5u_tunnel = NULL;
    ogs_gtp_create_session_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_gtp_f_teid_t *pgw_s5c_teid = NULL;
    ogs_gtp_f_teid_t sgw_s11_teid;
    ogs_gtp_f_teid_t *pgw_s5u_teid = NULL;
    ogs_gtp_f_teid_t sgw_s1u_teid;
    int len;

    ogs_assert(s5c_xact);
    s11_xact = s5c_xact->assoc_xact;
    ogs_assert(s11_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Create Session Response");

    if (!sess) {
        ogs_warn("No Context in TEID");
        sess = s5c_xact->data;
        ogs_assert(sess);
    }

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);

    rsp = &message->create_session_response;

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
            if (rsp->bearer_contexts_created.cause.presence) {
                cause = rsp->bearer_contexts_created.cause.data;
                ogs_assert(cause);

                cause_value = cause->value;
            } else {
                ogs_error("No Cause");
                cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
            }
        } else {
            /* Deliver PGW cause value to the MME */
            ogs_warn("Cause[%d] : No Accepted", cause_value);
            ogs_gtp_send_error_message(
                    s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
                    OGS_GTP_CREATE_SESSION_RESPONSE_TYPE,
                    cause_value);
            return;
        }
    } else {
        ogs_error("No Cause");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED)
        ogs_warn("Cause[%d] : No Accepted", cause_value);

    if (rsp->bearer_contexts_created.cause.presence == 0) {
        ogs_error("No EPS Bearer Cause");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts_created.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts_created.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        bearer = sgw_bearer_find_by_sess_ebi(sess,
                    rsp->bearer_contexts_created.eps_bearer_id.u8);
        ogs_assert(bearer);
        s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
        ogs_assert(s1u_tunnel);
        s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
        ogs_assert(s5u_tunnel);

        sgw_ue = sess->sgw_ue;
        ogs_assert(sgw_ue);
    }
    if (!bearer) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
            presence == 0) {
        ogs_error("No GTP TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.presence == 0) {
        ogs_error("No GTP TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
                OGS_GTP_CREATE_SESSION_RESPONSE_TYPE,
                cause_value);
        return;
    }

    /* Receive Control Plane(UL) : PGW-S5C */
    pgw_s5c_teid = rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
                data;
    ogs_assert(pgw_s5c_teid);
    sess->pgw_s5c_teid = ntohl(pgw_s5c_teid->teid);

    /* Receive Data Plane(UL) : PGW-S5U */
    pgw_s5u_teid = rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.data;
    ogs_assert(pgw_s5u_teid);
    s5u_tunnel->remote_teid = ntohl(pgw_s5u_teid->teid);
    
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]",
        s1u_tunnel->remote_teid, s1u_tunnel->local_teid);
    ogs_debug("    SGW_S5U_TEID[%d] PGW_S5U_TEID[%d]",
        s5u_tunnel->local_teid, s5u_tunnel->remote_teid);

    pgw = ogs_gtp_node_find_by_f_teid(&sgw_self()->pgw_s5u_list, pgw_s5u_teid);
    if (!pgw) {
        pgw = ogs_gtp_node_add_by_f_teid(
            &sgw_self()->pgw_s5u_list, pgw_s5u_teid, sgw_self()->gtpu_port,
            ogs_config()->parameter.no_ipv4,
            ogs_config()->parameter.no_ipv6,
            ogs_config()->parameter.prefer_ipv4);
        ogs_assert(pgw);

        rv = ogs_gtp_connect(
                sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, pgw);
        ogs_assert(rv == OGS_OK);
    }
    /* Setup GTP Node */
    OGS_SETUP_GTP_NODE(s5u_tunnel, pgw);

    /* Send Control Plane(UL) : SGW-S11 */
    memset(&sgw_s11_teid, 0, sizeof(ogs_gtp_f_teid_t));
    sgw_s11_teid.interface_type = OGS_GTP_F_TEID_S11_S4_SGW_GTP_C;
    sgw_s11_teid.teid = htonl(sgw_ue->sgw_s11_teid);
    rv = ogs_gtp_sockaddr_to_f_teid(
            sgw_self()->gtpc_addr, sgw_self()->gtpc_addr6, &sgw_s11_teid, &len);
    ogs_assert(rv == OGS_OK);
    rsp->sender_f_teid_for_control_plane.presence = 1;
    rsp->sender_f_teid_for_control_plane.data = &sgw_s11_teid;
    rsp->sender_f_teid_for_control_plane.len = len;

    /* Send Data Plane(UL) : SGW-S1U */
    memset(&sgw_s1u_teid, 0, sizeof(ogs_gtp_f_teid_t));
    sgw_s1u_teid.interface_type = s1u_tunnel->interface_type;
    sgw_s1u_teid.teid = htonl(s1u_tunnel->local_teid);
    if (sgw_self()->gtpu_addr) {
        addr = ogs_hash_get(sgw_self()->adv_gtpu_hash,
                        &sgw_self()->gtpu_addr->sin.sin_addr,
                        sizeof(sgw_self()->gtpu_addr->sin.sin_addr));
    }
    if (sgw_self()->gtpu_addr6) {
        addr6 = ogs_hash_get(sgw_self()->adv_gtpu_hash6,
                        &sgw_self()->gtpu_addr6->sin6.sin6_addr,
                        sizeof(sgw_self()->gtpu_addr6->sin6.sin6_addr));
    }
    // Swap the SGW-S1U IP to IP to be advertised to UE
    if (addr || addr6) {
        rv = ogs_gtp_sockaddr_to_f_teid(addr,  addr6, &sgw_s1u_teid, &len);
        ogs_assert(rv == OGS_OK);
    } else {
        rv = ogs_gtp_sockaddr_to_f_teid(
        sgw_self()->gtpu_addr,  sgw_self()->gtpu_addr6, &sgw_s1u_teid, &len);
        ogs_assert(rv == OGS_OK);
    }
    rsp->bearer_contexts_created.s1_u_enodeb_f_teid.presence = 1;
    rsp->bearer_contexts_created.s1_u_enodeb_f_teid.data = &sgw_s1u_teid;
    rsp->bearer_contexts_created.s1_u_enodeb_f_teid.len = len;

    message->h.type = OGS_GTP_CREATE_SESSION_RESPONSE_TYPE;
    message->h.teid = sgw_ue->mme_s11_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(s11_xact, &message->h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s5c_handle_delete_session_response(ogs_gtp_xact_t *s5c_xact,
    sgw_sess_t *sess, ogs_gtp_message_t *message)
{
    int rv;
    uint8_t cause_value;
    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp_delete_session_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(s5c_xact);
    s11_xact = s5c_xact->assoc_xact;
    ogs_assert(s11_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Delete Session Response");

    if (!sess) {
        ogs_warn("No Context in TEID");
        sess = s5c_xact->data;
        ogs_assert(sess);
    }

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);

    rsp = &message->delete_session_response;

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
    } else {
        ogs_error("No Cause");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
                OGS_GTP_DELETE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);

    /* Remove a pgw session */
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    sgw_sess_remove(sess);

    message->h.type = OGS_GTP_DELETE_SESSION_RESPONSE_TYPE;
    message->h.teid = sgw_ue->mme_s11_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(s11_xact, &message->h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s5c_handle_create_bearer_request(ogs_gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, ogs_gtp_message_t *message)
{
    int rv;
    uint8_t cause_value = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
    ogs_gtp_node_t *pgw = NULL;
    ogs_gtp_xact_t *s11_xact = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL, *s5u_tunnel = NULL;
    ogs_gtp_create_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_gtp_f_teid_t *pgw_s5u_teid = NULL;
    ogs_gtp_f_teid_t sgw_s1u_teid;
    int len;

    ogs_assert(s5c_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Create Bearer Request");

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;
    req = &message->create_bearer_request;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (req->linked_eps_bearer_id.presence == 0) {
        ogs_error("No Linked EBI");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.s5_s8_u_sgw_f_teid.presence == 0) {
        ogs_error("No GTP TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP_CREATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);

    bearer = sgw_bearer_add(sess);
    ogs_assert(bearer);
    s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
    ogs_assert(s1u_tunnel);
    s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
    ogs_assert(s5u_tunnel);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    /* Receive Data Plane(UL) : PGW-S5U */
    pgw_s5u_teid = req->bearer_contexts.s5_s8_u_sgw_f_teid.data;
    ogs_assert(pgw_s5u_teid);
    s5u_tunnel->remote_teid = ntohl(pgw_s5u_teid->teid);
    pgw = ogs_gtp_node_find_by_f_teid(&sgw_self()->pgw_s5u_list, pgw_s5u_teid);
    if (!pgw) {
        pgw = ogs_gtp_node_add_by_f_teid(
            &sgw_self()->pgw_s5u_list, pgw_s5u_teid, sgw_self()->gtpu_port,
            ogs_config()->parameter.no_ipv4,
            ogs_config()->parameter.no_ipv6,
            ogs_config()->parameter.prefer_ipv4);
        ogs_assert(pgw);

        rv = ogs_gtp_connect(
                sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, pgw);
        ogs_assert(rv == OGS_OK);
    }
    /* Setup GTP Node */
    OGS_SETUP_GTP_NODE(s5u_tunnel, pgw);

    /* Remove S5U-F-TEID */
    req->bearer_contexts.s5_s8_u_sgw_f_teid.presence = 0;

    /* Send Data Plane(UL) : SGW-S1U */
    memset(&sgw_s1u_teid, 0, sizeof(ogs_gtp_f_teid_t));
    sgw_s1u_teid.interface_type = s1u_tunnel->interface_type;
    sgw_s1u_teid.teid = htonl(s1u_tunnel->local_teid);
    if (sgw_self()->gtpu_addr) {
        addr = ogs_hash_get(sgw_self()->adv_gtpu_hash,
                        &sgw_self()->gtpu_addr->sin.sin_addr,
                        sizeof(sgw_self()->gtpu_addr->sin.sin_addr));
    }
    if (sgw_self()->gtpu_addr6) {
        addr6 = ogs_hash_get(sgw_self()->adv_gtpu_hash6,
                        &sgw_self()->gtpu_addr6->sin6.sin6_addr,
                        sizeof(sgw_self()->gtpu_addr6->sin6.sin6_addr));
    }
    // Swap the SGW-S1U IP to IP to be advertised to UE
    if (addr || addr6) {
        rv = ogs_gtp_sockaddr_to_f_teid(addr,  addr6, &sgw_s1u_teid, &len);
        ogs_assert(rv == OGS_OK);
    } else {
        rv = ogs_gtp_sockaddr_to_f_teid(
        sgw_self()->gtpu_addr,  sgw_self()->gtpu_addr6, &sgw_s1u_teid, &len);
        ogs_assert(rv == OGS_OK);
    }
    req->bearer_contexts.s1_u_enodeb_f_teid.presence = 1;
    req->bearer_contexts.s1_u_enodeb_f_teid.data = &sgw_s1u_teid;
    req->bearer_contexts.s1_u_enodeb_f_teid.len = len;

    message->h.type = OGS_GTP_CREATE_BEARER_REQUEST_TYPE;
    message->h.teid = sgw_ue->mme_s11_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    s11_xact = ogs_gtp_xact_local_create(
            sgw_ue->gnode, &message->h, pkbuf, timeout, sess);
    ogs_expect_or_return(s11_xact);

    ogs_gtp_xact_associate(s5c_xact, s11_xact);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s5c_handle_update_bearer_request(ogs_gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, ogs_gtp_message_t *message)
{
    int rv;
    uint8_t cause_value = 0;
    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp_update_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(s5c_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Update Bearer Request");

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;
    req = &message->update_bearer_request;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    message->h.type = OGS_GTP_UPDATE_BEARER_REQUEST_TYPE;
    message->h.teid = sgw_ue->mme_s11_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    s11_xact = s5c_xact->assoc_xact;
    if (!s11_xact) {
        s11_xact = ogs_gtp_xact_local_create(
                sgw_ue->gnode, &message->h, pkbuf, timeout, sess);
        ogs_expect_or_return(s11_xact);

        ogs_gtp_xact_associate(s5c_xact, s11_xact);
    } else {
        rv = ogs_gtp_xact_update_tx(s11_xact, &message->h, pkbuf);
        ogs_expect_or_return(rv == OGS_OK);
    }

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);

    ogs_debug("[SGW] Update Bearer Request : SGW <-- PGW");
}

void sgw_s5c_handle_delete_bearer_request(ogs_gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, ogs_gtp_message_t *message)
{
    int rv;
    uint8_t cause_value = 0;
    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp_delete_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(s5c_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Delete Bearer Request");

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;
    req = &message->delete_bearer_request;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (req->linked_eps_bearer_id.presence == 0 &&
        req->eps_bearer_ids.presence == 0) {
        ogs_error("No Linked EBI or EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP_DELETE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    message->h.type = OGS_GTP_DELETE_BEARER_REQUEST_TYPE;
    message->h.teid = sgw_ue->mme_s11_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    s11_xact = s5c_xact->assoc_xact;
    if (!s11_xact) {
        s11_xact = ogs_gtp_xact_local_create(
                sgw_ue->gnode, &message->h, pkbuf, timeout, sess);
        ogs_expect_or_return(s11_xact);

        ogs_gtp_xact_associate(s5c_xact, s11_xact);
    } else {
        rv = ogs_gtp_xact_update_tx(s11_xact, &message->h, pkbuf);
        ogs_expect_or_return(rv == OGS_OK);
    }

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s5c_handle_bearer_resource_failure_indication(ogs_gtp_xact_t *s5c_xact,
        sgw_sess_t *sess, ogs_gtp_message_t *message)
{
    uint8_t cause_value = 0;
    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp_bearer_resource_failure_indication_t *ind = NULL;

    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(s5c_xact);
    s11_xact = s5c_xact->assoc_xact;
    ogs_assert(s11_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Bearer Resource Failure Indication");

    ind = &message->bearer_resource_failure_indication;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (ind->cause.presence) {
        ogs_gtp_cause_t *cause = ind->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
    } else {
        ogs_error("No Cause");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);

    ogs_gtp_send_error_message(s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
            OGS_GTP_BEARER_RESOURCE_FAILURE_INDICATION_TYPE, cause_value);
}


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

#include "gtp/gtp-conv.h"
#include "gtp/gtp-types.h"
#include "gtp/gtp-node.h"
#include "gtp/gtp-path.h"

#include "app/context.h"
#include "sgw-event.h"
#include "sgw-context.h"
#include "sgw-gtp-path.h"
#include "sgw-s5c-handler.h"

void sgw_s5c_handle_create_session_response(gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, gtp_message_t *gtp_message)
{
    int rv;
    gtp_node_t *pgw = NULL;
    gtp_xact_t *s11_xact = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL, *s5u_tunnel = NULL;
    gtp_create_session_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    gtp_f_teid_t *pgw_s5c_teid = NULL;
    gtp_f_teid_t sgw_s11_teid;
    gtp_f_teid_t *pgw_s5u_teid = NULL;
    gtp_f_teid_t sgw_s1u_teid;
    int len;

    ogs_assert(sess);
    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);
    ogs_assert(s5c_xact);
    s11_xact = s5c_xact->assoc_xact;
    ogs_assert(s11_xact);
    ogs_assert(gtp_message);

    ogs_debug("[SGW] Create Session Response");
    rsp = &gtp_message->create_session_response;

    if (rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
            presence == 0) {
        ogs_error("No GTP TEID");
        return;
    }
    if (rsp->bearer_contexts_created.presence == 0) {
        ogs_error("No Bearer");
        return;
    }
    if (rsp->bearer_contexts_created.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        return;
    }
    if (rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.presence == 0) {
        ogs_error("No GTP TEID");
        return;
    }

    bearer = sgw_bearer_find_by_sess_ebi(sess, 
                rsp->bearer_contexts_created.eps_bearer_id.u8);
    ogs_assert(bearer);
    s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
    ogs_assert(s1u_tunnel);
    s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
    ogs_assert(s5u_tunnel);

    /* Receive Control Plane(UL) : PGW-S5C */
    pgw_s5c_teid = rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
                data;
    ogs_assert(pgw_s5c_teid);
    sess->pgw_s5c_teid = ntohl(pgw_s5c_teid->teid);
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
                presence = 0;

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

    pgw = gtp_node_find(&sgw_self()->pgw_s5u_list, pgw_s5u_teid);
    if (!pgw) {
        pgw = gtp_node_add(&sgw_self()->pgw_s5u_list, pgw_s5u_teid,
            sgw_self()->gtpu_port,
            context_self()->config.parameter.no_ipv4,
            context_self()->config.parameter.no_ipv6,
            context_self()->config.parameter.prefer_ipv4);
        ogs_assert(pgw);

        rv = gtp_connect(
                sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, pgw);
        ogs_assert(rv == OGS_OK);
    }
    /* Setup GTP Node */
    SETUP_GTP_NODE(s5u_tunnel, pgw);

    /* Remove S5C-F-TEID */
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.presence = 0;

    /* Send Control Plane(UL) : SGW-S11 */
    memset(&sgw_s11_teid, 0, sizeof(gtp_f_teid_t));
    sgw_s11_teid.interface_type = GTP_F_TEID_S11_S4_SGW_GTP_C;
    sgw_s11_teid.teid = htonl(sgw_ue->sgw_s11_teid);
    rv = gtp_sockaddr_to_f_teid(
            sgw_self()->gtpc_addr, sgw_self()->gtpc_addr6, &sgw_s11_teid, &len);
    ogs_assert(rv == OGS_OK);
    rsp->sender_f_teid_for_control_plane.presence = 1;
    rsp->sender_f_teid_for_control_plane.data = &sgw_s11_teid;
    rsp->sender_f_teid_for_control_plane.len = len;

    /* Send Data Plane(UL) : SGW-S1U */
    memset(&sgw_s1u_teid, 0, sizeof(gtp_f_teid_t));
    sgw_s1u_teid.interface_type = s1u_tunnel->interface_type;
    sgw_s1u_teid.teid = htonl(s1u_tunnel->local_teid);
    rv = gtp_sockaddr_to_f_teid(
        sgw_self()->gtpu_addr,  sgw_self()->gtpu_addr6, &sgw_s1u_teid, &len);
    ogs_assert(rv == OGS_OK);
    rsp->bearer_contexts_created.s1_u_enodeb_f_teid.presence = 1;
    rsp->bearer_contexts_created.s1_u_enodeb_f_teid.data = &sgw_s1u_teid;
    rsp->bearer_contexts_created.s1_u_enodeb_f_teid.len = len;

    rv = gtp_xact_commit(s5c_xact);
    ogs_assert(rv == OGS_OK);

    gtp_message->h.type = GTP_CREATE_SESSION_RESPONSE_TYPE;
    gtp_message->h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(s11_xact, &gtp_message->h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_commit(s11_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s5c_handle_delete_session_response(gtp_xact_t *s5c_xact,
    sgw_sess_t *sess, gtp_message_t *gtp_message)
{
    int rv;
    gtp_xact_t *s11_xact = NULL;
    gtp_delete_session_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    uint32_t mme_s11_teid;
    gtp_cause_t *cause = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(sess);
    sgw_ue = sess->sgw_ue;
    ogs_assert(s5c_xact);
    s11_xact = s5c_xact->assoc_xact;
    ogs_assert(s11_xact);
    ogs_assert(gtp_message);

    rsp = &gtp_message->delete_session_response;

    if (rsp->cause.presence == 0) {
        ogs_error("No Cause");
        return;
    }

    cause = rsp->cause.data;
    ogs_assert(cause);

    /* Remove a pgw session */
    if (sess) {
        ogs_debug("[SGW] Delete Session Response");
        ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
        ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);

        /* backup sgw_s5c_teid in session context */
        mme_s11_teid = sgw_ue->mme_s11_teid;

        if (sgw_sess_remove(sess) != OGS_OK)
        {
            ogs_error("Error on PGW session removal");
            cause->value = GTP_CAUSE_CONTEXT_NOT_FOUND;
        }
    } else {
        cause->value = GTP_CAUSE_INVALID_PEER;
        ogs_error("Cannot find session");
        return;
    }

    rv = gtp_xact_commit(s5c_xact);
    ogs_assert(rv == OGS_OK);

    gtp_message->h.type = GTP_DELETE_SESSION_RESPONSE_TYPE;
    gtp_message->h.teid = mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(s11_xact, &gtp_message->h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_commit(s11_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s5c_handle_create_bearer_request(gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, gtp_message_t *gtp_message)
{
    int rv;
    gtp_node_t *pgw = NULL;
    gtp_xact_t *s11_xact = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL, *s5u_tunnel = NULL;
    gtp_create_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    gtp_f_teid_t *pgw_s5u_teid = NULL;
    gtp_f_teid_t sgw_s1u_teid;
    int len;

    ogs_assert(sess);
    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);
    ogs_assert(s5c_xact);
    ogs_assert(gtp_message);

    req = &gtp_message->create_bearer_request;

    ogs_debug("[SGW] Create Bearer Request");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    if (req->linked_eps_bearer_id.presence == 0) {
        ogs_error("No Linked EBI");
        return;
    }
    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        return;
    }
    if (req->bearer_contexts.s5_s8_u_sgw_f_teid.presence == 0) {
        ogs_error("No GTP TEID");
        return;
    }

    bearer = sgw_bearer_add(sess);
    ogs_assert(bearer);
    s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
    ogs_assert(s1u_tunnel);
    s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
    ogs_assert(s5u_tunnel);

    /* Receive Data Plane(UL) : PGW-S5U */
    pgw_s5u_teid = req->bearer_contexts.s5_s8_u_sgw_f_teid.data;
    ogs_assert(pgw_s5u_teid);
    s5u_tunnel->remote_teid = ntohl(pgw_s5u_teid->teid);
    pgw = gtp_node_find(&sgw_self()->pgw_s5u_list, pgw_s5u_teid);
    if (!pgw) {
        pgw = gtp_node_add(&sgw_self()->pgw_s5u_list, pgw_s5u_teid,
            sgw_self()->gtpu_port,
            context_self()->config.parameter.no_ipv4,
            context_self()->config.parameter.no_ipv6,
            context_self()->config.parameter.prefer_ipv4);
        ogs_assert(pgw);

        rv = gtp_connect(
                sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, pgw);
        ogs_assert(rv == OGS_OK);
    }
    /* Setup GTP Node */
    SETUP_GTP_NODE(s5u_tunnel, pgw);

    /* Remove S5U-F-TEID */
    req->bearer_contexts.s5_s8_u_sgw_f_teid.presence = 0;

    /* Send Data Plane(UL) : SGW-S1U */
    memset(&sgw_s1u_teid, 0, sizeof(gtp_f_teid_t));
    sgw_s1u_teid.interface_type = s1u_tunnel->interface_type;
    sgw_s1u_teid.teid = htonl(s1u_tunnel->local_teid);
    rv = gtp_sockaddr_to_f_teid(
        sgw_self()->gtpu_addr, sgw_self()->gtpu_addr6, &sgw_s1u_teid, &len);
    ogs_assert(rv == OGS_OK);
    req->bearer_contexts.s1_u_enodeb_f_teid.presence = 1;
    req->bearer_contexts.s1_u_enodeb_f_teid.data = &sgw_s1u_teid;
    req->bearer_contexts.s1_u_enodeb_f_teid.len = len;

    gtp_message->h.type = GTP_CREATE_BEARER_REQUEST_TYPE;
    gtp_message->h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    ogs_assert(rv == OGS_OK);

    s11_xact = gtp_xact_local_create(sgw_ue->gnode, &gtp_message->h, pkbuf);
    ogs_assert(s11_xact);

    gtp_xact_associate(s5c_xact, s11_xact);

    rv = gtp_xact_commit(s11_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s5c_handle_update_bearer_request(gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, gtp_message_t *gtp_message)
{
    int rv;
    gtp_xact_t *s11_xact = NULL;
    gtp_update_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(sess);
    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);
    ogs_assert(s5c_xact);
    ogs_assert(gtp_message);

    ogs_debug("[SGW] Update Bearer Request");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    req = &gtp_message->update_bearer_request;

    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        return;
    }

    gtp_message->h.type = GTP_UPDATE_BEARER_REQUEST_TYPE;
    gtp_message->h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    ogs_assert(rv == OGS_OK);

    s11_xact = gtp_xact_local_create(sgw_ue->gnode, &gtp_message->h, pkbuf);
    ogs_assert(s11_xact);

    gtp_xact_associate(s5c_xact, s11_xact);

    rv = gtp_xact_commit(s11_xact);
    ogs_assert(rv == OGS_OK);

    ogs_debug("[SGW] Update Bearer Request : SGW <-- PGW");
}

void sgw_s5c_handle_delete_bearer_request(gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, gtp_message_t *gtp_message)
{
    int rv;
    gtp_xact_t *s11_xact = NULL;
    gtp_delete_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(sess);
    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);
    ogs_assert(s5c_xact);
    ogs_assert(gtp_message);

    req = &gtp_message->delete_bearer_request;

    ogs_debug("[SGW] Delete Bearer Request");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    if (req->linked_eps_bearer_id.presence == 0 &&
        req->eps_bearer_ids.presence == 0) {
        ogs_error("No Linked EBI or EPS Bearer ID");
        return;
    }

    gtp_message->h.type = GTP_DELETE_BEARER_REQUEST_TYPE;
    gtp_message->h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    ogs_assert(rv == OGS_OK);

    s11_xact = gtp_xact_local_create(sgw_ue->gnode, &gtp_message->h, pkbuf);
    ogs_assert(s11_xact);

    gtp_xact_associate(s5c_xact, s11_xact);

    rv = gtp_xact_commit(s11_xact);
    ogs_assert(rv == OGS_OK);
}


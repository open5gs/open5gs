#define TRACE_MODULE _sgw_s5c_handler

#include "core_debug.h"
#include "core_lib.h"

#include "gtp/gtp_conv.h"
#include "gtp/gtp_types.h"
#include "gtp/gtp_node.h"
#include "gtp/gtp_path.h"

#include "common/context.h"
#include "sgw_event.h"
#include "sgw_context.h"
#include "sgw_gtp_path.h"
#include "sgw_s5c_handler.h"

void sgw_s5c_handle_create_session_response(gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, gtp_message_t *gtp_message)
{
    status_t rv;
    gtp_node_t *pgw = NULL;
    gtp_xact_t *s11_xact = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL, *s5u_tunnel = NULL;
    gtp_create_session_response_t *rsp = NULL;
    pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    gtp_f_teid_t *pgw_s5c_teid = NULL;
    gtp_f_teid_t sgw_s11_teid;
    gtp_f_teid_t *pgw_s5u_teid = NULL;
    gtp_f_teid_t sgw_s1u_teid;
    int len;

    d_assert(sess, return, "Null param");
    sgw_ue = sess->sgw_ue;
    d_assert(sgw_ue, return, "Null param");
    d_assert(s5c_xact, return, "Null param");
    s11_xact = s5c_xact->assoc_xact;
    d_assert(s11_xact, return, "Null param");
    d_assert(gtp_message, return, "Null param");

    d_trace(3, "[SGW] Create Session Response\n");
    rsp = &gtp_message->create_session_response;

    if (rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
            presence == 0)
    {
        d_error("No GTP TEID");
        return;
    }
    if (rsp->bearer_contexts_created.presence == 0)
    {
        d_error("No Bearer");
        return;
    }
    if (rsp->bearer_contexts_created.eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }
    if (rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.presence == 0)
    {
        d_error("No GTP TEID");
        return;
    }

    bearer = sgw_bearer_find_by_sess_ebi(sess, 
                rsp->bearer_contexts_created.eps_bearer_id.u8);
    d_assert(bearer, return, "No Bearer Context");
    s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
    d_assert(s1u_tunnel, return, "No Tunnel Context");
    s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
    d_assert(s5u_tunnel, return, "No Tunnel Context");

    /* Receive Control Plane(UL) : PGW-S5C */
    pgw_s5c_teid = rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
                data;
    d_assert(pgw_s5c_teid, return, "Null param");
    sess->pgw_s5c_teid = ntohl(pgw_s5c_teid->teid);
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
                presence = 0;

    /* Receive Data Plane(UL) : PGW-S5U */
    pgw_s5u_teid = rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.data;
    d_assert(pgw_s5u_teid, return, "Null param");
    s5u_tunnel->remote_teid = ntohl(pgw_s5u_teid->teid);
    
    d_trace(5, "    MME_S11_TEID[%d] SGW_S11_TEID[%d]\n",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    d_trace(5, "    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]\n",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    d_trace(5, "    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]\n",
        s1u_tunnel->remote_teid, s1u_tunnel->local_teid);
    d_trace(5, "    SGW_S5U_TEID[%d] PGW_S5U_TEID[%d]\n",
        s5u_tunnel->local_teid, s5u_tunnel->remote_teid);

    pgw = gtp_find_node(&sgw_self()->pgw_s5u_list, pgw_s5u_teid);
    if (!pgw)
    {
        pgw = gtp_add_node_with_teid(&sgw_self()->pgw_s5u_list, pgw_s5u_teid,
            sgw_self()->gtpu_port,
            context_self()->parameter.no_ipv4,
            context_self()->parameter.no_ipv6,
            context_self()->parameter.prefer_ipv4);
        d_assert(pgw, return,);

        rv = gtp_client(pgw);
        d_assert(rv == CORE_OK, return,);
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
    d_assert(rv == CORE_OK, return,);
    rsp->sender_f_teid_for_control_plane.presence = 1;
    rsp->sender_f_teid_for_control_plane.data = &sgw_s11_teid;
    rsp->sender_f_teid_for_control_plane.len = len;

    /* Send Data Plane(UL) : SGW-S1U */
    memset(&sgw_s1u_teid, 0, sizeof(gtp_f_teid_t));
    sgw_s1u_teid.interface_type = s1u_tunnel->interface_type;
    sgw_s1u_teid.teid = htonl(s1u_tunnel->local_teid);
    rv = gtp_sockaddr_to_f_teid(
        sgw_self()->gtpu_addr,  sgw_self()->gtpu_addr6, &sgw_s1u_teid, &len);
    d_assert(rv == CORE_OK, return,);
    rsp->bearer_contexts_created.s1_u_enodeb_f_teid.presence = 1;
    rsp->bearer_contexts_created.s1_u_enodeb_f_teid.data = &sgw_s1u_teid;
    rsp->bearer_contexts_created.s1_u_enodeb_f_teid.len = len;

    rv = gtp_xact_commit(s5c_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    gtp_message->h.type = GTP_CREATE_SESSION_RESPONSE_TYPE;
    gtp_message->h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    rv = gtp_xact_update_tx(s11_xact, &gtp_message->h, pkbuf);
    d_assert(rv == CORE_OK, return, "gtp_xact_update_tx error");

    rv = gtp_xact_commit(s11_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void sgw_s5c_handle_delete_session_response(gtp_xact_t *s5c_xact,
    sgw_sess_t *sess, gtp_message_t *gtp_message)
{
    status_t rv;
    gtp_xact_t *s11_xact = NULL;
    gtp_delete_session_response_t *rsp = NULL;
    pkbuf_t *pkbuf = NULL;
    c_uint32_t mme_s11_teid;
    gtp_cause_t *cause = NULL;
    sgw_ue_t *sgw_ue = NULL;

    d_assert(sess, return, "Null param");
    sgw_ue = sess->sgw_ue;
    d_assert(s5c_xact, return, "Null param");
    s11_xact = s5c_xact->assoc_xact;
    d_assert(s11_xact, return, "Null param");
    d_assert(gtp_message, return, "Null param");

    rsp = &gtp_message->delete_session_response;

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    cause = rsp->cause.data;
    d_assert(cause, return, "Null param");

    /* Remove a pgw session */
    if (sess)
    {
        d_trace(3, "[SGW] Delete Session Response\n",
                sess->sgw_s5c_teid, sess->pgw_s5c_teid);
        d_trace(5, "    MME_S11_TEID[%d] SGW_S11_TEID[%d]\n",
            sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
        d_trace(5, "    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]\n",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);

        /* backup sgw_s5c_teid in session context */
        mme_s11_teid = sgw_ue->mme_s11_teid;

        if (sgw_sess_remove(sess) != CORE_OK)
        {
            d_error("Error on PGW session %d removal", sess->index);
            cause->value = GTP_CAUSE_CONTEXT_NOT_FOUND;
        }
    }
    else
    {
        cause->value = GTP_CAUSE_INVALID_PEER;
    }

    rv = gtp_xact_commit(s5c_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    gtp_message->h.type = GTP_DELETE_SESSION_RESPONSE_TYPE;
    gtp_message->h.teid = mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    rv = gtp_xact_update_tx(s11_xact, &gtp_message->h, pkbuf);
    d_assert(rv == CORE_OK, return, "gtp_xact_update_tx error");

    rv = gtp_xact_commit(s11_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void sgw_s5c_handle_create_bearer_request(gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, gtp_message_t *gtp_message)
{
    status_t rv;
    gtp_node_t *pgw = NULL;
    gtp_xact_t *s11_xact = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL, *s5u_tunnel = NULL;
    gtp_create_bearer_request_t *req = NULL;
    pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    gtp_f_teid_t *pgw_s5u_teid = NULL;
    gtp_f_teid_t sgw_s1u_teid;
    int len;

    d_assert(sess, return, "Null param");
    sgw_ue = sess->sgw_ue;
    d_assert(sgw_ue, return, "Null param");
    d_assert(s5c_xact, return, "Null param");
    d_assert(gtp_message, return, "Null param");

    req = &gtp_message->create_bearer_request;

    d_trace(3, "[SGW] Create Bearer Request\n",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    d_trace(5, "    MME_S11_TEID[%d] SGW_S11_TEID[%d]\n",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    d_trace(5, "    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]\n",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    if (req->linked_eps_bearer_id.presence == 0)
    {
        d_error("No Linked EBI");
        return;
    }
    if (req->bearer_contexts.presence == 0)
    {
        d_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }
    if (req->bearer_contexts.s5_s8_u_sgw_f_teid.presence == 0)
    {
        d_error("No GTP TEID");
        return;
    }

    bearer = sgw_bearer_add(sess);
    d_assert(bearer, return, "No Bearer Context");
    s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
    d_assert(s1u_tunnel, return, "No Tunnel Context");
    s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
    d_assert(s5u_tunnel, return, "No Tunnel Context");

    /* Receive Data Plane(UL) : PGW-S5U */
    pgw_s5u_teid = req->bearer_contexts.s5_s8_u_sgw_f_teid.data;
    d_assert(pgw_s5u_teid, return, "Null param");
    s5u_tunnel->remote_teid = ntohl(pgw_s5u_teid->teid);
    pgw = gtp_find_node(&sgw_self()->pgw_s5u_list, pgw_s5u_teid);
    if (!pgw)
    {
        pgw = gtp_add_node_with_teid(&sgw_self()->pgw_s5u_list, pgw_s5u_teid,
            sgw_self()->gtpu_port,
            context_self()->parameter.no_ipv4,
            context_self()->parameter.no_ipv6,
            context_self()->parameter.prefer_ipv4);
        d_assert(pgw, return,);

        rv = gtp_client(pgw);
        d_assert(rv == CORE_OK, return,);
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
    d_assert(rv == CORE_OK, return,);
    req->bearer_contexts.s1_u_enodeb_f_teid.presence = 1;
    req->bearer_contexts.s1_u_enodeb_f_teid.data = &sgw_s1u_teid;
    req->bearer_contexts.s1_u_enodeb_f_teid.len = len;

    gtp_message->h.type = GTP_CREATE_BEARER_REQUEST_TYPE;
    gtp_message->h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    s11_xact = gtp_xact_local_create(sgw_ue->gnode, &gtp_message->h, pkbuf);
    d_assert(s11_xact, return, "Null param");

    gtp_xact_associate(s5c_xact, s11_xact);

    rv = gtp_xact_commit(s11_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void sgw_s5c_handle_update_bearer_request(gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, gtp_message_t *gtp_message)
{
    status_t rv;
    gtp_xact_t *s11_xact = NULL;
    gtp_update_bearer_request_t *req = NULL;
    pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    d_assert(sess, return, "Null param");
    sgw_ue = sess->sgw_ue;
    d_assert(sgw_ue, return, "Null param");
    d_assert(s5c_xact, return, "Null param");
    d_assert(gtp_message, return, "Null param");

    d_trace(3, "[SGW] Update Bearer Request\n",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    d_trace(5, "    MME_S11_TEID[%d] SGW_S11_TEID[%d]\n",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    d_trace(5, "    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]\n",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    req = &gtp_message->update_bearer_request;

    if (req->bearer_contexts.presence == 0)
    {
        d_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }

    gtp_message->h.type = GTP_UPDATE_BEARER_REQUEST_TYPE;
    gtp_message->h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    s11_xact = gtp_xact_local_create(sgw_ue->gnode, &gtp_message->h, pkbuf);
    d_assert(s11_xact, return, "Null param");

    gtp_xact_associate(s5c_xact, s11_xact);

    rv = gtp_xact_commit(s11_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    d_trace(3, "[SGW] Update Bearer Request : SGW <-- PGW\n");
}

void sgw_s5c_handle_delete_bearer_request(gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, gtp_message_t *gtp_message)
{
    status_t rv;
    gtp_xact_t *s11_xact = NULL;
    gtp_delete_bearer_request_t *req = NULL;
    pkbuf_t *pkbuf = NULL;
    sgw_ue_t *sgw_ue = NULL;

    d_assert(sess, return, "Null param");
    sgw_ue = sess->sgw_ue;
    d_assert(sgw_ue, return, "Null param");
    d_assert(s5c_xact, return, "Null param");
    d_assert(gtp_message, return, "Null param");

    req = &gtp_message->delete_bearer_request;

    d_trace(3, "[SGW] Delete Bearer Request\n",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    d_trace(5, "    MME_S11_TEID[%d] SGW_S11_TEID[%d]\n",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    d_trace(5, "    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]\n",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    if (req->linked_eps_bearer_id.presence == 0 &&
        req->eps_bearer_ids.presence == 0)
    {
        d_error("No Linked EBI or EPS Bearer ID");
        return;
    }

    gtp_message->h.type = GTP_DELETE_BEARER_REQUEST_TYPE;
    gtp_message->h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    s11_xact = gtp_xact_local_create(sgw_ue->gnode, &gtp_message->h, pkbuf);
    d_assert(s11_xact, return, "Null param");

    gtp_xact_associate(s5c_xact, s11_xact);

    rv = gtp_xact_commit(s11_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}


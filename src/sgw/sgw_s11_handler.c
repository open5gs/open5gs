#define TRACE_MODULE _sgw_s11_handler

#include "core_debug.h"
#include "core_lib.h"

#include "gtp_types.h"

#include "sgw_event.h"
#include "sgw_context.h"
#include "sgw_gtp_path.h"
#include "sgw_s11_handler.h"

void sgw_s11_handle_create_session_request(gtp_xact_t *s11_xact,
        sgw_ue_t *sgw_ue, gtp_message_t *gtp_message)
{
    status_t rv;
    c_uint16_t decoded;
    gtp_create_session_request_t *req = NULL;
    pkbuf_t *pkbuf = NULL;
    gtp_f_teid_t *mme_s11_teid = NULL;
    gtp_f_teid_t *pgw_s5c_teid = NULL;
    sgw_pgw_t *pgw = NULL;
    c_uint32_t addr;
    c_uint16_t port;
    gtp_f_teid_t sgw_s5c_teid, sgw_s5u_teid;
    gtp_uli_t uli;

    gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;

    d_assert(s11_xact, return, "Null param");
    d_assert(sgw_ue, return, "Null param");
    d_assert(gtp_message, return, "Null param");

    req = &gtp_message->create_session_request;

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No TEID");
        return;
    }
    if (req->bearer_contexts_to_be_created.presence == 0)
    {
        d_error("No Bearer");
        return;
    }
    if (req->bearer_contexts_to_be_created.eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }
    if (req->access_point_name.presence == 0)
    {
        d_error("No APN");
        return;
    }
    if (req->pgw_s5_s8_address_for_control_plane_or_pmip.presence == 0)
    {
        d_error("No PGW IP");
        return;
    }
    if (req->user_location_information.presence == 0)
    {
        d_error("No User Location Inforamtion");
        return;
    }

    sess = sgw_sess_find_by_ebi(sgw_ue,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);
    if (!sess)
    {
        c_int8_t apn[MAX_APN_LEN];
        apn_parse(apn, req->access_point_name.data, req->access_point_name.len);
        sess = sgw_sess_add(sgw_ue, apn,
                req->bearer_contexts_to_be_created.eps_bearer_id.u8);
    }
    d_assert(sess, return, "Null param");
    bearer = sgw_default_bearer_in_sess(sess);
    d_assert(bearer, return, "Null param");

    /* Receive Control Plane(DL) : MME-S11 */
    mme_s11_teid = req->sender_f_teid_for_control_plane.data;
    d_assert(mme_s11_teid, return, "Null param");
    sgw_ue->mme_s11_teid = ntohl(mme_s11_teid->teid);
    sgw_ue->mme_s11_addr = mme_s11_teid->ipv4_addr;

    /* Send Control Plane(DL) : SGW-S5C */
    memset(&sgw_s5c_teid, 0, sizeof(gtp_f_teid_t));
    sgw_s5c_teid.teid = htonl(sess->sgw_s5c_teid);
    sgw_s5c_teid.ipv4_addr = sess->sgw_s5c_addr;
    sgw_s5c_teid.interface_type = GTP_F_TEID_S5_S8_SGW_GTP_C;
    req->sender_f_teid_for_control_plane.presence = 1;
    req->sender_f_teid_for_control_plane.data = &sgw_s5c_teid;
    req->sender_f_teid_for_control_plane.len = GTP_F_TEID_IPV4_LEN;

    /* Remove PGW-S5C */
    pgw_s5c_teid = req->pgw_s5_s8_address_for_control_plane_or_pmip.data;
    d_assert(pgw_s5c_teid, return, "Null param");

    addr = pgw_s5c_teid->ipv4_addr;
    port = GTPV2_C_UDP_PORT;

    pgw = sgw_pgw_find(addr, port);
    if (!pgw)
    {
        pgw = sgw_pgw_add();
        d_assert(pgw, return, "Can't add PGW-GTP node");

        pgw->addr = addr;
        pgw->port = port;
        pgw->sock = sgw_self()->s5c_sock;
    }

    /* Setup GTP Node */
    CONNECT_MME_GTP_NODE(sess, s11_xact);
    CONNECT_PGW_GTP_NODE(sess, pgw);

    req->pgw_s5_s8_address_for_control_plane_or_pmip.presence = 0;

    /* Data Plane(DL) : SGW-S5U */
    memset(&sgw_s5u_teid, 0, sizeof(gtp_f_teid_t));
    sgw_s5u_teid.teid = htonl(bearer->sgw_s5u_teid);
    sgw_s5u_teid.ipv4_addr = bearer->sgw_s5u_addr;
    sgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_SGW_GTP_U;
    req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.presence = 1;
    req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.data = &sgw_s5u_teid;
    req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.len = 
        GTP_F_TEID_IPV4_LEN;

    /* Set User Location Information */
    decoded = gtp_parse_uli(&uli, &req->user_location_information);
    d_assert(req->user_location_information.len == decoded, return,);
    memcpy(&bearer->tai.plmn_id, &uli.tai.plmn_id, sizeof(uli.tai.plmn_id));
    bearer->tai.tac = uli.tai.tac;
    memcpy(&bearer->e_cgi.plmn_id, &uli.e_cgi.plmn_id, sizeof(uli.e_cgi.plmn_id));
    bearer->e_cgi.cell_id = uli.e_cgi.cell_id;

    gtp_message->h.type = GTP_CREATE_SESSION_REQUEST_TYPE;
    gtp_message->h.teid = sess->pgw_s5c_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    s5c_xact = gtp_xact_local_create(sess->pgw, &gtp_message->h, pkbuf);
    d_assert(s5c_xact, return, "Null param");

    gtp_xact_associate(s11_xact, s5c_xact);

    rv = gtp_xact_commit(s5c_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    d_trace(3, "[GTP] Create Session Reqeust : "
            "SGW[%d] --> PGW\n", sess->sgw_s5c_teid);
}

CORE_DECLARE(void) sgw_s11_handle_modify_bearer_request(gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, gtp_modify_bearer_request_t *req)
{
    status_t rv;
    c_uint16_t decoded;
    sgw_bearer_t *bearer = NULL;
    gtp_modify_bearer_response_t *rsp = NULL;
    pkbuf_t *pkbuf = NULL;
    gtp_message_t gtp_message;
    
    gtp_cause_t cause;
    gtp_f_teid_t *enb_s1u_teid = NULL;
    gtp_uli_t uli;

    d_assert(s11_xact, return, "Null param");
    d_assert(sgw_ue, return, "Null param");
    d_assert(req, return, "Null param");

    if (req->bearer_contexts_to_be_modified.presence == 0)
    {
        d_error("No Bearer");
        return;
    }
    if (req->bearer_contexts_to_be_modified.eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }
    if (req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.presence == 0)
    {
        d_error("No eNB TEID");
        return;
    }

    bearer = sgw_bearer_find_by_ue_ebi(sgw_ue, 
                req->bearer_contexts_to_be_modified.eps_bearer_id.u8);
    d_assert(bearer, return, "No Bearer Context");

    /* Set User Location Information */
    if (req->user_location_information.presence == 1)
    {
        decoded = gtp_parse_uli(&uli, &req->user_location_information);
        d_assert(req->user_location_information.len == decoded, return,);
        memcpy(&bearer->tai.plmn_id, &uli.tai.plmn_id, sizeof(uli.tai.plmn_id));
        bearer->tai.tac = uli.tai.tac;
        memcpy(&bearer->e_cgi.plmn_id, &uli.e_cgi.plmn_id, sizeof(uli.e_cgi.plmn_id));
        if (bearer->e_cgi.cell_id != uli.e_cgi.cell_id)
        {
            rv = sgw_gtp_send_end_marker(bearer);
            d_assert(rv == CORE_OK, return, "gtp send failed");

            bearer->e_cgi.cell_id = uli.e_cgi.cell_id;
        }
    }

    /* Data Plane(DL) : eNB-S1U */
    enb_s1u_teid = req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.data;
    bearer->enb_s1u_teid = ntohl(enb_s1u_teid->teid);
    bearer->enb_s1u_addr = enb_s1u_teid->ipv4_addr;

    /* Reset UE state */
    SGW_RESET_UE_STATE(sgw_ue, SGW_S1U_INACTIVE);

    rsp = &gtp_message.modify_bearer_response;
    memset(&gtp_message, 0, sizeof(gtp_message_t));

    memset(&cause, 0, sizeof(cause));
    cause.value = GTP_CAUSE_REQUEST_ACCEPTED;

    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);

    gtp_message.h.type = GTP_MODIFY_BEARER_RESPONSE_TYPE;
    gtp_message.h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, &gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    rv = gtp_xact_update_tx(s11_xact, &gtp_message.h, pkbuf);
    d_assert(rv == CORE_OK, return, "gtp_xact_update_tx error");

    rv = gtp_xact_commit(s11_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    d_trace(3, "[GTP] Modify Bearer Reqeust : "
        "MME[%d] --> SGW[%d]\n", sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
}

void sgw_s11_handle_delete_session_request(gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, gtp_message_t *gtp_message)
{
    status_t rv;
    pkbuf_t *pkbuf = NULL;
    gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    gtp_delete_session_request_t *req = NULL;

    d_assert(s11_xact, return, "Null param");
    d_assert(sgw_ue, return, "Null param");
    d_assert(gtp_message, return, "Null param");

    req = &gtp_message->delete_session_request;
    d_assert(req, return, "Null param");

    if (req->linked_eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }

    sess = sgw_sess_find_by_ebi(sgw_ue, req->linked_eps_bearer_id.u8);
    d_assert(sess, return, "Null param");

    gtp_message->h.type = GTP_DELETE_SESSION_REQUEST_TYPE;
    gtp_message->h.teid = sess->pgw_s5c_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    s5c_xact = gtp_xact_local_create(sess->pgw, &gtp_message->h, pkbuf);
    d_assert(s5c_xact, return, "Null param");

    gtp_xact_associate(s11_xact, s5c_xact);

    rv = gtp_xact_commit(s5c_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    d_trace(3, "[GTP] Delete Session Reqeust : "
        "SGW[%d] --> PGW[%d]\n", sess->sgw_s5c_teid, sess->pgw_s5c_teid);
}

void sgw_s11_handle_create_bearer_response(gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, gtp_message_t *gtp_message)
{
    status_t rv;
    c_uint16_t decoded;
    pkbuf_t *pkbuf = NULL;
    gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL;
    gtp_create_bearer_response_t *req = NULL;

    gtp_f_teid_t *sgw_s1u_teid = NULL, *enb_s1u_teid = NULL;
    gtp_f_teid_t sgw_s5u_teid, pgw_s5u_teid;
    gtp_uli_t uli;

    d_assert(s11_xact, return, "Null param");
    d_assert(sgw_ue, return, "Null param");
    d_assert(gtp_message, return, "Null param");
    s5c_xact = s11_xact->assoc_xact;
    d_assert(s5c_xact, return, "Null param");

    req = &gtp_message->create_bearer_response;
    d_assert(req, return, "Null param");

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
    if (req->bearer_contexts.s1_u_enodeb_f_teid.presence == 0)
    {
        d_error("No eNB TEID");
        return;
    }
    if (req->bearer_contexts.s4_u_sgsn_f_teid.presence == 0)
    {
        d_error("No SGW TEID");
        return;
    }
    if (req->user_location_information.presence == 0)
    {
        d_error("No User Location Inforamtion");
        return;
    }

    /* Correlate with SGW-S1U-TEID */
    sgw_s1u_teid = req->bearer_contexts.s4_u_sgsn_f_teid.data;
    d_assert(sgw_s1u_teid, return, "Null param");
    req->bearer_contexts.s4_u_sgsn_f_teid.presence = 0;

    /* Find the Tunnel by SGW-S1U-TEID */
    tunnel = sgw_tunnel_find_by_teid(ntohl(sgw_s1u_teid->teid));
    d_assert(tunnel, return, "Null param");
    bearer = tunnel->bearer;
    d_assert(bearer, return, "Null param");
    sess = bearer->sess;
    d_assert(sess, return, "Null param");

    /* Set EBI */
    bearer->ebi = req->bearer_contexts.eps_bearer_id.u8;

    /* Data Plane(DL) : eNB-S1U */
    enb_s1u_teid = req->bearer_contexts.s1_u_enodeb_f_teid.data;
    bearer->enb_s1u_teid = ntohl(enb_s1u_teid->teid);
    bearer->enb_s1u_addr = enb_s1u_teid->ipv4_addr;
    req->bearer_contexts.s1_u_enodeb_f_teid.presence = 0;

    decoded = gtp_parse_uli(&uli, &req->user_location_information);
    d_assert(req->user_location_information.len == decoded, return,);
    memcpy(&bearer->tai.plmn_id, &uli.tai.plmn_id, sizeof(uli.tai.plmn_id));
    bearer->tai.tac = uli.tai.tac;
    memcpy(&bearer->e_cgi.plmn_id, &uli.e_cgi.plmn_id, sizeof(uli.e_cgi.plmn_id));
    bearer->e_cgi.cell_id = uli.e_cgi.cell_id;

    /* Reset UE state */
    SGW_RESET_UE_STATE(sgw_ue, SGW_S1U_INACTIVE);

    /* Data Plane(DL) : SGW-S5U */
    memset(&sgw_s5u_teid, 0, sizeof(gtp_f_teid_t));
    sgw_s5u_teid.teid = htonl(bearer->sgw_s5u_teid);
    sgw_s5u_teid.ipv4_addr = bearer->sgw_s5u_addr;
    sgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_SGW_GTP_U;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.presence = 1;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.data = &sgw_s5u_teid;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.len = GTP_F_TEID_IPV4_LEN;

    /* Data Plane(DL) : PGW-S5U */
    memset(&pgw_s5u_teid, 0, sizeof(gtp_f_teid_t));
    pgw_s5u_teid.teid = htonl(tunnel->remote_teid);
    pgw_s5u_teid.ipv4_addr = tunnel->remote_addr;
    pgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_U;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.presence = 1;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.data = &pgw_s5u_teid;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.len = GTP_F_TEID_IPV4_LEN;

    gtp_message->h.type = GTP_CREATE_BEARER_RESPONSE_TYPE;
    gtp_message->h.teid = sess->pgw_s5c_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    rv = gtp_xact_update_tx(s5c_xact, &gtp_message->h, pkbuf);
    d_assert(s5c_xact, return, "Null param");

    rv = gtp_xact_commit(s5c_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    d_trace(3, "[GTP] Create Bearer Response : "
        "SGW[%d] --> PGW[%d]\n", sess->sgw_s5c_teid, sess->pgw_s5c_teid);
}

void sgw_s11_handle_release_access_bearers_request(gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, gtp_release_access_bearers_request_t *req)
{
    status_t rv;
    gtp_release_access_bearers_response_t *rsp = NULL;
    pkbuf_t *pkbuf = NULL;
    gtp_message_t gtp_message;
    sgw_bearer_t *bearer = NULL, *next_bearer = NULL;
    sgw_sess_t *sess = NULL;
    
    gtp_cause_t cause;

    d_assert(sgw_ue, return, "Null param");
    d_assert(s11_xact, return, "Null param");
    d_assert(req, return, "Null param");

    /* Set UE state to S1UE_INACTIVE */
    SGW_SET_UE_STATE(sgw_ue, SGW_S1U_INACTIVE);
    /* ReSet UE state to S1UE_INACTIVE */
    SGW_RESET_UE_STATE(sgw_ue, SGW_DL_NOTI_SENT);

    /* Release S1U(DL) path */
    sess = sgw_sess_first(sgw_ue);
    while (sess)
    {
        bearer = list_first(&sess->bearer_list);
        while (bearer)
        {
            next_bearer = list_next(bearer);

            bearer->enb_s1u_teid = 0;
            bearer->enb_s1u_addr = 0;

            bearer = next_bearer;
        }

        sess = sgw_sess_next(sess);
    }

    rsp = &gtp_message.release_access_bearers_response;
    memset(&gtp_message, 0, sizeof(gtp_message_t));

    memset(&cause, 0, sizeof(cause));
    cause.value = GTP_CAUSE_REQUEST_ACCEPTED;

    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);

    gtp_message.h.type = GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE;
    gtp_message.h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, &gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    rv = gtp_xact_update_tx(s11_xact, &gtp_message.h, pkbuf);
    d_assert(rv == CORE_OK, return, "gtp_xact_update_tx error");

    rv = gtp_xact_commit(s11_xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    d_trace(3, "[GTP] Release Access Bearers Reqeust : "
        "MME[%d] --> SGW[%d]\n", sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
}

void sgw_s11_handle_lo_dldata_notification(sgw_bearer_t *bearer)
{
    status_t rv;
    gtp_downlink_data_notification_t *noti = NULL;
    pkbuf_t *pkbuf = NULL;
    gtp_message_t gtp_message;
    sgw_ue_t *sgw_ue = NULL;
    sgw_sess_t *sess = NULL;
    gtp_xact_t *xact = NULL;
    /* FIXME : ARP should be retrieved from ? */
    c_uint8_t arp = 0x61;

    d_assert(bearer, return, "Null param");

    sess = bearer->sess;
    d_assert(sess, return, "Null param");
    sgw_ue = sess->sgw_ue;
    d_assert(sgw_ue, return, "Null param");

    /* Build downlink notification message */
    noti = &gtp_message.downlink_data_notification;
    memset(&gtp_message, 0, sizeof(gtp_message_t));

    noti->eps_bearer_id.presence = 1;
    noti->eps_bearer_id.u8 = bearer->ebi;

    /* FIXME : ARP should be retrieved from ? */
    noti->allocation_retention_priority.presence = 1;
    noti->allocation_retention_priority.data = &arp;
    noti->allocation_retention_priority.len = sizeof(arp);

    gtp_message.h.type = GTP_DOWNLINK_DATA_NOTIFICATION_TYPE;
    gtp_message.h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, &gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    xact = gtp_xact_local_create(sess->mme, &gtp_message.h, pkbuf);
    d_assert(xact, return, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    d_trace(3, "[GTP] Downlink Data Notification : "
        "SGW[%d] --> MME[%d]\n", sgw_ue->sgw_s11_teid, sgw_ue->mme_s11_teid);
}

void sgw_s11_handle_downlink_data_notification_ack(sgw_ue_t *sgw_ue,
        gtp_downlink_data_notification_acknowledge_t *ack)
{
    d_trace(3, "[GTP] Downlink Data Notification Ack: "
        "MME[%d] --> SGW[%d]\n", sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
}

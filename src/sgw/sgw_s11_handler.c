#include "gtp/gtp-types.h"
#include "gtp/gtp-conv.h"
#include "gtp/gtp-node.h"
#include "gtp/gtp-path.h"

#include "app/context.h"
#include "sgw_event.h"
#include "sgw_context.h"
#include "sgw_gtp_path.h"
#include "sgw_s11_handler.h"

void sgw_s11_handle_create_session_request(
        gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue, gtp_message_t *gtp_message)
{
    int rv;
    uint16_t decoded;
    gtp_create_session_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_f_teid_t *mme_s11_teid = NULL;
    gtp_f_teid_t *pgw_s5c_teid = NULL;
    int len = 0;
    gtp_node_t *pgw = NULL;
    gtp_f_teid_t sgw_s5c_teid, sgw_s5u_teid;
    gtp_uli_t uli;

    gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s5u_tunnel = NULL;

    ogs_assert(s11_xact);
    ogs_assert(gtp_message);

    req = &gtp_message->create_session_request;

    ogs_debug("[SGW] Create Session Request");
    if (req->bearer_contexts_to_be_created.presence == 0)
    {
        ogs_error("No Bearer");
        return;
    }
    if (req->bearer_contexts_to_be_created.eps_bearer_id.presence == 0)
    {
        ogs_error("No EPS Bearer ID");
        return;
    }

    ogs_assert(sgw_ue);
    sess = sgw_sess_find_by_ebi(sgw_ue,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);
    if (!sess)
    {
        char apn[MAX_APN_LEN];

        if (req->access_point_name.presence == 0)
        {
            ogs_error("No APN");
            return;
        }

        apn_parse(apn, req->access_point_name.data, req->access_point_name.len);
        sess = sgw_sess_add(sgw_ue, apn,
                req->bearer_contexts_to_be_created.eps_bearer_id.u8);
        ogs_assert(sess);
    }

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        ogs_error("No Sender F-TEID");
        return;
    }
    if (req->pgw_s5_s8_address_for_control_plane_or_pmip.presence == 0)
    {
        ogs_error("No PGW IP");
        return;
    }
    if (req->user_location_information.presence == 0)
    {
        ogs_error("No User Location Inforamtion");
        return;
    }

    ogs_assert(sess);
    bearer = sgw_default_bearer_in_sess(sess);
    ogs_assert(bearer);
    s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
    ogs_assert(s5u_tunnel);

    /* Receive Control Plane(DL) : MME-S11 */
    mme_s11_teid = req->sender_f_teid_for_control_plane.data;
    ogs_assert(mme_s11_teid);
    sgw_ue->mme_s11_teid = ntohl(mme_s11_teid->teid);

    /* Send Control Plane(DL) : SGW-S5C */
    memset(&sgw_s5c_teid, 0, sizeof(gtp_f_teid_t));
    sgw_s5c_teid.interface_type = GTP_F_TEID_S5_S8_SGW_GTP_C;
    sgw_s5c_teid.teid = htonl(sess->sgw_s5c_teid);
    rv = gtp_sockaddr_to_f_teid(
        sgw_self()->gtpc_addr, sgw_self()->gtpc_addr6, &sgw_s5c_teid, &len);
    ogs_assert(rv == OGS_OK);
    req->sender_f_teid_for_control_plane.presence = 1;
    req->sender_f_teid_for_control_plane.data = &sgw_s5c_teid;
    req->sender_f_teid_for_control_plane.len = len;

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    ogs_debug("    SGW_S5U_TEID[%d] PGW_S5U_TEID[%d]",
        s5u_tunnel->local_teid, s5u_tunnel->remote_teid);

    pgw_s5c_teid = req->pgw_s5_s8_address_for_control_plane_or_pmip.data;
    ogs_assert(pgw_s5c_teid);

    pgw = gtp_node_find(&sgw_self()->pgw_s5c_list, pgw_s5c_teid);
    if (!pgw)
    {
        pgw = gtp_node_add(&sgw_self()->pgw_s5c_list, pgw_s5c_teid,
            sgw_self()->gtpc_port,
            context_self()->config.parameter.no_ipv4,
            context_self()->config.parameter.no_ipv6,
            context_self()->config.parameter.prefer_ipv4);
        ogs_assert(pgw);

        rv = gtp_connect(sgw_self()->gtpc_sock, sgw_self()->gtpc_sock6, pgw);
        ogs_assert(rv == OGS_OK);
    }
    /* Setup GTP Node */
    SETUP_GTP_NODE(sess, pgw);

    /* Remove PGW-S5C */
    req->pgw_s5_s8_address_for_control_plane_or_pmip.presence = 0;

    /* Data Plane(DL) : SGW-S5U */
    memset(&sgw_s5u_teid, 0, sizeof(gtp_f_teid_t));
    sgw_s5u_teid.teid = htonl(s5u_tunnel->local_teid);
    sgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_SGW_GTP_U;
    rv = gtp_sockaddr_to_f_teid(
        sgw_self()->gtpu_addr,  sgw_self()->gtpu_addr6, &sgw_s5u_teid, &len);
    ogs_assert(rv == OGS_OK);
    req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.presence = 1;
    req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.data = &sgw_s5u_teid;
    req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.len = len;

    /* Set User Location Information */
    decoded = gtp_parse_uli(&uli, &req->user_location_information);
    ogs_assert(req->user_location_information.len == decoded);
    memcpy(&bearer->tai.plmn_id, &uli.tai.plmn_id, sizeof(uli.tai.plmn_id));
    bearer->tai.tac = uli.tai.tac;
    memcpy(&bearer->e_cgi.plmn_id, &uli.e_cgi.plmn_id, sizeof(uli.e_cgi.plmn_id));
    bearer->e_cgi.cell_id = uli.e_cgi.cell_id;

    gtp_message->h.type = GTP_CREATE_SESSION_REQUEST_TYPE;
    gtp_message->h.teid = sess->pgw_s5c_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    ogs_assert(rv == OGS_OK);

    s5c_xact = gtp_xact_local_create(sess->gnode, &gtp_message->h, pkbuf);
    ogs_assert(s5c_xact);

    gtp_xact_associate(s11_xact, s5c_xact);

    rv = gtp_xact_commit(s5c_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s11_handle_modify_bearer_request(gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, gtp_modify_bearer_request_t *req)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];

    uint16_t decoded;
    gtp_node_t *enb = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL;
    gtp_modify_bearer_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_message_t gtp_message;
    
    gtp_cause_t cause;
    gtp_f_teid_t *enb_s1u_teid = NULL;
    gtp_uli_t uli;

    ogs_assert(s11_xact);
    ogs_assert(sgw_ue);
    ogs_assert(req);

    ogs_debug("[SGW] Modify Bearer Reqeust");
    if (req->bearer_contexts_to_be_modified.presence == 0)
    {
        ogs_error("No Bearer");
        return;
    }
    if (req->bearer_contexts_to_be_modified.eps_bearer_id.presence == 0)
    {
        ogs_error("No EPS Bearer ID");
        return;
    }
    if (req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.presence == 0)
    {
        ogs_error("No eNB TEID");
        return;
    }

    bearer = sgw_bearer_find_by_ue_ebi(sgw_ue, 
                req->bearer_contexts_to_be_modified.eps_bearer_id.u8);
    ogs_assert(bearer);
    s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
    ogs_assert(s1u_tunnel);

    /* Data Plane(DL) : eNB-S1U */
    enb_s1u_teid = req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.data;
    s1u_tunnel->remote_teid = ntohl(enb_s1u_teid->teid);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]",
        s1u_tunnel->remote_teid, s1u_tunnel->local_teid);

    enb = gtp_node_find(&sgw_self()->enb_s1u_list, enb_s1u_teid);
    if (!enb)
    {
        enb = gtp_node_add(&sgw_self()->enb_s1u_list, enb_s1u_teid,
            sgw_self()->gtpu_port,
            context_self()->config.parameter.no_ipv4,
            context_self()->config.parameter.no_ipv6,
            context_self()->config.parameter.prefer_ipv4);
        ogs_assert(enb);

        rv = gtp_connect(sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, enb);
        ogs_assert(rv == OGS_OK);
    }

#if ULI_END_MARKER
    /* if ULI's Cell ID changes, End Marker is sent out or not */
    if (req->user_location_information.presence == 1)
    {
        /* Set User Location Information */
        decoded = gtp_parse_uli(&uli, &req->user_location_information);
        ogs_assert(req->user_location_information.len == decoded);
        memcpy(&bearer->tai.plmn_id, &uli.tai.plmn_id, sizeof(uli.tai.plmn_id));
        bearer->tai.tac = uli.tai.tac;
        memcpy(&bearer->e_cgi.plmn_id, &uli.e_cgi.plmn_id,
                sizeof(uli.e_cgi.plmn_id));
        ogs_debug("    ULI Presence: CellID[OLD:0x%x, NEW:0x%x]",
            bearer->e_cgi.cell_id, uli.e_cgi.cell_id);
        if (bearer->e_cgi.cell_id != uli.e_cgi.cell_id)
        {
            ogs_debug("[SGW] SEND End Marker to ENB[%s]: TEID[0x%x]",
                OGS_ADDR(&s1u_tunnel->gnode->conn, buf),
                s1u_tunnel->remote_teid);
            rv = sgw_gtp_send_end_marker(s1u_tunnel);
            if (rv != OGS_OK)
                ogs_error("gtp send end marker failed");

            bearer->e_cgi.cell_id = uli.e_cgi.cell_id;
        }
    }
#else /* GNODE_END_MARKER */
    /* if GTP Node changes, End Marker is sent out or not */
    if (req->user_location_information.presence == 1)
    {
        /* Set User Location Information */
        decoded = gtp_parse_uli(&uli, &req->user_location_information);
        ogs_assert(req->user_location_information.len == decoded);
        memcpy(&bearer->tai.plmn_id, &uli.tai.plmn_id, sizeof(uli.tai.plmn_id));
        bearer->tai.tac = uli.tai.tac;
        memcpy(&bearer->e_cgi.plmn_id, &uli.e_cgi.plmn_id,
                sizeof(uli.e_cgi.plmn_id));
        bearer->e_cgi.cell_id = uli.e_cgi.cell_id;
        ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
                plmn_id_hexdump(&bearer->tai.plmn_id),
                bearer->tai.tac);
        ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
                plmn_id_hexdump(&bearer->e_cgi.plmn_id),
                bearer->e_cgi.cell_id);
    }

    if (s1u_tunnel->gnode && s1u_tunnel->gnode != enb)
    {
        ogs_assert(s1u_tunnel->gnode->sock);

        ogs_debug("[SGW] SEND End Marker to ENB[%s]: TEID[0x%x]",
            OGS_ADDR(&s1u_tunnel->gnode->conn, buf),
            s1u_tunnel->remote_teid);
        rv = sgw_gtp_send_end_marker(s1u_tunnel);
        if (rv != OGS_OK)
            ogs_error("gtp send end marker failed");
    }
#endif

    /* Setup GTP Node */
    SETUP_GTP_NODE(s1u_tunnel, enb);

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
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(s11_xact, &gtp_message.h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_commit(s11_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s11_handle_delete_session_request(gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, gtp_message_t *gtp_message)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    gtp_delete_session_request_t *req = NULL;

    ogs_assert(s11_xact);
    ogs_assert(sgw_ue);
    ogs_assert(gtp_message);

    req = &gtp_message->delete_session_request;
    ogs_assert(req);

    ogs_debug("[SGW] Delete Session Reqeust");
    if (req->linked_eps_bearer_id.presence == 0)
    {
        ogs_error("No EPS Bearer ID");
        return;
    }

    sess = sgw_sess_find_by_ebi(sgw_ue, req->linked_eps_bearer_id.u8);
    ogs_assert(sess);
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    gtp_message->h.type = GTP_DELETE_SESSION_REQUEST_TYPE;
    gtp_message->h.teid = sess->pgw_s5c_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    ogs_assert(rv == OGS_OK);

    s5c_xact = gtp_xact_local_create(sess->gnode, &gtp_message->h, pkbuf);
    ogs_assert(s5c_xact);

    gtp_xact_associate(s11_xact, s5c_xact);

    rv = gtp_xact_commit(s5c_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s11_handle_create_bearer_response(gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, gtp_message_t *gtp_message)
{
    int rv;
    uint16_t decoded;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_node_t *enb = NULL;
    gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL, *s5u_tunnel = NULL;
    gtp_create_bearer_response_t *req = NULL;

    gtp_f_teid_t *sgw_s1u_teid = NULL, *enb_s1u_teid = NULL;
    gtp_f_teid_t sgw_s5u_teid, pgw_s5u_teid;
    int len;
    gtp_uli_t uli;

    ogs_assert(s11_xact);
    ogs_assert(sgw_ue);
    ogs_assert(gtp_message);
    s5c_xact = s11_xact->assoc_xact;
    ogs_assert(s5c_xact);

    ogs_debug("[SGW] Cerate Bearer Reqeust");
    req = &gtp_message->create_bearer_response;
    ogs_assert(req);

    if (req->bearer_contexts.presence == 0)
    {
        ogs_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0)
    {
        ogs_error("No EPS Bearer ID");
        return;
    }
    if (req->bearer_contexts.s1_u_enodeb_f_teid.presence == 0)
    {
        ogs_error("No eNB TEID");
        return;
    }
    if (req->bearer_contexts.s4_u_sgsn_f_teid.presence == 0)
    {
        ogs_error("No SGW TEID");
        return;
    }
    if (req->user_location_information.presence == 0)
    {
        ogs_error("No User Location Inforamtion");
        return;
    }

    /* Correlate with SGW-S1U-TEID */
    sgw_s1u_teid = req->bearer_contexts.s4_u_sgsn_f_teid.data;
    ogs_assert(sgw_s1u_teid);
    req->bearer_contexts.s4_u_sgsn_f_teid.presence = 0;

    /* Find the Tunnel by SGW-S1U-TEID */
    s1u_tunnel = sgw_tunnel_find_by_teid(ntohl(sgw_s1u_teid->teid));
    ogs_assert(s1u_tunnel);
    bearer = s1u_tunnel->bearer;
    ogs_assert(bearer);
    s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
    ogs_assert(s5u_tunnel);
    sess = bearer->sess;
    ogs_assert(sess);

    /* Set EBI */
    bearer->ebi = req->bearer_contexts.eps_bearer_id.u8;

    /* Data Plane(DL) : eNB-S1U */
    enb_s1u_teid = req->bearer_contexts.s1_u_enodeb_f_teid.data;
    s1u_tunnel->remote_teid = ntohl(enb_s1u_teid->teid);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]",
        s1u_tunnel->remote_teid, s1u_tunnel->local_teid);
    ogs_debug("    SGW_S5U_TEID[%d] PGW_S5U_TEID[%d]",
        s5u_tunnel->local_teid, s5u_tunnel->remote_teid);

    enb = gtp_node_find(&sgw_self()->enb_s1u_list, enb_s1u_teid);
    if (!enb)
    {
        enb = gtp_node_add(&sgw_self()->enb_s1u_list, enb_s1u_teid,
            sgw_self()->gtpu_port,
            context_self()->config.parameter.no_ipv4,
            context_self()->config.parameter.no_ipv6,
            context_self()->config.parameter.prefer_ipv4);
        ogs_assert(enb);

        rv = gtp_connect(sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, enb);
        ogs_assert(rv == OGS_OK);
    }
    /* Setup GTP Node */
    SETUP_GTP_NODE(s1u_tunnel, enb);

    /* Remove S1U-F-TEID */
    req->bearer_contexts.s1_u_enodeb_f_teid.presence = 0;

    decoded = gtp_parse_uli(&uli, &req->user_location_information);
    ogs_assert(req->user_location_information.len == decoded);
    memcpy(&bearer->tai.plmn_id, &uli.tai.plmn_id, sizeof(uli.tai.plmn_id));
    bearer->tai.tac = uli.tai.tac;
    memcpy(&bearer->e_cgi.plmn_id, &uli.e_cgi.plmn_id, sizeof(uli.e_cgi.plmn_id));
    bearer->e_cgi.cell_id = uli.e_cgi.cell_id;

    /* Reset UE state */
    SGW_RESET_UE_STATE(sgw_ue, SGW_S1U_INACTIVE);

    /* Data Plane(DL) : SGW-S5U */
    memset(&sgw_s5u_teid, 0, sizeof(gtp_f_teid_t));
    sgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_SGW_GTP_U;
    sgw_s5u_teid.teid = htonl(s5u_tunnel->local_teid);
    rv = gtp_sockaddr_to_f_teid(
        sgw_self()->gtpu_addr,  sgw_self()->gtpu_addr6, &sgw_s5u_teid, &len);
    ogs_assert(rv == OGS_OK);
    req->bearer_contexts.s5_s8_u_sgw_f_teid.presence = 1;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.data = &sgw_s5u_teid;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.len = len;

    /* Data Plane(DL) : PGW-S5U */
    ogs_assert(s5u_tunnel->gnode);
    pgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_U;
    pgw_s5u_teid.teid = htonl(s5u_tunnel->remote_teid);
    rv = gtp_ip_to_f_teid(&s5u_tunnel->gnode->ip, &pgw_s5u_teid, &len);
    req->bearer_contexts.s5_s8_u_pgw_f_teid.presence = 1;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.data = &pgw_s5u_teid;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.len = len;

    gtp_message->h.type = GTP_CREATE_BEARER_RESPONSE_TYPE;
    gtp_message->h.teid = sess->pgw_s5c_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(s5c_xact, &gtp_message->h, pkbuf);
    ogs_assert(s5c_xact);

    rv = gtp_xact_commit(s5c_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s11_handle_update_bearer_response(gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, gtp_message_t *gtp_message)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    gtp_update_bearer_response_t *req = NULL;

    ogs_assert(s11_xact);
    ogs_assert(sgw_ue);
    ogs_assert(gtp_message);
    s5c_xact = s11_xact->assoc_xact;
    ogs_assert(s5c_xact);

    req = &gtp_message->update_bearer_response;
    ogs_assert(req);

    ogs_debug("[SGW] Update Bearer Reqeust");
    if (req->bearer_contexts.presence == 0)
    {
        ogs_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0)
    {
        ogs_error("No EPS Bearer ID");
        return;
    }

    bearer = sgw_bearer_find_by_ue_ebi(
            sgw_ue, req->bearer_contexts.eps_bearer_id.u8);
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    gtp_message->h.type = GTP_UPDATE_BEARER_RESPONSE_TYPE;
    gtp_message->h.teid = sess->pgw_s5c_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(s5c_xact, &gtp_message->h, pkbuf);
    ogs_assert(s5c_xact);

    rv = gtp_xact_commit(s5c_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s11_handle_delete_bearer_response(gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, gtp_message_t *gtp_message)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    gtp_delete_bearer_response_t *req = NULL;

    ogs_assert(s11_xact);
    ogs_assert(sgw_ue);
    ogs_assert(gtp_message);
    s5c_xact = s11_xact->assoc_xact;
    ogs_assert(s5c_xact);

    req = &gtp_message->delete_bearer_response;
    ogs_assert(req);

    ogs_debug("[SGW] Delete Bearer Response");
    if (req->bearer_contexts.presence == 0)
    {
        ogs_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0)
    {
        ogs_error("No EPS Bearer ID");
        return;
    }

    bearer = sgw_bearer_find_by_ue_ebi(
            sgw_ue, req->bearer_contexts.eps_bearer_id.u8);
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    gtp_message->h.type = GTP_DELETE_BEARER_RESPONSE_TYPE;
    gtp_message->h.teid = sess->pgw_s5c_teid;

    rv = gtp_build_msg(&pkbuf, gtp_message);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(s5c_xact, &gtp_message->h, pkbuf);
    ogs_assert(s5c_xact);

    rv = gtp_xact_commit(s5c_xact);
    ogs_assert(rv == OGS_OK);

    sgw_bearer_remove(bearer);
}

void sgw_s11_handle_release_access_bearers_request(gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, gtp_release_access_bearers_request_t *req)
{
    int rv;
    gtp_release_access_bearers_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_message_t gtp_message;
    sgw_bearer_t *bearer = NULL, *next_bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL;
    sgw_sess_t *sess = NULL;
    
    gtp_cause_t cause;

    ogs_assert(sgw_ue);
    ogs_assert(s11_xact);
    ogs_assert(req);

    ogs_debug("[SGW] Release Access Bearers Request");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    /* Set UE state to S1UE_INACTIVE */
    SGW_SET_UE_STATE(sgw_ue, SGW_S1U_INACTIVE);
    /* ReSet UE state to S1UE_INACTIVE */
    SGW_RESET_UE_STATE(sgw_ue, SGW_DL_NOTI_SENT);

    /* Release S1U(DL) path */
    sess = sgw_sess_first(sgw_ue);
    while (sess)
    {
        bearer = ogs_list_first(&sess->bearer_list);
        while (bearer)
        {
            next_bearer = ogs_list_next(bearer);

            s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
            ogs_assert(s1u_tunnel);

            s1u_tunnel->remote_teid = 0;

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
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(s11_xact, &gtp_message.h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_commit(s11_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s11_handle_lo_dldata_notification(sgw_bearer_t *bearer)
{
    int rv;
    gtp_downlink_data_notification_t *noti = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_message_t gtp_message;
    sgw_ue_t *sgw_ue = NULL;
    gtp_xact_t *xact = NULL;
    /* FIXME : ARP should be retrieved from ? */
    uint8_t arp = 0x61;

    ogs_assert(bearer);

    sgw_ue = bearer->sgw_ue;
    ogs_assert(sgw_ue);

    ogs_debug("[SGW] Downlink Data Notification");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

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
    ogs_assert(rv == OGS_OK);

    xact = gtp_xact_local_create(sgw_ue->gnode, &gtp_message.h, pkbuf);
    ogs_assert(xact);

    rv = gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s11_handle_downlink_data_notification_ack(
        gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue,
        gtp_downlink_data_notification_acknowledge_t *ack)
{
    int rv;
    ogs_assert(sgw_ue);
    ogs_assert(s11_xact);

    ogs_debug("[SGW] Downlink Data Notification Acknowledge");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    rv = gtp_xact_commit(s11_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s11_handle_create_indirect_data_forwarding_tunnel_request(
        gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue,
        gtp_create_indirect_data_forwarding_tunnel_request_t *req)
{
    int rv;
    gtp_create_indirect_data_forwarding_tunnel_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_message_t gtp_message;
    int i;

    gtp_node_t *enb = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL;
    
    gtp_cause_t cause;
    tlv_bearer_context_t *req_bearers[GTP_MAX_NUM_OF_INDIRECT_TUNNEL];
    tlv_bearer_context_t *rsp_bearers[GTP_MAX_NUM_OF_INDIRECT_TUNNEL];
    gtp_f_teid_t *req_teid = NULL;
    gtp_f_teid_t rsp_dl_teid[GTP_MAX_NUM_OF_INDIRECT_TUNNEL];
    gtp_f_teid_t rsp_ul_teid[GTP_MAX_NUM_OF_INDIRECT_TUNNEL];
    int len;

    ogs_assert(sgw_ue);
    ogs_assert(s11_xact);
    ogs_assert(req);

    ogs_debug("[SGW] Create Indirect Data Forwarding Tunnel Request");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    rsp = &gtp_message.create_indirect_data_forwarding_tunnel_response;
    memset(&gtp_message, 0, sizeof(gtp_message_t));

    gtp_bearers_in_create_indirect_tunnel_request(&req_bearers, req);
    gtp_bearers_in_create_indirect_tunnel_response(&rsp_bearers, rsp);

    memset(&cause, 0, sizeof(cause));
    cause.value = GTP_CAUSE_REQUEST_ACCEPTED;

    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);

    for (i = 0; req_bearers[i]->presence; i++)
    {
        if (req_bearers[i]->eps_bearer_id.presence == 0)
        {
            ogs_error("No EBI");
            return;
        }

        bearer = sgw_bearer_find_by_ue_ebi(sgw_ue, 
                    req_bearers[i]->eps_bearer_id.u8);
        ogs_assert(bearer);

        if (req_bearers[i]->s1_u_enodeb_f_teid.presence)
        {
            req_teid = req_bearers[i]->s1_u_enodeb_f_teid.data;
            ogs_assert(req_teid);

            tunnel = sgw_tunnel_add(bearer,
                    GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING);
            ogs_assert(tunnel);

            tunnel->remote_teid = ntohl(req_teid->teid);
            enb = gtp_node_find(&sgw_self()->enb_s1u_list, req_teid);
            if (!enb)
            {
                enb = gtp_node_add(&sgw_self()->enb_s1u_list, req_teid,
                    sgw_self()->gtpu_port,
                    context_self()->config.parameter.no_ipv4,
                    context_self()->config.parameter.no_ipv6,
                    context_self()->config.parameter.prefer_ipv4);
                ogs_assert(enb);

                rv = gtp_connect(
                        sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, enb);
                ogs_assert(rv == OGS_OK);
            }
            /* Setup GTP Node */
            SETUP_GTP_NODE(tunnel, enb);

            memset(&rsp_dl_teid[i], 0, sizeof(gtp_f_teid_t));
            rsp_dl_teid[i].interface_type = tunnel->interface_type;
            rsp_dl_teid[i].teid = htonl(tunnel->local_teid);
            rv = gtp_sockaddr_to_f_teid(sgw_self()->gtpu_addr,
                    sgw_self()->gtpu_addr6, &rsp_dl_teid[i], &len);
            ogs_assert(len > 0);
            rsp_bearers[i]->s4_u_sgsn_f_teid.presence = 1;
            rsp_bearers[i]->s4_u_sgsn_f_teid.data = &rsp_dl_teid[i];
            rsp_bearers[i]->s4_u_sgsn_f_teid.len = len;

            ogs_debug("    SGW_DL_TEID[%d] ENB_DL_TEID[%d]",
                    tunnel->local_teid, tunnel->remote_teid);
        }

        if (req_bearers[i]->s12_rnc_f_teid.presence)
        {
            req_teid = req_bearers[i]->s12_rnc_f_teid.data;
            ogs_assert(req_teid);

            tunnel = sgw_tunnel_add(bearer,
                    GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING);
            ogs_assert(tunnel);

            tunnel->remote_teid = ntohl(req_teid->teid);
            enb = gtp_node_find(&sgw_self()->enb_s1u_list, req_teid);
            if (!enb)
            {
                enb = gtp_node_add(&sgw_self()->enb_s1u_list, req_teid,
                    sgw_self()->gtpu_port,
                    context_self()->config.parameter.no_ipv4,
                    context_self()->config.parameter.no_ipv6,
                    context_self()->config.parameter.prefer_ipv4);
                ogs_assert(enb);

                rv = gtp_connect(
                        sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, enb);
                ogs_assert(rv == OGS_OK);
            }
            /* Setup GTP Node */
            SETUP_GTP_NODE(tunnel, enb);

            memset(&rsp_ul_teid[i], 0, sizeof(gtp_f_teid_t));
            rsp_ul_teid[i].teid = htonl(tunnel->local_teid);
            rsp_ul_teid[i].interface_type = tunnel->interface_type;
            rv = gtp_sockaddr_to_f_teid(sgw_self()->gtpu_addr,
                    sgw_self()->gtpu_addr6, &rsp_ul_teid[i], &len);
            ogs_assert(rv == OGS_OK);
            rsp_bearers[i]->s2b_u_epdg_f_teid_5.presence = 1;
            rsp_bearers[i]->s2b_u_epdg_f_teid_5.data = &rsp_ul_teid[i];
            rsp_bearers[i]->s2b_u_epdg_f_teid_5.len = len;
            ogs_debug("    SGW_UL_TEID[%d] ENB_UL_TEID[%d]",
                    tunnel->local_teid, tunnel->remote_teid);
        }

        if (req_bearers[i]->s1_u_enodeb_f_teid.presence ||
            req_bearers[i]->s12_rnc_f_teid.presence)
        {
            rsp_bearers[i]->presence = 1;
            rsp_bearers[i]->eps_bearer_id.presence = 1;
            rsp_bearers[i]->eps_bearer_id.u8 = bearer->ebi;
        }
    }

    gtp_message.h.type =
        GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE;
    gtp_message.h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, &gtp_message);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(s11_xact, &gtp_message.h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_commit(s11_xact);
    ogs_assert(rv == OGS_OK);
}

void sgw_s11_handle_delete_indirect_data_forwarding_tunnel_request(
        gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue)
{
    int rv;
    gtp_delete_indirect_data_forwarding_tunnel_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_message_t gtp_message;

    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL, *next_tunnel;
    
    gtp_cause_t cause;

    ogs_assert(sgw_ue);
    ogs_assert(s11_xact);

    ogs_debug("[SGW] Delete Indirect Data Forwarding Tunnel Request");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    /* Delete Indirect Tunnel */
    sess = sgw_sess_first(sgw_ue);
    while (sess)
    {
        bearer = sgw_bearer_first(sess);
        while (bearer)
        {
            tunnel = sgw_tunnel_first(bearer);
            while(tunnel)
            {
                next_tunnel = sgw_tunnel_next(tunnel);

                if (tunnel->interface_type ==
                    GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING ||
                    tunnel->interface_type ==
                    GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING)
                        sgw_tunnel_remove(tunnel);

                tunnel = next_tunnel;
            }
            

            bearer = sgw_bearer_next(bearer);
        }

        sess = sgw_sess_next(sess);
    }

    rsp = &gtp_message.delete_indirect_data_forwarding_tunnel_response;
    memset(&gtp_message, 0, sizeof(gtp_message_t));

    memset(&cause, 0, sizeof(cause));
    cause.value = GTP_CAUSE_REQUEST_ACCEPTED;

    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);

    gtp_message.h.type =
        GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE;
    gtp_message.h.teid = sgw_ue->mme_s11_teid;

    rv = gtp_build_msg(&pkbuf, &gtp_message);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(s11_xact, &gtp_message.h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_commit(s11_xact);
    ogs_assert(rv == OGS_OK);
}

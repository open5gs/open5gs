#define TRACE_MODULE _pgw_s5c_handler

#include "core_debug.h"
#include "core_lib.h"

#include "gtp_types.h"

#include "pgw_event.h"
#include "pgw_context.h"
#include "pgw_gtp_path.h"
#include "pgw_s5c_handler.h"

void pgw_s5c_handle_create_session_request(
        gtp_xact_t *xact, pgw_sess_t *sess, gtp_create_session_request_t *req)
{
    gtp_f_teid_t *sgw_s5c_teid, *sgw_s5u_teid;
    pgw_bearer_t *bearer = NULL;
    gtp_bearer_qos_t bearer_qos;
    gtp_ambr_t *ambr = NULL;
    gtp_uli_t uli;

    d_assert(xact, return, "Null param");
    d_assert(sess, return, "Null param");
    d_assert(req, return, "Null param");
    bearer = pgw_default_bearer_in_sess(sess);
    d_assert(bearer, return, "Null param");

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No IMSI");
        return;
    }
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
    if (req->bearer_contexts_to_be_created.bearer_level_qos.presence == 0)
    {
        d_error("No EPS Bearer QoS");
        return;
    }
    if (req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.presence == 0)
    {
        d_error("No TEID");
        return;
    }
    if (req->user_location_information.presence == 0)
    {
        d_error("No User Location Inforamtion");
        return;
    }
    
    /* Set IMSI */
    sess->imsi_len = req->imsi.len;
    memcpy(sess->imsi, req->imsi.data, sess->imsi_len);
    core_buffer_to_bcd(sess->imsi, sess->imsi_len, sess->imsi_bcd);

    /* Control Plane(DL) : SGW-S5C */
    sgw_s5c_teid = req->sender_f_teid_for_control_plane.data;
    d_assert(sgw_s5c_teid, return, "Null param");
    sess->sgw_s5c_teid = ntohl(sgw_s5c_teid->teid);
    sess->sgw_s5c_addr = sgw_s5c_teid->ipv4_addr;

    /* Control Plane(DL) : SGW-S5U */
    sgw_s5u_teid = req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.data;
    d_assert(sgw_s5u_teid, return, "Null param");
    bearer->sgw_s5u_teid = ntohl(sgw_s5u_teid->teid);
    bearer->sgw_s5u_addr = sgw_s5u_teid->ipv4_addr;

    d_assert(gtp_parse_bearer_qos(&bearer_qos,
        &req->bearer_contexts_to_be_created.bearer_level_qos) ==
        req->bearer_contexts_to_be_created.bearer_level_qos.len, return,);
    sess->pdn.qos.qci = bearer_qos.qci;
    sess->pdn.qos.arp.priority_level = bearer_qos.priority_level;
    sess->pdn.qos.arp.pre_emption_capability =
                    bearer_qos.pre_emption_capability;
    sess->pdn.qos.arp.pre_emption_vulnerability =
                    bearer_qos.pre_emption_vulnerability;

    /* Set AMBR if available */
    if (req->aggregate_maximum_bit_rate.presence)
    {
        ambr = req->aggregate_maximum_bit_rate.data;
        sess->pdn.ambr.downlink = ntohl(ambr->downlink);
        sess->pdn.ambr.uplink = ntohl(ambr->uplink);
    }
    
    /* Set User Location Information */
    d_assert(gtp_parse_uli(&uli, &req->user_location_information) ==
        req->user_location_information.len, return,);
    memcpy(&sess->tai.plmn_id, &uli.tai.plmn_id, sizeof(uli.tai.plmn_id));
    sess->tai.tac = uli.tai.tac;
    memcpy(&sess->e_cgi.plmn_id, &uli.e_cgi.plmn_id, sizeof(uli.e_cgi.plmn_id));
    sess->e_cgi.cell_id = uli.e_cgi.cell_id;

    d_trace(3, "[GTP] Create Session Reqeust : "
            "SGW[%d] --> PGW[%d]\n", sess->sgw_s5c_teid, sess->pgw_s5c_teid);
}

void pgw_s5c_handle_delete_session_request(
        gtp_xact_t *xact, pgw_sess_t *sess, gtp_delete_session_request_t *req)
{
}

void pgw_s5c_handle_create_bearer_response(
        gtp_xact_t *xact, pgw_sess_t *sess, gtp_create_bearer_response_t *req)
{
    status_t rv;
    gtp_f_teid_t *sgw_s5u_teid, *pgw_s5u_teid;
    pgw_bearer_t *bearer = NULL;

    d_assert(xact, return, "Null param");
    d_assert(sess, return, "Null param");
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
    if (req->bearer_contexts.s5_s8_u_pgw_f_teid.presence == 0)
    {
        d_error("No PGW TEID");
        return;
    }
    if (req->bearer_contexts.s5_s8_u_sgw_f_teid.presence == 0)
    {
        d_error("No SGW TEID");
        return;
    }

    /* Correlate with PGW-S%U-TEID */
    pgw_s5u_teid = req->bearer_contexts.s5_s8_u_pgw_f_teid.data;
    d_assert(pgw_s5u_teid, return, "Null param");

    /* Find the Bearer by PGW-S5U-TEID */
    bearer = pgw_bearer_find_by_pgw_s5u_teid(ntohl(pgw_s5u_teid->teid));
    d_assert(bearer, return, "Null param");

    /* Set EBI */
    bearer->ebi = req->bearer_contexts.eps_bearer_id.u8;

    /* Data Plane(DL) : SGW-S5U */
    sgw_s5u_teid = req->bearer_contexts.s5_s8_u_sgw_f_teid.data;
    bearer->sgw_s5u_teid = ntohl(sgw_s5u_teid->teid);
    bearer->sgw_s5u_addr = sgw_s5u_teid->ipv4_addr;

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
    
    d_trace(3, "[GTP] Create Bearer Response : "
            "SGW[%d] --> PGW[%d]\n", sess->sgw_s5c_teid, sess->pgw_s5c_teid);
}

#define TRACE_MODULE _mme_s11_handler

#include "core_debug.h"

#include "gtp_types.h"

#include "mme_event.h"
#include "mme_context.h"

#include "s1ap_build.h"
#include "s1ap_path.h"
#include "mme_s11_build.h"
#include "mme_s11_handler.h"
#include "mme_gtp_path.h"
#include "esm_build.h"
#include "nas_path.h"

void mme_s11_handle_create_session_request(mme_sess_t *sess)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;
    gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    /* Use round-robin for selecting SGW */
    CONNECT_SGW_GTP_NODE(sess);

    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return, "Null param");

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_CREATE_SESSION_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_create_session_request(&pkbuf, h.type, sess);
    d_assert(rv == CORE_OK, return,
            "S11 build error");

    xact = gtp_xact_local_create(sess->sgw, &h, pkbuf);
    d_assert(xact, return, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void mme_s11_handle_create_session_response(gtp_xact_t *xact,
        mme_bearer_t *bearer, gtp_create_session_response_t *rsp)
{
    status_t rv;
    gtp_f_teid_t *sgw_s11_teid = NULL;
    gtp_f_teid_t *sgw_s1u_teid = NULL;

    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    pdn_t *pdn = NULL;
    
    d_assert(xact, return, "Null param");
    d_assert(bearer, return, "Null param");
    d_assert(rsp, return, "Null param");

    if (rsp->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No S11 TEID");
        return;
    }
    if (rsp->pdn_address_allocation.presence == 0)
    {
        d_error("No PDN Address Allocation");
        return;
    }
    if (rsp->bearer_contexts_created.s1_u_enodeb_f_teid.presence == 0)
    {
        d_error("No S1U TEID");
        return;
    }

    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return, "Null param");
    sess = bearer->sess;
    d_assert(sess, return, "Null param");
    pdn = sess->pdn;
    d_assert(pdn, return, "Null param");

    /* Control Plane(UL) : SGW-S11 */
    sgw_s11_teid = rsp->sender_f_teid_for_control_plane.data;
    mme_ue->sgw_s11_teid = ntohl(sgw_s11_teid->teid);
    mme_ue->sgw_s11_addr = sgw_s11_teid->ipv4_addr;

    memcpy(&pdn->paa, rsp->pdn_address_allocation.data,
            rsp->pdn_address_allocation.len);

    /* PCO */
    if (rsp->protocol_configuration_options.presence)
    {
        TLV_STORE_DATA(&sess->pgw_pco, &rsp->protocol_configuration_options);
    }

    /* Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = rsp->bearer_contexts_created.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);
    bearer->sgw_s1u_addr = sgw_s1u_teid->ipv4_addr;

    d_trace(3, "[GTP] Create Session Response : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void mme_s11_handle_modify_bearer_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_modify_bearer_response_t *rsp)
{
    status_t rv;

    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(rsp, return, "Null param");

    d_trace(3, "[GTP] Modify Bearer Response : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void mme_s11_handle_create_bearer_request(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_create_bearer_request_t *req)
{
    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;

    gtp_f_teid_t *sgw_s1u_teid = NULL;
    gtp_bearer_qos_t bearer_qos;

    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(req, return, "Null param");

    d_trace(3, "[GTP] Create Bearer Request : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

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
    if (req->bearer_contexts.s1_u_enodeb_f_teid.presence == 0)
    {
        d_error("No GTP TEID");
        return;
    }
    if (req->bearer_contexts.bearer_level_qos.presence == 0)
    {
        d_error("No QoS");
        return;
    }
    if (req->bearer_contexts.tft.presence == 0)
    {
        d_error("No TFT");
        return;
    }

    sess = mme_sess_find_by_ebi(mme_ue, req->linked_eps_bearer_id.u8);
    d_assert(sess, return, 
            "No Session Context(EBI:%d)", req->linked_eps_bearer_id);

    bearer = mme_bearer_add(sess);
    d_assert(bearer, return, "No Bearer Context");

    /* Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = req->bearer_contexts.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);
    bearer->sgw_s1u_addr = sgw_s1u_teid->ipv4_addr;

    /* Bearer QoS */
    d_assert(gtp_parse_bearer_qos(&bearer_qos,
        &req->bearer_contexts.bearer_level_qos) ==
        req->bearer_contexts.bearer_level_qos.len, return,);
    bearer->qos.qci = bearer_qos.qci;
    bearer->qos.arp.priority_level = bearer_qos.priority_level;
    bearer->qos.arp.pre_emption_capability =
                    bearer_qos.pre_emption_capability;
    bearer->qos.arp.pre_emption_vulnerability =
                    bearer_qos.pre_emption_vulnerability;
    bearer->qos.mbr.downlink = bearer_qos.dl_mbr;
    bearer->qos.mbr.uplink = bearer_qos.ul_mbr;
    bearer->qos.gbr.downlink = bearer_qos.dl_gbr;
    bearer->qos.gbr.uplink = bearer_qos.ul_gbr;

    /* Save Bearer TFT */
    TLV_STORE_DATA(&bearer->tft, &req->bearer_contexts.tft);

    /* Save Transaction. will be handled after EMM-attached */
    bearer->xact = xact;
}

void mme_s11_handle_release_access_bearers_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue,
        gtp_release_access_bearers_response_t *rsp)
{
    status_t rv;

    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(rsp, return, "Null param");

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    d_trace(3, "[GTP] Release Access Bearers Response : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void mme_s11_handle_downlink_data_notification(
        gtp_xact_t *xact, mme_ue_t *mme_ue,
        gtp_downlink_data_notification_t *noti)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *s11buf = NULL;

    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(noti, return, "Null param");

    d_trace(3, "[GTP] Downlink Data Notification : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    /* Build Downlink data notification ack */
    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_downlink_data_notification_ack(&s11buf, h.type, mme_ue);
    d_assert(rv == CORE_OK, return, "S11 build error");

    rv = gtp_xact_update_tx(xact, &h, s11buf);
    d_assert(rv == CORE_OK, return, "xact_update_tx error");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

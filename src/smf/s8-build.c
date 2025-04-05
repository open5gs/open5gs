/* smf/s8-build.c */
#include "context.h"
#include "s8-build.h"
#include "s8-path.h"
#include "ogs-gtp.h"
#include "ogs-pkbuf.h"

ogs_pkbuf_t *smf_s8_build_create_session_response(
    smf_sess_t *sess, ogs_gtp_xact_t *xact,
    ogs_gtp2_create_session_request_t *req)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_create_session_response_t *rsp = NULL;
    ogs_gtp2_bearer_context_t *bearer = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    pkbuf = ogs_gtp2_build_create_session_response(
        OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE,
        req->bearer_contexts_to_be_created.eps_bearer_id.u8,
        req->sender_f_teid_for_control_plane.teid,
        req->sender_f_teid_for_control_plane.ipv4 ?
            &req->sender_f_teid_for_control_plane.ipv4->addr : NULL,
        req->sender_f_teid_for_control_plane.ipv6 ?
            &req->sender_f_teid_for_control_plane.ipv6->addr : NULL);
    ogs_assert(pkbuf);

    rsp = &pkbuf->gtp2_message->create_session_response;
    ogs_assert(rsp);

    rsp->cause.presence = 1;
    rsp->cause.data = &rsp->cause_value;
    rsp->cause.len = 1;
    rsp->cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    rsp->pgw_s5s8_f_teid.presence = 1;
    rsp->pgw_s5s8_f_teid.data = &rsp->pgw_s5s8_f_teid_data;
    rsp->pgw_s5s8_f_teid.len = sizeof(ogs_gtp2_f_teid_t);
    rsp->pgw_s5s8_f_teid_data.interface_type = OGS_GTP2_F_TEID_S5_S8_SGW_GTP_C;
    rsp->pgw_s5s8_f_teid_data.teid = sess->pgw_s5c_teid;
    rsp->pgw_s5s8_f_teid_data.ipv4 = 1;
    rsp->pgw_s5s8_f_teid_data.addr = sess->pgw_s5c_addr;

    rsp->paa.presence = 1;
    rsp->paa.data = &sess->paa;
    rsp->paa.len = sizeof(ogs_paa_t);

    bearer = &rsp->bearer_contexts_created.bearer_context[0];
    bearer->eps_bearer_id.presence = 1;
    bearer->eps_bearer_id.data = &bearer->eps_bearer_id_value;
    bearer->eps_bearer_id.len = 1;
    bearer->eps_bearer_id_value = req->bearer_contexts_to_be_created.eps_bearer_id.u8;

    bearer->s1u_sgw_f_teid.presence = 1;
    bearer->s1u_sgw_f_teid.data = &bearer->s1u_sgw_f_teid_data;
    bearer->s1u_sgw_f_teid.len = sizeof(ogs_gtp2_f_teid_t);
    bearer->s1u_sgw_f_teid_data.interface_type = OGS_GTP2_F_TEID_S1U_SGW_GTP_U;
    bearer->s1u_sgw_f_teid_data.teid = sess->sgw_s1u_teid;
    bearer->s1u_sgw_f_teid_data.ipv4 = 1;
    bearer->s1u_sgw_f_teid_data.addr = sess->sgw_s1u_addr;

    return pkbuf;
}

ogs_pkbuf_t *smf_s8_build_delete_session_response(
    smf_sess_t *sess, ogs_gtp_xact_t *xact,
    ogs_gtp2_delete_session_request_t *req)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_delete_session_response_t *rsp = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    pkbuf = ogs_gtp2_build_delete_session_response(
        OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE,
        req->linked_eps_bearer_id.u8,
        req->sender_f_teid_for_control_plane.teid,
        req->sender_f_teid_for_control_plane.ipv4 ?
            &req->sender_f_teid_for_control_plane.ipv4->addr : NULL,
        req->sender_f_teid_for_control_plane.ipv6 ?
            &req->sender_f_teid_for_control_plane.ipv6->addr : NULL);
    ogs_assert(pkbuf);

    rsp = &pkbuf->gtp2_message->delete_session_response;
    ogs_assert(rsp);

    rsp->cause.presence = 1;
    rsp->cause.data = &rsp->cause_value;
    rsp->cause.len = 1;
    rsp->cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    return pkbuf;
}

int smf_s8_gtp_send_modify_bearer_response(
    smf_sess_t *sess, ogs_gtp_xact_t *xact,
    ogs_gtp2_modify_bearer_request_t *req)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_modify_bearer_response_t *rsp = NULL;
    int rv;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    pkbuf = ogs_gtp2_build_modify_bearer_response(
        OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE,
        req->bearer_contexts_to_be_modified.eps_bearer_id.u8,
        req->sender_f_teid_for_control_plane.teid,
        req->sender_f_teid_for_control_plane.ipv4 ?
            &req->sender_f_teid_for_control_plane.ipv4->addr : NULL,
        req->sender_f_teid_for_control_plane.ipv6 ?
            &req->sender_f_teid_for_control_plane.ipv6->addr : NULL);
    ogs_assert(pkbuf);

    rsp = &pkbuf->gtp2_message->modify_bearer_response;
    ogs_assert(rsp);

    rsp->cause.presence = 1;
    rsp->cause.data = &rsp->cause_value;
    rsp->cause.len = 1;
    rsp->cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    rv = ogs_gtp_xact_update_tx(xact, &rsp->h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

ogs_pkbuf_t *smf_s8_build_create_bearer_request(
    smf_sess_t *sess, ogs_gtp_xact_t *xact,
    ogs_gtp2_bearer_context_t *bearer)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_create_bearer_request_t *req = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(bearer);

    pkbuf = ogs_gtp2_build_create_bearer_request(
        OGS_GTP2_CREATE_BEARER_REQUEST_TYPE,
        bearer->eps_bearer_id_value,
        sess->pgw_s5c_teid,
        &sess->pgw_s5c_addr,
        NULL);
    ogs_assert(pkbuf);

    req = &pkbuf->gtp2_message->create_bearer_request;
    ogs_assert(req);

    req->pti.presence = 1;
    req->pti.data = &req->pti_value;
    req->pti.len = 1;
    req->pti_value = bearer->procedure_transaction_id;

    req->bearer_contexts.bearer_context[0].eps_bearer_id.presence = 1;
    req->bearer_contexts.bearer_context[0].eps_bearer_id.data = 
        &req->bearer_contexts.bearer_context[0].eps_bearer_id_value;
    req->bearer_contexts.bearer_context[0].eps_bearer_id.len = 1;
    req->bearer_contexts.bearer_context[0].eps_bearer_id_value = bearer->eps_bearer_id_value;

    req->bearer_contexts.bearer_context[0].bearer_level_qos.presence = 1;
    req->bearer_contexts.bearer_context[0].bearer_level_qos.data = 
        &req->bearer_contexts.bearer_context[0].bearer_level_qos_data;
    req->bearer_contexts.bearer_context[0].bearer_level_qos.len = 
        sizeof(ogs_gtp2_bearer_level_qos_t);
    memcpy(&req->bearer_contexts.bearer_context[0].bearer_level_qos_data,
        &bearer->bearer_level_qos_data, sizeof(ogs_gtp2_bearer_level_qos_t));

    if (bearer->tft.presence) {
        req->bearer_contexts.bearer_context[0].tft.presence = 1;
        req->bearer_contexts.bearer_context[0].tft.data = bearer->tft.data;
        req->bearer_contexts.bearer_context[0].tft.len = bearer->tft.len;
    }

    return pkbuf;
}

ogs_pkbuf_t *smf_s8_build_update_bearer_request(
    smf_sess_t *sess, ogs_gtp_xact_t *xact,
    ogs_gtp2_bearer_context_t *bearer)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_update_bearer_request_t *req = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(bearer);

    pkbuf = ogs_gtp2_build_update_bearer_request(
        OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE,
        bearer->eps_bearer_id_value,
        sess->pgw_s5c_teid,
        &sess->pgw_s5c_addr,
        NULL);
    ogs_assert(pkbuf);

    req = &pkbuf->gtp2_message->update_bearer_request;
    ogs_assert(req);

    req->pti.presence = 1;
    req->pti.data = &req->pti_value;
    req->pti.len = 1;
    req->pti_value = bearer->procedure_transaction_id;

    req->bearer_contexts.bearer_context[0].eps_bearer_id.presence = 1;
    req->bearer_contexts.bearer_context[0].eps_bearer_id.data = 
        &req->bearer_contexts.bearer_context[0].eps_bearer_id_value;
    req->bearer_contexts.bearer_context[0].eps_bearer_id.len = 1;
    req->bearer_contexts.bearer_context[0].eps_bearer_id_value = bearer->eps_bearer_id_value;

    req->bearer_contexts.bearer_context[0].bearer_level_qos.presence = 1;
    req->bearer_contexts.bearer_context[0].bearer_level_qos.data = 
        &req->bearer_contexts.bearer_context[0].bearer_level_qos_data;
    req->bearer_contexts.bearer_context[0].bearer_level_qos.len = 
        sizeof(ogs_gtp2_bearer_level_qos_t);
    memcpy(&req->bearer_contexts.bearer_context[0].bearer_level_qos_data,
        &bearer->bearer_level_qos_data, sizeof(ogs_gtp2_bearer_level_qos_t));

    if (bearer->tft.presence) {
        req->bearer_contexts.bearer_context[0].tft.presence = 1;
        req->bearer_contexts.bearer_context[0].tft.data = bearer->tft.data;
        req->bearer_contexts.bearer_context[0].tft.len = bearer->tft.len;
    }

    return pkbuf;
}

ogs_pkbuf_t *smf_s8_build_delete_bearer_request(
    smf_sess_t *sess, ogs_gtp_xact_t *xact,
    ogs_gtp2_bearer_context_t *bearer)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_delete_bearer_request_t *req = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(bearer);

    pkbuf = ogs_gtp2_build_delete_bearer_request(
        OGS_GTP2_DELETE_BEARER_REQUEST_TYPE,
        bearer->eps_bearer_id_value,
        sess->pgw_s5c_teid,
        &sess->pgw_s5c_addr,
        NULL);
    ogs_assert(pkbuf);

    req = &pkbuf->gtp2_message->delete_bearer_request;
    ogs_assert(req);

    req->pti.presence = 1;
    req->pti.data = &req->pti_value;
    req->pti.len = 1;
    req->pti_value = bearer->procedure_transaction_id;

    req->failed_bearer_contexts.bearer_context[0].eps_bearer_id.presence = 1;
    req->failed_bearer_contexts.bearer_context[0].eps_bearer_id.data = 
        &req->failed_bearer_contexts.bearer_context[0].eps_bearer_id_value;
    req->failed_bearer_contexts.bearer_context[0].eps_bearer_id.len = 1;
    req->failed_bearer_contexts.bearer_context[0].eps_bearer_id_value = bearer->eps_bearer_id_value;

    return pkbuf;
}
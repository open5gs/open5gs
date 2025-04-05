/* smf/s8-build.c */
#include "context.h"
#include "s8-build.h"
#include "s8-path.h"
#include "ogs-gtp.h"
#include "ogs-pkbuf.h"

ogs_pkbuf_t *smf_s8_build_create_session_response(
    smf_sess_t *sess, ogs_gtp_xact_t *xact)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_create_session_response_t *rsp = NULL;
    ogs_gtp2_tlv_bearer_context_t *bearer = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    pkbuf = ogs_gtp2_build_create_session_response(
        OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE,
        sess->sgw_s5c_teid, 0);
    ogs_assert(pkbuf);

    rsp = &pkbuf->gtp2_message->create_session_response;
    ogs_assert(rsp);

    rsp->cause.presence = 1;
    rsp->cause.data = &rsp->cause_value;
    rsp->cause.len = 1;
    rsp->cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.presence = 1;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.data = &sess->pgw_s5c_teid;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.len = sizeof(ogs_gtp2_f_teid_t);

    rsp->paa.presence = 1;
    rsp->paa.data = &sess->paa;
    rsp->paa.len = sizeof(ogs_paa_t);

    bearer = &rsp->bearer_contexts_created.bearer_context[0];
    bearer->eps_bearer_id.presence = 1;
    bearer->eps_bearer_id.data = &bearer->eps_bearer_id_value;
    bearer->eps_bearer_id.len = 1;
    bearer->eps_bearer_id_value = sess->bearer[0]->ebi;

    bearer->s5_s8_u_pgw_f_teid.presence = 1;
    bearer->s5_s8_u_pgw_f_teid.data = &bearer->s5_s8_u_pgw_f_teid_data;
    bearer->s5_s8_u_pgw_f_teid.len = sizeof(ogs_gtp2_f_teid_t);
    bearer->s5_s8_u_pgw_f_teid_data.interface_type = OGS_GTP2_F_TEID_S5_S8_PGW_GTP_U;
    bearer->s5_s8_u_pgw_f_teid_data.teid = sess->bearer[0]->pgw_s5u_teid;
    bearer->s5_s8_u_pgw_f_teid_data.ipv4 = 1;
    bearer->s5_s8_u_pgw_f_teid_data.addr = sess->bearer[0]->pgw_s5u_addr;

    return pkbuf;
}

ogs_pkbuf_t *smf_s8_build_delete_session_response(
    smf_sess_t *sess, ogs_gtp_xact_t *xact)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_delete_session_response_t *rsp = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    pkbuf = ogs_gtp2_build_delete_session_response(
        OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE,
        sess->sgw_s5c_teid, 0);
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
        sess->sgw_s5c_teid, 0);
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
    ogs_gtp2_tlv_bearer_context_t *bearer)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_create_bearer_request_t *req = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(bearer);

    pkbuf = ogs_gtp2_build_create_bearer_request(
        OGS_GTP2_CREATE_BEARER_REQUEST_TYPE,
        sess->sgw_s5c_teid, 0);
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
    ogs_gtp2_tlv_bearer_context_t *bearer)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_update_bearer_request_t *req = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(bearer);

    pkbuf = ogs_gtp2_build_update_bearer_request(
        OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE,
        sess->sgw_s5c_teid, 0);
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
    ogs_gtp2_tlv_bearer_context_t *bearer)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_delete_bearer_request_t *req = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(bearer);

    pkbuf = ogs_gtp2_build_delete_bearer_request(
        OGS_GTP2_DELETE_BEARER_REQUEST_TYPE,
        sess->sgw_s5c_teid, 0);
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
/* smf/s8-build.c */
#include "context.h"
#include "s8-build.h"

ogs_pkbuf_t *smf_s8_build_create_session_response(
    smf_sess_t *sess,
    ogs_gtp_xact_t *xact)
{
    ogs_gtp2_create_session_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    smf_bearer_t *bearer = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(bearer);

    pkbuf = ogs_gtp2_build_create_session_response(
            OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE,
            sess->sgw_s8_teid, 0);
    ogs_assert(pkbuf);

    rsp = &pkbuf->gtp2_message->create_session_response;
    ogs_assert(rsp);

    /* Set Cause */
    rsp->cause.presence = 1;
    rsp->cause.u8 = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    /* Set PGW S8 F-TEID */
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.presence = 1;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.data = &bearer->pgw_s5u_teid;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.len =
        sizeof(ogs_gtp2_f_teid_t);

    /* Set PAA */
    rsp->paa.presence = 1;
    rsp->paa.data = &sess->paa;
    rsp->paa.len = sizeof(ogs_paa_t);

    /* Set Bearer Contexts Created */
    rsp->bearer_contexts_created->presence = 1;
    rsp->bearer_contexts_created->eps_bearer_id.presence = 1;
    rsp->bearer_contexts_created->eps_bearer_id.u8 = bearer->ebi;

    /* Set S5/S8-U PGW F-TEID */
    rsp->bearer_contexts_created->s5_s8_u_pgw_f_teid.presence = 1;
    rsp->bearer_contexts_created->s5_s8_u_pgw_f_teid.data = &bearer->pgw_s5u_teid;
    rsp->bearer_contexts_created->s5_s8_u_pgw_f_teid.len =
        sizeof(ogs_gtp2_f_teid_t);

    return pkbuf;
}

ogs_pkbuf_t *smf_s8_build_delete_session_response(
    smf_sess_t *sess,
    ogs_gtp_xact_t *xact)
{
    ogs_gtp2_delete_session_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    pkbuf = ogs_gtp2_build_delete_session_response(
            OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE,
            sess->sgw_s8_teid, 0);
    ogs_assert(pkbuf);

    rsp = &pkbuf->gtp2_message->delete_session_response;
    ogs_assert(rsp);

    /* Set Cause */
    rsp->cause.presence = 1;
    rsp->cause.u8 = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    return pkbuf;
}

int smf_s8_gtp_send_modify_bearer_response(
    smf_sess_t *sess,
    ogs_gtp_xact_t *xact,
    ogs_gtp2_modify_bearer_request_t *req)
{
    ogs_gtp2_modify_bearer_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    pkbuf = ogs_gtp2_build_modify_bearer_response(
            OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE,
            sess->sgw_s8_teid, 0);
    ogs_assert(pkbuf);

    rsp = &pkbuf->gtp2_message->modify_bearer_response;
    ogs_assert(rsp);

    /* Set Cause */
    rsp->cause.presence = 1;
    rsp->cause.u8 = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    rv = ogs_gtp_xact_update_tx(xact, &pkbuf);
    if (rv != OGS_OK) {
        ogs_error("Failed to update transaction");
        ogs_pkbuf_free(pkbuf);
        return rv;
    }

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("Failed to commit transaction");
        return rv;
    }

    return OGS_OK;
}

ogs_pkbuf_t *smf_s8_build_create_bearer_request(
    smf_bearer_t *bearer,
    uint8_t pti,
    ogs_gtp2_tft_t *tft)
{
    ogs_gtp2_create_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    ogs_assert(bearer->sess);

    pkbuf = ogs_gtp2_build_create_bearer_request(
            OGS_GTP2_CREATE_BEARER_REQUEST_TYPE,
            bearer->sess->sgw_s8_teid, 0);
    ogs_assert(pkbuf);

    req = &pkbuf->gtp2_message->create_bearer_request;
    ogs_assert(req);

    /* Set PTI */
    req->procedure_transaction_id.presence = 1;
    req->procedure_transaction_id.u8 = pti;

    /* Set Bearer Contexts */
    req->bearer_contexts.eps_bearer_id.presence = 1;
    req->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

    /* Set S5/S8-U PGW F-TEID */
    req->bearer_contexts.s5_s8_u_pgw_f_teid.presence = 1;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.data = &bearer->pgw_s5u_teid;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.len =
        sizeof(ogs_gtp2_f_teid_t);

    /* Set TFT */
    if (tft) {
        req->bearer_contexts.tft.presence = 1;
        req->bearer_contexts.tft.data = tft;
        req->bearer_contexts.tft.len = sizeof(ogs_gtp2_tft_t);
    }

    return pkbuf;
}

ogs_pkbuf_t *smf_s8_build_update_bearer_request(
    smf_bearer_t *bearer,
    uint8_t pti,
    ogs_gtp2_tft_t *tft)
{
    ogs_gtp2_update_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    ogs_assert(bearer->sess);

    pkbuf = ogs_gtp2_build_update_bearer_request(
            OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE,
            bearer->sess->sgw_s8_teid, 0);
    ogs_assert(pkbuf);

    req = &pkbuf->gtp2_message->update_bearer_request;
    ogs_assert(req);

    /* Set PTI */
    req->procedure_transaction_id.presence = 1;
    req->procedure_transaction_id.u8 = pti;

    /* Set Bearer Contexts */
    req->bearer_contexts.eps_bearer_id.presence = 1;
    req->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

    /* Set TFT */
    if (tft) {
        req->bearer_contexts.tft.presence = 1;
        req->bearer_contexts.tft.data = tft;
        req->bearer_contexts.tft.len = sizeof(ogs_gtp2_tft_t);
    }

    return pkbuf;
}

ogs_pkbuf_t *smf_s8_build_delete_bearer_request(
    smf_bearer_t *bearer,
    uint8_t pti)
{
    ogs_gtp2_delete_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    ogs_assert(bearer->sess);

    pkbuf = ogs_gtp2_build_delete_bearer_request(
            OGS_GTP2_DELETE_BEARER_REQUEST_TYPE,
            bearer->sess->sgw_s8_teid, 0);
    ogs_assert(pkbuf);

    req = &pkbuf->gtp2_message->delete_bearer_request;
    ogs_assert(req);

    /* Set PTI */
    req->procedure_transaction_id.presence = 1;
    req->procedure_transaction_id.u8 = pti;

    /* Set Bearer Contexts */
    req->bearer_contexts.eps_bearer_id.presence = 1;
    req->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

    return pkbuf;
}
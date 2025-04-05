/* smf/s8-build.c */
#include "s8-build.h"

ogs_pkbuf_t *smf_s8_build_create_session_response(
        uint8_t type, smf_sess_t *sess, uint8_t cause_value)
{
    ogs_gtp2_message_t message;
    ogs_gtp2_create_session_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    smf_bearer_t *bearer = NULL;

    ogs_assert(sess);
    bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(bearer);

    ogs_debug("[SMF] Create Session Response");

    memset(&message, 0, sizeof(ogs_gtp2_message_t));
    message.h.type = type;
    message.h.teid = sess->sgw_s8_teid;

    rsp = &message.create_session_response;

    /* Set Cause */
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(ogs_gtp2_cause_t);
    rsp->cause.data = &cause_value;

    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        /* Set PGW S8 TEID */
        rsp->pgw_s8_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.presence = 1;
        rsp->pgw_s8_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.data = &bearer->pgw_s8_teid;
        rsp->pgw_s8_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.len =
            sizeof(ogs_gtp2_f_teid_t);

        /* Set PAA */
        rsp->paa.presence = 1;
        rsp->paa.data = &sess->paa;
        rsp->paa.len = sizeof(ogs_paa_t);

        /* Bearer Context Created */
        rsp->bearer_contexts_created.presence = 1;
        rsp->bearer_contexts_created.eps_bearer_id.presence = 1;
        rsp->bearer_contexts_created.eps_bearer_id.u8 = bearer->ebi;

        /* Set S8-U PGW TEID */
        rsp->bearer_contexts_created.s5_s8_u_pgw_f_teid.presence = 1;
        rsp->bearer_contexts_created.s5_s8_u_pgw_f_teid.data = &bearer->pgw_s8u_teid;
        rsp->bearer_contexts_created.s5_s8_u_pgw_f_teid.len =
            sizeof(ogs_gtp2_f_teid_t);
    }

    pkbuf = ogs_gtp2_build_msg(&message);
    ogs_assert(pkbuf);

    return pkbuf;
}

ogs_pkbuf_t *smf_s8_build_delete_session_response(
        uint8_t type, smf_sess_t *sess, uint8_t cause_value)
{
    ogs_gtp2_message_t message;
    ogs_gtp2_delete_session_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);

    ogs_debug("[SMF] Delete Session Response");

    memset(&message, 0, sizeof(ogs_gtp2_message_t));
    message.h.type = type;
    message.h.teid = sess->sgw_s8_teid;

    rsp = &message.delete_session_response;

    /* Set Cause */
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(ogs_gtp2_cause_t);
    rsp->cause.data = &cause_value;

    pkbuf = ogs_gtp2_build_msg(&message);
    ogs_assert(pkbuf);

    return pkbuf;
}

int smf_s8_gtp_send_modify_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_modify_bearer_request_t *req)
{
    int rv;
    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    /* Initialize GTP header */
    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE;
    h.teid = sess->sgw_s8_teid;

    /* Build response message */
    pkbuf = smf_s8_build_modify_bearer_response(sess, xact, req);
    if (!pkbuf) {
        ogs_error("smf_s8_build_modify_bearer_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_update_tx() failed");
        ogs_pkbuf_free(pkbuf);
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

ogs_pkbuf_t *smf_s8_build_create_bearer_request(
        smf_bearer_t *bearer, uint8_t pti, ogs_gtp2_tft_t *tft)
{
    ogs_gtp2_message_t message;
    ogs_gtp2_create_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);

    ogs_debug("[SMF] Create Bearer Request");

    memset(&message, 0, sizeof(ogs_gtp2_message_t));
    message.h.type = OGS_GTP2_CREATE_BEARER_REQUEST_TYPE;
    message.h.teid = bearer->sess->sgw_s8_teid;

    req = &message.create_bearer_request;

    /* Bearer Context */
    req->bearer_contexts.presence = 1;
    req->bearer_contexts.eps_bearer_id.presence = 1;
    req->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

    /* Set S8-U PGW TEID */
    req->bearer_contexts.s5_s8_u_pgw_f_teid.presence = 1;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.data = &bearer->pgw_s8u_teid;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.len = sizeof(ogs_gtp2_f_teid_t);

    /* Bearer QoS */
    req->bearer_contexts.bearer_level_qos.presence = 1;
    req->bearer_contexts.bearer_level_qos.data = &bearer->qos;
    req->bearer_contexts.bearer_level_qos.len = sizeof(bearer->qos);

    /* Set TFT */
    if (tft && tft->num_of_packet_filter) {
        req->bearer_contexts.tft.presence = 1;
        req->bearer_contexts.tft.data = tft;
        req->bearer_contexts.tft.len = OGS_GTP2_TFT_IPV4_LEN;
    }

    pkbuf = ogs_gtp2_build_msg(&message);
    ogs_assert(pkbuf);

    return pkbuf;
}
/* Add to smf/s8-build.c */

ogs_pkbuf_t *smf_s8_build_update_bearer_request(
        smf_bearer_t *bearer, uint8_t pti, ogs_gtp2_tft_t *tft)
{
    ogs_gtp2_message_t message;
    ogs_gtp2_update_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);

    ogs_debug("[SMF] Update Bearer Request");

    memset(&message, 0, sizeof(ogs_gtp2_message_t));
    message.h.type = OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE;
    message.h.teid = bearer->sess->sgw_s8_teid;

    req = &message.update_bearer_request;

    /* Bearer Context */
    req->bearer_contexts.presence = 1;
    req->bearer_contexts.eps_bearer_id.presence = 1;
    req->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

    /* Bearer QoS */
    if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
        bearer->qos.gbr.downlink || bearer->qos.gbr.uplink) {
        req->bearer_contexts.bearer_level_qos.presence = 1;
        req->bearer_contexts.bearer_level_qos.data = &bearer->qos;
        req->bearer_contexts.bearer_level_qos.len = sizeof(bearer->qos);
    }

    /* Set TFT */
    if (tft && tft->num_of_packet_filter) {
        req->bearer_contexts.tft.presence = 1;
        req->bearer_contexts.tft.data = tft;
        req->bearer_contexts.tft.len = OGS_GTP2_TFT_IPV4_LEN;
    }

    pkbuf = ogs_gtp2_build_msg(&message);
    ogs_assert(pkbuf);

    return pkbuf;
}

ogs_pkbuf_t *smf_s8_build_delete_bearer_request(
        smf_bearer_t *bearer, uint8_t pti)
{
    ogs_gtp2_message_t message;
    ogs_gtp2_delete_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);

    ogs_debug("[SMF] Delete Bearer Request");

    memset(&message, 0, sizeof(ogs_gtp2_message_t));
    message.h.type = OGS_GTP2_DELETE_BEARER_REQUEST_TYPE;
    message.h.teid = bearer->sess->sgw_s8_teid;

    req = &message.delete_bearer_request;

    req->eps_bearer_ids.presence = 1;
    req->eps_bearer_ids.u8 = bearer->ebi;

    if (pti) {
        req->procedure_transaction_id.presence = 1;
        req->procedure_transaction_id.u8 = pti;
    }

    pkbuf = ogs_gtp2_build_msg(&message);
    ogs_assert(pkbuf);

    return pkbuf;
}
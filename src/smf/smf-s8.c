/* Create new file src/smf/smf-s8.c */
#include "smf-context.h"
#include "smf-s8.h"

void smf_s8_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp2_echo_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SMF] Received Echo Request");
    /* Send Echo Response */
    ogs_gtp2_send_echo_response(xact, req->recovery.u8);
}

void smf_s8_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp2_echo_response_t *rsp)
{
    /* TODO: Process Echo Response */
    ogs_debug("[SMF] Received Echo Response");
}

uint8_t smf_s8_handle_create_session_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_create_session_request_t *req)
{
    uint8_t cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    ogs_assert(xact);
    ogs_assert(req);
    ogs_assert(sess);

    /* TODO: Process Create Session Request */
    ogs_debug("[SMF] Received Create Session Request");

    /* Send Create Session Response */
    smf_s8_gtp_send_create_session_response(sess, xact);

    return cause_value;
}

uint8_t smf_s8_handle_delete_session_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_delete_session_request_t *req)
{
    uint8_t cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    ogs_assert(xact);
    ogs_assert(req);
    ogs_assert(sess);

    /* TODO: Process Delete Session Request */
    ogs_debug("[SMF] Received Delete Session Request");

    /* Send Delete Session Response */
    smf_s8_gtp_send_delete_session_response(sess, xact);

    return cause_value;
}

void smf_s8_handle_modify_bearer_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_modify_bearer_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);
    ogs_assert(sess);

    /* TODO: Process Modify Bearer Request */
    ogs_debug("[SMF] Received Modify Bearer Request");

    /* Send Modify Bearer Response */
    smf_s8_gtp_send_modify_bearer_response(sess, xact, req);
}
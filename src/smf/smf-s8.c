#include "context.h"
#include "smf-s8.h"
#include "s8-path.h"

void smf_s8_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp2_echo_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SMF] Received Echo Request");
    /* Note: NULL in the last parameter means no specific node */
    ogs_gtp2_send_echo_response(xact, req->recovery.u8, NULL);
}

void smf_s8_handle_create_session_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_create_session_request_t *req)
{
    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SMF] Received Create Session Request");
    smf_s8_send_create_session_response(sess, xact, OGS_GTP2_CAUSE_REQUEST_ACCEPTED);
}

void smf_s8_handle_delete_session_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_delete_session_request_t *req)
{
    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SMF] Received Delete Session Request");
    smf_s8_send_delete_session_response(sess, xact, OGS_GTP2_CAUSE_REQUEST_ACCEPTED);
}

void smf_s8_handle_modify_bearer_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_modify_bearer_request_t *req)
{
    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SMF] Received Modify Bearer Request");
    smf_s8_send_modify_bearer_response(sess, xact, req);
}
/* Create new file src/smf/smf-s8.h */
#ifndef SMF_S8_H
#define SMF_S8_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

    void smf_s8_handle_echo_request(
            ogs_gtp_xact_t *xact, ogs_gtp2_echo_request_t *req);
    void smf_s8_handle_echo_response(
            ogs_gtp_xact_t *xact, ogs_gtp2_echo_response_t *rsp);
    void smf_s8_handle_create_session_request(
            smf_sess_t *sess, ogs_gtp_xact_t *xact,
            ogs_gtp2_create_session_request_t *req);
    void smf_s8_handle_delete_session_request(
            smf_sess_t *sess, ogs_gtp_xact_t *xact,
            ogs_gtp2_delete_session_request_t *req);
    void smf_s8_handle_modify_bearer_request(
            smf_sess_t *sess, ogs_gtp_xact_t *xact,
            ogs_gtp2_modify_bearer_request_t *req);

#ifdef __cplusplus
}
#endif

#endif /* SMF_S8_H */
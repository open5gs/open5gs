#ifndef SMF_S8_PATH_H
#define SMF_S8_PATH_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

    int smf_s8_init(void);
    void smf_s8_final(void);

    void smf_s8_send_create_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact, uint8_t cause);
    void smf_s8_send_delete_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact, uint8_t cause);
    void smf_s8_send_modify_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_modify_bearer_request_t *req);

#ifdef __cplusplus
}
#endif

#endif /* SMF_S8_PATH_H */
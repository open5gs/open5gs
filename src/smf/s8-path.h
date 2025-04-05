/* smf/s8-path.h */
#ifndef SMF_S8_PATH_H
#define SMF_S8_PATH_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

    int smf_s8_init(void);
    void smf_s8_final(void);

    /* S8 GTP path management */
    void smf_s8_send_create_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact, uint8_t cause);
    void smf_s8_send_delete_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact, uint8_t cause);
    void smf_s8_send_modify_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact, uint8_t cause);

    /* Bearer management */
    void smf_s8_send_create_bearer_request(
        smf_bearer_t *bearer, uint8_t pti, ogs_gtp2_tft_t *tft);
    void smf_s8_send_update_bearer_request(
        smf_bearer_t *bearer, uint8_t pti, ogs_gtp2_tft_t *tft);
    void smf_s8_send_delete_bearer_request(
        smf_bearer_t *bearer, uint8_t pti);

#ifdef __cplusplus
}
#endif

#endif
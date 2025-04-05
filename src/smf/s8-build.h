#ifndef SMF_S8_BUILD_H
#define SMF_S8_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

    ogs_pkbuf_t *smf_s8_build_create_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact);

    ogs_pkbuf_t *smf_s8_build_delete_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact);

    ogs_pkbuf_t *smf_s8_build_modify_bearer_response(
        smf_sess_t *sess,
        ogs_gtp_xact_t *xact,
        ogs_gtp2_modify_bearer_request_t *req);

    ogs_pkbuf_t *smf_s8_build_create_bearer_request(
        smf_bearer_t *bearer,
        uint8_t pti,
        ogs_gtp2_tft_t *tft);

    ogs_pkbuf_t *smf_s8_build_update_bearer_request(
        smf_bearer_t *bearer,
        uint8_t pti,
        ogs_gtp2_tft_t *tft);

    ogs_pkbuf_t *smf_s8_build_delete_bearer_request(
        smf_bearer_t *bearer,
        uint8_t pti);

#ifdef __cplusplus
}
#endif

#endif /* SMF_S8_BUILD_H */
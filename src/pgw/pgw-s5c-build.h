#ifndef __PGW_S5C_BUILD_H__
#define __PGW_S5C_BUILD_H__

#include "gtp/gtp-message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int pgw_s5c_build_create_session_response(
        ogs_pkbuf_t **pkbuf, uint8_t type, pgw_sess_t *sess,
        gx_message_t *gx_message, gtp_create_session_request_t *req);
int pgw_s5c_build_delete_session_response(
        ogs_pkbuf_t **pkbuf, uint8_t type,
        gx_message_t *gx_message, gtp_delete_session_request_t *req);

int pgw_s5c_build_create_bearer_request(
        ogs_pkbuf_t **pkbuf, uint8_t type, pgw_bearer_t *bearer);
int pgw_s5c_build_update_bearer_request(
        ogs_pkbuf_t **pkbuf, uint8_t type, pgw_bearer_t *bearer,
        int qos_presence, int tft_presence);
int pgw_s5c_build_delete_bearer_request(
        ogs_pkbuf_t **pkbuf, uint8_t type, pgw_bearer_t *bearer);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_S5C_BUILD_H__ */

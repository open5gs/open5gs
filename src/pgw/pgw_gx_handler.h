#ifndef __PGW_GX_HANDLER_H__
#define __PGW_GX_HANDLER_H__

#include "gtp/gtp_message.h"

#include "pgw_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) pgw_gx_handle_cca_initial_request(
        pgw_sess_t *sess, gx_message_t *gx_message,
        gtp_xact_t *xact, gtp_create_session_request_t *req);
CORE_DECLARE(void) pgw_gx_handle_cca_termination_request(
        pgw_sess_t *sess, gx_message_t *gx_message,
        gtp_xact_t *xact, gtp_delete_session_request_t *req);
CORE_DECLARE(void) pgw_gx_handle_re_auth_request(
        pgw_sess_t *sess, gx_message_t *gx_message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_GX_HANDLER_H__ */

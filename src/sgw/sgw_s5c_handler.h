#ifndef __SGW_S5C_HANDLER_H__
#define __SGW_S5C_HANDLER_H__

#include "gtp/gtp_message.h"

#include "sgw_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) sgw_s5c_handle_create_session_response(gtp_xact_t *s5c_xact,
        sgw_sess_t *sess, gtp_message_t *gtp_message);
CORE_DECLARE(void) sgw_s5c_handle_delete_session_response(gtp_xact_t *s5c_xact,
        sgw_sess_t *sess, gtp_message_t *gtp_message);
CORE_DECLARE(void) sgw_s5c_handle_create_bearer_request(gtp_xact_t *s5c_xact,
        sgw_sess_t *sess, gtp_message_t *gtp_message);
CORE_DECLARE(void) sgw_s5c_handle_update_bearer_request(gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, gtp_message_t *gtp_message);
CORE_DECLARE(void) sgw_s5c_handle_delete_bearer_request(gtp_xact_t *s5c_xact, 
    sgw_sess_t *sess, gtp_message_t *gtp_message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_S5C_HANDLER_H__ */

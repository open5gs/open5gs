#ifndef __PGW_S5C_HANDLER_H__
#define __PGW_S5C_HANDLER_H__

#include "gtp_tlv.h"

#include "pgw_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) pgw_s5c_handle_create_session_request(
        gtp_xact_t *xact, pgw_sess_t *sess, gtp_create_session_request_t *req);
CORE_DECLARE(void) pgw_s5c_handle_create_session_response(
        gtp_xact_t *xact, pgw_sess_t *sess);
CORE_DECLARE(void) pgw_s5c_handle_delete_session_request(
        gtp_xact_t *xact, pgw_sess_t *sess, gtp_delete_session_request_t *req);
CORE_DECLARE(void) pgw_s5c_handle_delete_session_response(
        gtp_xact_t *xact, pgw_sess_t *sess);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_S5C_HANDLER_H__ */

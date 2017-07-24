#ifndef __PGW_HANDLER_H__
#define __PGW_HANDLER_H__

#include "gtp_tlv.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) pgw_handle_create_session_request(
        gtp_xact_t *xact, gtp_create_session_request_t *req);
CORE_DECLARE(void) pgw_handle_delete_session_request(
        gtp_xact_t *xact, pgw_sess_t *sess, gtp_delete_session_request_t *req);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_HANDLER_H__ */

#ifndef __PGW_GX_HANDLER_H__
#define __PGW_GX_HANDLER_H__

#include "gtp_tlv.h"

#include "pgw_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) pgw_gx_handle_cca_initial_request(
        gtp_xact_t *xact, pgw_sess_t *sess,
        gx_cca_message_t *cca_message, gtp_create_session_request_t *req);
CORE_DECLARE(void) pgw_gx_handle_cca_termination_request(
        gtp_xact_t *xact, pgw_sess_t *sess,
        gx_cca_message_t *cca_message, gtp_delete_session_request_t *req);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_GX_HANDLER_H__ */

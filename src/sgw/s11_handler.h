#ifndef __SGW_HANDLER_H__
#define __SGW_HANDLER_H__

#include "gtp_tlv.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) sgw_handle_create_session_request(
        gtp_xact_t *xact, gtp_create_session_request_t *req);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_HANDLER_H__ */

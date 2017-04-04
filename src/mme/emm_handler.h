#ifndef __EMM_HANDLER_H__
#define __EMM_HANDLER_H__

#include "nas_message.h"

#include "mme_ctx.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) emm_handle_attach_request(
 ue_ctx_t *ue, nas_attach_request_t *attach_request);
CORE_DECLARE(void) emm_handle_authentication_response(
 ue_ctx_t *ue, nas_authentication_response_t *authentication_response);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EMM_HANDLER_H__ */

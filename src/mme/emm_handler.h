#ifndef __EMM_HANDLER_H__
#define __EMM_HANDLER_H__

#include "nas_message.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) emm_handle_attach_request(
 mme_ue_t *ue, nas_attach_request_t *attach_request);
CORE_DECLARE(void) emm_handle_authentication_response(
 mme_ue_t *ue, nas_authentication_response_t *authentication_response);
CORE_DECLARE(void) emm_handle_security_mode_complete(mme_ue_t *ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EMM_HANDLER_H__ */

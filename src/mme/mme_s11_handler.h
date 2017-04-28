#ifndef __MME_HANDLER_H__
#define __MME_HANDLER_H__

#include "gtp_tlv.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) mme_s11_handle_create_session_response(
        mme_ue_t *ue, gtp_create_session_response_t *rsp);
CORE_DECLARE(void) mme_s11_handle_modify_bearer_response(
        mme_ue_t *ue, gtp_modify_bearer_response_t *rsp);
CORE_DECLARE(void) mme_s11_handle_delete_session_response(
        mme_ue_t *ue, gtp_delete_session_response_t *rsp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_HANDLER_H__ */

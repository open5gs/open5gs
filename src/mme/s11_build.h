#ifndef __S11_BUILD_H__
#define __S11_BUILD_H__

#include "gtp_tlv.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) s11_build_create_session_req(
                        gtp_message_t *gtp_message, ue_ctx_t *ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S11_BUILD_H__ */

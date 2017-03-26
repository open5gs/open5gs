#ifndef __S11_BUILD_H__
#define __S11_BUILD_H__

#include "s1ap_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) s11_build_create_session_req(
                        pkbuf_t **pkbuf, ue_ctx_t *ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S11_BUILD_H__ */

#ifndef __MME_S11_BUILD_H__
#define __MME_S11_BUILD_H__

#include "gtp_tlv.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) mme_s11_build_create_session_req(
                        c_uint8_t *type, pkbuf_t **pkbuf, ue_ctx_t *ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_S11_BUILD_H__ */

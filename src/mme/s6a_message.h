#ifndef _S6A_AUTH_H__
#define _S6A_AUTH_H__

#include "core_errno.h"

#include "plmn_id.h"

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern struct session_handler *s6a_mme_reg;

CORE_DECLARE(status_t) s6a_mme_init(void);
CORE_DECLARE(void) s6a_mme_final(void);

CORE_DECLARE(int) s6a_send_auth_info_req(ue_ctx_t *ue, c_uint8_t *plmn_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


#ifndef __S6A_SM_H__
#define __S6A_SM_H__

#include "core_errno.h"

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) s6a_sm_init(void);
CORE_DECLARE(void) s6a_sm_final(void);

CORE_DECLARE(int) s6a_send_auth_info_req(ue_ctx_t *ue, plmn_t *plmn_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S6A_SM_H__ */


#ifndef __MME_S6A_HANDLER_H__
#define __MME_S6A_HANDLER_H__

#include "core_errno.h"

#include "mme_ctx.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) mme_s6a_init(void);
CORE_DECLARE(void) mme_s6a_final(void);

CORE_DECLARE(int) mme_s6a_send_auth_info_req(
                    ue_ctx_t *ue, plmn_id_t *plmn_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_S6A_HANDLER_H__ */


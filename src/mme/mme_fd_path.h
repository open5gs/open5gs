#ifndef __MME_FD_PATH_H__
#define __MME_FD_PATH_H__

#include "core_errno.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) mme_fd_init(void);
CORE_DECLARE(void) mme_fd_final(void);

/* MME Sends Authentication Information Request to HSS */
CORE_DECLARE(void) mme_s6a_send_air(mme_ue_t *mme_ue,
    nas_authentication_failure_parameter_t *authentication_failure_parameter);
/* MME Sends Update Location Request to HSS */
CORE_DECLARE(void) mme_s6a_send_ulr(mme_ue_t *mme_ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_FD_PATH_H__ */


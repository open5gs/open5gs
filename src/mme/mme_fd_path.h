#ifndef __MME_FD_PATH_H__
#define __MME_FD_PATH_H__

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int mme_fd_init(void);
void mme_fd_final(void);

/* MME Sends Authentication Information Request to HSS */
void mme_s6a_send_air(mme_ue_t *mme_ue,
    nas_authentication_failure_parameter_t *authentication_failure_parameter);
/* MME Sends Update Location Request to HSS */
void mme_s6a_send_ulr(mme_ue_t *mme_ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_FD_PATH_H__ */


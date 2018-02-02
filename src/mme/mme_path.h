#ifndef __MME_PATH_H__
#define __MME_PATH_H__

#include "core_errno.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) mme_send_detach_accept(mme_ue_t *mme_ue);
CORE_DECLARE(status_t) mme_send_ue_context_release_command(
        mme_ue_t *mme_ue, enb_ue_t *enb_ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_PATH_H__ */

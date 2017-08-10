#ifndef __NAS_PATH_H__
#define __NAS_PATH_H__

#include "core_pkbuf.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) nas_send_to_enb(enb_ue_t *enb_ue, pkbuf_t *pkbuf);
CORE_DECLARE(status_t) nas_send_to_downlink_nas_transport(
    mme_ue_t *mme_ue, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_PATH_H__ */

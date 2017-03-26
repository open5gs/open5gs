#ifndef __NAS_BUILD_H__
#define __NAS_BUILD_H__

#include "core_pkbuf.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) nas_send_s1ap_to_enb(ue_ctx_t *ue, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_BUILD_H__ */

#ifndef __PGW_GX_HANDLER_H__
#define __PGW_GX_HANDLER_H__

#include "core_errno.h"

#include "pgw_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(int) pgw_gx_init(void);
CORE_DECLARE(void) pgw_gx_final(void);

/* PGW Sends Credit Control Request to PCRF */
#if 0
CORE_DECLARE(void) pgw_gx_send_ccr(mme_ue_t *mme_ue);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_GX_HANDLER_H__ */


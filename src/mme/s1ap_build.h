#ifndef __S1AP_BUILD_H__
#define __S1AP_BUILD_H__

#include "s1ap_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) s1ap_build_setup_rsp(pkbuf_t **pkbuf);
CORE_DECLARE(status_t) s1ap_build_setup_failure(
        pkbuf_t **pkbuf, S1ap_Cause_t cause);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S1AP_BUILD_H__ */

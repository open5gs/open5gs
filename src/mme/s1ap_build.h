#ifndef __S1AP_BUILD_H__
#define __S1AP_BUILD_H__

#include "s1ap_message.h"
#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) s1ap_build_setup_rsp(pkbuf_t **pkbuf);
CORE_DECLARE(status_t) s1ap_build_setup_failure(
        pkbuf_t **pkbuf, S1ap_Cause_t cause);
CORE_DECLARE(status_t) s1ap_build_initial_context_setup_request(
            pkbuf_t **s1apbuf, mme_esm_t *esm, pkbuf_t *emmbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S1AP_BUILD_H__ */

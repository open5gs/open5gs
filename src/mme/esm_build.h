#ifndef __ESM_BUILD_H__
#define __ESM_BUILD_H__

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) esm_build_pdn_connectivity_reject(pkbuf_t **pkbuf,
                            mme_sess_t *sess, nas_esm_cause_t esm_cause);
CORE_DECLARE(status_t) esm_build_information_request(pkbuf_t **pkbuf,
                            mme_bearer_t *bearer);
CORE_DECLARE(status_t) esm_build_activate_default_bearer_context_request(
                            pkbuf_t **pkbuf, mme_sess_t *sess);
CORE_DECLARE(status_t) esm_build_activate_dedicated_bearer_context_request(
                            pkbuf_t **pkbuf, mme_bearer_t *bearer);
CORE_DECLARE(status_t) esm_build_modify_bearer_context_request(
        pkbuf_t **pkbuf, mme_bearer_t *bearer, int qos_presence, int tft_presence);
CORE_DECLARE(status_t) esm_build_deactivate_bearer_context_request(
        pkbuf_t **pkbuf, mme_bearer_t *bearer, nas_esm_cause_t esm_cause);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ESM_BUILD_H__ */

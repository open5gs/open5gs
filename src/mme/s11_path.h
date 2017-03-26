#ifndef __MME_S11_PATH_H__
#define __MME_S11_PATH_H__

#include "gtp_path.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) mme_s11_open(void *sgw);
CORE_DECLARE(status_t) mme_s11_close(void *sgw);

CORE_DECLARE(status_t) mme_s11_send_to_sgw(void *sgw, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_S11_PATH_H__ */

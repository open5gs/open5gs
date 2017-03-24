#ifndef __SGW_S11_PATH_H__
#define __SGW_S11_PATH_H__

#include "gtp_path.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) sgw_s11_open();
CORE_DECLARE(status_t) sgw_s11_close();

CORE_DECLARE(status_t) sgw_s11_send_to_mme(pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_S11_PATH_H__ */

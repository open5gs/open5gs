#ifndef __SGW_PATH_H__
#define __SGW_PATH_H__

#include "gtp_path.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) sgw_path_open();
CORE_DECLARE(status_t) sgw_path_close();

CORE_DECLARE(status_t) sgw_path_send_to_s11(pkbuf_t *pkbuf);
CORE_DECLARE(status_t) sgw_path_send_to_s5c(pkbuf_t *pkbuf);
CORE_DECLARE(status_t) sgw_path_send_to_s5u(pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_PATH_H__ */

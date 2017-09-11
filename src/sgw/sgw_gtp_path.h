#ifndef __SGW_PATH_H__
#define __SGW_PATH_H__

#include "gtp_xact.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) sgw_gtp_open();
CORE_DECLARE(status_t) sgw_gtp_close();

CORE_DECLARE(status_t) sgw_gtp_send_end_marker(sgw_bearer_t *bearer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_PATH_H__ */

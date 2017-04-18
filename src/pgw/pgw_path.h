#ifndef __PGW_PATH_H__
#define __PGW_PATH_H__

#include "gtp_xact.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) pgw_path_open();
CORE_DECLARE(status_t) pgw_path_close();

CORE_DECLARE(status_t) pgw_s5c_send_to_sgw(
        gtp_xact_t *xact, c_uint8_t type, c_uint32_t teid, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_PATH_H__ */

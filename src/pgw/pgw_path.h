#ifndef __PGW_PATH_H__
#define __PGW_PATH_H__

#include "gtp_xact.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) pgw_path_open();
CORE_DECLARE(status_t) pgw_path_close();

CORE_DECLARE(gtp_xact_t *) pgw_s5c_send_to_sgw(gtp_message_t *gtp_messgae);
CORE_DECLARE(status_t) pgw_s5u_send_to_sgw(pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_PATH_H__ */

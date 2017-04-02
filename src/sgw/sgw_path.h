#ifndef __SGW_PATH_H__
#define __SGW_PATH_H__

#include "gtp_xact.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) sgw_path_open();
CORE_DECLARE(status_t) sgw_path_close();

CORE_DECLARE(status_t) sgw_s11_send_to_mme(gtp_message_t *gtp_message);
CORE_DECLARE(status_t) sgw_s5c_send_to_pgw(
        gtp_xact_t *s11_xact, gtp_message_t *gtp_message);
CORE_DECLARE(status_t) sgw_s5u_send_to_pgw(pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_PATH_H__ */

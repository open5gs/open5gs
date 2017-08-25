#ifndef __PGW_FD_PATH_H__
#define __PGW_FD_PATH_H__

#include "core_errno.h"

#include "pgw_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(int) pgw_fd_init(void);
CORE_DECLARE(void) pgw_fd_final(void);

CORE_DECLARE(void) pgw_gx_send_ccr(gtp_xact_t *xact, pgw_sess_t *sess,
        pkbuf_t *gtpbuf, c_uint32_t cc_request_type);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_FD_PATH_H__ */


#ifndef __PGW_FD_PATH_H__
#define __PGW_FD_PATH_H__

#include "pgw_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct gtp_xact_s gtp_xact_t;

int pgw_fd_init(void);
void pgw_fd_final(void);

void pgw_gx_send_ccr(pgw_sess_t *sess, gtp_xact_t *xact,
        ogs_pkbuf_t *gtpbuf, uint32_t cc_request_type);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_FD_PATH_H__ */


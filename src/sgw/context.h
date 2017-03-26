#ifndef __SGW_CTX_H__
#define __SGW_CTX_H__

#include "core_list.h"
#include "core_errno.h"
#include "core_net.h"

#include "sm.h"

#include "gtp_path.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _sgw_ctx_t {
    gtp_node_t      s11_node;
    gtp_node_t      s5c_node;
    gtp_node_t      s5u_node;

    msgq_id         queue_id;
    tm_service_t    tm_service;

} sgw_ctx_t;

CORE_DECLARE(status_t)      sgw_ctx_init(void);
CORE_DECLARE(status_t)      sgw_ctx_final(void);

CORE_DECLARE(sgw_ctx_t*)    sgw_self(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_CTX_H__ */

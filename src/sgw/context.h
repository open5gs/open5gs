#ifndef __SGW_CTX_H__
#define __SGW_CTX_H__

#include "core_list.h"
#include "core_errno.h"
#include "core_net.h"

#include "3gpp_message.h"
#include "sm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _sgw_ctx_t {
    c_uint32_t      sgw_local_addr;
    c_uint32_t      mme_remote_addr;
    c_uint32_t      pgw_remote_addr;

    net_sock_t      *s11_sock;
    c_uint16_t      s11_local_port;
    c_uint16_t      s11_remote_port;

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

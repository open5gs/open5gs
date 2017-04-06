#ifndef __PGW_CTX_H__
#define __PGW_CTX_H__

#include "core_list.h"
#include "core_errno.h"
#include "core_net.h"

#include "gtp_xact.h"

#include "pgw_sm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _pgw_ctx_t {
    c_uint32_t      s5c_addr;  /* PGW S5-C local address */
    c_uint32_t      s5c_port;  /* PGW S5-C local port */
    net_sock_t*     s5c_sock;  /* PGW S5-C local listen socket */
    gtp_node_t      s5c_node;  /* PGW S5-C remote GTPv2-C node */

    c_uint32_t      s5u_addr;  /* PGW S5-U local address */
    c_uint32_t      s5u_port;  /* PGW S5-U local port */
    net_sock_t*     s5u_sock;  /* PGW S5-U local listen socket */
    gtp_node_t      s5u_node;  /* PGW S5-U remote GTPv1-U node */

    msgq_id         queue_id;       /* Queue for processing PGW control plane */
    tm_service_t    tm_service;     /* Timer Service */
    gtp_xact_ctx_t  gtp_xact_ctx;   /* GTP Transaction Context */
} pgw_ctx_t;

CORE_DECLARE(status_t)      pgw_ctx_init(void);
CORE_DECLARE(status_t)      pgw_ctx_final(void);

CORE_DECLARE(pgw_ctx_t*)    pgw_self(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_CTX_H__ */

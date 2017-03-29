#ifndef __SGW_CTX_H__
#define __SGW_CTX_H__

#include "core_list.h"
#include "core_errno.h"
#include "core_net.h"

#include "sm.h"

#include "gtp_xact.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _sgw_ctx_t {
    c_uint32_t      s11_addr;  /* SGW S11 local address */
    c_uint32_t      s11_port;  /* SGW S11 local port */
    net_sock_t*     s11_sock;  /* SGW S11 local listen socket */
    gtp_node_t      s11_node;  /* MME S11 remote GTPv2-C node */

    c_uint32_t      s5c_addr;  /* SGW S5-C local address */
    c_uint32_t      s5c_port;  /* SGW S5-C local port */
    net_sock_t*     s5c_sock;  /* SGW S5-C local listen socket */
    gtp_node_t      s5c_node;  /* PGW S5-C remote GTPv2-C node */

    c_uint32_t      s5u_addr;  /* SGW S5-U local address */
    c_uint32_t      s5u_port;  /* SGW S5-U local port */
    net_sock_t*     s5u_sock;  /* SGW S5-U local listen socket */
    gtp_node_t      s5u_node;  /* PGW S5-U remote GTPv1-U node */

    msgq_id         queue_id;       /* Queue for processing SGW control plane */
    tm_service_t    tm_service;     /* Timer Service */
    gtp_xact_ctx_t  gtp_xact_ctx;   /* GTP Transaction Context */
} sgw_ctx_t;

typedef struct _s11_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint32_t      teid;
} s11_ctx_t;

typedef struct _s5c_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint32_t      teid;
} s5c_ctx_t;

CORE_DECLARE(status_t)      sgw_ctx_init(void);
CORE_DECLARE(status_t)      sgw_ctx_final(void);

CORE_DECLARE(sgw_ctx_t*)    sgw_self(void);

CORE_DECLARE(s11_ctx_t*)    sgw_ctx_s11_add();
CORE_DECLARE(status_t)      sgw_ctx_s11_remove(s11_ctx_t *s11);
CORE_DECLARE(status_t)      sgw_ctx_s11_remove_all();
CORE_DECLARE(s11_ctx_t*)    sgw_ctx_s11_find_by_teid(c_uint32_t teid);
CORE_DECLARE(s11_ctx_t*)    sgw_ctx_s11_first();
CORE_DECLARE(s11_ctx_t*)    sgw_ctx_s11_next(s11_ctx_t *s11);

CORE_DECLARE(s5c_ctx_t*)    sgw_ctx_s5c_add();
CORE_DECLARE(status_t)      sgw_ctx_s5c_remove(s5c_ctx_t *s5c);
CORE_DECLARE(status_t)      sgw_ctx_s5c_remove_all();
CORE_DECLARE(s5c_ctx_t*)    sgw_ctx_s5c_find_by_teid(c_uint32_t teid);
CORE_DECLARE(s5c_ctx_t*)    sgw_ctx_s5c_first();
CORE_DECLARE(s5c_ctx_t*)    sgw_ctx_s5c_next(s5c_ctx_t *s5c);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_CTX_H__ */

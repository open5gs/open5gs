#ifndef __SGW_CTX_H__
#define __SGW_CTX_H__

#include "core_list.h"
#include "core_errno.h"
#include "core_net.h"
#include "core_event.h"
#include "core_hash.h"

#include "gtp_xact.h"

#include "sgw_sm.h"

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

typedef struct _sgw_gtpc_t {
    c_uint32_t      teid;       /* SGW-S11-TEID, SGW-S5C-TEID */
    c_uint32_t      mme_teid;   /* MME-S11-TEID */
    c_uint32_t      pgw_teid;   /* PGW-S11-TEID */
} sgw_gtpc_t;

CORE_DECLARE(status_t)      sgw_ctx_init(void);
CORE_DECLARE(status_t)      sgw_ctx_final(void);

CORE_DECLARE(sgw_ctx_t*)    sgw_self(void);

CORE_DECLARE(sgw_gtpc_t*)   sgw_ctx_gtpc_add();
CORE_DECLARE(status_t )     sgw_ctx_gtpc_remove(sgw_gtpc_t *gtpc);
CORE_DECLARE(status_t )     sgw_ctx_gtpc_remove_all();
CORE_DECLARE(sgw_gtpc_t*)   sgw_ctx_gtpc_find(c_uint32_t teid);
CORE_DECLARE(hash_index_t*) sgw_ctx_gtpc_first();
CORE_DECLARE(hash_index_t*) sgw_ctx_gtpc_next(hash_index_t *hi);
CORE_DECLARE(sgw_gtpc_t*)   sgw_ctx_gtpc_this(hash_index_t *hi);
CORE_DECLARE(unsigned int)  sgw_ctx_gtpc_count();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_CTX_H__ */

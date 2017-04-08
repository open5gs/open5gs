#ifndef __PGW_CONTEXT_H__
#define __PGW_CONTEXT_H__

#include "core_list.h"
#include "core_errno.h"
#include "core_net.h"
#include "core_hash.h"

#include "gtp_types.h"
#include "gtp_xact.h"

#include "pgw_sm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _pgw_context_t {
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

    c_uint32_t      sess_tunnel_id; /* GTPv2-C F-TEID generator */
    hash_t          *sess_hash;     /* hash table for pgw_sess_t structure */
} pgw_context_t;

typedef struct _pgw_sess_t {
    c_uint32_t      teid;       /* PGW-S5C-F-TEID */
    c_uint32_t      sgw_ipv4;   /* SGW-S11-F-TEID IPv4 Address */
    c_uint32_t      sgw_teid;   /* SGW-S11-F-TEID */
} pgw_sess_t;

CORE_DECLARE(status_t)      pgw_context_init(void);
CORE_DECLARE(status_t)      pgw_context_final(void);

CORE_DECLARE(pgw_context_t*) pgw_self(void);

CORE_DECLARE(pgw_sess_t*)   pgw_sess_add();
CORE_DECLARE(status_t )     pgw_sess_remove(pgw_sess_t *sess);
CORE_DECLARE(status_t )     pgw_sess_remove_all();
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find(c_uint32_t teid);
CORE_DECLARE(hash_index_t*) pgw_sess_first();
CORE_DECLARE(hash_index_t*) pgw_sess_next(hash_index_t *hi);
CORE_DECLARE(pgw_sess_t*)   pgw_sess_this(hash_index_t *hi);
CORE_DECLARE(unsigned int)  pgw_sess_count();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_CONTEXT_H__ */

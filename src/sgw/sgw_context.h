#ifndef __SGW_CONTEXT_H__
#define __SGW_CONTEXT_H__

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

typedef struct _sgw_context_t {
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

    list_t          sess_list;
} sgw_context_t;

typedef struct _sgw_sess_t {
    lnode_t         node;       /**< A node of list_t */
    index_t         index;      /**< An index of this node */

    /* IMPORTANT! 
     * SGW-S11-F-TEID is same with an index */
    c_uint32_t      sgw_s11_teid;       
    c_uint32_t      sgw_s11_addr;       
    c_uint32_t      mme_s11_teid;   /* MME-S11-F-TEID */
    c_uint32_t      mme_s11_addr;   /* MME-S11-F-TEID IPv4 Address */

    /* IMPORTANT! 
     * SGW-S5C-F-TEID is same with an index */
    c_uint32_t      sgw_s5c_teid;       
    c_uint32_t      sgw_s5c_addr;       
    c_uint32_t      pgw_s5c_teid;   /* PGW-S5C-F-TEID */
    c_uint32_t      pgw_s5c_addr;   /* PGW-S5C-F-TEID IPv4 Address */
} sgw_sess_t;

CORE_DECLARE(status_t)      sgw_context_init(void);
CORE_DECLARE(status_t)      sgw_context_final(void);

CORE_DECLARE(sgw_context_t*) sgw_self(void);

CORE_DECLARE(sgw_sess_t*)   sgw_sess_add();
CORE_DECLARE(status_t )     sgw_sess_remove(sgw_sess_t *sess);
CORE_DECLARE(status_t )     sgw_sess_remove_all();
CORE_DECLARE(sgw_sess_t*)   sgw_sess_find(index_t index);
CORE_DECLARE(sgw_sess_t*)   sgw_sess_find_by_teid(c_uint32_t teid);
CORE_DECLARE(sgw_sess_t *)  sgw_sess_first();
CORE_DECLARE(sgw_sess_t *)  sgw_sess_next(sgw_sess_t *sess);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_CONTEXT_H__ */

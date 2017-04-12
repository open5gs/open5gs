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

    c_uint32_t      pdn_id;     /* PDN ID Generator */
    list_t          pdn_list;

    list_t          sess_list;
} pgw_context_t;

typedef struct _pgw_sess_t {
    lnode_t         node;       /**< A node of list_t */
    index_t         index;      /**< An index of this node */

    /* IMPORTANT! 
     * PGW-S5C-F-TEID is same with an index */
    c_uint32_t      teid;       

    c_uint32_t      sgw_addr;   /* SGW-S5C-F-TEID IPv4 Address */
    c_uint32_t      sgw_teid;   /* SGW-S5C-F-TEID */
} pgw_sess_t;

CORE_DECLARE(status_t)      pgw_context_init(void);
CORE_DECLARE(status_t)      pgw_context_final(void);

CORE_DECLARE(pgw_context_t*) pgw_self(void);

CORE_DECLARE(pgw_sess_t*)   pgw_sess_add();
CORE_DECLARE(status_t )     pgw_sess_remove(pgw_sess_t *sess);
CORE_DECLARE(status_t )     pgw_sess_remove_all();
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find(index_t index);
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find_by_teid(c_uint32_t teid);
CORE_DECLARE(pgw_sess_t *)  pgw_sess_first();
CORE_DECLARE(pgw_sess_t *)  pgw_sess_next(pgw_sess_t *sess);

CORE_DECLARE(pdn_t*)        pgw_pdn_add();
CORE_DECLARE(status_t)      pgw_pdn_remove(pdn_t *pdn);
CORE_DECLARE(status_t)      pgw_pdn_remove_all(void);
CORE_DECLARE(pdn_t*)        pgw_pdn_find_by_id(pdn_id_t id);
CORE_DECLARE(pdn_t*)        pgw_pdn_find_by_apn(c_int8_t *apn);
CORE_DECLARE(pdn_t*)        pgw_pdn_first(void);
CORE_DECLARE(pdn_t*)        pgw_pdn_next(pdn_t *pdn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_CONTEXT_H__ */

#ifndef __PGW_CONTEXT_H__
#define __PGW_CONTEXT_H__

#include "core_list.h"
#include "core_errno.h"
#include "core_net.h"
#include "core_hash.h"

#include "gtp_types.h"
#include "gtp_xact.h"

#include "pgw_sm.h"

#define MAX_NUM_OF_IP_POOL 16

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _pgw_context_t {
    c_uint32_t      pgw_addr;  /* PGW local address */

    c_uint32_t      s5c_addr;  /* PGW S5-C local address */
    c_uint32_t      s5c_port;  /* PGW S5-C local port */
    net_sock_t*     s5c_sock;  /* PGW S5-C local listen socket */
    gtp_node_t      s5c_node;  /* PGW S5-C remote GTPv2-C node */

    c_uint32_t      s5u_addr;  /* PGW S5-U local address */
    c_uint32_t      s5u_port;  /* PGW S5-U local port */
    net_sock_t*     s5u_sock;  /* PGW S5-U local listen socket */

    msgq_id         queue_id;       /* Qsesssess for processing PGW control plane */
    tm_service_t    tm_service;     /* Timer Service */
    gtp_xact_ctx_t  gtp_xact_ctx;   /* GTP Transaction Context */

    char            *tun_dev_name;  /* PGW Tunnel device name */
    net_link_t*     tun_link;       /* PGW Tun Interace for U-plane */

    struct {
        c_uint32_t prefix;
        c_uint8_t  mask;
    } ip_pool[MAX_NUM_OF_IP_POOL];
    c_uint8_t       num_of_ip_pool;

    c_uint32_t      primary_dns_addr;
    c_uint32_t      secondary_dns_addr;

    list_t          sess_list;
    list_t          ip_pool_list;
} pgw_context_t;

typedef struct _pgw_ip_pool_t {
    lnode_t         node; /**< A node of list_t */

    c_uint32_t      ue_addr;
} pgw_ip_pool_t;

typedef struct _pgw_sess_t {
    lnode_t         node;       /**< A node of list_t */
    index_t         index;      /**< An index of this node */

    /* IMPORTANT! 
     * PGW-S5C-F-TEID is same with an index */
    c_uint32_t      pgw_s5c_teid;       
    c_uint32_t      pgw_s5c_addr;       

    c_uint32_t      sgw_s5c_teid;
    c_uint32_t      sgw_s5c_addr;

    c_int8_t        apn[MAX_APN_LEN];
    paa_t           paa;
    pgw_ip_pool_t*  ip_pool;

    list_t          bearer_list;
} pgw_sess_t;

typedef struct _pgw_bearer_t {
    lnode_t         node; /**< A node of list_t */
    index_t         index;

    c_uint8_t       id;

    /* IMPORTANT! 
     * PGW-S5U-TEID is same with an index */
    c_uint32_t      pgw_s5u_teid;
    c_uint32_t      pgw_s5u_addr;

    c_uint32_t      sgw_s5u_teid;  
    c_uint32_t      sgw_s5u_addr;

    qos_t           qos;

    pgw_sess_t      *sess;
} pgw_bearer_t;

CORE_DECLARE(status_t)      pgw_context_init(void);
CORE_DECLARE(status_t)      pgw_context_final(void);
CORE_DECLARE(pgw_context_t*) pgw_self(void);

CORE_DECLARE(status_t)      pgw_context_parse_config(void);
CORE_DECLARE(status_t)      pgw_context_setup_trace_module(void);

CORE_DECLARE(pgw_bearer_t*) pgw_sess_add(c_int8_t *apn, c_uint8_t id);
CORE_DECLARE(status_t )     pgw_sess_remove(pgw_sess_t *sess);
CORE_DECLARE(status_t )     pgw_sess_remove_all();
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find(index_t index);
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find_by_teid(c_uint32_t teid);
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find_by_apn(c_int8_t *apn);
CORE_DECLARE(pgw_sess_t *)  pgw_sess_first();
CORE_DECLARE(pgw_sess_t *)  pgw_sess_next(pgw_sess_t *sess);

#if 0
CORE_DECLARE(pdn_t*)        pgw_pdn_add(pgw_sess_t *sess, c_int8_t *apn);
CORE_DECLARE(status_t)      pgw_pdn_remove(pdn_t *pdn);
CORE_DECLARE(status_t)      pgw_pdn_remove_all(pgw_sess_t *sess);
CORE_DECLARE(pdn_t*)        pgw_pdn_find_by_apn(pgw_sess_t *sess, c_int8_t *apn);
CORE_DECLARE(pdn_t*)        pgw_pdn_first(pgw_sess_t *sess);
CORE_DECLARE(pdn_t*)        pgw_pdn_next(pdn_t *pdn);
#endif

CORE_DECLARE(pgw_bearer_t*) pgw_bearer_add(pgw_sess_t *sess, c_uint8_t id);
CORE_DECLARE(status_t)      pgw_bearer_remove(pgw_bearer_t *bearer);
CORE_DECLARE(status_t)      pgw_bearer_remove_all(pgw_sess_t *sess);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_find_by_id(
                                pgw_sess_t *sess, c_uint8_t id);
CORE_DECLARE(pgw_bearer_t*) pgw_default_bearer_in_sess(pgw_sess_t *sess);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_first(pgw_sess_t *sess);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_next(pgw_bearer_t *bearer);

CORE_DECLARE(pgw_bearer_t*) pgw_bearer_find_by_packet(pkbuf_t *pkt);

CORE_DECLARE(status_t )     pgw_ip_pool_generate();
CORE_DECLARE(pgw_ip_pool_t*) pgw_ip_pool_alloc();
CORE_DECLARE(status_t )     pgw_ip_pool_free(pgw_ip_pool_t *ip_pool);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_CONTEXT_H__ */

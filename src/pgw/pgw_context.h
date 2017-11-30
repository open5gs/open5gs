#ifndef __PGW_CONTEXT_H__
#define __PGW_CONTEXT_H__

#include "core_list.h"
#include "core_index.h"
#include "core_errno.h"
#include "core_hash.h"
#include "core_network.h"
#include "core_msgq.h"
#include "core_timer.h"

#include "gtp_types.h"
#include "gtp_message.h"

#define MAX_NUM_OF_UE_NETWORK 16

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _gtp_node_t gtp_node_t;
typedef gtp_node_t pgw_sgw_t;

typedef struct _pgw_context_t {
    c_uint32_t      pgw_addr;  /* PGW local address */

    c_uint32_t      gtpc_addr;  /* PGW GTP-C local address */
    c_uint32_t      gtpc_port;  /* PGW GTP-C local port */
    sock_id         gtpc_sock;  /* PGW GTP-C local listen socket */

    c_uint32_t      gtpu_addr;  /* PGW GTP-U local address */
    c_uint32_t      gtpu_port;  /* PGW GTP-U local port */
    sock_id         gtpu_sock;  /* PGW GTP-U local listen socket */

    const char*     fd_conf_path;   /* PGW freeDiameter conf path */

    msgq_id         queue_id;       /* Qsesssess for processing PGW control plane */
    tm_service_t    tm_service;     /* Timer Service */

    struct {
        sock_id     tun_link;       /* PGW Tun Interace for U-plane */
        const char *if_name;
        struct {
            c_uint32_t addr;
            c_uint8_t  bits;
        } ipv4;
    } ue_network[MAX_NUM_OF_UE_NETWORK];
    c_uint8_t       num_of_ue_network;

    struct {
        c_uint32_t primary;
        c_uint32_t secondary;
    } dns;

    list_t          sgw_list;  /* SGW GTP Node List */
    list_t          ip_pool_list;

    hash_t          *sess_hash; /* hash table (IMSI+APN) */
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

    /* IMSI */
    c_uint8_t       imsi[MAX_IMSI_LEN];
    int             imsi_len;
    c_int8_t        imsi_bcd[MAX_IMSI_BCD_LEN+1];

    /* APN Configuration */
    pdn_t           pdn;
    pgw_ip_pool_t*  ip_pool;

    /* User-Lication-Info */
    tai_t           tai;
    e_cgi_t         e_cgi;

    /* Hash Key : IMSI+APN */
    c_uint8_t       hash_keybuf[MAX_IMSI_LEN+MAX_APN_LEN+1];
    int             hash_keylen;

    list_t          bearer_list;

    /* Related Context */
#define CONNECT_SGW_GTP_NODE(__sESS, __gNODE) \
    do { \
        d_assert((__sESS), return NULL, "Null param"); \
        d_assert((__gNODE), return NULL, "Null param"); \
        (__sESS)->sgw = (__gNODE); \
    } while(0)
    pgw_sgw_t       *sgw;
} pgw_sess_t;

typedef struct _pgw_bearer_t {
    lnode_t         node; /**< A node of list_t */
    index_t         index;

    c_uint8_t       ebi;

    /* IMPORTANT! 
     * PGW-S5U-TEID is same with an index */
    c_uint32_t      pgw_s5u_teid;
    c_uint32_t      pgw_s5u_addr;

    c_uint32_t      sgw_s5u_teid;  
    c_uint32_t      sgw_s5u_addr;

    /* QoS Infomration */
    qos_t           qos;

    /* Packet Filter Identifier Generator(1~15) */
    c_uint8_t       pf_identifier;
    /* Packet Filter List */
    list_t          pf_list;

    pgw_sess_t      *sess;
} pgw_bearer_t;

typedef struct _pgw_rule_t {
    c_uint8_t proto;
    struct {
        struct {
            c_uint32_t addr;
            c_uint32_t mask;
        } local;
        struct {
            c_uint32_t addr;
            c_uint32_t mask;
        } remote;
    } ipv4;
    struct {
        struct {
            c_uint16_t low;
            c_uint16_t high;
        } local;
        struct {
            c_uint16_t low;
            c_uint16_t high;
        } remote;
    } port;
} pgw_rule_t;

typedef struct _pgw_pf_t {
    lnode_t         node;

ED3(c_uint8_t spare:2;,
    c_uint8_t direction:2;,
    c_uint8_t identifier:4;)
    pgw_rule_t      rule;

    pgw_bearer_t    *bearer;
} pgw_pf_t;

CORE_DECLARE(status_t)      pgw_context_init(void);
CORE_DECLARE(status_t)      pgw_context_final(void);
CORE_DECLARE(pgw_context_t*) pgw_self(void);

CORE_DECLARE(status_t)      pgw_context_parse_config(void);
CORE_DECLARE(status_t)      pgw_context_setup_trace_module(void);

CORE_DECLARE(pgw_sgw_t*)    pgw_sgw_add(void);
CORE_DECLARE(status_t)      pgw_sgw_remove(pgw_sgw_t *sgw);
CORE_DECLARE(status_t)      pgw_sgw_remove_all(void);
CORE_DECLARE(pgw_sgw_t*)    pgw_sgw_find(c_uint32_t addr);
CORE_DECLARE(pgw_sgw_t*)    pgw_sgw_first(void);
CORE_DECLARE(pgw_sgw_t*)    pgw_sgw_next(pgw_sgw_t *sgw);

CORE_DECLARE(pgw_sess_t*)   pgw_sess_add(gtp_f_teid_t *sgw_s5c_teid,
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn, c_uint8_t ebi);
CORE_DECLARE(status_t )     pgw_sess_remove(pgw_sess_t *sess);
CORE_DECLARE(status_t )     pgw_sess_remove_all();
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find(index_t index);
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find_by_teid(c_uint32_t teid);
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find_by_imsi_apn(
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn);
CORE_DECLARE(pgw_sess_t *)  pgw_sess_find_or_add_by_message(
        gtp_message_t *gtp_message);
CORE_DECLARE(hash_index_t *)  pgw_sess_first();
CORE_DECLARE(hash_index_t *)  pgw_sess_next(hash_index_t *hi);
CORE_DECLARE(pgw_sess_t *)  pgw_sess_this(hash_index_t *hi);

CORE_DECLARE(pgw_bearer_t*) pgw_bearer_add(pgw_sess_t *sess);
CORE_DECLARE(status_t)      pgw_bearer_remove(pgw_bearer_t *bearer);
CORE_DECLARE(status_t)      pgw_bearer_remove_all(pgw_sess_t *sess);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_find(index_t index);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_find_by_pgw_s5u_teid(
                                c_uint32_t pgw_s5u_teid);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_find_by_ebi(
                                pgw_sess_t *sess, c_uint8_t ebi);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_find_by_qci_arp(pgw_sess_t *sess, 
                                c_uint8_t qci,
                                c_uint8_t priority_level,
                                c_uint8_t pre_emption_capability,
                                c_uint8_t pre_emption_vulnerability);
CORE_DECLARE(pgw_bearer_t*) pgw_default_bearer_in_sess(pgw_sess_t *sess);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_first(pgw_sess_t *sess);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_next(pgw_bearer_t *bearer);

CORE_DECLARE(pgw_bearer_t*) pgw_bearer_find_by_packet(pkbuf_t *pkt);

CORE_DECLARE(pgw_pf_t*)     pgw_pf_add(
                                pgw_bearer_t *bearer, c_uint32_t precedence);
CORE_DECLARE(status_t )     pgw_pf_remove(pgw_pf_t *pf);
CORE_DECLARE(status_t )     pgw_pf_remove_all(pgw_bearer_t *bearer);
CORE_DECLARE(pgw_pf_t*)     pgw_pf_find_by_id(
                                pgw_bearer_t *pgw_bearer, c_uint8_t id);
CORE_DECLARE(pgw_pf_t*)     pgw_pf_first(pgw_bearer_t *bearer);
CORE_DECLARE(pgw_pf_t*)     pgw_pf_next(pgw_pf_t *pf);

CORE_DECLARE(status_t )     pgw_ip_pool_generate();
CORE_DECLARE(pgw_ip_pool_t*) pgw_ip_pool_alloc();
CORE_DECLARE(status_t )     pgw_ip_pool_free(pgw_ip_pool_t *ip_pool);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_CONTEXT_H__ */

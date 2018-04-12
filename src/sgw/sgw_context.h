#ifndef __SGW_CONTEXT_H__
#define __SGW_CONTEXT_H__

#include "core_list.h"
#include "core_index.h"
#include "core_errno.h"
#include "core_event.h"
#include "core_hash.h"
#include "core_network.h"

#include "3gpp_types.h"
#include "gtp/gtp_types.h"
#include "gtp/gtp_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _sgw_tunnel_t sgw_tunnel_t;
typedef struct _gtp_node_t gtp_node_t;

typedef struct _sgw_context_t {
    c_uint32_t      gtpc_port;      /* Default GTPC port */
    c_uint32_t      gtpu_port;      /* Default GTPU port */

    list_t          gtpc_list;      /* SGW GTPC IPv4 Server List */
    list_t          gtpc_list6;     /* SGW GTPC IPv6 Server List */
    sock_id         gtpc_sock;      /* SGW GTPC IPv4 Socket */
    sock_id         gtpc_sock6;     /* SGW GTPC IPv6 Socket */
    c_sockaddr_t    *gtpc_addr;     /* SGW GTPC IPv4 Address */
    c_sockaddr_t    *gtpc_addr6;    /* SGW GTPC IPv6 Address */

    list_t          gtpu_list;      /* SGW GTPU IPv4 Server List */
    list_t          gtpu_list6;     /* SGW GTPU IPv6 Server List */
    sock_id         gtpu_sock;      /* SGW GTPU IPv4 Socket */
    sock_id         gtpu_sock6;     /* SGW GTPU IPv6 Socket */
    c_sockaddr_t    *gtpu_addr;     /* SGW GTPU IPv4 Address */
    c_sockaddr_t    *gtpu_addr6;    /* SGW GTPU IPv6 Address */

    msgq_id         queue_id;  /* Queue for processing SGW control plane */
    tm_service_t    tm_service;/* Timer Service */

    list_t          mme_s11_list;  /* MME GTPC Node List */
    list_t          pgw_s5c_list;  /* PGW GTPC Node List */
    list_t          enb_s1u_list;  /* eNB GTPU Node List */
    list_t          pgw_s5u_list;  /* PGW GTPU Node List */

    hash_t          *imsi_ue_hash;  /* hash table (IMSI : SGW_UE) */
} sgw_context_t;

typedef struct _sgw_ue_t {
    index_t         index;  /* An index of this node */

    c_uint32_t      sgw_s11_teid;   /* SGW-S11-TEID is derived from INDEX */
    c_uint32_t      mme_s11_teid;   /* MME-S11-TEID is received from MME */

    /* UE identity */
    c_uint8_t       imsi[MAX_IMSI_LEN];
    int             imsi_len;
    c_int8_t        imsi_bcd[MAX_IMSI_BCD_LEN+1];

#define SGW_S1U_INACTIVE  0x0001
#define SGW_DL_NOTI_SENT  0x0002

#define SGW_GET_UE_STATE(__uE)  ((__uE)->state)
#define SGW_SET_UE_STATE(__uE,__sTATE)  ((__uE)->state |= (__sTATE))
#define SGW_RESET_UE_STATE(__uE, __sTATE)  ((__uE)->state &= ~(__sTATE))

    c_uint32_t      state;

    list_t          sess_list;

    gtp_node_t       *gnode;
} sgw_ue_t;

typedef struct _sgw_sess_t {
    lnode_t         node;       /* A node of list_t */
    index_t         index;      /* An index of this node */

    /* 
     * SGW-S5C-TEID     = INDEX         | 0x80000000 
     * INDEX            = SGW-S5C-TEID  & ~0x80000000
     */
#define SGW_S5C_TEID(__tEID) (__tEID & 0x80000000)
#define SGW_S5C_TEID_TO_INDEX(__iNDEX) (__iNDEX & ~0x80000000)
#define SGW_S5C_INDEX_TO_TEID(__iNDEX) (__iNDEX | 0x80000000)
    c_uint32_t      sgw_s5c_teid;   /* SGW-S5C-TEID is derived from INDEX */    
    c_uint32_t      pgw_s5c_teid;   /* PGW-S5C-TEID is received from PGW */

    /* APN Configuration */
    pdn_t           pdn;

    list_t          bearer_list;

    /* Related Context */
    gtp_node_t      *gnode;
    sgw_ue_t        *sgw_ue;
} sgw_sess_t;

typedef struct _sgw_bearer_t {
    lnode_t         node; /**< A node of list_t */
    index_t         index;

    c_uint8_t       ebi;

    /* User-Lication-Info */
    tai_t           tai;
    e_cgi_t         e_cgi;

    /* Pkts which will be buffered in case of UE-IDLE */
    c_uint32_t      num_buffered_pkt;

    /* FIXME: The value should be depdendant on the clbuf number. */
#define MAX_NUM_BUFFER_PKT      10 
    pkbuf_t*        buffered_pkts[MAX_NUM_BUFFER_PKT];

    list_t          tunnel_list;
    sgw_sess_t      *sess;
    sgw_ue_t        *sgw_ue;
} sgw_bearer_t;

typedef struct _sgw_tunnel_t {
    lnode_t         node; /**< A node of list_t */
    index_t         index;

    c_uint8_t       interface_type;

    c_uint32_t      local_teid;
    c_uint32_t      remote_teid;

    /* Related Context */
    sgw_bearer_t    *bearer;
    gtp_node_t      *gnode;
} sgw_tunnel_t;

CORE_DECLARE(status_t)      sgw_context_init(void);
CORE_DECLARE(status_t)      sgw_context_final(void);
CORE_DECLARE(sgw_context_t*) sgw_self(void);

CORE_DECLARE(status_t)      sgw_context_parse_config(void);
CORE_DECLARE(status_t)      sgw_context_setup_trace_module(void);

CORE_DECLARE(gtp_node_t *)  sgw_mme_add_by_message(gtp_message_t *message);
CORE_DECLARE(sgw_ue_t *)    sgw_ue_add_by_message(gtp_message_t *message);

CORE_DECLARE(sgw_ue_t*)     sgw_ue_add(c_uint8_t *imsi, int imsi_len);
CORE_DECLARE(status_t)      sgw_ue_remove(sgw_ue_t *sgw_ue);
CORE_DECLARE(status_t)      sgw_ue_remove_all();

CORE_DECLARE(sgw_ue_t*)     sgw_ue_find_by_imsi(c_uint8_t *imsi, int imsi_len);
CORE_DECLARE(sgw_ue_t*)     sgw_ue_find_by_imsi_bcd(c_int8_t *imsi_bcd);
CORE_DECLARE(sgw_ue_t*)     sgw_ue_find_by_teid(c_uint32_t teid);

CORE_DECLARE(hash_index_t *) sgw_ue_first();
CORE_DECLARE(hash_index_t *) sgw_ue_next(hash_index_t *hi);
CORE_DECLARE(sgw_ue_t *)    sgw_ue_this(hash_index_t *hi);

CORE_DECLARE(sgw_sess_t*)   sgw_sess_add(sgw_ue_t *sgw_ue,
                                c_int8_t *apn, c_uint8_t ebi);
CORE_DECLARE(status_t )     sgw_sess_remove(sgw_sess_t *sess);
CORE_DECLARE(status_t )     sgw_sess_remove_all(sgw_ue_t *sgw_ue);
CORE_DECLARE(sgw_sess_t*)   sgw_sess_find_by_apn(
                                sgw_ue_t *sgw_ue, c_int8_t *apn);
CORE_DECLARE(sgw_sess_t*)   sgw_sess_find_by_ebi(
                                sgw_ue_t *sgw_ue, c_uint8_t ebi);
CORE_DECLARE(sgw_sess_t*)   sgw_sess_find_by_teid(c_uint32_t teid);
CORE_DECLARE(sgw_sess_t*)   sgw_sess_first(sgw_ue_t *sgw_ue);
CORE_DECLARE(sgw_sess_t*)   sgw_sess_next(sgw_sess_t *sess);

CORE_DECLARE(sgw_bearer_t*) sgw_bearer_add(sgw_sess_t *sess);
CORE_DECLARE(status_t)      sgw_bearer_remove(sgw_bearer_t *bearer);
CORE_DECLARE(status_t)      sgw_bearer_remove_all(sgw_sess_t *sess);
CORE_DECLARE(sgw_bearer_t*) sgw_bearer_find(index_t index);
CORE_DECLARE(sgw_bearer_t*) sgw_bearer_find_by_sgw_s5u_teid(
                                c_uint32_t sgw_s5u_teid);
CORE_DECLARE(sgw_bearer_t*) sgw_bearer_find_by_sess_ebi(
                                sgw_sess_t *sess, c_uint8_t ebi);
CORE_DECLARE(sgw_bearer_t*) sgw_bearer_find_by_ue_ebi(
                                sgw_ue_t *sgw_ue, c_uint8_t ebi);
CORE_DECLARE(sgw_bearer_t*) sgw_default_bearer_in_sess(sgw_sess_t *sess);
CORE_DECLARE(sgw_bearer_t*) sgw_bearer_first(sgw_sess_t *sess);
CORE_DECLARE(sgw_bearer_t*) sgw_bearer_next(sgw_bearer_t *bearer);
CORE_DECLARE(sgw_bearer_t*) sgw_bearer_find(index_t index);

CORE_DECLARE(sgw_tunnel_t*) sgw_tunnel_add(
        sgw_bearer_t *bearer, c_uint8_t interface_type);
CORE_DECLARE(status_t) sgw_tunnel_remove(sgw_tunnel_t *tunnel);
CORE_DECLARE(status_t) sgw_tunnel_remove_all(sgw_bearer_t *bearer);
CORE_DECLARE(sgw_tunnel_t*) sgw_tunnel_find(index_t index);
CORE_DECLARE(sgw_tunnel_t*) sgw_tunnel_find_by_teid(c_uint32_t teid);
CORE_DECLARE(sgw_tunnel_t*) sgw_tunnel_find_by_interface_type(
        sgw_bearer_t *bearer, c_uint8_t interface_type);
CORE_DECLARE(sgw_tunnel_t*) sgw_s1u_tunnel_in_bearer(sgw_bearer_t *bearer);
CORE_DECLARE(sgw_tunnel_t*) sgw_s5u_tunnel_in_bearer(sgw_bearer_t *bearer);
CORE_DECLARE(sgw_tunnel_t*) sgw_tunnel_first(sgw_bearer_t *bearer);
CORE_DECLARE(sgw_tunnel_t*) sgw_tunnel_next(sgw_tunnel_t *tunnel);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_CONTEXT_H__ */

#ifndef __PGW_CONTEXT_H__
#define __PGW_CONTEXT_H__

#include "core_list.h"
#include "core_index.h"
#include "core_errno.h"
#include "core_hash.h"
#include "core_network.h"
#include "core_msgq.h"
#include "core_timer.h"

#include "gtp/gtp_types.h"
#include "gtp/gtp_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_NUM_OF_DEV          16
#define MAX_NUM_OF_SUBNET       16

typedef struct _gtp_node_t gtp_node_t;
typedef struct _fd_config_t fd_config_t;

typedef struct _pgw_context_t {
    const char*     fd_conf_path;   /* PGW freeDiameter conf path */
    fd_config_t     *fd_config;     /* PGW freeDiameter config */

    c_uint32_t      gtpc_port;      /* Default: PGW GTP-C local port */
    c_uint32_t      gtpu_port;      /* Default: PGW GTP-U local port */
    const char      *tun_ifname;    /* Default:: pgwtun */

    list_t          gtpc_list;      /* PGW GTPC IPv4 Server List */
    list_t          gtpc_list6;     /* PGW GTPC IPv6 Server List */
    sock_id         gtpc_sock;      /* PGW GTPC IPv4 Socket */
    sock_id         gtpc_sock6;     /* PGW GTPC IPv6 Socket */
    c_sockaddr_t    *gtpc_addr;     /* PGW GTPC IPv4 Address */
    c_sockaddr_t    *gtpc_addr6;    /* PGW GTPC IPv6 Address */

    list_t          gtpu_list;      /* PGW GTPU IPv4 Server List */
    list_t          gtpu_list6;     /* PGW GTPU IPv6 Server List */
    sock_id         gtpu_sock;      /* PGW GTPU IPv4 Socket */
    sock_id         gtpu_sock6;     /* PGW GTPU IPv6 Socket */
    c_sockaddr_t    *gtpu_addr;     /* PGW GTPU IPv4 Address */
    c_sockaddr_t    *gtpu_addr6;    /* PGW GTPU IPv6 Address */

    list_t          dev_list;       /* PGW Tun Device List */
    list_t          subnet_list;    /* PGW UE Subnet List */

    msgq_id         queue_id;       /* Qsesssess for processing PGW control plane */
    tm_service_t    tm_service;     /* Timer Service */

#define MAX_NUM_OF_DNS              2
    const char      *dns[MAX_NUM_OF_DNS];
    const char      *dns6[MAX_NUM_OF_DNS];

#define MAX_NUM_OF_P_CSCF           16
    const char      *p_cscf[MAX_NUM_OF_P_CSCF];
    int             num_of_p_cscf;
    int             p_cscf_index;
    const char      *p_cscf6[MAX_NUM_OF_P_CSCF];
    int             num_of_p_cscf6;
    int             p_cscf6_index;

    list_t          sgw_s5c_list;  /* SGW GTPC Node List */
    list_t          sgw_s5u_list;  /* SGW GTPU Node List */
    list_t          ip_pool_list;

    hash_t          *sess_hash; /* hash table (IMSI+APN) */
} pgw_context_t;

typedef struct _pgw_subnet_t pgw_subnet_t;
typedef struct _pgw_ue_ip_t {
    c_uint32_t      addr[4];

    /* Related Context */
    pgw_subnet_t    *subnet;
} pgw_ue_ip_t;

typedef struct _pgw_dev_t {
    lnode_t     node;

    c_int8_t    ifname[IFNAMSIZ];
    sock_id     sock;

    c_sockaddr_t *link_local_addr;
} pgw_dev_t;

typedef struct _pgw_subnet_t {
    lnode_t     node;

    ipsubnet_t  sub;                    /* Subnet : cafe::0/64 */
    ipsubnet_t  gw;                     /* Gateway : cafe::1 */
    c_int8_t    apn[MAX_APN_LEN];       /* APN : "internet", "volte", .. */

    int         family;                 /* AF_INET or AF_INET6 */
    c_uint8_t   prefixlen;              /* prefixlen */

    struct {
        int head, tail;
        int size, avail;
        mutex_id mut;
        pgw_ue_ip_t *free[MAX_POOL_OF_SESS], pool[MAX_POOL_OF_SESS];
    } pool;

    /* Related Context */
    pgw_dev_t   *dev;
} pgw_subnet_t;

typedef struct _pgw_sess_t {
    index_t         index;          /**< An index of this node */

    c_uint32_t      pgw_s5c_teid;   /* PGW-S5C-TEID is derived from INDEX */
    c_uint32_t      sgw_s5c_teid;   /* SGW-S5C-TEID is received from SGW */

    c_int8_t        *gx_sid;        /* Gx Session ID */

    /* IMSI */
    c_uint8_t       imsi[MAX_IMSI_LEN];
    int             imsi_len;
    c_int8_t        imsi_bcd[MAX_IMSI_BCD_LEN+1];

    /* APN Configuration */
    pdn_t           pdn;
    pgw_ue_ip_t*    ipv4;
    pgw_ue_ip_t*    ipv6;

    /* User-Lication-Info */
    tai_t           tai;
    e_cgi_t         e_cgi;

    /* Hash Key : IMSI+APN */
    c_uint8_t       hash_keybuf[MAX_IMSI_LEN+MAX_APN_LEN+1];
    int             hash_keylen;

    list_t          bearer_list;

    /* Related Context */
    gtp_node_t      *gnode;
} pgw_sess_t;

typedef struct _pgw_bearer_t {
    lnode_t         node; /**< A node of list_t */
    index_t         index;

    c_uint8_t       ebi;

    c_uint32_t      pgw_s5u_teid;   /* PGW_S5U is derived from INDEX */
    c_uint32_t      sgw_s5u_teid;   /* SGW_S5U is received from SGW */

    c_int8_t        *name;          /* PCC Rule Name */
    qos_t           qos;            /* QoS Infomration */

    /* Packet Filter Identifier Generator(1~15) */
    c_uint8_t       pf_identifier;
    /* Packet Filter List */
    list_t          pf_list;

    pgw_sess_t      *sess;
    gtp_node_t      *gnode;
} pgw_bearer_t;

typedef struct _pgw_rule_t {
    c_uint8_t proto;
ED5(c_uint8_t ipv4_local:1;,
    c_uint8_t ipv4_remote:1;,
    c_uint8_t ipv6_local:1;,
    c_uint8_t ipv6_remote:1;,
    c_uint8_t reserved:4;)
    struct {
        struct {
            c_uint32_t addr[4];
            c_uint32_t mask[4];
        } local;
        struct {
            c_uint32_t addr[4];
            c_uint32_t mask[4];
        } remote;
    } ip;
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

CORE_DECLARE(gtp_node_t *)  pgw_sgw_add_by_message(gtp_message_t *message);
CORE_DECLARE(pgw_sess_t *)  pgw_sess_add_by_message(gtp_message_t *message);

CORE_DECLARE(pgw_sess_t*)   pgw_sess_add(
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn,
        c_uint8_t pdn_type, c_uint8_t ebi);
CORE_DECLARE(status_t )     pgw_sess_remove(pgw_sess_t *sess);
CORE_DECLARE(status_t )     pgw_sess_remove_all();
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find(index_t index);
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find_by_teid(c_uint32_t teid);
CORE_DECLARE(pgw_sess_t*)   pgw_sess_find_by_imsi_apn(
        c_uint8_t *imsi, int imsi_len, c_int8_t *apn);
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
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_find_by_name(
                                pgw_sess_t *sess, c_int8_t *name);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_find_by_qci_arp(pgw_sess_t *sess, 
                                c_uint8_t qci,
                                c_uint8_t priority_level,
                                c_uint8_t pre_emption_capability,
                                c_uint8_t pre_emption_vulnerability);
CORE_DECLARE(pgw_bearer_t*) pgw_default_bearer_in_sess(pgw_sess_t *sess);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_first(pgw_sess_t *sess);
CORE_DECLARE(pgw_bearer_t*) pgw_bearer_next(pgw_bearer_t *bearer);

CORE_DECLARE(pgw_pf_t*)     pgw_pf_add(
                                pgw_bearer_t *bearer, c_uint32_t precedence);
CORE_DECLARE(status_t )     pgw_pf_remove(pgw_pf_t *pf);
CORE_DECLARE(status_t )     pgw_pf_remove_all(pgw_bearer_t *bearer);
CORE_DECLARE(pgw_pf_t*)     pgw_pf_find_by_id(
                                pgw_bearer_t *pgw_bearer, c_uint8_t id);
CORE_DECLARE(pgw_pf_t*)     pgw_pf_first(pgw_bearer_t *bearer);
CORE_DECLARE(pgw_pf_t*)     pgw_pf_next(pgw_pf_t *pf);

CORE_DECLARE(status_t )     pgw_ue_pool_generate();
CORE_DECLARE(pgw_ue_ip_t *) pgw_ue_ip_alloc(int family, const char *apn);
CORE_DECLARE(status_t)      pgw_ue_ip_free(pgw_ue_ip_t *ip);

CORE_DECLARE(pgw_dev_t*)    pgw_dev_add(const char *ifname);
CORE_DECLARE(status_t )     pgw_dev_remove(pgw_dev_t *dev);
CORE_DECLARE(status_t )     pgw_dev_remove_all();
CORE_DECLARE(pgw_dev_t*)    pgw_dev_find_by_ifname(const char *ifname);
CORE_DECLARE(pgw_dev_t*)    pgw_dev_first();
CORE_DECLARE(pgw_dev_t*)    pgw_dev_next(pgw_dev_t *dev);

CORE_DECLARE(pgw_subnet_t*) pgw_subnet_add(
        const char *ipstr, const char *mask_or_numbits,
        const char *apn, const char *ifname);
CORE_DECLARE(status_t )     pgw_subnet_remove(pgw_subnet_t *subnet);
CORE_DECLARE(status_t )     pgw_subnet_remove_all();
CORE_DECLARE(pgw_subnet_t*) pgw_subnet_first();
CORE_DECLARE(pgw_subnet_t*) pgw_subnet_next(pgw_subnet_t *subnet);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_CONTEXT_H__ */

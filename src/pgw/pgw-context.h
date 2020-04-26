/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PGW_CONTEXT_H
#define PGW_CONTEXT_H

#include "pgw-config.h"

#if HAVE_NET_IF_H
#include <net/if.h>
#endif

#include "ogs-gtp.h"
#include "ogs-diameter-gx.h"
#include "ogs-app.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUM_OF_DEV          16
#define MAX_NUM_OF_SUBNET       16

extern int __pgw_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __pgw_log_domain

typedef struct pgw_context_s {
    const char*         diam_conf_path;   /* PGW Diameter conf path */
    ogs_diam_config_t   *diam_config;     /* PGW Diameter config */

    uint32_t        gtpc_port;      /* Default: PGW GTP-C local port */
    uint32_t        gtpu_port;      /* Default: PGW GTP-U local port */
    const char      *tun_ifname;    /* Default: ogstun */

    ogs_list_t      gtpc_list;      /* PGW GTPC IPv4 Server List */
    ogs_list_t      gtpc_list6;     /* PGW GTPC IPv6 Server List */
    ogs_sock_t      *gtpc_sock;     /* PGW GTPC IPv4 Socket */
    ogs_sock_t      *gtpc_sock6;    /* PGW GTPC IPv6 Socket */
    ogs_sockaddr_t  *gtpc_addr;     /* PGW GTPC IPv4 Address */
    ogs_sockaddr_t  *gtpc_addr6;    /* PGW GTPC IPv6 Address */

    ogs_list_t      gtpu_list;      /* PGW GTPU IPv4 Server List */
    ogs_list_t      gtpu_list6;     /* PGW GTPU IPv6 Server List */
    ogs_sock_t      *gtpu_sock;     /* PGW GTPU IPv4 Socket */
    ogs_sock_t      *gtpu_sock6;    /* PGW GTPU IPv6 Socket */
    ogs_sockaddr_t  *gtpu_addr;     /* PGW GTPU IPv4 Address */
    ogs_sockaddr_t  *gtpu_addr6;    /* PGW GTPU IPv6 Address */

    ogs_list_t      dev_list;       /* PGW Tun Device List */
    ogs_list_t      subnet_list;    /* PGW UE Subnet List */

    ogs_queue_t     *queue;         /* Queue for processing PGW control */
    ogs_timer_mgr_t *timer_mgr;     /* Timer Manager */
    ogs_pollset_t   *pollset;       /* Poll Set for I/O Multiplexing */

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

    ogs_list_t      sgw_s5c_list;   /* SGW GTPC Node List */
    ogs_list_t      sgw_s5u_list;   /* SGW GTPU Node List */
    ogs_list_t      ip_pool_list;

    ogs_hash_t      *sess_hash;     /* hash table (IMSI+APN) */
    ogs_hash_t      *ipv4_hash;     /* hash table (IPv4 Address) */
    ogs_hash_t      *ipv6_hash;     /* hash table (IPv6 Address) */

    uint16_t        mtu;            /* MTU to advertise in PCO */

    ogs_list_t      sess_list;
} pgw_context_t;

typedef struct pgw_subnet_s pgw_subnet_t;
typedef struct pgw_ue_ip_s {
    uint32_t        addr[4];
    bool            static_ip;

    /* Related Context */
    pgw_subnet_t    *subnet;
} pgw_ue_ip_t;

typedef struct pgw_dev_s {
    ogs_lnode_t     lnode;

    char            ifname[IFNAMSIZ];
    ogs_socket_t    fd;

    ogs_sockaddr_t  *link_local_addr;
    ogs_poll_t      *poll;
} pgw_dev_t;

typedef struct pgw_subnet_s {
    ogs_lnode_t     node;

    ogs_ipsubnet_t  sub;                /* Subnet : cafe::0/64 */
    ogs_ipsubnet_t  gw;                 /* Gateway : cafe::1 */
    char            apn[OGS_MAX_APN_LEN];   /* APN : "internet", "volte", .. */

#define MAX_NUM_OF_SUBNET_RANGE         16
    struct {
        const char *low;
        const char *high;
    } range[MAX_NUM_OF_SUBNET_RANGE];
    int num_of_range;

    int             family;         /* AF_INET or AF_INET6 */
    uint8_t         prefixlen;      /* prefixlen */
    OGS_POOL(pool, pgw_ue_ip_t);

    pgw_dev_t       *dev;           /* Related Context */
} pgw_subnet_t;

typedef struct pgw_sess_s {
    ogs_lnode_t     lnode;
    uint32_t        index;          /**< An index of this node */

    uint32_t        pgw_s5c_teid;   /* PGW-S5C-TEID is derived from INDEX */
    uint32_t        sgw_s5c_teid;   /* SGW-S5C-TEID is received from SGW */

    char            *gx_sid;        /* Gx Session ID */

    /* IMSI */
    uint8_t         imsi[OGS_MAX_IMSI_LEN];
    int             imsi_len;
    char            imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    /* APN Configuration */
    ogs_pdn_t       pdn;
    pgw_ue_ip_t*    ipv4;
    pgw_ue_ip_t*    ipv6;

    uint8_t         hash_keybuf[OGS_MAX_IMSI_LEN+OGS_MAX_APN_LEN+1];
    int             hash_keylen;

    ogs_tlv_octet_t ue_pco; /* Save Protocol Configuration Options from UE */
    ogs_tlv_octet_t user_location_information; /* User Location Information */
    ogs_tlv_octet_t ue_timezone; /* UE Timezone */

    ogs_list_t      bearer_list;

    /* Related Context */
    ogs_gtp_node_t  *gnode;
} pgw_sess_t;

typedef struct pgw_bearer_s {
    ogs_lnode_t     lnode; /**< A node of list_t */
    uint32_t        index;

    uint8_t         ebi;

    uint32_t        pgw_s5u_teid;   /* PGW_S5U is derived from INDEX */
    uint32_t        sgw_s5u_teid;   /* SGW_S5U is received from SGW */

    char            *name;          /* PCC Rule Name */
    ogs_qos_t       qos;            /* QoS Infomration */

    /* Packet Filter Identifier Generator(1~15) */
    uint8_t         pf_identifier;
    /* Packet Filter List */
    ogs_list_t      pf_list;

    pgw_sess_t      *sess;
    ogs_gtp_node_t  *gnode;
} pgw_bearer_t;

typedef struct pgw_rule_s {
    uint8_t proto;
ED5(uint8_t ipv4_local:1;,
    uint8_t ipv4_remote:1;,
    uint8_t ipv6_local:1;,
    uint8_t ipv6_remote:1;,
    uint8_t reserved:4;)
    struct {
        struct {
            uint32_t addr[4];
            uint32_t mask[4];
        } local;
        struct {
            uint32_t addr[4];
            uint32_t mask[4];
        } remote;
    } ip;
    struct {
        struct {
            uint16_t low;
            uint16_t high;
        } local;
        struct {
            uint16_t low;
            uint16_t high;
        } remote;
    } port;
} pgw_rule_t;

typedef struct pgw_pf_s {
    ogs_lnode_t     lnode;

ED3(uint8_t spare:2;,
    uint8_t direction:2;,
    uint8_t identifier:4;)
    pgw_rule_t      rule;

    pgw_bearer_t    *bearer;
} pgw_pf_t;

void pgw_context_init(void);
void pgw_context_final(void);
pgw_context_t *pgw_self(void);

int pgw_context_parse_config(void);

pgw_sess_t *pgw_sess_add_by_message(ogs_gtp_message_t *message);

pgw_sess_t *pgw_sess_add(
        uint8_t *imsi, int imsi_len, char *apn,
        uint8_t pdn_type, uint8_t ebi, ogs_paa_t *addr);

int pgw_sess_remove(pgw_sess_t *sess);
void pgw_sess_remove_all(void);
pgw_sess_t *pgw_sess_find(uint32_t index);
pgw_sess_t *pgw_sess_find_by_teid(uint32_t teid);
pgw_sess_t *pgw_sess_find_by_imsi_apn(uint8_t *imsi, int imsi_len, char *apn);
pgw_sess_t *pgw_sess_find_by_ipv4(uint32_t addr);
pgw_sess_t *pgw_sess_find_by_ipv6(uint32_t *addr6);

pgw_bearer_t *pgw_bearer_add(pgw_sess_t *sess);
int pgw_bearer_remove(pgw_bearer_t *bearer);
void pgw_bearer_remove_all(pgw_sess_t *sess);
pgw_bearer_t *pgw_bearer_find(uint32_t index);
pgw_bearer_t *pgw_bearer_find_by_pgw_s5u_teid(uint32_t pgw_s5u_teid);
pgw_bearer_t *pgw_bearer_find_by_ebi(pgw_sess_t *sess, uint8_t ebi);
pgw_bearer_t *pgw_bearer_find_by_name(pgw_sess_t *sess, char *name);
pgw_bearer_t *pgw_bearer_find_by_qci_arp(pgw_sess_t *sess, 
                                uint8_t qci,
                                uint8_t priority_level,
                                uint8_t pre_emption_capability,
                                uint8_t pre_emption_vulnerability);
pgw_bearer_t *pgw_default_bearer_in_sess(pgw_sess_t *sess);
pgw_bearer_t *pgw_bearer_first(pgw_sess_t *sess);
pgw_bearer_t *pgw_bearer_next(pgw_bearer_t *bearer);

pgw_pf_t *pgw_pf_add(pgw_bearer_t *bearer, uint32_t precedence);
int pgw_pf_remove(pgw_pf_t *pf);
void pgw_pf_remove_all(pgw_bearer_t *bearer);
pgw_pf_t *pgw_pf_find_by_id(pgw_bearer_t *pgw_bearer, uint8_t id);
pgw_pf_t *pgw_pf_first(pgw_bearer_t *bearer);
pgw_pf_t *pgw_pf_next(pgw_pf_t *pf);

int pgw_ue_pool_generate(void);
pgw_ue_ip_t *pgw_ue_ip_alloc(int family, const char *apn, uint8_t *addr);
int pgw_ue_ip_free(pgw_ue_ip_t *ip);

pgw_dev_t *pgw_dev_add(const char *ifname);
int pgw_dev_remove(pgw_dev_t *dev);
void pgw_dev_remove_all(void);
pgw_dev_t *pgw_dev_find_by_ifname(const char *ifname);
pgw_dev_t *pgw_dev_first(void);
pgw_dev_t *pgw_dev_next(pgw_dev_t *dev);

pgw_subnet_t *pgw_subnet_add(
        const char *ipstr, const char *mask_or_numbits,
        const char *apn, const char *ifname);
pgw_subnet_t *pgw_subnet_next(pgw_subnet_t *subnet);
int pgw_subnet_remove(pgw_subnet_t *subnet);
void pgw_subnet_remove_all(void);
pgw_subnet_t *pgw_subnet_first(void);

void stats_add_session(void);
void stats_remove_session(void);

#ifdef __cplusplus
}
#endif

#endif /* PGW_CONTEXT_H */

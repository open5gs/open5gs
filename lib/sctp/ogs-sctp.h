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

#ifndef OGS_SCTP_H
#define OGS_SCTP_H

#include "app/ogs-app.h"

#include "sctp/sctp-config.h"

#define OGS_SCTP_INSIDE

/* Nothing */

#undef OGS_SCTP_INSIDE

#ifdef __cplusplus
extern "C" {
#endif

extern int __ogs_sctp_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sctp_domain

#define OGS_S1AP_SCTP_PORT              36412
#define OGS_SGSAP_SCTP_PORT             29118
#define OGS_NGAP_SCTP_PORT              38412

#define OGS_SCTP_S1AP_PPID              18
#define OGS_SCTP_X2AP_PPID              27
#define OGS_SCTP_SGSAP_PPID             0
#define OGS_SCTP_NGAP_PPID              60

#define ogs_sctp_ppid_in_pkbuf(__pkBUF)         (__pkBUF)->param[0]
#define ogs_sctp_stream_no_in_pkbuf(__pkBUF)    (__pkBUF)->param[1]

#if HAVE_USRSCTP

#undef MSG_NOTIFICATION
#define MSG_NOTIFICATION 0x2000

#ifndef INET
#define INET            1
#endif
#ifndef INET6
#define INET6           1
#endif

#include <usrsctp.h>

ogs_sockaddr_t *ogs_usrsctp_remote_addr(union sctp_sockstore *store);
void ogs_sctp_destroy(ogs_sock_t *sock);
ogs_sock_t *ogs_sctp_accept(ogs_sock_t *sock);

#else

#if HAVE_NETINET_SCTP_H
#include <netinet/sctp.h>
#endif

#define ogs_sctp_destroy ogs_sock_destroy
#define ogs_sctp_accept ogs_sock_accept

#endif

typedef struct ogs_sctp_sock_s {
    int             type;           /* SOCK_STREAM or SOCK_SEQPACKET */

    ogs_sock_t      *sock;          /* Socket */
    ogs_sockaddr_t  *addr;          /* Address */

    struct {
        ogs_poll_t  *read;          /* Read Poll */
        ogs_poll_t  *write;         /* Write Poll */
    } poll;

    ogs_list_t      write_queue;    /* Write Queue for Sending S1AP message */
} ogs_sctp_sock_t;

typedef struct ogs_sctp_info_s {
    uint32_t ppid;
    uint16_t stream_no;
    uint16_t inbound_streams;
    uint16_t outbound_streams;
} ogs_sctp_info_t;

void ogs_sctp_init(uint16_t port);
void ogs_sctp_final(void);

ogs_sock_t *ogs_sctp_socket(int family, int type);

ogs_sock_t *ogs_sctp_server(
        int type, ogs_sockaddr_t *sa_list, ogs_sockopt_t *socket_option);
ogs_sock_t *ogs_sctp_client(
        int type,
        ogs_sockaddr_t *sa_list, ogs_sockaddr_t *local_sa_list,
        ogs_sockopt_t *socket_option);

int ogs_sctp_bind(ogs_sock_t *sock, ogs_sockaddr_t *sa_list);
int ogs_sctp_connect(ogs_sock_t *sock, ogs_sockaddr_t *sa_list);
int ogs_sctp_listen(ogs_sock_t *sock);

int ogs_sctp_peer_addr_params(ogs_sock_t *sock, ogs_sockopt_t *option);
int ogs_sctp_rto_info(ogs_sock_t *sock, ogs_sockopt_t *option);
int ogs_sctp_initmsg(ogs_sock_t *sock, ogs_sockopt_t *option);
int ogs_sctp_nodelay(ogs_sock_t *sock, int on);
int ogs_sctp_so_linger(ogs_sock_t *sock, int l_linger);

int ogs_sctp_sendmsg(ogs_sock_t *sock, const void *msg, size_t len,
        ogs_sockaddr_t *to, uint32_t ppid, uint16_t stream_no);
int ogs_sctp_recvmsg(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo, int *msg_flags);
int ogs_sctp_recvdata(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo);

int ogs_sctp_senddata(ogs_sock_t *sock,
        ogs_pkbuf_t *pkbuf, ogs_sockaddr_t *addr);
void ogs_sctp_write_to_buffer(ogs_sctp_sock_t *sctp, ogs_pkbuf_t *pkbuf);
void ogs_sctp_flush_and_destroy(ogs_sctp_sock_t *sctp);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SCTP_H */

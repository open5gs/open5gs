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

#include "base/base.h"

#if HAVE_NETINET_SCTP_H
#include <netinet/sctp.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sctp_info_s {
    uint32_t ppid;
    uint16_t stream_no;
    uint16_t inbound_streams;
    uint16_t outbound_streams;
} ogs_sctp_info_t;

void ogs_sctp_set_num_ostreams(int sctp_streams);

ogs_sock_t *ogs_sctp_socket(int family, int type, ogs_socknode_t *node);
ogs_sock_t *ogs_sctp_server(int type, ogs_socknode_t *node);
ogs_sock_t *ogs_sctp_client(int type, ogs_socknode_t *node);
int ogs_sctp_connect(ogs_sock_t *sock, ogs_sockaddr_t *sa_list);
int ogs_sctp_sendmsg(ogs_sock_t *sock, const void *msg, size_t len,
        ogs_sockaddr_t *to, uint32_t ppid, uint16_t stream_no);
int ogs_sctp_recvmsg(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo, int *msg_flags);
int ogs_sctp_recvdata(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SCTP_H */

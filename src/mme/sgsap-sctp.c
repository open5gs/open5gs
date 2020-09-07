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

#include "ogs-sctp.h"
#include "sgsap-path.h"

#include "mme-context.h"
#include "mme-event.h"
#include "s1ap-path.h"

#if HAVE_USRSCTP
static void usrsctp_recv_handler(struct socket *socket, void *data, int flags);
#else
static void lksctp_recv_handler(short when, ogs_socket_t fd, void *data);
#endif

static void recv_handler(ogs_sock_t *sock);

ogs_sock_t *sgsap_client(mme_vlr_t *vlr)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_socknode_t node;
    ogs_sock_t *sock = NULL;

    ogs_assert(vlr);

    memset(&node, 0, sizeof node);
    node.addr = vlr->sa_list;

    ogs_socknode_sctp_option(&node, &ogs_app()->sockopt);
    ogs_socknode_nodelay(&node, true);
#if 0 /* Try to remove LINGER in usrsctp */
#if HAVE_USRSCTP
    ogs_socknode_linger(&node, true, 0);
#endif
#endif

    sock = ogs_sctp_client(SOCK_SEQPACKET, &node);
    if (sock) {
        vlr->sock = sock;
#if HAVE_USRSCTP
        vlr->addr = node.addr;
        usrsctp_set_non_blocking((struct socket *)sock, 1);
        usrsctp_set_upcall((struct socket *)sock, usrsctp_recv_handler, NULL);
#else
        vlr->addr = &sock->remote_addr;
        vlr->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, lksctp_recv_handler, sock);
#endif
        ogs_info("sgsap client() [%s]:%d",
                OGS_ADDR(vlr->addr, buf), OGS_PORT(vlr->addr));
    }

    return sock;
}

#if HAVE_USRSCTP
static void usrsctp_recv_handler(struct socket *socket, void *data, int flags)
{
	int events;

	while ((events = usrsctp_get_events(socket)) &&
           (events & SCTP_EVENT_READ)) {
        recv_handler((ogs_sock_t *)socket);
    }
}
#else
static void lksctp_recv_handler(short when, ogs_socket_t fd, void *data)
{
    ogs_sock_t *sock = NULL;

    sock = data;
    ogs_assert(fd != INVALID_SOCKET);
    ogs_assert(sock);
    
    recv_handler(sock);
}
#endif

static void recv_handler(ogs_sock_t *sock)
{
    ogs_pkbuf_t *pkbuf;
    int size;
    ogs_sockaddr_t *addr = NULL;
    ogs_sockaddr_t from;
    ogs_sctp_info_t sinfo;
    int flags = 0;

    ogs_assert(sock);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);
    size = ogs_sctp_recvmsg(
            sock, pkbuf->data, pkbuf->len, &from, &sinfo, &flags);
    if (size < 0) {
        ogs_error("ogs_sctp_recvmsg(%d) failed(%d:%s)",
                size, errno, strerror(errno));
        ogs_pkbuf_free(pkbuf);
        return;
    }

    if (flags & MSG_NOTIFICATION) {
        union sctp_notification *not =
            (union sctp_notification *)pkbuf->data;

        switch(not->sn_header.sn_type) {
        case SCTP_ASSOC_CHANGE :
            ogs_debug("SCTP_ASSOC_CHANGE:"
                    "[T:%d, F:0x%x, S:%d, I/O:%d/%d]", 
                    not->sn_assoc_change.sac_type,
                    not->sn_assoc_change.sac_flags,
                    not->sn_assoc_change.sac_state,
                    not->sn_assoc_change.sac_inbound_streams,
                    not->sn_assoc_change.sac_outbound_streams);

            if (not->sn_assoc_change.sac_state == SCTP_COMM_UP) {
                ogs_debug("SCTP_COMM_UP");

                addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
                ogs_assert(addr);
                memcpy(addr, &from, sizeof(ogs_sockaddr_t));

                sgsap_event_push(MME_EVT_SGSAP_LO_SCTP_COMM_UP,
                        sock, addr, NULL,
                        not->sn_assoc_change.sac_inbound_streams,
                        not->sn_assoc_change.sac_outbound_streams);
            } else if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP ||
                    not->sn_assoc_change.sac_state == SCTP_COMM_LOST) {

                if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP)
                    ogs_debug("SCTP_SHUTDOWN_COMP");
                if (not->sn_assoc_change.sac_state == SCTP_COMM_LOST)
                    ogs_debug("SCTP_COMM_LOST");

                addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
                ogs_assert(addr);
                memcpy(addr, &from, sizeof(ogs_sockaddr_t));

                sgsap_event_push(MME_EVT_SGSAP_LO_CONNREFUSED,
                        sock, addr, NULL, 0, 0);
            }
            break;
        case SCTP_SHUTDOWN_EVENT :
        case SCTP_SEND_FAILED :
            if (not->sn_header.sn_type == SCTP_SHUTDOWN_EVENT)
                ogs_debug("SCTP_SHUTDOWN_EVENT:[T:%d, F:0x%x, L:%d]", 
                        not->sn_shutdown_event.sse_type,
                        not->sn_shutdown_event.sse_flags,
                        not->sn_shutdown_event.sse_length);
            if (not->sn_header.sn_type == SCTP_SEND_FAILED)
#if HAVE_USRSCTP
                ogs_error("SCTP_SEND_FAILED:[T:%d, F:0x%x, S:%d]", 
                        not->sn_send_failed_event.ssfe_type,
                        not->sn_send_failed_event.ssfe_flags,
                        not->sn_send_failed_event.ssfe_error);
#else
                ogs_error("SCTP_SEND_FAILED:[T:%d, F:0x%x, S:%d]", 
                        not->sn_send_failed.ssf_type,
                        not->sn_send_failed.ssf_flags,
                        not->sn_send_failed.ssf_error);
#endif

            addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
            ogs_assert(addr);
            memcpy(addr, &from, sizeof(ogs_sockaddr_t));

            sgsap_event_push(MME_EVT_SGSAP_LO_CONNREFUSED,
                    sock, addr, NULL, 0, 0);
            break;
        case SCTP_PEER_ADDR_CHANGE:
            ogs_warn("SCTP_PEER_ADDR_CHANGE:[T:%d, F:0x%x, S:%d]", 
                    not->sn_paddr_change.spc_type,
                    not->sn_paddr_change.spc_flags,
                    not->sn_paddr_change.spc_error);
            break;
        case SCTP_REMOTE_ERROR:
            ogs_warn("SCTP_REMOTE_ERROR:[T:%d, F:0x%x, S:%d]", 
                    not->sn_remote_error.sre_type,
                    not->sn_remote_error.sre_flags,
                    not->sn_remote_error.sre_error);
            break;
        default :
            ogs_error("Discarding event with unknown flags:0x%x type:0x%x",
                    flags, not->sn_header.sn_type);
            break;
        }
    } else if (flags & MSG_EOR) {
        ogs_pkbuf_trim(pkbuf, size);

        addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        ogs_assert(addr);
        memcpy(addr, &from, sizeof(ogs_sockaddr_t));

        sgsap_event_push(MME_EVT_SGSAP_MESSAGE, sock, addr, pkbuf, 0, 0);
        return;
    } else {
        ogs_assert_if_reached();
    }
    ogs_pkbuf_free(pkbuf);
}

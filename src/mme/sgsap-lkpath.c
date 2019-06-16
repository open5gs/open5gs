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

#include "app/context.h"
#include "mme-event.h"
#include "s1ap-path.h"

static void sgsap_recv_handler(short when, ogs_socket_t fd, void *data);

ogs_sock_t *sgsap_client(ogs_socknode_t *node)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *sock = NULL;

    ogs_assert(node);

    ogs_socknode_sctp_option(node, &context_self()->config.sockopt);
    ogs_socknode_nodelay(node, true);
    ogs_socknode_set_poll(node, mme_self()->pollset,
            OGS_POLLIN, sgsap_recv_handler, node);

    sock = ogs_sctp_client(SOCK_STREAM, node);
    if (sock)
        ogs_info("sgsap client() [%s]:%d",
                OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));

    return sock;
}

static void sgsap_recv_handler(short when, ogs_socket_t fd, void *data)
{
#if 0
    int rv;
    ogs_pkbuf_t *pkbuf;
    int size;
    mme_event_t *e = NULL;
    ogs_sock_t *sock = data;
    ogs_sockaddr_t *addr = NULL;
    ogs_sctp_info_t sinfo;
    int flags = 0;

    ogs_assert(sock);
    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
#if DEPRECATED
    if (pkbuf == NULL) {
        char tmp_buf[MAX_SDU_LEN];

        d_fatal("Can't allocate pkbuf");

        /* Read data from socket to exit from select */
        ogs_recv(fd, tmp_buf, MAX_SDU_LEN, 0);

        return;
    }
#endif
    ogs_pkbuf_put(pkbuf, MAX_SDU_LEN);
    size = ogs_sctp_recvmsg(
            sock, pkbuf->data, pkbuf->len, NULL, &sinfo, &flags);
    if (size < 0) {
        ogs_error("ogs_sctp_recvmsg(%d) failed(%d:%s)",
                size, errno, strerror(errno));
        return;
    }

    if (flags & MSG_NOTIFICATION) {
        union sctp_notification *not =
            (union sctp_notification *)pkbuf->data;

        switch(not->sn_header.sn_type) {
        case SCTP_ASSOC_CHANGE :
        {
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
                memcpy(addr, &sock->remote_addr, sizeof(ogs_sockaddr_t));

                e = mme_event_new(MME_EVT_S1AP_LO_SCTP_COMM_UP);
                ogs_assert(e);
                e->vlr_sock = sock;
                e->vlr_addr = addr;
                e->max_num_of_istreams = 
                    not->sn_assoc_change.sac_inbound_streams;
                e->max_num_of_ostreams = 
                    not->sn_assoc_change.sac_outbound_streams;
                rv = ogs_queue_push(mme_self()->queue, e);
                if (rv != OGS_OK) {
                    ogs_warn("ogs_queue_push() failed:%d", (int)rv);
                    ogs_free(e->vlr_addr);
                    mme_event_free(e);
                }
            } else if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP ||
                    not->sn_assoc_change.sac_state == SCTP_COMM_LOST) {

                if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP)
                    ogs_debug("SCTP_SHUTDOWN_COMP");
                if (not->sn_assoc_change.sac_state == SCTP_COMM_LOST)
                    ogs_debug("SCTP_COMM_LOST");

                addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
                ogs_assert(addr);
                memcpy(addr, &sock->remote_addr, sizeof(ogs_sockaddr_t));

                e = mme_event_new(MME_EVT_S1AP_LO_CONNREFUSED);
                ogs_assert(e);
                e->vlr_sock = sock;
                e->vlr_addr = addr;
                rv = ogs_queue_push(mme_self()->queue, e);
                if (rv != OGS_OK) {
                    ogs_warn("ogs_queue_push() failed:%d", (int)rv);
                    ogs_free(e->vlr_addr);
                    mme_event_free(e);
                }
            }
            break;
        }
        case SCTP_SHUTDOWN_EVENT :
        {
            ogs_debug("SCTP_SHUTDOWN_EVENT:[T:%d, F:0x%x, L:%d]", 
                    not->sn_shutdown_event.sse_type,
                    not->sn_shutdown_event.sse_flags,
                    not->sn_shutdown_event.sse_length);

            addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
            ogs_assert(addr);
            memcpy(addr, &sock->remote_addr, sizeof(ogs_sockaddr_t));

            e = mme_event_new(MME_EVT_S1AP_LO_CONNREFUSED);
            ogs_assert(e);
            e->vlr_sock = sock;
            e->vlr_addr = addr;
            rv = ogs_queue_push(mme_self()->queue, e);
            if (rv != OGS_OK) {
                ogs_warn("ogs_queue_push() failed:%d", (int)rv);
                ogs_free(e->vlr_addr);
                mme_event_free(e);
            }
            break;
        }
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
        case SCTP_SEND_FAILED :
            ogs_error("SCTP_SEND_FAILED:[T:%d, F:0x%x, S:%d]", 
                    not->sn_send_failed.ssf_type,
                    not->sn_send_failed.ssf_flags,
                    not->sn_send_failed.ssf_error);
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
        memcpy(addr, &sock->remote_addr, sizeof(ogs_sockaddr_t));

        e = mme_event_new(MME_EVT_S1AP_MESSAGE);
        ogs_assert(e);
        e->vlr_sock = sock;
        e->vlr_addr = addr;
        e->pkbuf = pkbuf;
        rv = ogs_queue_push(mme_self()->queue, e);
        if (rv != OGS_OK) {
            ogs_warn("ogs_queue_push() failed:%d", (int)rv);
            ogs_free(e->vlr_addr);
            ogs_pkbuf_free(e->pkbuf);
            mme_event_free(e);
        }

        return;
    } else {
        ogs_assert_if_reached();
    }
    ogs_pkbuf_free(pkbuf);
#endif
}

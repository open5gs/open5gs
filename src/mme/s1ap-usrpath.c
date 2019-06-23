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

static int usrsctp_recv_handler(struct socket *sock,
        union sctp_sockstore addr, void *data, size_t datalen,
        struct sctp_rcvinfo rcv, int flags, void *ulp_info);

ogs_sock_t *s1ap_server(ogs_socknode_t *node)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *sock = NULL;

    ogs_assert(node);

    ogs_socknode_sctp_option(node, &context_self()->config.sockopt);
    ogs_socknode_nodelay(node, true);
    ogs_socknode_set_poll(node, mme_self()->pollset,
            OGS_POLLIN, usrsctp_recv_handler, node);

    /* FIXME : libsctp 0.9.3.0 is not properly working in SOCK_STREAM */
    sock = ogs_sctp_server(SOCK_SEQPACKET, node);
    ogs_assert(sock);

    ogs_info("s1ap_server() [%s]:%d",
            OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));

    return sock;
}

void s1ap_recv_handler(short when, ogs_socket_t fd, void *data)
{
    /* At this point, nextepc does not use SOCK_STREAM in libusrsctp */
    ogs_assert_if_reached();
}

static int usrsctp_recv_handler(struct socket *sock,
    union sctp_sockstore store, void *data, size_t datalen,
    struct sctp_rcvinfo rcv, int flags, void *ulp_info)
{
    if (data) {
        if (flags & MSG_NOTIFICATION) {
            union sctp_notification *not = (union sctp_notification *)data;
            if (not->sn_header.sn_length == (uint32_t)datalen) {
                switch(not->sn_header.sn_type) {
                case SCTP_ASSOC_CHANGE :
                    ogs_debug("SCTP_ASSOC_CHANGE:"
                            "[T:%d, F:0x%x, S:%d, I/O:%d/%d]", 
                            not->sn_assoc_change.sac_type,
                            not->sn_assoc_change.sac_flags,
                            not->sn_assoc_change.sac_state,
                            not->sn_assoc_change.sac_inbound_streams,
                            not->sn_assoc_change.sac_outbound_streams);

                    if (not->sn_assoc_change.sac_state == 
                            SCTP_SHUTDOWN_COMP ||
                        not->sn_assoc_change.sac_state == 
                            SCTP_COMM_LOST) {
                        ogs_sockaddr_t *addr =
                            ogs_usrsctp_remote_addr(&store);
                        ogs_assert(addr);

                        if (not->sn_assoc_change.sac_state == 
                            SCTP_SHUTDOWN_COMP)
                            ogs_debug("SCTP_SHUTDOWN_COMP");
                        if (not->sn_assoc_change.sac_state == 
                            SCTP_COMM_LOST)
                            ogs_debug("SCTP_COMM_LOST");

                        s1ap_event_push(MME_EVT_S1AP_LO_CONNREFUSED,
                                sock, addr, NULL, 0, 0);

                    } else if (not->sn_assoc_change.sac_state == SCTP_COMM_UP) {
                        ogs_sockaddr_t *addr =
                            ogs_usrsctp_remote_addr(&store);
                        ogs_assert(addr);

                        ogs_debug("SCTP_COMM_UP");

                        s1ap_event_push(MME_EVT_S1AP_LO_SCTP_COMM_UP,
                                sock, addr, NULL,
                                not->sn_assoc_change.sac_inbound_streams,
                                not->sn_assoc_change.sac_outbound_streams);
                    }
                    break;
                case SCTP_SHUTDOWN_EVENT :
                {
                    ogs_sockaddr_t *addr = ogs_usrsctp_remote_addr(&store);
                    ogs_assert(addr);

                    s1ap_event_push(MME_EVT_S1AP_LO_CONNREFUSED,
                            sock, addr, NULL, 0, 0);
                    break;
                }
                case SCTP_PEER_ADDR_CHANGE:
                    ogs_warn("SCTP_PEER_ADDR_CHANGE:"
                            "[T:%d, F:0x%x, S:%d]", 
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
                            not->sn_send_failed_event.ssfe_type,
                            not->sn_send_failed_event.ssfe_flags,
                            not->sn_send_failed_event.ssfe_error);
                    break;
                default :
                    ogs_error("Discarding event with "
                            "unknown flags:0x%x type:0x%x",
                            flags, not->sn_header.sn_type);
                    break;
                }
            }
        } else if (flags & MSG_EOR) {
            ogs_pkbuf_t *pkbuf;
            ogs_sockaddr_t *addr = NULL;

            pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
            ogs_pkbuf_put_data(pkbuf, data, datalen);

            addr = ogs_usrsctp_remote_addr(&store);
            ogs_assert(addr);

            s1ap_event_push(MME_EVT_S1AP_MESSAGE, sock, addr, pkbuf, 0, 0);
        } else {
            ogs_error("Not engough buffer. Need more recv : 0x%x", flags);
        }
        free(data);
    }
    return (1);
}

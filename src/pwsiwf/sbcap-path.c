/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "sbcap-path.h"
#include "context.h"
#include "sbcap-handler.h"

static void lksctp_accept_handler(short when, ogs_socket_t fd, void *data);

int sbcap_open(void)
{
    ogs_socknode_t *node = NULL;
    char buf[OGS_ADDRSTRLEN];

    // Start SBCAP server only
    ogs_list_for_each(&pwsiws_self()->sbcap_list, node) {
        if (sbcap_server(node) == NULL) return OGS_ERROR;
        ogs_info("PWS-IWS SBCAP server listening on [%s]:%d",
                OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));
    }

    return OGS_OK;
}

void sbcap_close(void)
{
    ogs_socknode_remove_all(&pwsiws_self()->sbcap_list);
}

ogs_sock_t *sbcap_server(ogs_socknode_t *node)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *sock = NULL;
#if !HAVE_USRSCTP
    ogs_poll_t *poll = NULL;
#endif

    ogs_assert(node);

#if HAVE_USRSCTP
    sock = ogs_sctp_server(SOCK_SEQPACKET, node->addr, node->option);
    if (!sock) return NULL;
    usrsctp_set_non_blocking((struct socket *)sock, 1);
    usrsctp_set_upcall((struct socket *)sock, usrsctp_recv_handler, NULL);
#else
    sock = ogs_sctp_server(SOCK_STREAM, node->addr, node->option);
    if (!sock) return NULL;
    poll = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLIN, sock->fd, lksctp_accept_handler, sock);
    ogs_assert(poll);

    node->poll = poll;
#endif

    node->sock = sock;
    node->cleanup = ogs_sctp_destroy;

    ogs_info("sbcap_server() [%s]:%d",
            OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));

    return sock;
}

void sbcap_recv_upcall(short when, ogs_socket_t fd, void *data)
{
    ogs_sock_t *sock = NULL;

    ogs_assert(fd != INVALID_SOCKET);
    sock = data;
    ogs_assert(sock);

    sbcap_recv_handler(sock);
}

#if HAVE_USRSCTP
static void usrsctp_recv_handler(struct socket *socket, void *data, int flags)
{
    int events;

    while ((events = usrsctp_get_events(socket)) &&
           (events & SCTP_EVENT_READ)) {
        sbcap_recv_handler((ogs_sock_t *)socket);
    }
}
#else
static void lksctp_accept_handler(short when, ogs_socket_t fd, void *data)
{
    ogs_assert(data);
    ogs_assert(fd != INVALID_SOCKET);

    sbcap_accept_handler(data);
}
#endif

void sbcap_accept_handler(ogs_sock_t *sock)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *new = NULL;
#if !HAVE_USRSCTP
    ogs_poll_t *poll = NULL;
#endif

    ogs_assert(sock);

    new = ogs_sock_accept(sock);
    if (new) {
        ogs_sockaddr_t *addr = NULL;

        addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        ogs_assert(addr);
        memcpy(addr, &new->remote_addr, sizeof(ogs_sockaddr_t));

        ogs_info("SBCAP accepted[%s]:%d in sbcap_path module", 
            OGS_ADDR(addr, buf), OGS_PORT(addr));
         
        ogs_info("SBCAP event: SCTP_ACCEPT");

#if HAVE_USRSCTP
        usrsctp_set_non_blocking((struct socket *)new, 1);
        usrsctp_set_upcall((struct socket *)new, usrsctp_recv_handler, NULL);
#else
        poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, new->fd, sbcap_recv_upcall, new);
        ogs_assert(poll);
#endif

    } else {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "accept() failed");
    }
}

void sbcap_recv_handler(ogs_sock_t *sock)
{
    ogs_pkbuf_t *pkbuf;
    int size;
    ogs_sockaddr_t *addr = NULL;
    ogs_sockaddr_t from;
    ogs_sctp_info_t sinfo;
    int flags = 0;
    pwsiws_connection_t *connection = NULL;

    ogs_assert(sock);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);
    size = ogs_sctp_recvmsg(
            sock, pkbuf->data, pkbuf->len, &from, &sinfo, &flags);
    if (size == 0) {
        ogs_info("SCTP connection closed by peer");

        // Find and remove the connection object, if it exists
        addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        ogs_assert(addr);
        memcpy(addr, &from, sizeof(ogs_sockaddr_t));
        connection = pwsiws_connection_find_by_addr(addr);
        if (connection) {
            pwsiws_connection_remove(connection);
        }
        ogs_free(addr);

        // Remove from pollset if needed (depends on your poll implementation)
        // ogs_pollset_remove(ogs_app()->pollset, sock->fd);

        // Close the socket
        ogs_sctp_destroy(sock);

        ogs_pkbuf_free(pkbuf);
        return;
    }
    if (size < 0 || size >= OGS_MAX_SDU_LEN) {
        ogs_error("ogs_sctp_recvmsg(%d) failed(%d:%s-0x%x)",
                size, errno, strerror(errno), flags);
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

                ogs_info("SBCAP event: SCTP_ACCEPT");
            } else if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP ||
                    not->sn_assoc_change.sac_state == SCTP_COMM_LOST) {

                if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP)
                    ogs_debug("SCTP_SHUTDOWN_COMP");
                if (not->sn_assoc_change.sac_state == SCTP_COMM_LOST)
                    ogs_debug("SCTP_COMM_LOST");

                addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
                ogs_assert(addr);
                memcpy(addr, &from, sizeof(ogs_sockaddr_t));

                ogs_info("SBCAP event: SCTP_CLOSE");
            }
            break;

        case SCTP_SHUTDOWN_EVENT :
            ogs_debug("SCTP_SHUTDOWN_EVENT:[T:%d, F:0x%x, L:%d]",
                    not->sn_shutdown_event.sse_type,
                    not->sn_shutdown_event.sse_flags,
                    not->sn_shutdown_event.sse_length);

            addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
            ogs_assert(addr);
            memcpy(addr, &from, sizeof(ogs_sockaddr_t));

            ogs_info("SBCAP event: SCTP_CLOSE");
            break;

        default :
            ogs_debug("SCTP_NOTIFICATION:[T:%d, F:0x%x, L:%d]",
                    not->sn_header.sn_type,
                    not->sn_header.sn_flags,
                    not->sn_header.sn_length);
            break;
        }

        ogs_pkbuf_free(pkbuf);
        return;
    }

    if (flags & MSG_EOR) {
        ogs_pkbuf_trim(pkbuf, size);

        addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        ogs_assert(addr);
        memcpy(addr, &from, sizeof(ogs_sockaddr_t));

        ogs_info("SBCAP event: SCTP_DATA");

        /* Find or create connection */
        connection = pwsiws_connection_find_by_addr(addr);
        if (!connection) {
            connection = pwsiws_connection_add(sock, addr);
            if (!connection) {
                ogs_error("Failed to create PWS-IWS connection");
                ogs_pkbuf_free(pkbuf);
                return;
            }
        }

        /* Decode SBCAP message */
        ogs_debug("Trying to decode SBCAP-PDU...");
        ogs_log_hexdump(OGS_LOG_DEBUG, pkbuf->data, pkbuf->len);

        SBCAP_SBC_AP_PDU_t *pdu = NULL;
        asn_dec_rval_t dec_ret;

        ogs_info("SBCAP message received, size: %d", size);
        ogs_info("SBCAP message data: %p, len: %d", pkbuf->data, pkbuf->len);

        dec_ret = aper_decode_complete(NULL, &asn_DEF_SBCAP_SBC_AP_PDU,
                (void **)&pdu, pkbuf->data, pkbuf->len);
        if (dec_ret.code != RC_OK) {
            ogs_error("Failed to decode SBCAP-PDU: code=%d, consumed=%ld", 
                    dec_ret.code, dec_ret.consumed);
            ogs_pkbuf_free(pkbuf);
            return;
        }

        ogs_info("SBCAP PDU decoded successfully");

        /* Process SBCAP message */
        if (pdu) {
            SBCAP_SuccessfulOutcome_t *successfulOutcome = NULL;
            SBCAP_InitiatingMessage_t *initiatingMessage = NULL;

            ogs_info("SBCAP PDU present: %d", pdu->present);

            switch (pdu->present) {
            case SBCAP_SBC_AP_PDU_PR_successfulOutcome:
                successfulOutcome = pdu->choice.successfulOutcome;
                ogs_assert(successfulOutcome);

                ogs_info("SBCAP SuccessfulOutcome procedureCode: %ld", successfulOutcome->procedureCode);

                switch (successfulOutcome->procedureCode) {
                case SBCAP_id_Write_Replace_Warning:
                    ogs_info("Calling pwsiws_sbcap_handle_write_replace_warning_response");
                    pwsiws_sbcap_handle_write_replace_warning_response(connection, pdu);
                    ogs_info("Write_Replace_Warning_Response");
                    break;
                default:
                    ogs_error("Not implemented(choice:%d, proc:%d)",
                            pdu->present, (int)successfulOutcome->procedureCode);
                    break;
                }
                break;

            case SBCAP_SBC_AP_PDU_PR_initiatingMessage:
                initiatingMessage = pdu->choice.initiatingMessage;
                ogs_assert(initiatingMessage);

                ogs_info("SBCAP InitiatingMessage procedureCode: %ld", initiatingMessage->procedureCode);

                switch (initiatingMessage->procedureCode) {
                case SBCAP_id_Write_Replace_Warning:
                    ogs_info("Calling pwsiws_sbcap_handle_write_replace_warning_request");
                    pwsiws_sbcap_handle_write_replace_warning_request(connection, pdu);
                    ogs_info("Write_Replace_Warning_Request");
                    break;

                case SBCAP_id_Stop_Warning:
                    ogs_info("Calling pwsiws_sbcap_handle_stop_warning_request");
                    pwsiws_sbcap_handle_stop_warning_request(connection, pdu);
                    ogs_info("Stop_Warning_Request");
                    break;
                default:
                    ogs_error("Not implemented(choice:%d, proc:%d)",
                            pdu->present, (int)initiatingMessage->procedureCode);
                    break;
                }
                break;

            default:
                ogs_error("Not implemented(choice:%d)", pdu->present);
                break;
            }

            ASN_STRUCT_FREE(asn_DEF_SBCAP_SBC_AP_PDU, pdu);
        } else {
            ogs_error("SBCAP PDU is NULL after decoding");
        }

        ogs_pkbuf_free(pkbuf);
        return;
    } else {
        if (ogs_socket_errno != OGS_EAGAIN) {
            ogs_error("ogs_sctp_recvmsg(%d) failed(%d:%s-0x%x)",
                    size, errno, strerror(errno), flags);
        }
    }

    ogs_pkbuf_free(pkbuf);
}

int sbcap_send(ogs_sock_t *sock,
        ogs_pkbuf_t *pkbuf, ogs_sockaddr_t *addr, uint16_t stream_no)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(pkbuf);

    ogs_debug("    IP[%s] STREAM[%d]",
            OGS_ADDR(addr, buf), stream_no);

    ogs_sctp_ppid_in_pkbuf(pkbuf) = OGS_SCTP_SBCAP_PPID;
    ogs_sctp_stream_no_in_pkbuf(pkbuf) = stream_no;

    return ogs_sctp_senddata(sock, pkbuf, addr);
} 
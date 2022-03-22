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
#include "ogs-app.h"

int __ogs_sctp_domain;

static void sctp_write_callback(short when, ogs_socket_t fd, void *data);

int ogs_sctp_recvdata(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo)
{
    int size;
    int flags = 0;

    do {
        size = ogs_sctp_recvmsg(sock, msg, len, from, sinfo, &flags);
        if (size < 0) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                    "ogs_sctp_recvdata(%d)", size);
            return size;
        }

        if (flags & MSG_NOTIFICATION) {
            /* Nothing */
        }
        else if (flags & MSG_EOR) {
            break;
        }
        else {
            ogs_assert_if_reached();
        }
    } while(1);

    return size;
}

int ogs_sctp_senddata(ogs_sock_t *sock,
        ogs_pkbuf_t *pkbuf, ogs_sockaddr_t *addr)
{
    int sent;

    ogs_assert(sock);
    ogs_assert(pkbuf);

    sent = ogs_sctp_sendmsg(sock, pkbuf->data, pkbuf->len, addr,
            ogs_sctp_ppid_in_pkbuf(pkbuf), ogs_sctp_stream_no_in_pkbuf(pkbuf));
    if (sent < 0 || sent != pkbuf->len) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_sctp_senddata(len:%d,ssn:%d)",
                pkbuf->len, (int)ogs_sctp_stream_no_in_pkbuf(pkbuf));
        ogs_pkbuf_free(pkbuf);
        return OGS_ERROR;
    }

    ogs_pkbuf_free(pkbuf);
    return OGS_OK;
}

void ogs_sctp_write_to_buffer(ogs_sctp_sock_t *sctp, ogs_pkbuf_t *pkbuf)
{
    ogs_assert(sctp);
    ogs_assert(pkbuf);

    ogs_list_add(&sctp->write_queue, pkbuf);

    if (!sctp->poll.write) {
        ogs_assert(sctp->sock);
        sctp->poll.write = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLOUT, sctp->sock->fd, sctp_write_callback, sctp);
        ogs_assert(sctp->poll.write);
    }
}

static void sctp_write_callback(short when, ogs_socket_t fd, void *data)
{
    ogs_sctp_sock_t *sctp = data;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sctp);
    if (ogs_list_empty(&sctp->write_queue) == true) {
        ogs_assert(sctp->poll.write);
        ogs_pollset_remove(sctp->poll.write);
        sctp->poll.write = NULL;
        return;
    }

    pkbuf = ogs_list_first(&sctp->write_queue);
    ogs_assert(pkbuf);
    ogs_list_remove(&sctp->write_queue, pkbuf);

    ogs_assert(sctp->sock);
    ogs_sctp_senddata(sctp->sock, pkbuf, NULL);
}

void ogs_sctp_flush_and_destroy(ogs_sctp_sock_t *sctp)
{
    ogs_pkbuf_t *pkbuf = NULL, *next_pkbuf = NULL;

    ogs_assert(sctp);

    ogs_assert(sctp->addr);
    ogs_free(sctp->addr);

    if (sctp->type == SOCK_STREAM) {
        ogs_assert(sctp->poll.read);
        ogs_pollset_remove(sctp->poll.read);

        if (sctp->poll.write)
            ogs_pollset_remove(sctp->poll.write);

        ogs_sctp_destroy(sctp->sock);

        ogs_list_for_each_safe(&sctp->write_queue, next_pkbuf, pkbuf) {
            ogs_list_remove(&sctp->write_queue, pkbuf);
            ogs_pkbuf_free(pkbuf);
        }
    }
}

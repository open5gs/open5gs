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

#include "core-config-private.h"

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_EVENTFD
#include <sys/eventfd.h>
#endif

#include "ogs-core.h"
#include "ogs-poll-private.h"

static void ogs_drain_pollset(short when, ogs_socket_t fd, void *data);

void ogs_notify_init(ogs_pollset_t *pollset)
{
#if !defined(HAVE_EVENTFD)
    int rc;
#endif
    ogs_assert(pollset);

#if defined(HAVE_EVENTFD)
    pollset->notify.fd[0] = eventfd(0, 0);
    ogs_assert(pollset->notify.fd[0] != INVALID_SOCKET);
#else
    rc = ogs_socketpair(AF_SOCKPAIR, SOCK_STREAM, 0, pollset->notify.fd);
    ogs_assert(rc == OGS_OK);
#endif

    pollset->notify.poll = ogs_pollset_add(pollset, OGS_POLLIN,
            pollset->notify.fd[0], ogs_drain_pollset, NULL);
    ogs_assert(pollset->notify.poll);
}

void ogs_notify_final(ogs_pollset_t *pollset)
{
    ogs_assert(pollset);

    ogs_pollset_remove(pollset->notify.poll);

    ogs_closesocket(pollset->notify.fd[0]);
#if !defined(HAVE_EVENTFD)
    ogs_closesocket(pollset->notify.fd[1]);
#endif
}

int ogs_notify_pollset(ogs_pollset_t *pollset)
{
    ssize_t r;
#if defined(HAVE_EVENTFD)
    uint64_t msg = 1;
#else
    char buf[1];
    buf[0] = 0;
#endif

    ogs_assert(pollset);

#if defined(HAVE_EVENTFD)
    r = write(pollset->notify.fd[0], (void*)&msg, sizeof(msg));
#else
    r = send(pollset->notify.fd[1], buf, 1, 0);
#endif

    if (r < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "notify failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

static void ogs_drain_pollset(short when, ogs_socket_t fd, void *data)
{
    ssize_t r;
#if defined(HAVE_EVENTFD)
    uint64_t msg;
#else
    unsigned char buf[1024];
#endif

    ogs_assert(when == OGS_POLLIN);

#if defined(HAVE_EVENTFD)
    r = read(fd, (char *)&msg, sizeof(msg));
#else
    r = recv(fd, (char *)buf, sizeof(buf), 0);
#endif
    if (r < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "drain failed");
    }
}

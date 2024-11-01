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

#include <sys/epoll.h>

#include "ogs-core.h"
#include "ogs-poll-private.h"

static void epoll_init(ogs_pollset_t *pollset);
static void epoll_cleanup(ogs_pollset_t *pollset);
static int epoll_add(ogs_poll_t *poll);
static int epoll_remove(ogs_poll_t *poll);
static int epoll_process(ogs_pollset_t *pollset, ogs_time_t timeout);

const ogs_pollset_actions_t ogs_epoll_actions = {
    epoll_init,
    epoll_cleanup,

    epoll_add,
    epoll_remove,
    epoll_process,

    ogs_notify_pollset,
};

struct epoll_map_s {
    ogs_poll_t *read;
    ogs_poll_t *write;
};

struct epoll_context_s {
    int epfd;

    ogs_hash_t *map_hash;
    struct epoll_event *event_list;
};

static void epoll_init(ogs_pollset_t *pollset)
{
    struct epoll_context_s *context = NULL;
    ogs_assert(pollset);

    context = ogs_calloc(1, sizeof *context);
    ogs_assert(context);
    pollset->context = context;

    context->event_list = ogs_calloc(
            pollset->capacity, sizeof(struct epoll_event));
    ogs_assert(context->event_list);

    context->map_hash = ogs_hash_make();
    ogs_assert(context->map_hash);

    context->epfd = epoll_create(pollset->capacity);
    if (context->epfd < 0) {
        ogs_log_message(OGS_LOG_FATAL, ogs_errno,
                "epoll_create() failed [%d]", pollset->capacity);
        ogs_assert_if_reached();
        return;
    }

    ogs_notify_init(pollset);
}

static void epoll_cleanup(ogs_pollset_t *pollset)
{
    struct epoll_context_s *context = NULL;

    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    ogs_notify_final(pollset);
    close(context->epfd);
    ogs_free(context->event_list);
    ogs_hash_destroy(context->map_hash);

    ogs_free(context);
}

static int epoll_add(ogs_poll_t *poll)
{
    int rv, op;
    ogs_pollset_t *pollset = NULL;
    struct epoll_context_s *context = NULL;
    struct epoll_map_s *map = NULL;
    struct epoll_event ee;

    ogs_assert(poll);
    pollset = poll->pollset;
    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    map = ogs_hash_get(context->map_hash, &poll->fd, sizeof(poll->fd));
    if (!map) {
        map = ogs_calloc(1, sizeof(*map));
        if (!map) {
            ogs_error("ogs_calloc() failed");
            return OGS_ERROR;
        }

        op = EPOLL_CTL_ADD;
        ogs_hash_set(context->map_hash, &poll->fd, sizeof(poll->fd), map);
    } else {
        op = EPOLL_CTL_MOD;
    }

    if (poll->when & OGS_POLLIN)
        map->read = poll;
    if (poll->when & OGS_POLLOUT)
        map->write = poll;

    memset(&ee, 0, sizeof ee);

    ee.events = 0;
    if (map->read)
        ee.events |= (EPOLLIN|EPOLLRDHUP);
    if (map->write)
        ee.events |= EPOLLOUT;
    ee.data.fd = poll->fd;

    rv = epoll_ctl(context->epfd, op, poll->fd, &ee);
    if (rv < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "epoll_ctl[%d] failed", op);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int epoll_remove(ogs_poll_t *poll)
{
    int rv, op;
    ogs_pollset_t *pollset = NULL;
    struct epoll_context_s *context = NULL;
    struct epoll_map_s *map = NULL;
    struct epoll_event ee;

    ogs_assert(poll);
    pollset = poll->pollset;
    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    map = ogs_hash_get(context->map_hash, &poll->fd, sizeof(poll->fd));
    ogs_assert(map);

    if (poll->when & OGS_POLLIN)
        map->read = NULL;
    if (poll->when & OGS_POLLOUT)
        map->write = NULL;

    memset(&ee, 0, sizeof ee);

    ee.events = 0;
    if (map->read)
        ee.events |= (EPOLLIN|EPOLLRDHUP);
    if (map->write)
        ee.events |= EPOLLOUT;

    if (map->read || map->write) {
        op = EPOLL_CTL_MOD;
        ee.data.fd = poll->fd;
    } else {
        op = EPOLL_CTL_DEL;
        ee.data.fd = INVALID_SOCKET;

        ogs_hash_set(context->map_hash, &poll->fd, sizeof(poll->fd), NULL);
        ogs_free(map);
    }

    rv = epoll_ctl(context->epfd, op, poll->fd, &ee);
    if (rv < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "epoll_remove[%d] failed", op);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int epoll_process(ogs_pollset_t *pollset, ogs_time_t timeout)
{
    struct epoll_context_s *context = NULL;
    int num_of_poll;
    int i;

    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    num_of_poll = epoll_wait(context->epfd, context->event_list,
            pollset->capacity,
            timeout == OGS_INFINITE_TIME ? OGS_INFINITE_TIME :
                ogs_time_to_msec(timeout));
    if (num_of_poll < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "epoll failed");
        return OGS_ERROR;
    } else if (num_of_poll == 0) {
        return OGS_TIMEUP;
    }

    for (i = 0; i < num_of_poll; i++) {
        struct epoll_map_s *map = NULL;
        uint32_t received;
        short when = 0;
        ogs_socket_t fd;

        received = context->event_list[i].events;
        if (received & EPOLLERR) {
        /*
         * The libevent library has OGS_POLLOUT turned on in EPOLLERR.
         *
         * However, SIGPIPE can occur if write() is called
         * when the peer connection is closed.
         *
         * Therefore, Open5GS turns off OGS_POLLOUT
         * so that write() cannot be called in case of EPOLLERR.
         *
         * See also #2411 and #2312
         */
#if 0
            when = OGS_POLLIN|OGS_POLLOUT;
#else
            when = OGS_POLLIN;
#endif
        } else if ((received & EPOLLHUP) && !(received & EPOLLRDHUP)) {
            when = OGS_POLLIN|OGS_POLLOUT;
        } else {
            if (received & EPOLLIN) {
                when |= OGS_POLLIN;
            }
            if (received & EPOLLOUT) {
                when |= OGS_POLLOUT;
            }
            if (received & EPOLLRDHUP) {
                when |= OGS_POLLIN;
                when &= ~OGS_POLLOUT;
            }
        }

        if (!when)
            continue;

        fd = context->event_list[i].data.fd;
        ogs_assert(fd != INVALID_SOCKET);

        map = ogs_hash_get(context->map_hash, &fd, sizeof(fd));
        if (!map) continue;

        if (map->read && map->write && map->read == map->write) {
            map->read->handler(when, map->read->fd, map->read->data);
        } else {
            if ((when & OGS_POLLIN) && map->read)
                map->read->handler(when, map->read->fd, map->read->data);

            /*
             * map->read->handler() can call ogs_remove_epoll()
             * So, we need to check map instance
             */
            map = ogs_hash_get(context->map_hash, &fd, sizeof(fd));
            if (!map) continue;

            if ((when & OGS_POLLOUT) && map->write)
                map->write->handler(when, map->write->fd, map->write->data);
        }
    }
    
    return OGS_OK;
}

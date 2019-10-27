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
static int epoll_add(ogs_poll_t *poll, short when);
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

struct epoll_context_s {
    int epfd;

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
        ogs_core()->socket.pool, sizeof(struct epoll_event));
	ogs_assert(context->event_list);

    context->epfd = epoll_create(ogs_core()->socket.pool);
    ogs_assert(context->epfd >= 0);

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

    ogs_free(context);
}

static int epoll_add(ogs_poll_t *poll, short when)
{
    ogs_pollset_t *pollset = NULL;
    struct epoll_context_s *context = NULL;
    int rv;
    struct epoll_event ee;

    ogs_assert(poll);
    pollset = poll->pollset;
    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    ee.events = 0;
    if (when == OGS_POLLIN)
        ee.events |= (EPOLLIN|EPOLLRDHUP);
    if (when == OGS_POLLOUT)
        ee.events |= EPOLLOUT;

    ee.data.ptr = poll;

    rv = epoll_ctl(context->epfd, EPOLL_CTL_ADD, poll->fd, &ee);
    if (rv < 0) {
		ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "epoll_ctl failed");
		return OGS_ERROR;
    }

    return OGS_OK;
}

static int epoll_remove(ogs_poll_t *poll)
{
    int rv;
    ogs_pollset_t *pollset = NULL;
    struct epoll_context_s *context = NULL;
    struct epoll_event ee;

    ogs_assert(poll);
    pollset = poll->pollset;
    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    ee.events = 0;
    ee.data.ptr = NULL;

    rv = epoll_ctl(context->epfd, EPOLL_CTL_DEL, poll->fd, &ee);
    if (rv < 0) {
		ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "epoll_remove failed");
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
            ogs_core()->socket.pool,
            timeout == OGS_INFINITE_TIME ? OGS_INFINITE_TIME :
                ogs_time_to_msec(timeout));
    if (num_of_poll < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "epoll failed");
        return OGS_ERROR;
    } else if (num_of_poll == 0) {
        return OGS_TIMEUP;
    }

	for (i = 0; i < num_of_poll; i++) {
		ogs_poll_t *poll = NULL;
		uint32_t received;
        short when = 0;

		received = context->event_list[i].events;
		if (received & (EPOLLERR|EPOLLHUP)) {
            when = OGS_POLLIN|OGS_POLLOUT;
		} else {
            if (received & (EPOLLIN|EPOLLRDHUP)) {
                when |= OGS_POLLIN;
            }
            if (received & EPOLLOUT) {
                when |= OGS_POLLOUT;
            } 
        }

        if (!when)
            continue;

        poll = context->event_list[i].data.ptr;
        ogs_assert(poll);

        if (poll->handler) {
            poll->handler(when, poll->fd, poll->data);
        }
    }
    
    return OGS_OK;
}

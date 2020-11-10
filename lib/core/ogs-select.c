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

#include "ogs-core.h"
#include "ogs-poll-private.h"

static void select_init(ogs_pollset_t *pollset);
static void select_cleanup(ogs_pollset_t *pollset);
static int select_add(ogs_poll_t *poll);
static int select_remove(ogs_poll_t *poll);
static int select_process(ogs_pollset_t *pollset, ogs_time_t timeout);

const ogs_pollset_actions_t ogs_select_actions = {
    select_init,
    select_cleanup,

    select_add,
    select_remove,
    select_process,

    ogs_notify_pollset,
};

struct select_context_s {
    int max_fd;
    fd_set master_read_fd_set;
    fd_set master_write_fd_set;
    fd_set work_read_fd_set;
    fd_set work_write_fd_set;

    ogs_list_t list;
};

static void select_init(ogs_pollset_t *pollset)
{
    struct select_context_s *context = NULL;
    ogs_assert(pollset);

    context = ogs_calloc(1, sizeof *context);
    ogs_assert(context);
    pollset->context = context;

    ogs_list_init(&context->list);

    context->max_fd = -1;
    FD_ZERO(&context->master_read_fd_set);
    FD_ZERO(&context->master_write_fd_set);

    ogs_notify_init(pollset);
}

static void select_cleanup(ogs_pollset_t *pollset)
{
    struct select_context_s *context = NULL;

    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    ogs_notify_final(pollset);
    ogs_free(context);
}

static int select_add(ogs_poll_t *poll)
{
    ogs_pollset_t *pollset = NULL;
    struct select_context_s *context = NULL;

    ogs_assert(poll);
    pollset = poll->pollset;
    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    if (poll->when & OGS_POLLIN) {
        FD_SET(poll->fd, &context->master_read_fd_set);
    }

    if (poll->when & OGS_POLLOUT) {
        FD_SET(poll->fd, &context->master_write_fd_set);
    }

    if (poll->fd > context->max_fd)
        context->max_fd = poll->fd;

    ogs_list_add(&context->list, poll);

    return OGS_OK;
}

static int select_remove(ogs_poll_t *poll)
{
    ogs_pollset_t *pollset = NULL;
    struct select_context_s *context = NULL;

    ogs_assert(poll);
    pollset = poll->pollset;
    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    if (poll->when & OGS_POLLIN)
        FD_CLR(poll->fd, &context->master_read_fd_set);

    if (poll->when & OGS_POLLOUT)
        FD_CLR(poll->fd, &context->master_write_fd_set);

    if (context->max_fd == poll->fd) {
        context->max_fd = -1;
    }

    ogs_list_remove(&context->list, poll);

    return OGS_OK;
}

static int select_process(ogs_pollset_t *pollset, ogs_time_t timeout)
{
    struct select_context_s *context = NULL;
    ogs_poll_t *poll = NULL, *next_poll = NULL;
    int rc;
    struct timeval tv, *tp;

    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    if (context->max_fd == -1) {
        ogs_list_for_each(&context->list, poll) {
            if (context->max_fd < poll->fd) {
                context->max_fd = poll->fd;
            }
        }
        ogs_debug("change max_fd: %d", context->max_fd);
    }

    context->work_read_fd_set = context->master_read_fd_set;
    context->work_write_fd_set = context->master_write_fd_set;

    if (timeout == OGS_INFINITE_TIME) {
        tp = NULL;
    } else {
        tv.tv_sec = ogs_time_sec(timeout);
#if defined(_WIN32) /* I don't know why windows need more time */
        tv.tv_usec = ogs_time_usec(timeout) + ogs_time_from_msec(1);
#else
        tv.tv_usec = ogs_time_usec(timeout);
#endif

        tp = &tv;
    }

    rc = select(context->max_fd + 1,
            &context->work_read_fd_set, &context->work_write_fd_set, NULL, tp);
    if (rc < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "select() failed");
        return OGS_ERROR;
    } else if (rc == 0) {
        return OGS_TIMEUP;
    }

    ogs_list_for_each_safe(&context->list, next_poll, poll) {
        short when = 0;
        if ((poll->when & OGS_POLLIN) &&
            FD_ISSET(poll->fd, &context->work_read_fd_set)) {
            when |= OGS_POLLIN;
        }

        if ((poll->when & OGS_POLLOUT) &&
            FD_ISSET(poll->fd, &context->work_write_fd_set)) {
            when |= OGS_POLLOUT;
        }

        if (when && poll->handler) {
            poll->handler(when, poll->fd, poll->data);
        }
    }
    
    return OGS_OK;
}

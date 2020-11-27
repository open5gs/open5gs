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

#include "ogs-core.h"

#include "ogs-poll-private.h"

extern const ogs_pollset_actions_t ogs_kqueue_actions;
extern const ogs_pollset_actions_t ogs_epoll_actions;
extern const ogs_pollset_actions_t ogs_select_actions;

static void *self_handler_data = NULL;

ogs_pollset_actions_t ogs_pollset_actions;
bool ogs_pollset_actions_initialized = false;

ogs_pollset_t *ogs_pollset_create(unsigned int capacity)
{
    ogs_pollset_t *pollset = ogs_calloc(1, sizeof *pollset);
    ogs_assert(pollset);

    pollset->capacity = capacity;

    ogs_pool_init(&pollset->pool, capacity);

    if (ogs_pollset_actions_initialized == false) {
#if defined(HAVE_KQUEUE)
        ogs_pollset_actions = ogs_kqueue_actions;
#elif defined(HAVE_EPOLL)
        ogs_pollset_actions = ogs_epoll_actions;
#else
        ogs_pollset_actions = ogs_select_actions;
#endif
        ogs_pollset_actions_initialized = true;
    }

    ogs_pollset_actions.init(pollset);

    return pollset;
}

void ogs_pollset_destroy(ogs_pollset_t *pollset)
{
    ogs_assert(pollset);

    ogs_pollset_actions.cleanup(pollset);

    ogs_pool_final(&pollset->pool);
    ogs_free(pollset);
}

ogs_poll_t *ogs_pollset_add(ogs_pollset_t *pollset, short when,
        ogs_socket_t fd, ogs_poll_handler_f handler, void *data)
{
    ogs_poll_t *poll = NULL;
    int rc;

    ogs_assert(pollset);

    ogs_assert(fd != INVALID_SOCKET);
    ogs_assert(handler);

    ogs_pool_alloc(&pollset->pool, &poll);
    ogs_assert(poll);

    rc = ogs_nonblocking(fd);
    ogs_assert(rc == OGS_OK);
    rc = ogs_closeonexec(fd);
    ogs_assert(rc == OGS_OK);

    poll->when = when;
    poll->fd = fd;
    poll->handler = handler;

    if (data == &self_handler_data)
        poll->data = poll;
    else
        poll->data = data;

    poll->pollset = pollset;

    rc = ogs_pollset_actions.add(poll);
    if (rc != OGS_OK) {
        ogs_error("cannot add poll");
        ogs_pool_free(&pollset->pool, poll);
        return NULL;
    }

    return poll;
}

void ogs_pollset_remove(ogs_poll_t *poll)
{
    int rc;
    ogs_pollset_t *pollset = NULL;

    ogs_assert(poll);
    pollset = poll->pollset;
    ogs_assert(pollset);

    rc = ogs_pollset_actions.remove(poll);
    if (rc != OGS_OK) {
        ogs_error("cannot delete poll");
    }

    ogs_pool_free(&pollset->pool, poll);
}

ogs_poll_t *ogs_pollset_cycle(ogs_pollset_t *pollset, ogs_poll_t *poll)
{
    ogs_assert(pollset);
    return ogs_pool_cycle(&pollset->pool, poll);
}

void *ogs_pollset_self_handler_data(void)
{
    return &self_handler_data;
}

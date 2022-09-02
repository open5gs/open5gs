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

#include <sys/queue.h>
#include <sys/event.h>

#include "ogs-core.h"
#include "ogs-poll-private.h"

static void kqueue_init(ogs_pollset_t *pollset);
static void kqueue_cleanup(ogs_pollset_t *pollset);
static int kqueue_add(ogs_poll_t *poll);
static int kqueue_remove(ogs_poll_t *poll);
static int kqueue_process(ogs_pollset_t *pollset, ogs_time_t timeout);

static void kqueue_notify_init(ogs_pollset_t *pollset);
static int kqueue_notify_pollset(ogs_pollset_t *pollset);

const ogs_pollset_actions_t ogs_kqueue_actions = {
    kqueue_init,
    kqueue_cleanup,

    kqueue_add,
    kqueue_remove,
    kqueue_process,

    kqueue_notify_pollset,
};

struct kqueue_context_s {
    int kqueue;

    struct kevent *change_list;
    struct kevent *event_list;
    int nchanges, nevents;
};

static void kqueue_init(ogs_pollset_t *pollset)
{
    struct kqueue_context_s *context = NULL;
    ogs_assert(pollset);

    context = ogs_calloc(1, sizeof *context);
    ogs_assert(context);
    pollset->context = context;

    context->change_list = ogs_calloc(
        pollset->capacity, sizeof(struct kevent));
    ogs_assert(context->change_list);
    context->event_list = ogs_calloc(
        pollset->capacity, sizeof(struct kevent));
    ogs_assert(context->change_list);
    context->nchanges = 0;
    context->nevents = pollset->capacity;

    context->kqueue = kqueue();
    ogs_assert(context->kqueue != -1);

    kqueue_notify_init(pollset);
}

static void kqueue_cleanup(ogs_pollset_t *pollset)
{
    struct kqueue_context_s *context = NULL;

    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    ogs_free(context->change_list);
    ogs_free(context->event_list);

    close(context->kqueue);

    ogs_free(context);
}

static int kqueue_set(ogs_poll_t *poll, int filter, int flags)
{
    ogs_pollset_t *pollset = NULL;
    struct kqueue_context_s *context = NULL;
    struct kevent *kev;

    ogs_assert(poll);
    pollset = poll->pollset;
    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    ogs_assert(context->nchanges < pollset->capacity);

    kev = &context->change_list[context->nchanges];
    memset(kev, 0, sizeof *kev);
    kev->ident = poll->fd;
    kev->filter = filter;
    kev->flags = flags;
    kev->udata = poll;

    poll->index = context->nchanges;
    context->nchanges++;

    return OGS_OK;
}

static int kqueue_add(ogs_poll_t *poll)
{
    int filter = 0;

    if (poll->when & OGS_POLLIN) {
        filter = EVFILT_READ;
    }
    if (poll->when & OGS_POLLOUT) {
        filter = EVFILT_WRITE;
    }

    return kqueue_set(poll, filter, EV_ADD|EV_ENABLE);
}

#if 0 /* ogs_pollset_remove() is not working, SHOULD remove the below code */
static int kqueue_remove(ogs_poll_t *poll)
{
    ogs_pollset_t *pollset = NULL;
    struct kqueue_context_s *context = NULL;
    struct kevent *kev;
    ogs_poll_t *last = NULL;

    ogs_assert(poll);
    pollset = poll->pollset;
    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    ogs_assert(poll->index < context->nchanges);

    context->nchanges--;
    kev = &context->change_list[context->nchanges];

    ogs_assert(kev);
    context->change_list[poll->index] = *kev;

    last = kev->udata;
    ogs_assert(last);

    last->index = poll->index;

    return OGS_OK;
}
#else /* New approach : ogs_pollset_remove() is properly working. */

static int kqueue_remove(ogs_poll_t *poll)
{
    int filter = 0;

    if (poll->when & OGS_POLLIN) {
        filter = EVFILT_READ;
    }
    if (poll->when & OGS_POLLOUT) {
        filter = EVFILT_WRITE;
    }

    return kqueue_set(poll, filter, EV_DELETE);
}
#endif

static int kqueue_process(ogs_pollset_t *pollset, ogs_time_t timeout)
{
    struct kqueue_context_s *context = NULL;
    struct timespec ts, *tp;
    int i, n;

    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    if (timeout == OGS_INFINITE_TIME) {
        tp = NULL;
    } else {
        ts.tv_sec = ogs_time_sec(timeout);
        ts.tv_nsec = ogs_time_usec(timeout) * 1000;
        tp = &ts;
    }

    n = kevent(context->kqueue,
            context->change_list, context->nchanges,
            context->event_list, context->nevents, tp);

    context->nchanges = 0;

    if (n < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "kqueue failed");
        return OGS_ERROR;
    } else if (n == 0) {
        return OGS_TIMEUP;
    }

    for (i = 0; i < n; i++) {
        ogs_poll_t *poll = NULL;
        short when = 0;

        if (context->event_list[i].flags & EV_ERROR) {
            switch (context->event_list[i].data) {

            /* Can occur on delete if we are not currently
             * watching any events on this fd.  That can
             * happen when the fd was closed and another
             * file was opened with that fd. */
            case ENOENT:
            /* Can occur for reasons not fully understood
             * on FreeBSD. */
            case EINVAL:
                continue;
#if defined(__FreeBSD__)
            /*
             * This currently occurs if an FD is closed
             * before the EV_DELETE makes it out via kevent().
             * The FreeBSD capabilities code sees the blank
             * capability set and rejects the request to
             * modify an event.
             *
             * To be strictly correct - when an FD is closed,
             * all the registered events are also removed.
             * Queuing EV_DELETE to a closed FD is wrong.
             * The event(s) should just be deleted from
             * the pending changelist.
             */
            case ENOTCAPABLE:
                continue;
#endif

            /* Can occur on a delete if the fd is closed. */
            case EBADF:
                /* XXXX On NetBSD, we can also get EBADF if we
                 * try to add the write side of a pipe, but
                 * the read side has already been closed.
                 * Other BSDs call this situation 'EPIPE'. It
                 * would be good if we had a way to report
                 * this situation. */
                continue;
            /* These two can occur on an add if the fd was one side
             * of a pipe, and the other side was closed. */
            case EPERM:
            case EPIPE:
                /* Report read events, if we're listening for
                 * them, so that the user can learn about any
                 * add errors.  (If the operation was a
                 * delete, then udata should be cleared.) */
                if (context->event_list[i].udata) {
                    /* The operation was an add:
                     * report the error as a read. */
                    when |= OGS_POLLIN;
                    break;
                } else {
                    /* The operation was a del:
                     * report nothing. */
                    continue;
                }

            /* Other errors shouldn't occur. */
            default:
                ogs_error("kevent() error : flags = 0x%x, errno = %d",
                        context->event_list[i].flags,
                        (int)context->event_list[i].data);
                return OGS_ERROR;
            }
        } else if (context->event_list[i].filter == EVFILT_READ) {
            when |= OGS_POLLIN;
        } else if (context->event_list[i].filter == EVFILT_WRITE) {
            when |= OGS_POLLOUT;
        } else if (context->event_list[i].filter == EVFILT_USER) {
            /* Nothing */
        } else {
            ogs_warn("kevent() unknown filter = 0x%x\n",
                context->event_list[i].filter);
        }

        if (!when)
            continue;

        poll = (ogs_poll_t *)context->event_list[i].udata;
        ogs_assert(poll);

        if (poll->handler) {
            poll->handler(when, poll->fd, poll->data);
        }
    }
    
    return OGS_OK;
}

#define NOTIFY_IDENT 42 /* Magic number */

static void kqueue_notify_init(ogs_pollset_t *pollset)
{
    int rc;
    struct kqueue_context_s *context = NULL;
    struct kevent kev;
    struct timespec timeout = { 0, 0 };
    ogs_assert(pollset);

    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    memset(&kev, 0, sizeof kev);
    kev.ident = NOTIFY_IDENT;
    kev.filter = EVFILT_USER;
    kev.flags = EV_ADD | EV_CLEAR;

    rc = kevent(context->kqueue, &kev, 1, NULL, 0, &timeout);
    ogs_assert(rc != -1);
}

static int kqueue_notify_pollset(ogs_pollset_t *pollset)
{
    int rc;
    struct kqueue_context_s *context = NULL;
    struct kevent kev;
    struct timespec timeout = { 0, 0 };
    ogs_assert(pollset);

    ogs_assert(pollset);
    context = pollset->context;
    ogs_assert(context);

    memset(&kev, 0, sizeof kev);
    kev.ident = NOTIFY_IDENT;
    kev.filter = EVFILT_USER;
    kev.fflags = NOTE_TRIGGER;

    rc = kevent(context->kqueue, &kev, 1, NULL, 0, &timeout);
    if (rc == -1) {
        ogs_warn("kevent() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

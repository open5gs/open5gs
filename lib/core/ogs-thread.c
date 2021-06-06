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

#include "ogs-core.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_thread_domain

#if !defined(_WIN32)
#define ogs_thread_id_t pthread_t
#define ogs_thread_join(_n) pthread_join((_n), NULL)
#else
#define ogs_thread_id_t HANDLE
static ogs_inline DWORD ogs_thread_join(ogs_thread_id_t thread)
{
    DWORD ret = WaitForSingleObject(thread, INFINITE);
    if (!CloseHandle (thread)) {
        ogs_log_message(
            OGS_LOG_ERROR, ogs_errno, "Couldn't close thread handle");
    }

    return ret;
}
#endif

typedef struct ogs_thread_s {
    ogs_thread_id_t id;

    ogs_thread_mutex_t mutex;
    ogs_thread_cond_t cond;

    bool running;

    void (*func)(void *);
    void *data;
} ogs_thread_t;

static void *thread_worker(void *arg)
{
    ogs_thread_t *thread = arg;
    ogs_assert(thread);

    ogs_thread_mutex_lock(&thread->mutex);

    thread->running = true;
    ogs_thread_cond_signal(&thread->cond);

    ogs_thread_mutex_unlock(&thread->mutex);

    ogs_debug("[%p] worker signal", thread);
    thread->func(thread->data);

    ogs_thread_mutex_lock(&thread->mutex);
    thread->running = false;
    ogs_thread_mutex_unlock(&thread->mutex);
    ogs_debug("[%p] worker done", thread);

    return NULL;
}

ogs_thread_t *ogs_thread_create(void (*func)(void *), void *data)
{
    ogs_thread_t *thread = ogs_calloc(1, sizeof *thread);
    ogs_expect_or_return_val(thread, NULL);

    ogs_thread_mutex_init(&thread->mutex);
    ogs_thread_cond_init(&thread->cond);

    ogs_thread_mutex_lock(&thread->mutex);

    thread->running = false;

    thread->func = func;
    thread->data = data;

#if !defined(_WIN32)
    pthread_create(&thread->id, NULL, thread_worker, thread);
#else
    thread->id = CreateThread(NULL, 0,
                    (LPTHREAD_START_ROUTINE)thread_worker, thread, 0, NULL);
#endif

    ogs_thread_cond_wait(&thread->cond, &thread->mutex);
    ogs_thread_mutex_unlock(&thread->mutex);
    ogs_debug("[%p] thread started", thread);

    return thread;
}

void ogs_thread_destroy(ogs_thread_t *thread)
{
    const ogs_time_t deadline = ogs_get_monotonic_time() + 5 * 1000 * 1000;
    ogs_assert(thread);

    ogs_debug("[%p] thread running(%d)", thread, thread->running);
    while(ogs_get_monotonic_time() <= deadline) {
        /* wait 5 seconds */
        ogs_thread_mutex_lock(&thread->mutex);
        if (!thread->running) {
            ogs_thread_mutex_unlock(&thread->mutex);
            break;
        }
        ogs_thread_mutex_unlock(&thread->mutex);
        ogs_usleep(1000);
    }

    ogs_debug("[%p] thread destroy", thread);
    ogs_thread_mutex_lock(&thread->mutex);
    if (thread->running) {
        ogs_fatal("thread still running after 3 seconds");
        ogs_assert_if_reached();
    }
    ogs_thread_mutex_unlock(&thread->mutex);

    ogs_thread_join(thread->id);
    ogs_debug("[%p] thread join", thread);

    ogs_thread_cond_destroy(&thread->cond);
    ogs_thread_mutex_destroy(&thread->mutex);

    ogs_free(thread);
    ogs_debug("[%p] thread done", thread);
}

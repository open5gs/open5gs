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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_THREAD_H
#define OGS_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The following code is stolen from mongodb-c-driver
 * https://github.com/mongodb/mongo-c-driver/blob/master/src/libmongoc/src/mongoc/mongoc-thread-private.h
 */
#if !defined(_WIN32)
#define ogs_thread_mutex_t pthread_mutex_t
#define ogs_thread_mutex_init(_n) (void)pthread_mutex_init((_n), NULL)
#define ogs_thread_mutex_lock (void)pthread_mutex_lock
#define ogs_thread_mutex_unlock (void)pthread_mutex_unlock
#define ogs_thread_mutex_destroy (void)pthread_mutex_destroy
#define ogs_thread_cond_t pthread_cond_t
#define ogs_thread_cond_init(_n) (void)pthread_cond_init((_n), NULL)
#define ogs_thread_cond_wait pthread_cond_wait
static ogs_inline int ogs_thread_cond_timedwait(
        pthread_cond_t *cond, pthread_mutex_t *mutex, ogs_time_t timeout)
{
    int r;
    struct timespec to;
    struct timeval tv;
    ogs_time_t usec;

    ogs_gettimeofday(&tv);

    usec = ogs_time_from_sec(tv.tv_sec) + tv.tv_usec + timeout;

    to.tv_sec = ogs_time_sec(usec);
    to.tv_nsec = ogs_time_usec(usec) * 1000;

    r = pthread_cond_timedwait(cond, mutex, &to);
    if (r == 0)
        return OGS_OK; 
    else if (r == OGS_ETIMEDOUT)
        return OGS_TIMEUP;
    else 
        return OGS_ERROR;
}
#define ogs_thread_cond_signal (void)pthread_cond_signal
#define ogs_thread_cond_broadcast pthread_cond_broadcast
#define ogs_thread_cond_destroy (void)pthread_cond_destroy
#define ogs_thread_id_t pthread_t
#define ogs_thread_join(_n) pthread_join((_n), NULL)
#else
#define ogs_thread_mutex_t CRITICAL_SECTION
#define ogs_thread_mutex_init InitializeCriticalSection
#define ogs_thread_mutex_lock EnterCriticalSection
#define ogs_thread_mutex_unlock LeaveCriticalSection
#define ogs_thread_mutex_destroy DeleteCriticalSection
#define ogs_thread_cond_t CONDITION_VARIABLE
#define ogs_thread_cond_init InitializeConditionVariable
#define ogs_thread_cond_wait(_c, _m) \
    ogs_thread_cond_timedwait ((_c), (_m), INFINITE)
static ogs_inline int ogs_thread_cond_timedwait(
    ogs_thread_cond_t *cond, ogs_thread_mutex_t *mutex, ogs_time_t timeout)
{
    int r;

    if (SleepConditionVariableCS(cond, mutex,
                (DWORD)ogs_time_to_msec(timeout))) {
        return OGS_OK;
    } else {
        r = GetLastError();

        if (r == WAIT_TIMEOUT || r == ERROR_TIMEOUT) {
            return OGS_TIMEUP;
        } else {
            return OGS_ERROR;
        }
    }
}
#define ogs_thread_cond_signal WakeConditionVariable
#define ogs_thread_cond_broadcast WakeAllConditionVariable
static ogs_inline int ogs_thread_cond_destroy(ogs_thread_cond_t *_ignored)
{
   return 0;
}
#endif

typedef struct ogs_thread_s ogs_thread_t;

ogs_thread_t *ogs_thread_create(void (*func)(void *), void *data);
void ogs_thread_destroy(ogs_thread_t *thread);

#ifdef __cplusplus
}
#endif

#endif /* OGS_THREAD_H */

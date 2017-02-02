#ifndef __CORE_COND_H__
#define __CORE_COND_H__

/**
 * @file cond.h
 * @brief CORE Condition Variable Routines
 */

#include "core.h"
#include "core_errno.h"
#include "core_time.h"
#include "core_mutex.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup cond Condition Variable Routines
 * @ingroup CORE
 * @{
 */

/** Opaque structure for thread condition variables */
typedef c_uintptr_t cond_id;

/**
 * Initialize Conditional Pool
 */
CORE_DECLARE(status_t) cond_init(void);

/**
 * Finalize Conditional Pool
 */
CORE_DECLARE(status_t) cond_final(void);

/**
 * Note: destroying a condition variable (or likewise, destroying or
 * clearing the pool from which a condition variable was allocated) if
 * any threads are blocked waiting on it gives undefined results.
 */

/**
 * Create and initialize a condition variable that can be used to signal
 * and schedule threads in a single process.
 * @param id the memory address where the newly created condition variable
 *        will be stored.
 */
CORE_DECLARE(status_t) cond_create(cond_id *id);

/**
 * Put the active calling thread to sleep until signaled to wake up. Each
 * condition variable must be associated with a mutex, and that mutex must
 * be locked before  calling this function, or the behavior will be
 * undefined. As the calling thread is put to sleep, the given mutex
 * will be simultaneously released; and as this thread wakes up the lock
 * is again simultaneously acquired.
 * @param id the condition variable on which to block.
 * @param mid the mutex that must be locked upon entering this function,
 *        is released while the thread is asleep, and is again acquired before
 *        returning from this function.
 * @remark Spurious wakeups may occur. Before and after every call to wait on
 * a condition variable, the caller should test whether the condition is already
 * met.
 */
CORE_DECLARE(status_t) cond_wait(cond_id id, mutex_id mid);

/**
 * Put the active calling thread to sleep until signaled to wake up or
 * the timeout is reached. Each condition variable must be associated
 * with a mutex, and that mutex must be locked before calling this
 * function, or the behavior will be undefined. As the calling thread
 * is put to sleep, the given mutex will be simultaneously released;
 * and as this thread wakes up the lock is again simultaneously acquired.
 * @param id the condition variable on which to block.
 * @param mid the mutex that must be locked upon entering this function,
 *        is released while the thread is asleep, and is again acquired before
 *        returning from this function.
 * @param timeout The amount of time in microseconds to wait. This is
 *        a maximum, not a minimum. If the condition is signaled, we
 *        will wake up before this time, otherwise the error CORE_TIMEUP
 *        is returned.
 */
CORE_DECLARE(status_t) cond_timedwait(
        cond_id id, mutex_id mid, c_time_t timeout);

/**
 * Signals a single thread, if one exists, that is blocking on the given
 * condition variable. That thread is then scheduled to wake up and acquire
 * the associated mutex. Although it is not required, if predictable scheduling
 * is desired, that mutex must be locked while calling this function.
 * @param id the condition variable on which to produce the signal.
 * @remark If no threads are waiting on the condition variable, nothing happens.
 */
CORE_DECLARE(status_t) cond_signal(cond_id id);

/**
 * Signals all threads blocking on the given condition variable.
 * Each thread that was signaled is then scheduled to wake up and acquire
 * the associated mutex. This will happen in a serialized manner.
 * @param id the condition variable on which to produce the broadcast.
 * @remark If no threads are waiting on the condition variable, nothing happens.
 */
CORE_DECLARE(status_t) cond_broadcast(cond_id id);

/**
 * Destroy the condition variable and free the associated memory.
 * @param id the condition variable to destroy.
 */
CORE_DECLARE(status_t) cond_delete(cond_id id);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* ! __CORE_COND_H__ */

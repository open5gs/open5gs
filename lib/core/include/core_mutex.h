#ifndef __MUTEX_H__
#define __MUTEX_H__

/**
 * @file mutex.h
 * @brief Core Mutex Routines
 */

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup mutex Thread Mutex Routines
 * @ingroup CORE
 * @{
 */

/** Opaque thread-local mutex structure */
typedef c_uintptr_t mutex_id;

#define MUTEX_DEFAULT  0x0   /**< platform-optimal lock behavior */
#define MUTEX_NESTED   0x1   /**< enable nested (recursive) locks */
#define MUTEX_UNNESTED 0x2   /**< disable nested locks */

/**
 * Initialize Mutex Pool
 */
CORE_DECLARE(status_t) mutex_init(void);

/**
 * Finalize Mutex Pool
 */
CORE_DECLARE(status_t) mutex_final(void);

/**
 * Create and initialize a mutex that can be used to synchronize threads.
 * @param id the memory address where the newly created mutex will be
 *        stored.
 * @param flags Or'ed value of:
 * <PRE>
 *           MUTEX_DEFAULT   platform-optimal lock behavior.
 *           MUTEX_NESTED    enable nested (recursive) locks.
 *           MUTEX_UNNESTED  disable nested locks (non-recursive).
 * </PRE>
 * @param pool the pool from which to allocate the mutex.
 * @warning Be cautious in using MUTEX_DEFAULT.  While this is the
 * most optimial mutex based on a given platform's performance charateristics,
 * it will behave as either a nested or an unnested lock.
 */
CORE_DECLARE(status_t) mutex_create(mutex_id *id, unsigned int flags);
/**
 * Acquire the lock for the given mutex. If the mutex is already locked,
 * the current thread will be put to sleep until the lock becomes available.
 * @param id the mutex on which to acquire the lock.
 */
CORE_DECLARE(status_t) mutex_lock(mutex_id id);

/**
 * Attempt to acquire the lock for the given mutex. If the mutex has already
 * been acquired, the call returns immediately with CORE_EBUSY. Note: it
 * is important that the CORE_STATUS_IS_EBUSY(s) macro be used to determine
 * if the return value was CORE_EBUSY, for portability reasons.
 * @param id the mutex on which to attempt the lock acquiring.
 */
CORE_DECLARE(status_t) mutex_trylock(mutex_id id);

/**
 * Release the lock for the given mutex.
 * @param id the mutex from which to release the lock.
 */
CORE_DECLARE(status_t) mutex_unlock(mutex_id id);

/**
 * Destroy the mutex and free the memory associated with the lock.
 * @param id the mutex to destroy.
 */
CORE_DECLARE(status_t) mutex_delete(mutex_id id);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* ! __MUTEX_H__ */

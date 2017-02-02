#ifndef __CORE_RWLOCK_H__
#define __CORE_RWLOCK_H__

/**
 * @file rwlock.h
 * @brief CORE Reader/Writer Lock Routines
 */

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup rwlock Reader/Writer Lock Routines
 * @ingroup CORE
 * @{
 */

/** Opaque read-write thread-safe lock. */
typedef c_uintptr_t rwlock_id;

/**
 * Initialize Read-Write Lock Pool
 */
CORE_DECLARE(status_t) rwlock_init(void);

/**
 * Finalize Read-Write Lock Pool
 */
CORE_DECLARE(status_t) rwlock_final(void);

/**
 * Note: The following operations have undefined results: unlocking a
 * read-write lock which is not locked in the calling thread; write
 * locking a read-write lock which is already locked by the calling
 * thread; destroying a read-write lock more than once; clearing or
 * destroying the pool from which a <b>locked</b> read-write lock is
 * allocated.
 */

/**
 * Create and initialize a read-write lock that can be used to synchronize
 * threads.
 * @param id the memory address where the newly created readwrite lock
 *        will be stored.
 */
CORE_DECLARE(status_t) rwlock_create(rwlock_id *id);
/**
 * Acquire a shared-read lock on the given read-write lock. This will allow
 * multiple threads to enter the same critical section while they have acquired
 * the read lock.
 * @param id the read-write lock on which to acquire the shared read.
 */
CORE_DECLARE(status_t) rwlock_rdlock(rwlock_id id);

/**
 * Attempt to acquire the shared-read lock on the given read-write lock. This
 * is the same as rwlock_rdlock(), only that the function fails
 * if there is another thread holding the write lock, or if there are any
 * write threads blocking on the lock. If the function fails for this case,
 * CORE_EBUSY will be returned. Note: it is important that the
 * CORE_STATUS_IS_EBUSY(s) macro be used to determine if the return value was
 * CORE_EBUSY, for portability reasons.
 * @param id the rwlock on which to attempt the shared read.
 */
CORE_DECLARE(status_t) rwlock_tryrdlock(rwlock_id id);

/**
 * Acquire an exclusive-write lock on the given read-write lock. This will
 * allow only one single thread to enter the critical sections. If there
 * are any threads currently holding the read-lock, this thread is put to
 * sleep until it can have exclusive access to the lock.
 * @param id the read-write lock on which to acquire the exclusive write.
 */
CORE_DECLARE(status_t) rwlock_wrlock(rwlock_id id);

/**
 * Attempt to acquire the exclusive-write lock on the given read-write lock.
 * This is the same as rwlock_wrlock(), only that the function fails
 * if there is any other thread holding the lock (for reading or writing),
 * in which case the function will return CORE_EBUSY. Note: it is important
 * that the CORE_STATUS_IS_EBUSY(s) macro be used to determine if the return
 * value was CORE_EBUSY, for portability reasons.
 * @param id the rwlock on which to attempt the exclusive write.
 */
CORE_DECLARE(status_t) rwlock_trywrlock(rwlock_id id);

/**
 * Release either the read or write lock currently held by the calling thread
 * associated with the given read-write lock.
 * @param id the read-write lock to be released (unlocked).
 */
CORE_DECLARE(status_t) rwlock_unlock(rwlock_id id);

/**
 * Destroy the read-write lock and free the associated memory.
 * @param id the rwlock to destroy.
 */
CORE_DECLARE(status_t) rwlock_delete(rwlock_id id);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* ! __CORE_RWLOCK_H__ */

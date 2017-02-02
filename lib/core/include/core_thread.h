#ifndef __CORE_THREAD_H__
#define __CORE_THREAD_H__

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** Opaque Thread structure. */
typedef c_uintptr_t thread_id;

/** Opaque Thread attributes structure. */
typedef struct threadattr_t       threadattr_t;

typedef void *(THREAD_FUNC *thread_start_t)(void*);

/**
 * Check if Thread Should Stop
 */
CORE_DECLARE(int) thread_should_stop(void);

/**
 * Initialize Thread
 */
CORE_DECLARE(status_t) thread_init(void);

/**
 * Finalize Thread
 */
CORE_DECLARE(status_t) thread_final(void);

/**
 * Create and initialize a new threadattr variable
 * @param new_attr The newly created threadattr.
 */
CORE_DECLARE(status_t) threadattr_create(threadattr_t **new_attr);

/**
 * Set the stack size of newly created threads.
 * @param attr The threadattr to affect
 * @param stacksize The stack size in bytes
 */
CORE_DECLARE(status_t) threadattr_stacksize_set(
        threadattr_t *attr, size_t stacksize);

/**
 * Delete and initialize a new threadattr variable
 * @param new_attr The newly created threadattr.
 */
CORE_DECLARE(status_t) threadattr_delete(threadattr_t *attr);

/**
 * Create a new thread of execution
 * @param id The newly created thread handle.
 * @param attr The threadattr to use to determine how to create the thread
 * @param func The function to start the new thread in
 * @param data Any data to be passed to the starting function
 */
CORE_DECLARE(status_t) thread_create(thread_id *id,
    threadattr_t *attr, thread_start_t func, void *data);

/**
 * Delete a new thread of execution
 * @param id The thread to delete
 */
CORE_DECLARE(status_t) thread_delete(thread_id id);

/**
 * force the current thread to yield the processor
 */
CORE_DECLARE(void) thread_yield(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __CORE_THREAD_H__ */

#ifndef __CORE_PROC_H__
#define __CORE_PROC_H__

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** Opaque Thread structure. */
typedef c_uintptr_t proc_id;

typedef void *(PROC_FUNC *proc_start_t)(proc_id id, void*);

/**
 * Check if Thread Should Stop
 */
CORE_DECLARE(int) proc_should_stop(void);

/**
 * Initialize Thread
 */
CORE_DECLARE(status_t) proc_init(void);

/**
 * Finalize Thread
 */
CORE_DECLARE(status_t) proc_final(void);

/**
 * Create a new proc of execution
 * @param id The newly created proc handle.
 * @param attr The procattr to use to determine how to create the proc
 * @param func The function to start the new proc in
 * @param data Any data to be passed to the starting function
 */
CORE_DECLARE(status_t) proc_create(proc_id *id, proc_start_t func, void *data);

/**
 * Delete a new proc of execution
 * @param id The proc to delete
 */
CORE_DECLARE(status_t) proc_delete(proc_id id);

CORE_DECLARE(status_t) os_proc_get(void **theproc, proc_id id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __CORE_PROC_H__ */

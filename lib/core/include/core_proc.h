#ifndef __CORE_PROC_H__
#define __CORE_PROC_H__

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** Opaque Thread structure. */
typedef c_uintptr_t proc_id;

typedef void *(PROC_FUNC *proc_func_t)(proc_id id, void*);

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
 */
CORE_DECLARE(status_t) proc_create(proc_id *id, 
        proc_func_t start_func, proc_func_t stop_func,
        void *data);

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

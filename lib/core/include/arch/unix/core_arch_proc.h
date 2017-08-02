#ifndef __CORE_ARCH_PROC_H__
#define __CORE_ARCH_PROC_H__

#include "core_proc.h"
#include "core_semaphore.h"

typedef struct _proc_t {
    pid_t proc;
    void *data;
    proc_func_t start_func;
    proc_func_t stop_func;
    status_t exitval;

    semaphore_id sem1;
    semaphore_id sem2;
} proc_t;

#endif  /* __CORE_ARCH_PROC_H__ */


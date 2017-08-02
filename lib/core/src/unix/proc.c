#define TRACE_MODULE _proc

#include "core.h"
#include "core_arch_proc.h"
#include "core_portable.h"
#include "core_errno.h"
#include "core_param.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pool.h"
#include "core_semaphore.h"
#include "core_signal.h"

pool_declare(proc_pool, proc_t, MAX_NUM_OF_PROC);

status_t proc_init(void)
{
    pool_init(&proc_pool, MAX_NUM_OF_PROC);

    return CORE_OK;
}

status_t proc_final(void)
{
    pool_final(&proc_pool);

    return CORE_OK;
}

static void *dummy_worker(void *opaque)
{
    void *func = NULL;
    proc_t *proc = (proc_t *)opaque;

    proc->proc = getpid();
    semaphore_post(proc->semaphore);
    d_trace(3, "[%d] dummy_worker post semaphore for creating\n", proc->proc);

    func = proc->func((proc_id)proc, proc->data);

    semaphore_post(proc->semaphore);
    d_trace(3, "[%d] dummy_worker post semaphore for deleting\n", proc->proc);

    return func;
}

status_t proc_create(proc_id *id, proc_start_t func, void *data)
{
    proc_t *new = NULL;

    pool_alloc_node(&proc_pool, &new);
    d_assert(new, return CORE_ENOMEM, "proc_pool(%d) is not enough\n",
            MAX_NUM_OF_PROC);
    memset(new, 0, sizeof(proc_id));

    new->data = data;
    new->func = func;

    semaphore_create(&new->semaphore, 0);

    new->proc = fork();
    d_assert(new->proc >= 0, _exit(EXIT_FAILURE), "fork() failed");

    if (new->proc == 0)
    {
        dummy_worker(new);

        _exit(EXIT_SUCCESS);
    }

    d_trace(3, "proc_create wait\n");
    semaphore_wait(new->semaphore);
    d_trace(3, "proc_create done\n");

    *id = (proc_id)new;

    return CORE_OK;
}

status_t proc_delete(proc_id id)
{
    proc_t *proc = (proc_t *)id;

    d_trace(3, "core_kill for %d\n", proc->proc);
    core_kill(proc->proc, SIGTERM);
    d_trace(3, "core_kill done for %d\n", proc->proc);

    d_trace(3, "proc_delete wait\n");
    semaphore_wait(proc->semaphore);
    d_trace(3, "proc_delete done\n");

    semaphore_delete(proc->semaphore);
    pool_free_node(&proc_pool, proc);
    d_trace(3, "delete proc-related memory\n");

    return CORE_OK;
}

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

status_t proc_create(proc_id *id,
        proc_func_t start_func, proc_func_t stop_func,
        void *data)
{
    proc_t *new = NULL;

    pool_alloc_node(&proc_pool, &new);
    d_assert(new, return CORE_ENOMEM, "proc_pool(%d) is not enough\n",
            MAX_NUM_OF_PROC);
    memset(new, 0, sizeof(proc_id));

    new->data = data;
    new->start_func = start_func;
    new->stop_func = stop_func;

    semaphore_create(&new->sem1, 0);
    semaphore_create(&new->sem2, 0);

    new->proc = fork();
    d_assert(new->proc >= 0, _exit(EXIT_FAILURE), "fork() failed");

    if (new->proc == 0)
    {
        status_t rv;

        new->proc = getpid();

        semaphore_post(new->sem1);

        rv = new->start_func((proc_id)new, new->data);
        d_trace(3, "[%d] start_func done(rv = %d)\n", new->proc, rv);

        semaphore_wait(new->sem2);
        d_trace(3, "[%d] semaphore wait in child\n", new->proc);

        if (rv == CORE_OK)
        {
            new->stop_func(new->proc, new->data);
            d_trace(3, "[%d] stop_func done(rv = %d)\n", new->proc, rv);
        }

        semaphore_post(new->sem1);

        semaphore_delete(new->sem1);
        semaphore_delete(new->sem2);
        pool_free_node(&proc_pool, new);
        d_trace(3, "[%d] delete core proc memory in child\n", new->proc);

        core_terminate();
        d_trace(3, "[%d[ core proc terminate...done\n", new->proc);

        _exit(EXIT_SUCCESS);
    }

    d_trace(3, "proc_create wait\n");
    semaphore_wait(new->sem1);
    d_trace(3, "proc_create done\n");

    *id = (proc_id)new;

    return CORE_OK;
}

status_t proc_delete(proc_id id)
{
    proc_t *proc = (proc_t *)id;

#if 0
    core_kill(proc->proc, SIGTERM);
    d_trace(3, "core_kill done for %d\n", proc->proc);
#endif

    semaphore_post(proc->sem2);
    semaphore_wait(proc->sem1);
    d_trace(3, "[%d] semaphore wait in parent\n", proc->proc);

    semaphore_delete(proc->sem1);
    semaphore_delete(proc->sem2);
    pool_free_node(&proc_pool, proc);
    d_trace(3, "[%d] delete core proc memory in parent\n", proc->proc);

    return CORE_OK;
}

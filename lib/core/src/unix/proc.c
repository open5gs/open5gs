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

struct proc_stop_info {
    pid_t proc;
    semaphore_id semaphore;
};

pool_declare(proc_pool, proc_t, MAX_NUM_OF_PROC);

static struct proc_stop_info proc_stop_info;

int proc_should_stop(void)
{
    return (proc_stop_info.proc == getpid());
}

status_t proc_init(void)
{
    pool_init(&proc_pool, MAX_NUM_OF_PROC);

    memset(&proc_stop_info, 0, sizeof(proc_stop_info));

    semaphore_create(&proc_stop_info.semaphore, 0);
    return CORE_OK;
}

status_t proc_final(void)
{
    pool_final(&proc_pool);

    semaphore_delete(proc_stop_info.semaphore);
    return CORE_OK;
}

static void *dummy_worker(void *opaque)
{
    void *func = NULL;
    proc_t *proc = (proc_t *)opaque;

    proc->proc = getpid();
    semaphore_post(proc->semaphore);
    d_trace(3, "[%d] dummy_worker post semaphore\n", proc->proc);

    if (!proc_should_stop())
        func = proc->func((proc_id)proc, proc->data);

    d_trace(3, "[%d] proc stopped = %d\n",
            proc->proc, proc_should_stop());
    semaphore_post(proc_stop_info.semaphore);
    d_trace(3, "[%d] post semaphore for proc_stop_info.semaphore\n",
            proc->proc);

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

        d_trace_level(&_proc, 100);

    new->proc = fork();
    d_assert(new->proc >= 0, _exit(EXIT_FAILURE), "fork() failed");

    if (new->proc == 0)
    {
        d_trace_level(&_proc, 100);
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

    proc_stop_info.proc = proc->proc;
    d_trace(3, "proc_stop_info.proc for %d\n", proc_stop_info.proc);
    core_kill(proc_stop_info.proc, SIGTERM);
    d_trace(3, "core_kill for %d\n", proc_stop_info.proc);
    semaphore_wait(proc_stop_info.semaphore);
    d_trace(3, "semaphore_wait done\n");
    proc_stop_info.proc = 0;

    semaphore_delete(proc->semaphore);
    pool_free_node(&proc_pool, proc);
    d_trace(3, "delete proc-related memory\n");

    return CORE_OK;
}

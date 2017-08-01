#include "core.h"
#include "core_arch_thread.h"
#include "core_portable.h"
#include "core_errno.h"
#include "core_param.h"
#include "core_general.h"
#define TRACE_MODULE _thread
#include "core_debug.h"
#include "core_pool.h"
#include "core_semaphore.h"

struct thread_stop_info {
    pthread_t thread;
    semaphore_id semaphore;
};

pool_declare(thread_pool, thread_t, MAX_NUM_OF_THREAD);
pool_declare(threadattr_pool, threadattr_t, MAX_NUM_OF_THREADATTR);

static struct thread_stop_info thread_stop_info;

int thread_should_stop(void)
{
    return (thread_stop_info.thread == pthread_self());
}

status_t thread_init(void)
{
    pool_init(&thread_pool, MAX_NUM_OF_THREAD);
    pool_init(&threadattr_pool, MAX_NUM_OF_THREADATTR);

    memset(&thread_stop_info, 0, sizeof(thread_stop_info));

    semaphore_create(&thread_stop_info.semaphore, 0);
    return CORE_OK;
}

status_t thread_final(void)
{
    pool_final(&thread_pool);
    pool_final(&threadattr_pool);

    semaphore_delete(thread_stop_info.semaphore);
    return CORE_OK;
}

status_t threadattr_create(threadattr_t **new)
{
    status_t stat;

    pool_alloc_node(&threadattr_pool, &(*new));
    d_assert((*new), return CORE_ENOMEM, "threadattr_pool(%d) is not enough\n",
            MAX_NUM_OF_THREADATTR);
    stat = pthread_attr_init(&(*new)->attr);

    if (stat == 0)
    {
        return CORE_OK;
    }

    return stat;
}

status_t threadattr_stacksize_set(
        threadattr_t *attr, size_t stacksize)
{
    int stat;

    stat = pthread_attr_setstacksize(&attr->attr, stacksize);
    if (stat == 0)
    {
        return CORE_OK;
    }

    return stat;
}

status_t threadattr_delete(threadattr_t *attr)
{
    status_t stat;

    stat = pthread_attr_destroy(&attr->attr);
    pool_free_node(&threadattr_pool, attr);

    if (stat == 0)
    {
        return CORE_OK;
    }

    return stat;
}

static void *dummy_worker(void *opaque)
{
    void *func = NULL;
    thread_t *thread = (thread_t *)opaque;

    thread->thread = pthread_self();
    semaphore_post(thread->semaphore);
    d_trace(3, "[%d] dummy_worker post semaphore\n", thread->thread);

    if (!thread_should_stop())
        func = thread->func((thread_id)thread, thread->data);

    d_trace(3, "[%d] thread stopped = %d\n",
            thread->thread, thread_should_stop());
    semaphore_post(thread_stop_info.semaphore);
    d_trace(3, "[%d] post semaphore for thread_stop_info.semaphore\n",
            thread->thread);

    return func;
}

status_t thread_create(thread_id *id,
    threadattr_t *attr, thread_start_t func, void *data)
{
    status_t stat;
    pthread_attr_t *temp;
    thread_t *new = NULL;

    pool_alloc_node(&thread_pool, &new);
    d_assert(new, return CORE_ENOMEM, "thread_pool(%d) is not enough\n",
            MAX_NUM_OF_THREAD);
    memset(new, 0, sizeof(thread_id));

    new->data = data;
    new->func = func;

    semaphore_create(&new->semaphore, 0);

    if (attr)
        temp = &attr->attr;
    else
        temp = NULL;

    if ((stat = pthread_create(&new->thread, temp, dummy_worker, new)) != 0)
    {
        return stat;
    }

    d_trace(3, "thread_create wait\n");
    semaphore_wait(new->semaphore);
    d_trace(3, "thread_create done\n");

    *id = (thread_id)new;

    return CORE_OK;
}

status_t thread_delete(thread_id id)
{
    thread_t *thread = (thread_t *)id;

    thread_stop_info.thread = thread->thread;
    d_trace(3, "thread_stop_info.thread for %d\n", thread_stop_info.thread);
    semaphore_wait(thread_stop_info.semaphore);
    d_trace(3, "semaphore_wait done\n");
    thread_stop_info.thread = 0;

    pthread_join(thread->thread, 0);

    semaphore_delete(thread->semaphore);
    pool_free_node(&thread_pool, thread);
    d_trace(3, "delete thread-related memory\n");

    return CORE_OK;
}

status_t thread_join(status_t *retval, thread_id id)
{
    thread_t *thread = (thread_t *)id;

    status_t stat;
    status_t *thread_stat;

    if ((stat = pthread_join(thread->thread, (void *)&thread_stat)) == 0) 
    {
        *retval = thread->exitval;
    }

    semaphore_delete(thread->semaphore);
    pool_free_node(&thread_pool, thread);

    return stat;
}

status_t thread_exit(thread_id id, status_t retval)
{
    thread_t *thread = (thread_t *)id;

    thread->exitval = retval;
    pthread_exit(NULL);
    return CORE_OK;
}

status_t thread_detach(thread_id id)
{
    thread_t *thread = (thread_t *)id;

    return pthread_detach(thread->thread);
}

void thread_yield(void)
{
#ifdef HAVE_PTHREAD_YIELD
    pthread_yield(NULL);
#else
#ifdef HAVE_SCHED_YIELD
    sched_yield();
#endif
#endif
}

status_t os_thread_get(os_thread_t **thethd, thread_id id)
{
    thread_t *thread = (thread_t *)id;

    *thethd = &thread->thread;

    return CORE_OK;
}

os_thread_t os_thread_current(void)
{
    return pthread_self();
}

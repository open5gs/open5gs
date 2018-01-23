#define TRACE_MODULE _mutex

#include "core.h"
#include "core_arch_mutex.h"
#include "core_errno.h"
#include "core_param.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pool.h"

pool_declare(mutex_pool, mutex_t, MAX_NUM_OF_MUTEX);

status_t mutex_init(void)
{
    pool_init_wo_lock(&mutex_pool, MAX_NUM_OF_MUTEX);
    return CORE_OK;
}

status_t mutex_final(void)
{
    if (pool_size(&mutex_pool) != pool_avail(&mutex_pool))
        d_error("%d not freed in mutex_pool[%d]",
                pool_size(&mutex_pool) - pool_avail(&mutex_pool),
                pool_size(&mutex_pool));
    d_trace(9, "%d not freed in mutex_pool[%d]\n",
            pool_size(&mutex_pool) - pool_avail(&mutex_pool),
            pool_size(&mutex_pool));
    pool_final(&mutex_pool);
    return CORE_OK;
}

status_t mutex_create(mutex_id *id, unsigned int flags)
{
    mutex_t *new_mutex;
    status_t rv;

    pool_alloc_node(&mutex_pool, &new_mutex);
    d_assert(new_mutex, return CORE_ENOMEM, 
            "mutex_pool(%d) is not enough"
            "(new_mutex=%p, mut:%p, avail:%d,size:%d,head:%d,tail:%d \n",
            MAX_NUM_OF_MUTEX,
            new_mutex, mutex_pool.mut, 
            mutex_pool.avail, mutex_pool.size,
            mutex_pool.head, mutex_pool.tail);

    if (flags & MUTEX_NESTED)
    {
        pthread_mutexattr_t mattr;

        rv = pthread_mutexattr_init(&mattr);
        if (rv) return rv;

        rv = pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
        if (rv)
        {
            pthread_mutexattr_destroy(&mattr);
            return rv;
        }

        rv = pthread_mutex_init(&new_mutex->mutex, &mattr);

        pthread_mutexattr_destroy(&mattr);
    } else
        rv = pthread_mutex_init(&new_mutex->mutex, NULL);

    if (rv)
    {
        return rv;
    }

    *id = (mutex_id)new_mutex;
    return CORE_OK;
}

status_t mutex_lock(mutex_id id)
{
    status_t rv;
    mutex_t *mutex = (mutex_t *)id;

    rv = pthread_mutex_lock(&mutex->mutex);
    return rv;
}

status_t mutex_trylock(mutex_id id)
{
    status_t rv;
    mutex_t *mutex = (mutex_t *)id;

    rv = pthread_mutex_trylock(&mutex->mutex);
    if (rv)
    {
        return (rv == EBUSY) ? CORE_EBUSY : rv;
    }

    return CORE_OK;
}

status_t mutex_unlock(mutex_id id)
{
    status_t rv;
    mutex_t *mutex = (mutex_t *)id;

    rv = pthread_mutex_unlock(&mutex->mutex);

    return rv;
}

status_t mutex_delete(mutex_id id)
{
    status_t rv;
    mutex_t *mutex = (mutex_t *)id;

    rv = pthread_mutex_destroy(&mutex->mutex);
    pool_free_node(&mutex_pool, mutex);

    return rv;
}

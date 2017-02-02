#include "core.h"
#include "core_cond.h"
#include "core_arch_mutex.h"
#include "core_pool.h"
#include "core_param.h"
#include "core_general.h"
#include "core_debug.h"

typedef struct _cond_t {
    pthread_cond_t cond;
} cond_t;

pool_declare(cond_pool, cond_t, MAX_NUM_OF_COND);

status_t cond_init(void)
{
    pool_init(&cond_pool, MAX_NUM_OF_COND);
    return CORE_OK;
}

status_t cond_final(void)
{
    pool_final(&cond_pool);
    return CORE_OK;
}

status_t cond_create(cond_id *id)
{
    cond_t *new_cond = NULL;
    status_t rv;

    pool_alloc_node(&cond_pool, &new_cond);
    d_assert(new_cond, return CORE_ENOMEM, "cond_pool(%d) is not enough\n",
            MAX_NUM_OF_COND);

    if ((rv = pthread_cond_init(&new_cond->cond, NULL)))
    {
        return rv;
    }

    *id = (cond_id)new_cond;
    return CORE_OK;
}

status_t cond_wait(cond_id id, mutex_id mid)
{
    status_t rv;
    cond_t *cond = (cond_t *)id;
    mutex_t *mutex = (mutex_t *)mid;

    rv = pthread_cond_wait(&cond->cond, &mutex->mutex);
    return rv;
}

status_t cond_timedwait(cond_id id,
    mutex_id mid, c_time_t timeout)
{
    status_t rv;
    c_time_t then;
    struct timespec abstime;
    cond_t *cond = (cond_t *)id;
    mutex_t *mutex = (mutex_t *)mid;

    then = time_now() + timeout;
    abstime.tv_sec = time_sec(then);
    abstime.tv_nsec = time_usec(then) * 1000; /* nanoseconds */

    rv = pthread_cond_timedwait(&cond->cond, &mutex->mutex, &abstime);
    if (ETIMEDOUT == rv)
    {
        return CORE_TIMEUP;
    }
    return rv;
}


status_t cond_signal(cond_id id)
{
    status_t rv;
    cond_t *cond = (cond_t *)id;

    rv = pthread_cond_signal(&cond->cond);
    return rv;
}

status_t cond_broadcast(cond_id id)
{
    status_t rv;
    cond_t *cond = (cond_t *)id;

    rv = pthread_cond_broadcast(&cond->cond);
    return rv;
}

status_t cond_delete(cond_id id)
{
    status_t rv;
    cond_t *cond = (cond_t *)id;

    rv = pthread_cond_destroy(&cond->cond);
    pool_free_node(&cond_pool, cond);
    return rv;
}

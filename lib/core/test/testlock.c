#include "core_thread.h"
#include "core_mutex.h"
#include "core_cond.h"
#include "core_rwlock.h"
#include "core_semaphore.h"
#include "core_time.h"
#include "testutil.h"

#define MAX_ITER 40000
#define MAX_COUNTER 10000
#define MAX_RETRY 3

static void *THREAD_FUNC thread_rwlock_func(thread_id id, void *data);
static void *THREAD_FUNC thread_mutex_function(thread_id id, void *data);
static void *THREAD_FUNC thread_cond_producer(thread_id id, void *data);
static void *THREAD_FUNC thread_cond_consumer(thread_id id, void *data);

static mutex_id mutex;
static rwlock_id rwlock;
static semaphore_id semaphore;
static int i = 0, x = 0;

static int buff[MAX_COUNTER];

struct {
    mutex_id    mutex;
    int         nput;
    int         nval;
} put;

struct {
    mutex_id    mutex;
    cond_id     cond;
    int         nready;
} nready;

static mutex_id timeout_mutex;
static cond_id timeout_cond;

static void *THREAD_FUNC thread_rwlock_func(thread_id id, void *data)
{
    int exitLoop = 1;

    while (1)
    {
        rwlock_rdlock(rwlock);
        if (i == MAX_ITER)
            exitLoop = 0;
        rwlock_unlock(rwlock);

        if (!exitLoop)
            break;

        rwlock_wrlock(rwlock);
        if (i != MAX_ITER)
        {
            i++;
            x++;
        }
        rwlock_unlock(rwlock);
    }
    return NULL;
}

static void *THREAD_FUNC thread_mutex_function(thread_id id, void *data)
{
    int exitLoop = 1;

    /* slight delay to allow things to settle */
    core_sleep (1);

    while (1)
    {
        mutex_lock(mutex);
        if (i == MAX_ITER)
            exitLoop = 0;
        else
        {
            i++;
            x++;
        }
        mutex_unlock(mutex);

        if (!exitLoop)
            break;
    }
    return NULL;
}

static void *THREAD_FUNC thread_cond_producer(thread_id id, void *data)
{
    for (;;)
    {
        mutex_lock(put.mutex);
        if (put.nput >= MAX_COUNTER)
        {
            mutex_unlock(put.mutex);
            return NULL;
        }
        buff[put.nput] = put.nval;
        put.nput++;
        put.nval++;
        mutex_unlock(put.mutex);

        mutex_lock(nready.mutex);
        if (nready.nready == 0)
            cond_signal(nready.cond);
        nready.nready++;
        mutex_unlock(nready.mutex);

        *((int *) data) += 1;
    }

    return NULL;
}

static void *THREAD_FUNC thread_cond_consumer(thread_id id, void *data)
{
    int i;

    for (i = 0; i < MAX_COUNTER; i++)
    {
        mutex_lock(nready.mutex);
        while (nready.nready == 0)
            cond_wait(nready.cond, nready.mutex);
        nready.nready--;
        mutex_unlock(nready.mutex);

        if (buff[i] != i)
            printf("buff[%d] = %d\n", i, buff[i]);
    }

    return NULL;
}

static void *THREAD_FUNC thread_semaphore_function(thread_id id, void *data)
{
    int exitLoop = 1;

    /* slight delay to allow things to settle */
    core_sleep (1);

    while (1)
    {
        semaphore_wait(semaphore);
        if (i == MAX_ITER)
            exitLoop = 0;
        else
        {
            i++;
            x++;
        }
        semaphore_post(semaphore);

        if (!exitLoop)
            break;
    }
    return NULL;
}

static void test_mutex(abts_case *tc, void *data)
{
    thread_id t1, t2, t3, t4;
    status_t s1, s2, s3, s4;

    s1 = mutex_create(&mutex, MUTEX_DEFAULT);
    ABTS_INT_EQUAL(tc, CORE_OK, s1);

    i = 0;
    x = 0;

    s1 = thread_create(&t1, NULL, thread_mutex_function, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, s1);
    s2 = thread_create(&t2, NULL, thread_mutex_function, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, s2);
    s3 = thread_create(&t3, NULL, thread_mutex_function, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, s3);
    s4 = thread_create(&t4, NULL, thread_mutex_function, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, s4);

    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t1));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t2));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t3));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t4));

    ABTS_INT_EQUAL(tc, MAX_ITER, x);

    CORE_ASSERT_OK(tc, "delete mutex", mutex_delete(mutex));
}

static void test_thread_rwlock(abts_case *tc, void *data)
{
    thread_id t1, t2, t3, t4;
    status_t s1, s2, s3, s4;

    s1 = rwlock_create(&rwlock);
    if (s1 == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "rwlocks not implemented");
        return;
    }
    CORE_ASSERT_OK(tc, "rwlock_create", s1);

    i = 0;
    x = 0;

    s1 = thread_create(&t1, NULL, thread_rwlock_func, NULL);
    CORE_ASSERT_OK(tc, "create thread 1", s1);
    s2 = thread_create(&t2, NULL, thread_rwlock_func, NULL);
    CORE_ASSERT_OK(tc, "create thread 2", s2);
    s3 = thread_create(&t3, NULL, thread_rwlock_func, NULL);
    CORE_ASSERT_OK(tc, "create thread 3", s3);
    s4 = thread_create(&t4, NULL, thread_rwlock_func, NULL);
    CORE_ASSERT_OK(tc, "create thread 4", s4);

    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t1));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t2));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t3));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t4));

    ABTS_INT_EQUAL(tc, MAX_ITER, x);

    CORE_ASSERT_OK(tc, "delete rwlock", rwlock_delete(rwlock));
}

static void test_cond(abts_case *tc, void *data)
{
    thread_id p1, p2, p3, p4, c1;
    status_t s0, s1, s2, s3, s4;
    int count1, count2, count3, count4;
    int sum;

    CORE_ASSERT_OK(tc, "create put mutex",
               mutex_create(&put.mutex, MUTEX_DEFAULT));

    CORE_ASSERT_OK(tc, "create nready mutex",
                       mutex_create(&nready.mutex, MUTEX_DEFAULT));

    CORE_ASSERT_OK(tc, "create condvar",
                       cond_create(&nready.cond));

    count1 = count2 = count3 = count4 = 0;
    put.nput = put.nval = 0;
    nready.nready = 0;
    i = 0;
    x = 0;

    s0 = thread_create(&p1, NULL, thread_cond_producer, &count1);
    ABTS_INT_EQUAL(tc, CORE_OK, s0);
    s1 = thread_create(&p2, NULL, thread_cond_producer, &count2);
    ABTS_INT_EQUAL(tc, CORE_OK, s1);
    s2 = thread_create(&p3, NULL, thread_cond_producer, &count3);
    ABTS_INT_EQUAL(tc, CORE_OK, s2);
    s3 = thread_create(&p4, NULL, thread_cond_producer, &count4);
    ABTS_INT_EQUAL(tc, CORE_OK, s3);
    s4 = thread_create(&c1, NULL, thread_cond_consumer, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, s4);

    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(p1));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(p2));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(p3));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(p4));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(c1));

    CORE_ASSERT_OK(tc, "delete condvar",
                       cond_delete(nready.cond));
    CORE_ASSERT_OK(tc, "delete ready mutex", mutex_delete(nready.mutex));
    CORE_ASSERT_OK(tc, "delete put mutex", mutex_delete(put.mutex));

    sum = count1 + count2 + count3 + count4;
    ABTS_INT_EQUAL(tc, MAX_COUNTER, sum);
}

static void test_timeoutcond(abts_case *tc, void *data)
{
    status_t s;
    c_time_t timeout;
    c_time_t begin, end;
    int i;

    s = mutex_create(&timeout_mutex, MUTEX_DEFAULT);
    ABTS_INT_EQUAL(tc, CORE_OK, s);

    s = cond_create(&timeout_cond);
    ABTS_INT_EQUAL(tc, CORE_OK, s);

    timeout = time_from_sec(1);

    for (i = 0; i < MAX_RETRY; i++)
    {
        mutex_lock(timeout_mutex);

        begin = time_now();
        s = cond_timedwait(timeout_cond, timeout_mutex, timeout);
        end = time_now();
        mutex_unlock(timeout_mutex);

        if (s != CORE_OK && !STATUS_IS_TIMEUP(s))
        {
            continue;
        }
        ABTS_INT_EQUAL(tc, 1, STATUS_IS_TIMEUP(s));
        ABTS_ASSERT(tc,
            "Timer returned too late", end - begin - timeout < 100000);
        break;
    }
    ABTS_ASSERT(tc, "Too many retries", i < MAX_RETRY);
    CORE_ASSERT_OK(tc, "Unable to delete the conditional",
                       cond_delete(timeout_cond));
    CORE_ASSERT_OK(tc, "Unable to delete the mutex",
                       mutex_delete(timeout_mutex));
}

static void test_semaphore(abts_case *tc, void *data)
{
    thread_id t1, t2, t3, t4;
    status_t s1, s2, s3, s4;

    s1 = semaphore_create(&semaphore, 1);
    ABTS_INT_EQUAL(tc, CORE_OK, s1);

    i = 0;
    x = 0;

    s1 = thread_create(&t1, NULL, thread_semaphore_function, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, s1);
    s2 = thread_create(&t2, NULL, thread_semaphore_function, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, s2);
    s3 = thread_create(&t3, NULL, thread_semaphore_function, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, s3);
    s4 = thread_create(&t4, NULL, thread_semaphore_function, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, s4);

    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t1));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t2));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t3));
    ABTS_INT_EQUAL(tc, CORE_OK, thread_delete(t4));

    ABTS_INT_EQUAL(tc, MAX_ITER, x);

    CORE_ASSERT_OK(tc, "delete semaphore", semaphore_delete(semaphore));
}

#if HAVE_SEM_TIMEDWAIT
static semaphore_id timeout_semaphore;
static void test_timeoutsemaphore(abts_case *tc, void *data)
{
    status_t s;
    c_time_t timeout;
    c_time_t begin, end;
    int i;

    s = semaphore_create(&timeout_semaphore, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, s);

    timeout = time_from_sec(1);

    for (i = 0; i < MAX_RETRY; i++)
    {
        begin = time_now();
        s = semaphore_timedwait(timeout_semaphore, timeout);
        end = time_now();

        if (s != CORE_OK && !STATUS_IS_TIMEUP(s))
        {
            continue;
        }
        ABTS_INT_EQUAL(tc, 1, STATUS_IS_TIMEUP(s));
        ABTS_ASSERT(tc,
            "Timer returned too late", end - begin - timeout < 100000);
        break;
    }
    ABTS_ASSERT(tc, "Too many retries", i < MAX_RETRY);
    CORE_ASSERT_OK(tc, "Unable to delete the semaphore",
                       semaphore_delete(timeout_semaphore));
}
#endif

abts_suite *testlock(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_mutex, NULL);
    abts_run_test(suite, test_thread_rwlock, NULL);
    abts_run_test(suite, test_cond, NULL);
    abts_run_test(suite, test_timeoutcond, NULL);
    abts_run_test(suite, test_semaphore, NULL);
#if HAVE_SEM_TIMEDWAIT
    abts_run_test(suite, test_timeoutsemaphore, NULL);
#endif

    return suite;
}
